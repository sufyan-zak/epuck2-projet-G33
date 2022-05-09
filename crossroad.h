/*
 * crossroad.h
 *
 *  Created on: 9 mai 2022
 *      Author: Administrateur
 */

#ifndef CROSSROAD_H_
#define CROSSROAD_H_

enum orientation{east, south, west, north};
enum direction{start,right, left, forward, stop};

/**
 * @brief			computes which direction to take
 * 					when arriving at a crossroad
 *
 * @return			an int, corresponding to the direction
 */
int crossroad_instruction(int path[10], unsigned int get_size_path, unsigned int get_end_node,
							int *actual_node,enum orientation *current_orientation, enum direction actual_direction);
/**
 * @brief			the following functions operates to the wheels the
 * 					direction indicated by crossroad_instruction function
 *
 * @return			none
 */
void crossroad_turn_right(void);
void crossroad_turn_left(void);
void crossroad_forward(void);


#endif /* CROSSROAD_H_ */
