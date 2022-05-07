/*
 * motors_custom.c
 *
 *  Created on: 5 mai 2022
 *      Author: Yassir Belguerch
 */

#include <math.h>
#include <motors.h>


#define NUMBER_STEPS_ONE_TURN 1000
#define WHEEL_PERIMETER_CM 13
#define DEFAULT_SPEED_CM 8
#define ROBOT_PERIMETER M_PI*5.3

void motor_advance_cm(float postion_r, float position_l , float speed_r , float speed_l ){

	uint32_t objective_steps_r = (NUMBER_STEPS_ONE_TURN/WHEEL_PERIMETER_CM)*postion_r;
	uint32_t objective_steps_l= (NUMBER_STEPS_ONE_TURN/WHEEL_PERIMETER_CM)*position_l;
	int speed_r_steps = (NUMBER_STEPS_ONE_TURN/WHEEL_PERIMETER_CM)*speed_r;
	int speed_l_steps = (NUMBER_STEPS_ONE_TURN/WHEEL_PERIMETER_CM)*speed_l;
	uint32_t current_steps_r = 0;
	uint32_t current_steps_l = 0;

	_Bool right_position_reached = 0;
	_Bool left_position_reached = 0;



	left_motor_set_pos(0);
	right_motor_set_pos(0);

	left_motor_set_speed(speed_l_steps);
	right_motor_set_speed(speed_r_steps);


	while (1){

		current_steps_r = fabs(right_motor_get_pos());
		current_steps_l = fabs(left_motor_get_pos());

		if (current_steps_r > objective_steps_r)
		{
			right_motor_set_speed(0);
			right_position_reached = 1;
		}

		if (current_steps_l > objective_steps_l)
				{
					left_motor_set_speed(0);
					left_position_reached = 1;
				}
		if (right_position_reached && left_position_reached ) break;
	}

}


void motor_turn_right(void){
	motor_advance_cm(ROBOT_PERIMETER/4,ROBOT_PERIMETER/4,-DEFAULT_SPEED_CM,DEFAULT_SPEED_CM);
}


void motor_turn_left(void){
	motor_advance_cm(ROBOT_PERIMETER/4,ROBOT_PERIMETER/4,DEFAULT_SPEED_CM,-DEFAULT_SPEED_CM);
}

void motor_turn_half(void){
	motor_advance_cm(ROBOT_PERIMETER/2,ROBOT_PERIMETER/2,DEFAULT_SPEED_CM,-DEFAULT_SPEED_CM);

}
