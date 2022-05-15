/**
 * @file    path_regulator.c
 * @brief   module responsible for switching between the different
 * 			states of the robot(controlling the wheels and the LEDs),
 * 			also responsible for the directions using the path
 * 			calculated by Dijkstra
 */

// standard C headers
#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>

// e-puck2 main processor headers
#include <leds.h>
#include <motors_custom.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <sensors/proximity.h>

// module headers
#include <main.h>
#include <pid_regulator.h>
#include <crossroad.h>
#include <process_image.h>
#include <path_regulator.h>
#include <lecture.h>
#include <main.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#define MIN_TOF_DIST 2
#define OBSTACLE_DIST 70
#define SAFETY_DIST_IR_3 600
#define SAFETY_DIST_IR_2 230
#define TOF_OFFSET 50

// Regulator constants
	// Line follower	(P regulator)
	#define KP_LINE 2
	#define KI_LINE 0
	#define KD_LINE 0
	// Obstacle bypassing	(PID regulator)
	#define KP_IR 0.15
	#define KI_IR 0.025
	#define KD_IR 0.15

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static enum state current_state= free_path;
static enum direction current_direction = start;

/*===========================================================================*/
/* Module threads.	                                                         */
/*===========================================================================*/

/**
 * @brief This thread basically updates the speed of the wheels using the informations
 * 		  given by the camera, the ToF and the IR sensors, the details are explained
 * 		  inside the thread
 *
 *
 */
static THD_WORKING_AREA(waPathRegulator, 4096);
static THD_FUNCTION(PathRegulator, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;

    //indicates if we are going to or coming back from the final node
    uint8_t going_back = 0;
    int current_node = 0;

    //we initialize the orientation to east because of the location of the start point on the map
    enum orientation current_orientation = east;

    int16_t speed_correction_line = 0;
    int16_t speed_correction_obstacle_1 = 0;
    int16_t speed_correction_obstacle_2 = 0;

    //to have less noise due to ambient light for the camera
    set_body_led(1);

	//Calculates the shortest paths from every node to another
	do_djikstra(going_back);

	//copies the path table calculated by Dijkstra into "node_path"
	int node_path[MAX_PATH_SIZE] = {0};
	get_path(node_path);

	while(1){
        time = chVTGetSystemTime();
        uint16_t tof_distance = VL53L0X_get_dist_mm() - TOF_OFFSET;
        uint16_t IR3_distance = get_prox(6);
        uint16_t IR2_distance = get_prox(5);

        //computes a correction factor to let the robot rotate to be centered with the line
        speed_correction_line = pid_regulator(get_line_position(),(IMAGE_BUFFER_SIZE/2), 
											  KP_LINE, KI_LINE, KD_LINE);

        //enters one of the ifs according to the state of the robot
        if (tof_distance > OBSTACLE_DIST && current_state == free_path){

    	//enters the if when the camera spots a red line
			if (get_red_stop()){

				//updates the direction to take for the red_stop
				update_crossroad_instruction(node_path,get_size_path(),
											 &current_node, &current_orientation, &current_direction);

				//prints the number of node the robot is headed using UART3 (Bluetooth)
				chprintf((BaseSequentialStream *)&SD3, "Currently going to node  %d \n \r", current_node);

				if(going_back) reset_leds();

				if (current_direction==left) crossroad_turn_left();
				if (current_direction==right) crossroad_turn_right();
				if (current_direction==forward) crossroad_forward();
				if (current_direction==stop && going_back) {
					right_motor_set_speed(0);
					left_motor_set_speed(0);
					while(1){}
				}
				if (current_direction==stop && !going_back) {
					motor_arrival_animation();
					going_back = 1;
					do_djikstra(going_back);
					current_node = node_path[1];
					get_path(node_path);
					invert_orientation(&current_orientation);
					reset_red_stop();
				}

			}else{
				//continues following the line normally
				right_motor_set_speed(DEFAULT_SPEED_STEPS - speed_correction_line);
				left_motor_set_speed(DEFAULT_SPEED_STEPS + speed_correction_line);
			}

      }else if (tof_distance < OBSTACLE_DIST &&  current_state == free_path && tof_distance > MIN_TOF_DIST){
    	move_closer_obstacle();
    	reset_horizontal_line();
    	current_state = obstacle_around;
     }
     else if (current_state == obstacle_around){
        //computes the correction factor to let the robot stay at a safety distance from the obstacle
        speed_correction_obstacle_1 =  pid_regulator(IR3_distance,SAFETY_DIST_IR_3, KP_IR, KI_IR, KD_IR);
        speed_correction_obstacle_2 =  pid_regulator(IR2_distance,SAFETY_DIST_IR_2, KP_IR, KI_IR, KD_IR);

        //operates the speed correction for both of the IR sensors
   		right_motor_set_speed(DEFAULT_SPEED_STEPS - speed_correction_obstacle_1 - speed_correction_obstacle_2);
   	    left_motor_set_speed(DEFAULT_SPEED_STEPS + speed_correction_obstacle_1 + speed_correction_obstacle_2 );

   	    //enters the if when the robot spots a horizontal black line meaning it finished avoiding the obstacle
   	    if(get_horizontal_line()) {
   	    	//aligns the wheels with the black line and turns so it follows it again
   	    	realign_after_obstacle();
			reset_horizontal_line();
			reset_red_stop();
			current_state = free_path;
   	    }
     }

    chThdSleepUntilWindowed(time, time + MS2ST(10));
  }
}



