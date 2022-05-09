/*
 * crossroad.c
 *
 *  Created on: 9 mai 2022
 *      Author: Yassir Belguerch
 */

#include <main.h>
#include <motors_custom.h>
#include <crossroad.h>

#define CITY_SIZE_LINE 5
#define CROSSROAD_IN_DIST 8 //distance to advance before turning at crossroad
#define CROSSROAD_OUT_DIST 6 //distance to advance before finding the black line after turning at the crossroad
#define CROSSROAD_FORWARD_DIST 15


void crossroad_turn_right(void){
   	motor_advance_cm(CROSSROAD_IN_DIST, CROSSROAD_IN_DIST, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	motor_turn_right();
  	motor_advance_cm(CROSSROAD_OUT_DIST, CROSSROAD_OUT_DIST, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	reset_red_stop();
}

void crossroad_turn_left(void){
   	motor_advance_cm(CROSSROAD_IN_DIST, CROSSROAD_IN_DIST, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	motor_turn_left();
  	motor_advance_cm(CROSSROAD_OUT_DIST, CROSSROAD_OUT_DIST, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	reset_red_stop();
}

void crossroad_forward(void){
   	motor_advance_cm(CROSSROAD_FORWARD_DIST, CROSSROAD_FORWARD_DIST, DEFAULT_SPEED_CM, DEFAULT_SPEED_CM);
	reset_red_stop();
}

int crossroad_instruction(int path[10], unsigned int get_size_path, unsigned int get_end_node,  int *actual_node,
															enum orientation *current_orientation, enum direction actual_direction){
	int i  = get_size_path;
	while(i>=0){
		if(path[i] == *actual_node)	{
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
		 if(*current_orientation == east ) {
		      if(path[i] == *actual_node + 1) {
				  actual_direction = forward;
				  *actual_node =  path[i];
		      }
		      else if(path[i] == *actual_node + CITY_SIZE_LINE)	{
		    	  *current_orientation = south;
		    	  actual_direction = right;
		    	  *actual_node =  path[i];
		      }
		      else if(path[i] == *actual_node - CITY_SIZE_LINE)	{
		    	  *current_orientation = north;
		    	  actual_direction = left;
		    	  *actual_node =  path[i];
		      }
		 }
		 else if(*current_orientation == south ) {
		      if(path[i] == *actual_node + 1)	{
		    	  *current_orientation = east;
		    	  actual_direction = left;
		    	  *actual_node =  path[i];
		      }
		      else if(path[i] == *actual_node + CITY_SIZE_LINE) {
				  actual_direction = forward;
				  *actual_node =  path[i];
			  }
		      else if(path[i] == *actual_node - 1)	{
		    	  *current_orientation = west;
		    	  actual_direction = right;
		    	  *actual_node = path[i];
		      }
		 }
		 else if(*current_orientation == west ) {
		      if(path[i] == *actual_node + CITY_SIZE_LINE)	{
		    	  *current_orientation = south;
		    	  actual_direction = left;
		    	  *actual_node =  path[i];
		      }
		      else if(path[i] == *actual_node - 1)	 {
				  actual_direction = forward;
				  *actual_node =  path[i];
			  }

		      else if(path[i] == *actual_node - CITY_SIZE_LINE)	{
		    	  *current_orientation = north;
		    	  actual_direction = right;
		    	  *actual_node =  path[i];
		      }
		  }
		 else if(*current_orientation == north) {
		      if(path[i] == *actual_node - CITY_SIZE_LINE)	{
				  actual_direction = forward;
				  *actual_node =  path[i];

			  }
		      else if(path[i] == *actual_node -1)	{
		    	  *current_orientation = west;
		    	  actual_direction = left;
		    	  *actual_node =  path[i];
		      }
		      else if(path[i] == *actual_node + 1)	{
		    	  *current_orientation = east;
		    	  actual_direction = right;
		    	  *actual_node =  path[i];
		      }
		}
	}
	return actual_direction;
}
