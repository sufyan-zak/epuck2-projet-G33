/**
 * @file	crossroad.h
 *
 * @brief	Functions declarations and constants used to give orientation/direction
 * 			instructions to the robot
 */


#ifndef CROSSROAD_H_
#define CROSSROAD_H_

// module header
#include <main.h>

/*===========================================================================*/
/* Functions declaration.                                                    */
/*===========================================================================*/

enum orientation{east, south, west, north};
enum direction{start,right, left, forward, stop};
enum state{free_path, obstacle_around};

/**
 * @brief			computes which direction to take
 * 					when arriving at a cross road
 *
 * @param 			int array that is the path followed by the robot
 * 					unsigned int that is the number of nodes crossed by the robot
 * 					int that is the UID of the latest node crossed by the robot
 * 					Cardinal direction of the robot
 * 					Direction in which the robot headed at the last crossroad
 * @return			none
 */
void update_crossroad_instruction(int path[MAX_PATH_SIZE], unsigned int get_size_path,
							int *current_node,enum orientation *current_orientation, enum direction *current_direction);
/**
 * @brief			the following functions operates to the wheels the
 * 					direction indicated by crossroad_instruction function
 *
 * @return			none
 */
void crossroad_turn_right(void);
void crossroad_turn_left(void);
void crossroad_forward(void);
void invert_orientation(enum orientation *current_orientation);

#endif /* CROSSROAD_H_ */
