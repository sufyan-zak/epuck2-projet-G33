/*
 * pid_regulator.h
 *
 *  Created on: 9 mai 2022
 *      Author: Yassir Belguerch
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <motors.h>

#ifndef PID_REGULATOR_H_
#define PID_REGULATOR_H_

#define KD_PID 0.5
#define KI_PID 0.05
#define KP_PID 0.7
#define S_TIME_PID       0.01
#define S_FILTER_PID 5*S_TIME_PID
#define ERROR_THRESHOLD			0.1f
#define MAX_SUM_ERROR 			(MOTOR_SPEED_LIMIT/KI_PID)

/**
 * @brief			the following function calculates the speed correction given
 * 					a distance and a goal distance using a PID regulator
 *
 * @return			int speed : speed correction for the regulation
 */
int16_t pid_regulator(float distance, float goal);

#endif /* PID_REGULATOR_H_ */
