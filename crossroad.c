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
#define CROSSROAD_IN_DIST 7 //distance to advance before turning at crossroad
#define CROSSROAD_OUT_DIST 3//distance to advance before finding the black line after turning at the crossroad
#define CROSSROAD_FORWARD_DIST 10


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

int crossroad_instruction(int path[10], unsigned int get_size_path, unsigned int get_end_node,  int *current_node,
															enum orientation *current_orientation, enum direction current_direction){
	int i  = get_size_path;
	while(i>=0){
		if(path[i] == *current_node)	{
			--i;
			break;
		}
		--i;
	}

	if(i < 0 ) {
		current_direction = stop;
		return stop;
	}
	else if(i >= 0){
		 if(*current_orientation == east ) {
		      if(path[i] == *current_node + 1) {
				  current_direction = forward;
				  *current_node =  path[i];
		      }
		      else if(path[i] == *current_node + CITY_SIZE_LINE)	{
		    	  *current_orientation = south;
		    	  current_direction = right;
		    	  *current_node =  path[i];
		      }
		      else if(path[i] == *current_node - CITY_SIZE_LINE)	{
		    	  *current_orientation = north;
		    	  current_direction = left;
		    	  *current_node =  path[i];
		      }
		 }
		 else if(*current_orientation == south ) {
		      if(path[i] == *current_node + 1)	{
		    	  *current_orientation = east;
		    	  current_direction = left;
		    	  *current_node =  path[i];
		      }
		      else if(path[i] == *current_node + CITY_SIZE_LINE) {
				  current_direction = forward;
				  *current_node =  path[i];
			  }
		      else if(path[i] == *current_node - 1)	{
		    	  *current_orientation = west;
		    	  current_direction = right;
		    	  *current_node = path[i];
		      }
		 }
		 else if(*current_orientation == west ) {
		      if(path[i] == *current_node + CITY_SIZE_LINE)	{
		    	  *current_orientation = south;
		    	  current_direction = left;
		    	  *current_node =  path[i];
		      }
		      else if(path[i] == *current_node - 1)	 {
				  current_direction = forward;
				  *current_node =  path[i];
			  }

		      else if(path[i] == *current_node - CITY_SIZE_LINE)	{
		    	  *current_orientation = north;
		    	  current_direction = right;
		    	  *current_node =  path[i];
		      }
		  }
		 else if(*current_orientation == north) {
		      if(path[i] == *current_node - CITY_SIZE_LINE)	{
				  current_direction = forward;
				  *current_node =  path[i];

			  }
		      else if(path[i] == *current_node -1)	{
		    	  *current_orientation = west;
		    	  current_direction = left;
		    	  *current_node =  path[i];
		      }
		      else if(path[i] == *current_node + 1)	{
		    	  *current_orientation = east;
		    	  current_direction = right;
		    	  *current_node =  path[i];
		      }
		}
	}
	return current_direction;
}
void invert_orientation(enum orientation *current_orientation) {
  switch(*current_orientation) {
    case west:
      *current_orientation = east;
      break;
    case east:
      *current_orientation = west;
      break;
    case north:
      *current_orientation = south;
      break;
    case south:
      *current_orientation = north;
      break;
  }
}
