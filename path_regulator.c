#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <main.h>

#include <leds.h>
#include <motors_custom.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <sensors/proximity.h>

#include <pid_regulator.h>
#include <crossroad.h>
#include <process_image.h>
#include <lecture.h>

#include <path_regulator.h>

#define MIN_TOF_DIST 2
#define OBSTACLE_DIST 70
#define SAFETY_DIST_IR_3 600
#define SAFETY_DIST_IR_2 230

static enum state{free_path, obstacle, obstacle_around};
static enum state current_state= free_path;
static enum direction current_direction = start;
_Bool TESTO = 0;
static THD_WORKING_AREA(waPathRegulator, 4096);
static THD_FUNCTION(PathRegulator, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;
    int current_node = 0;
    enum orientation current_orientation = east;
    uint8_t going_back = 0;
    int16_t speed_correction_line = 0;
    int16_t speed_correction_obstacle_1 = 0;
    int16_t speed_correction_obstacle_2 = 0;
    reset_horizontal_line();

    //to have less noise due to ambient light for the camera
    set_body_led(1);

	//Calculates the shortest paths from every node to another
	do_djikstra(going_back);

	//copies the path table calculated by djikstra into "node_path"
	int node_path[10] = {0};
	get_path(node_path);

    while(1){
        time = chVTGetSystemTime();
        uint16_t tof_distance = VL53L0X_get_dist_mm()-50;
        uint16_t IR3_distance = get_prox(6);
        uint16_t IR2_distance = get_prox(5);

        //computes a correction factor to let the robot rotate to be centered with the line
        speed_correction_line = (get_line_position() - (IMAGE_BUFFER_SIZE/2));

        //if the line is nearly in front of the camera, don't rotate
     /*  if(abs(speed_correction_line) < ROTATION_THRESHOLD){

        	speed_correction_line = 0;
       }*/
      /* if(abs(speed_correction_obstacle_1) < ROTATION_THRESHOLD){

        	speed_correction_obstacle_1 = 0;
       }
       if(abs(speed_correction_obstacle_2) < ROTATION_THRESHOLD){

            speed_correction_obstacle_2 = 0;
       }*/

      //enters one of the ifs according to the state of the robot
      if (tof_distance > OBSTACLE_DIST && current_state == free_path){
    	//enters the if when the camera spots a red line
		if (get_red_stop()){


			current_direction = crossroad_instruction(node_path,get_size_path(), get_end_node(),
												&current_node, &current_orientation, current_direction);

		  	chprintf((BaseSequentialStream *)&SD3, "Currently going to node  %d \n \r", current_node);
		  	chprintf((BaseSequentialStream *)&SD3, "At the next crossroad turning  %d \n \r", current_direction);
		  	chprintf((BaseSequentialStream *)&SD3, "My front side is pointing   %d \n \r", current_orientation);

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
					motor_turn_half_left();
					/*for(int i = 0;i<10000;++i){
					 	chprintf((BaseSequentialStream *)&SD3, "IR_distance2 = %d \n \r", 5);
					}*/



					/*motor_turn_half_left();
					motor_turn_half_right();
					motor_turn_half_right();
					motor_turn_half_left();
					motor_turn_half_left();
					motor_turn_half_right();*/
					going_back = 1;
				   do_djikstra(going_back);
					current_node = node_path[1];
					get_path(node_path);
					invert_orientation(&current_orientation);
					reset_red_stop();
			}


		}else{

			right_motor_set_speed(DEFAULT_SPEED - ROTATION_COEFF * speed_correction_line);
			left_motor_set_speed(DEFAULT_SPEED + ROTATION_COEFF * speed_correction_line);
		}
		current_state = free_path;
     }
     else if (tof_distance < OBSTACLE_DIST &&  current_state == free_path && tof_distance > MIN_TOF_DIST){
    	right_motor_set_speed(0);
    	left_motor_set_speed(0);
    	motor_turn_half_right();
    	motor_advance_cm(4,4,-DEFAULT_SPEED_CM,-DEFAULT_SPEED_CM);
    	motor_turn_left();
    	reset_horizontal_line();
    	current_state = obstacle_around;
     }
     else if (current_state == obstacle_around){
   		set_check_test();

        //computes the correction factor to let the robot stay at a safety distance from the obstacle
        speed_correction_obstacle_1 =  pid_regulator(IR3_distance,SAFETY_DIST_IR_3,0.5,0.05,0.7);
        speed_correction_obstacle_2 =  pid_regulator(IR2_distance,SAFETY_DIST_IR_2,0.5,0.05,0.7);

        //operates the speed correction for both of the IR sensors
   		right_motor_set_speed(DEFAULT_SPEED - speed_correction_obstacle_1 - speed_correction_obstacle_2);
   	    left_motor_set_speed(DEFAULT_SPEED + speed_correction_obstacle_1 + speed_correction_obstacle_2 );

   	    //enters the if when the robot spots a horizontal black line meaning it finished avoiding the obstacle
   	    if(get_horizontal_line()){
   	    	//aligns the wheels with the black line and turns so it follows it again
			motor_advance_cm(4,4,DEFAULT_SPEED_CM,DEFAULT_SPEED_CM);
			motor_turn_right();
			motor_advance_cm(0.5,0.5,DEFAULT_SPEED_CM,DEFAULT_SPEED_CM);
			reset_horizontal_line();
			current_state = free_path;
   	    }
     }
    chThdSleepUntilWindowed(time, time + MS2ST(10));
  }
}



void path_regulator_start(void){
	chThdCreateStatic(waPathRegulator, sizeof(waPathRegulator), NORMALPRIO, PathRegulator, NULL);
}

static THD_WORKING_AREA(waLedToggle, 256);
static THD_FUNCTION(LedToggle, arg) {
	chRegSetThreadName(__FUNCTION__);
    (void)arg;
    uint8_t sequence= 0;
    int color = 0;
    while(1){

		if (current_state==obstacle_around && !sequence){
			++sequence;
			set_led(LED1,1);
			set_led(LED5,1);
			set_led(LED3,0);
			set_led(LED7,0);
			}
		else if (current_state==obstacle_around && sequence==1){
			set_led(LED3,1);
			set_led(LED7,1);
			set_led(LED1,0);
			set_led(LED5,0);
			sequence = 0;

		}
		if (current_direction==stop){
			toggle_rgb_led(LED2,color, RGB_MAX_INTENSITY);
			toggle_rgb_led(LED4,color, RGB_MAX_INTENSITY);
			toggle_rgb_led(LED6,color, RGB_MAX_INTENSITY);
			toggle_rgb_led(LED8,color, RGB_MAX_INTENSITY);
			++color;
			if(color>2) color=0;
		}
		chThdSleepMilliseconds(300);
	}

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