/**
 * @brief This thread changes the states of the RGB ,the red LEDS
 * 		  and the body LED depending on the current state and current direction
 * 		  of the robot
 *
 */
static THD_WORKING_AREA(waLedToggle, 256);
static THD_FUNCTION(LedToggle, arg) {
	chRegSetThreadName(__FUNCTION__);
    (void)arg;
	 _Bool sequence=0;
	  uint8_t color = 0;

    while(1){
		led_animation(current_state, current_direction, &sequence, &color);
		chThdSleepMilliseconds(300);
	}

}

/*===========================================================================*/
/* Module functions.	                                                     */
/*===========================================================================*/

void path_regulator_start(void){
	chThdCreateStatic(waPathRegulator, sizeof(waPathRegulator), NORMALPRIO, PathRegulator, NULL);
}

void leds_toggle_start(void){
	chThdCreateStatic(waLedToggle, sizeof(waLedToggle), NORMALPRIO+5, LedToggle, NULL);
}

void reset_leds(void){
	set_led(LED3,0);
	set_led(LED7,0);
	set_led(LED1,0);
	set_led(LED5,0);
	set_rgb_led(LED2,0,0,0);
	set_rgb_led(LED4,0,0,0);
	set_rgb_led(LED6,0,0,0);
	set_rgb_led(LED8,0,0,0);
}


void led_animation(enum state current_state, enum direction current_direction,
					_Bool *sequence, uint8_t *color){

	if (current_state==obstacle_around && !*sequence){
				*sequence = 1;
				set_led(LED1,1);
				set_led(LED5,1);
				set_led(LED3,0);
				set_led(LED7,0);
			}
			else if (current_state==obstacle_around && *sequence){
				set_led(LED3,1);
				set_led(LED7,1);
				set_led(LED1,0);
				set_led(LED5,0);
				*sequence = 0;
			}
			if (current_direction==stop){
				toggle_rgb_led(LED2,*color, RGB_MAX_INTENSITY);
				toggle_rgb_led(LED4,*color, RGB_MAX_INTENSITY);
				toggle_rgb_led(LED6,*color, RGB_MAX_INTENSITY);
				toggle_rgb_led(LED8,*color, RGB_MAX_INTENSITY);
				set_body_led(3); //to toggle the body led
				++*color;
				if(*color > 2) *color=0; //goes through the three RGB colors
			}
}
