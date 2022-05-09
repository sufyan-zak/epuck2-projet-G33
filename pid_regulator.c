/*
 * pid_regulator.c
 *
 *  Created on: 9 mai 2022
 *      Author: Administrateur
 */



#include <pid_regulator.h>
#include <math.h>

int16_t pid_regulator(float distance, float goal){

	float error = 0;
	float speed = 0;
	static float old_error = 0;

	static float sum_error = 0;
	static float sum_derivative = 0;
	error = distance - goal;

	//disables the PID regulator if the error is to small
	//this avoids to always move as we cannot exactly be where we want and
	//the camera is a bit noisy
	if(fabs(error) < ERROR_THRESHOLD){
		return 0;
	}

	sum_error += error;
    sum_derivative = ((sum_derivative*S_FILTER_PID)+0.5*(error-old_error))/(S_TIME_PID+S_FILTER_PID);

	//we set a maximum and a minimum for the integrating sum to avoid an uncontrolled growth

	if(sum_error > MAX_SUM_ERROR){
		sum_error = MAX_SUM_ERROR;
	}else if(sum_error < -MAX_SUM_ERROR){
		sum_error = -MAX_SUM_ERROR;
	}

	speed = KP_PID * error + KI_PID * sum_error + KD_PID * sum_derivative;
	old_error = error;
    return (int16_t)speed;
}
