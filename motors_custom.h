/*
 * motors_custom.h
 *
 *  Created on: 5 mai 2022
 *      Author: Yassir
 */

#ifndef MOTORS_CUSTOM_H_
#define MOTORS_CUSTOM_H_


void motor_advance_cm(float postion_r, float position_l , float speed_r , float speed_l );

void motor_turn_right(void);

void motor_turn_left(void);

void motor_turn_half_right(void);

void motor_turn_half_left(void);
#endif /* MOTORS_CUSTOM_H_ */
