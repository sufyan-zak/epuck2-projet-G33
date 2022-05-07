#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <leds.h>
#include <main.h>
#include <motors_custom.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <sensors/proximity.h>

#include <pi_regulator.h>
#include <process_image.h>
#include <lecture.h>


#define OBSTACLE_DIST 70
#define SAFETY_DIST_IR_3 600
#define SAFETY_DIST_IR_2 230
#define DEFAULT_SPEED 500
#define DEFAULT_SPEED_CM 8

#define S_TIME_PID       0.01
#define S_FILTER_PID 5*S_TIME_PID
#define KD_PID 0.5
#define KI_PID 0.05
#define KP_PID 0.7

static _Bool dijsktra_done = 0;
static enum state{free_path,obstacle,obstacle_around};
static enum direction{start,right, left, forward, stop};
static enum orientation{east, south, west, north};
static int actual_node = 0;
static enum orientation current_orientation = east;
static enum state current_state= free_path;
static enum direction actual_direction = start;
static unsigned int city_line_size =3;
int16_t speed_correction = 0;

static _Bool testo = 1;

//simple PI regulator implementation
int16_t pi_regulator(float distance, float goal){

	float error = 0;
	float speed = 0;
	static float old_error = 0;

	static float sum_error = 0;
	static float sum_derivative = 0;
	error = distance - goal;

	//disables the PI regulator if the error is to small
	//this avoids to always move as we cannot exactly be where we want and 
	//the camera is a bit noisy
	if(fabs(error) < ERROR_THRESHOLD){
		return 0;
	}

	sum_error += error;
    sum_derivative = ((sum_derivative*S_FILTER_PID)+0.5*(error-old_error))/(S_TIME_PID+S_FILTER_PID);

	//we set a maximum and a minimum for the sum to avoid an uncontrolled growth

	if(sum_error > MAX_SUM_ERROR){
		sum_error = MAX_SUM_ERROR;
	}else if(sum_error < -MAX_SUM_ERROR){
		sum_error = -MAX_SUM_ERROR;
	}

	speed = KP_PID * error + KI_PID * sum_error + KD_PID * sum_derivative;
	old_error = error;
    return (int16_t)speed;
}

