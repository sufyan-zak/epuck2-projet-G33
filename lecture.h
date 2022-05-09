/*
 * lecture.h
 *
 *  Created on: 28 avr. 2022
 *      Author: Administrateur
 */
#include <noeud.h>
#ifndef LECTURE_H_
#define LECTURE_H_
#define NB_NODES SIZE

void do_djikstra(void);
unsigned int get_size_path(void);
unsigned int get_start_node(void);
unsigned int get_end_node(void);
void get_path(int copy_path[10]);


#endif /* LECTURE_H_ */
