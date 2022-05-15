/**
 * @file    motors_custom.c
 * @brief   Declaration of functions used to control the wheels.
 * 			Based on the library "motors.c"
 *
 */
 
#ifndef MOTORS_CUSTOM_H_
#define MOTORS_CUSTOM_H_

/*===========================================================================*/
/* Functions declaration.                                                    */
/*===========================================================================*/

/**
 * @brief :		advances an amount of cm at a given speed 
 *
 * @param : 	First two parameters are the distance in cm to advance to each wheel
 * 				Last two parameters are the speed of each wheel
 * 
 * @return :	None
 */
void motor_advance_cm(float postion_r, float position_l , float speed_r , float speed_l);


/**
 * @brief :		makes the wheels turn 90 degrees left or right
 *
 * @return :	None
 */
void motor_turn_right(void);
void motor_turn_left(void);

/**
 * @brief :		turns 180 degrees left or right
 *
 *
 *
 * @return :	None
 */
void motor_turn_half_right(void);
void motor_turn_half_left(void);

/**
 * @brief :		movements when the robot reaches the end node and when 
 * 				it comes back to start node
 *
 *
 * @return :	None
 */
void motor_arrival_animation(void);

/**
 * @brief :		movements to realign the wheels of the robot and face the black line 
 * 				to continue the path
 *
 *
 * @return :	None
 */
void realign_after_obstacle(void);

/**
 * @brief :		moves closer to the obstacle backwards since we need to use the IR sensors
 * 				and the mirror is at the front
 *
 *
 * @return :	None
 */

void move_closer_obstacle(void);

#endif /* MOTORS_CUSTOM_H_ */
