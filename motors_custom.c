/**
 * @file    motors_custom.c
 * @brief   Functions used to control the wheels, using the functions in "motors.c"
 *
 */

//standard C header
#include <math.h>

//Module headers
#include <motors.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#define NUMBER_STEPS_ONE_TURN 1000
#define WHEEL_PERIMETER_CM    13
#define DEFAULT_SPEED_CM      8
#define ROBOT_DIAMETER       5.3
#define ROBOT_PERIMETER 	M_PI*ROBOT_DIAMETER

/*===========================================================================*/
/* Module functions.	                                                     */
/*===========================================================================*/

void motor_advance_cm(float postion_r, float position_l , float speed_r , float speed_l ){

	uint32_t objective_steps_r = (NUMBER_STEPS_ONE_TURN/WHEEL_PERIMETER_CM)*postion_r;
	uint32_t objective_steps_l= (NUMBER_STEPS_ONE_TURN/WHEEL_PERIMETER_CM)*position_l;
	int16_t speed_r_steps = (NUMBER_STEPS_ONE_TURN/WHEEL_PERIMETER_CM)*speed_r;
	int16_t speed_l_steps = (NUMBER_STEPS_ONE_TURN/WHEEL_PERIMETER_CM)*speed_l;
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

		if (current_steps_r > objective_steps_r) {
			right_motor_set_speed(0);
			right_position_reached = 1;
		}

		if (current_steps_l > objective_steps_l) {
			left_motor_set_speed(0);
			left_position_reached = 1;
		}
		if (right_position_reached && left_position_reached) break;
	}

}


void motor_turn_right(void){
	motor_advance_cm(ROBOT_PERIMETER/4,ROBOT_PERIMETER/4,-DEFAULT_SPEED_CM,DEFAULT_SPEED_CM);
}


void motor_turn_left(void){
	motor_advance_cm(ROBOT_PERIMETER/4,ROBOT_PERIMETER/4,DEFAULT_SPEED_CM,-DEFAULT_SPEED_CM);
}

void motor_turn_half_right(void){
	motor_advance_cm(ROBOT_PERIMETER/2,ROBOT_PERIMETER/2,DEFAULT_SPEED_CM,-DEFAULT_SPEED_CM);

}

void motor_turn_half_left(void){
	motor_advance_cm(ROBOT_PERIMETER/2,ROBOT_PERIMETER/2,-DEFAULT_SPEED_CM,DEFAULT_SPEED_CM);

}
void motor_arrival_animation(void){
	motor_turn_half_left();
	motor_turn_right();
	motor_turn_half_right();
	motor_turn_half_left();
	motor_turn_left();
}

void realign_after_obstacle(void){
	motor_advance_cm(3.5,3.5,DEFAULT_SPEED_CM,DEFAULT_SPEED_CM);
	motor_turn_left();
	motor_advance_cm(0.5,0.5,DEFAULT_SPEED_CM,DEFAULT_SPEED_CM);
}

void move_closer_obstacle(void){
	motor_turn_half_right();
	motor_advance_cm(5,5,-DEFAULT_SPEED_CM,-DEFAULT_SPEED_CM);
	motor_turn_right();
}
