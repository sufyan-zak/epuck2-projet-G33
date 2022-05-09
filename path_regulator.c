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

static enum state{free_path,obstacle,obstacle_around};


static enum state current_state= free_path;
static enum direction actual_direction = start;


static THD_WORKING_AREA(waPathRegulator, 4096);
static THD_FUNCTION(PathRegulator, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;
    int actual_node = 0;
    enum orientation current_orientation = east;
    int16_t speed_correction_line = 0;
    int16_t speed_correction_obstacle_1 = 0;
    int16_t speed_correction_obstacle_2 = 0;
    reset_horizontal_line();

    //to have less noise due to ambient light for the camera
    set_body_led(1);

	//Calculates the shortest paths from every node to another
	do_djikstra();

	//copies the path table calculated by djikstra into "node_path"
	int node_path[10] = {0};
	get_path(node_path);

    while(1){
        time = chVTGetSystemTime();
        uint16_t tof_distance = VL53L0X_get_dist_mm()-50;
        uint16_t IR3_distance = get_prox(2);
        uint16_t IR2_distance = get_prox(1);

        //computes a correction factor to let the robot rotate to be centered with the line
        speed_correction_line = (get_line_position() - (IMAGE_BUFFER_SIZE/2));

        //if the line is nearly in front of the camera, don't rotate
       if(abs(speed_correction_line) < ROTATION_THRESHOLD){

        	speed_correction_line = 0;
       }

       /*if(abs(speed_correction_obstacle_1) < ROTATION_THRESHOLD){

        	speed_correction_obstacle_1 = 0;
       }
       if(abs(speed_correction_obstacle_2) < ROTATION_THRESHOLD){

            speed_correction_obstacle_2 = 0;
       }*/
for (int i =0; i<10; ++i){
	chprintf((BaseSequentialStream *)&SD3, "node =%d \n \r", node_path[i]);
}
      //enters one of the ifs according to the state of the robot
      if (tof_distance > OBSTACLE_DIST && current_state == free_path){
    	//enters the if when the camera spots a red line
		if (get_red_stop()){

			actual_direction = crossroad_instruction(node_path,get_size_path(), get_end_node(),
													&actual_node, &current_orientation, actual_direction);
			if (actual_direction==left) crossroad_turn_left();
			if (actual_direction==right) crossroad_turn_right();
			if (actual_direction==forward) crossroad_forward();
			if (actual_direction==stop) {

				right_motor_set_speed(0);
				left_motor_set_speed(0);
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
    	motor_turn_half();
    	motor_advance_cm(4,4,-DEFAULT_SPEED_CM,-DEFAULT_SPEED_CM);
    	motor_turn_right();
    	reset_horizontal_line();
    	current_state = obstacle_around;
     }
     else if (current_state == obstacle_around){
   		set_check_test();

        //computes the correction factor to let the robot stay at a safety distance from the obstacle
        speed_correction_obstacle_1 =  pid_regulator(IR3_distance,SAFETY_DIST_IR_3);
        speed_correction_obstacle_2 =  pid_regulator(IR2_distance,SAFETY_DIST_IR_2);

        //operates the speed correction for both of the IR sensors
   		right_motor_set_speed(DEFAULT_SPEED + speed_correction_obstacle_1 + speed_correction_obstacle_2);
   	    left_motor_set_speed(DEFAULT_SPEED - speed_correction_obstacle_1 - speed_correction_obstacle_2 );

   	    //enters the if when the robot spots a horizontal black line meaning it finished avoiding the obstacle
   	    if(get_horizontal_line()){
   	    	//aligns the wheels with the black line and turns so it follows it again
			motor_advance_cm(4,4,DEFAULT_SPEED_CM,DEFAULT_SPEED_CM);
			motor_turn_left();
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


