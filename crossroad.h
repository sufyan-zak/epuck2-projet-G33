/*
 * crossroad.h
 *
 *  Created on: 9 mai 2022
 *      Author: Yassir Belguerch
 */


#ifndef CROSSROAD_H_
#define CROSSROAD_H_

#include <main.h>

enum orientation{east, south, west, north};
enum direction{start,right, left, forward, stop};
enum state{free_path, obstacle_around};

/**
 * @brief			computes which direction to take
 * 					when arriving at a cross road
 *
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