static THD_WORKING_AREA(waPiRegulator, 256);
static THD_FUNCTION(PiRegulator, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;


    int16_t speed = 0;
    int16_t speed_correction = 0;
    int16_t speed_correction_obstacle_1 = 0;
    int16_t speed_correction_obstacle_2 = 0;
    reset_horizontal_line();



    while(1){
        time = chVTGetSystemTime();
        uint16_t tof_distance = VL53L0X_get_dist_mm()-50;
        uint16_t IR_distance = get_prox(2);
        uint16_t IR_distance_d = get_prox(1);
        //computes the speed to give to the motors
        //distance_cm is modified by the image processing thread
        //speed = pi_regulator(get_distance_cm(), GOAL_DISTANCE);
        //computes a correction factor to let the robot rotate to be in front of the line
        speed_correction = (get_line_position() - (IMAGE_BUFFER_SIZE/2));
        speed_correction_obstacle_1 =  pi_regulator(get_prox(2),SAFETY_DIST_IR_3);
        speed_correction_obstacle_2 =  pi_regulator(get_prox(1),SAFETY_DIST_IR_2);


       // speed_correction_obstacle_d = pi_regulator(get_prox(1),SAFETY_DIST);

        //if the line is nearly in front of the camera, don't rotate
       if(abs(speed_correction) < ROTATION_THRESHOLD){
        	speed_correction = 0;
        }
        if(abs(speed_correction_obstacle_1) < ROTATION_THRESHOLD){
        	speed_correction_obstacle_1 = 0;
               }
        if(abs(speed_correction_obstacle_2) < ROTATION_THRESHOLD){
              	speed_correction_obstacle_2 = 0;
            }
       set_body_led(1);

      /* right_motor_set_speed(DEFAULT_SPEED - ROTATION_COEFF * speed_correction);
        left_motor_set_speed(DEFAULT_SPEED + ROTATION_COEFF * speed_correction);
         if (get_red_stop())
                	{
        	right_motor_set_speed(0);
            left_motor_set_speed(0);
                	}*/
//chprintf((BaseSequentialStream *)&SD3, "tof_distance = %d IR_distance1 = %d IR_distance2 = %d \n \r", tof_distance, IR_distance, IR_distance_d);
/*if (test){
	motor_advance_cm(2,2,DEFAULT_SPEED_CM,DEFAULT_SPEED_CM);
		test=0;
	}*/




 	if ((tof_distance > OBSTACLE_DIST && current_state == free_path)){
   		if (dijsktra_done){
			int test[10] = {0};
			get_path(test);
			if (get_red_stop())
        	{
				actual_direction = crossroad_instruction(test,get_size_path(), get_end_node());
				actual_direction=stop;
        		if (actual_direction==left) crossroad_turn_left();
        		if (actual_direction==right) crossroad_turn_right();
        		if (actual_direction==forward) crossroad_forward();
        		if (actual_direction==stop) {
        			right_motor_set_speed(0);
        			left_motor_set_speed(0);
        		}
        	}else{
        		right_motor_set_speed(DEFAULT_SPEED - ROTATION_COEFF * speed_correction);
        		left_motor_set_speed(DEFAULT_SPEED + ROTATION_COEFF * speed_correction);
        	}

   			current_state = free_path;
   		}
   	}
   	else if (tof_distance < OBSTACLE_DIST &&  current_state == free_path && tof_distance > 2  ){
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
   		right_motor_set_speed(DEFAULT_SPEED + speed_correction_obstacle_1 + speed_correction_obstacle_2);
   	    left_motor_set_speed(DEFAULT_SPEED - speed_correction_obstacle_1 - speed_correction_obstacle_2 );
   	 chprintf((BaseSequentialStream *)&SD3, "time = %d \n \r", get_horizontal_line());
   	 if(get_horizontal_line()){
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



void pi_regulator_start(void){
	chThdCreateStatic(waPiRegulator, sizeof(waPiRegulator), NORMALPRIO, PiRegulator, NULL);
}

void crossroad_turn_right(void){
   	motor_advance_cm(8, 8, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	motor_turn_right();
  	motor_advance_cm(6, 6, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	reset_red_stop();
}

void crossroad_turn_left(void){
   	motor_advance_cm(8, 8, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	motor_turn_left();
  	motor_advance_cm(6, 6, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	reset_red_stop();
}

void crossroad_forward(void){
   	motor_advance_cm(15, 15, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	reset_red_stop();
}

void set_dijsktra_done(void){
	dijsktra_done = 1;
}

int crossroad_instruction(int path[10], unsigned int get_size_path, unsigned int get_end_node){

	int i  = get_size_path;
	while(i>=0){
		if(path[i] == actual_node)	 {
			--i;
			break;
		}
		--i;
	}


	if(i < 0) {
		actual_direction = stop;
		return stop;
	}
	else if(i >= 0){
		 if(current_orientation == east ) {
		      if(path[i] == actual_node + 1) {
				  actual_direction = forward;
				  actual_node =  path[i];
				  return forward;
			  }
		      else if(path[i] == actual_node + city_line_size)	{
		    	  current_orientation = south;
		    	  actual_direction = right;
		    	  actual_node =  path[i];
		    	  return right;
		      }
		      else if(path[i] == actual_node - city_line_size)	{
		    	  current_orientation = north;
		    	  actual_direction = left;
		    	  actual_node =  path[i];
		    	  return left;
		      }
		 }
		 else if(current_orientation == south ) {
		      if(path[i] == actual_node + 1)	{
		    	  current_orientation = east;
		    	  actual_direction = left;
		    	  actual_node =  path[i];
		    	  return left;
		      }
		      else if(path[i] == actual_node + city_line_size) {
				  actual_direction = forward;
				  actual_node =  path[i];
				  return forward;
			  }
		      else if(path[i] == actual_node - 1)	{
		    	  current_orientation = west;
		    	  actual_direction = right;
		    	  actual_node =  path[i];
		    	  return right;
		      }
		 }
		 else if(current_orientation == west ) {
		      if(path[i] == actual_node + city_line_size)	{
		    	  current_orientation = south;
		    	  actual_direction = left;
		    	  actual_node =  path[i];
		    	  return left;
		      }
		      else if(path[i] == actual_node - 1)	 {
				  actual_direction = forward;
				  actual_node =  path[i];
				  return forward;
			  }

		      else if(path[i] == actual_node - city_line_size)	{
		    	  current_orientation = north;
		    	  actual_direction = right;
		    	  actual_node =  path[i];
		    	  return right;
		      }
		  }
		 else if(current_orientation == north) {
		      if(path[i] == actual_node - city_line_size)	{
				  actual_direction = forward;
				  actual_node =  path[i];
				  return forward;
			  }
		      else if(path[i] == actual_node -1)	{
		    	  current_orientation = west;
		    	  actual_direction = left;
		    	  actual_node =  path[i];
		    	  return left;
		      }
		      else if(path[i] == actual_node + 1)	{
		    	  current_orientation = east;
		    	  actual_direction = right;
		    	  actual_node =  path[i];
		    	  return right;
		      }


		}

	}

}
