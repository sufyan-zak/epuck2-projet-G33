#ifndef PI_REGULATOR_H
#define PI_REGULATOR_H

//start the PI regulator thread
void pi_regulator_start(void);
void crossroad_turn_right(void);
void crossroad_turn_left(void);
void crossroad_forward(void);
void set_dijsktra_done(void);
int crossroad_instruction(int path[10], unsigned int get_size_path, unsigned int get_end_node);
#endif /* PI_REGULATOR_H */
