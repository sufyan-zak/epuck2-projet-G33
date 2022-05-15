/*
 * pid_regulator.h
 *
 *  Created on: 9 mai 2022
 *      Author: Yassir Belguerch
 */

#ifndef PID_REGULATOR_H_
#define PID_REGULATOR_H_

//standard C headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//e-puck2 main processor headers
#include <motors.h>

/*===========================================================================*/
/* Functions declaration.                                                    */
/*===========================================================================*/
/**
 * @brief			the following function calculates the speed correction given
 * 					a distance and a goal distance using a PID regulator
 *
 * @param 			A float that is the actual distance
 * 					A float that is the goal distance
 * 					A float that is the proportionnal term of the regulator
 * 					A float that is the integrator term of the regulator
 * 					A float that is the derivative term of the regulator
 * 
 * @return			speed correction for the regulation, then given to the 
 * 					wheels
 */
int16_t pid_regulator(float distance, float goal, float kp, float ki, float kd);

#endif /* PID_REGULATOR_H_ */
