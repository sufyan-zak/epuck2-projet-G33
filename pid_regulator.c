/**
 * @file    pid_regulator.c
 * @brief   Implementation of a simple PID regulator
 */

//Module header
#include <pid_regulator.h>
#include <math.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/
#define S_TIME_PID       		0.01
#define S_FILTER_PID 	 		(5*S_TIME_PID)
#define ERROR_THRESHOLD	 		0.1f
#define SUM_ERROR_DIVISION 		3.5
#define MAX_SUM_ERROR 			(MOTOR_SPEED_LIMIT/SUM_ERROR_DIVISION)


/*===========================================================================*/
/* Module functions.	                                                     */
/*===========================================================================*/

int16_t pid_regulator(float distance, float goal, float kp, float ki, float kd){

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

	speed = kp * error + ki * sum_error + kd * sum_derivative;
	old_error = error;
    return (int16_t)speed;
}
