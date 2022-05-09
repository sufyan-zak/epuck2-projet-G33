#ifndef PI_REGULATOR_H
#define PI_REGULATOR_H

//start the Path regulator thread
void path_regulator_start(void);


/**
 * @brief			computes which direction to take
 * 					when arriving at a crossroad
 *
 * @return			an int, corresponding to the direction
 */
int crossroad_instruction(int path[10], unsigned int get_size_path, unsigned int get_end_node);

/**
 * @brief			the following functions operates to the wheels the
 * 					direction indicated by crossroad_instruction function
 *
 * @return			none
 */
void crossroad_turn_right(void);
void crossroad_turn_left(void);
void crossroad_forward(void);

#endif /* PI_REGULATOR_H */
