/**
 * @file    lecture.c
 * @brief   Calculates the shortest path when going and coming back from the END_NODE
 */
//standard C header
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Module header
#include <city_initialisation.h>

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/
static unsigned int size_path = 0;
static int path[MAX_PATH_SIZE]= {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

/*===========================================================================*/
/* Module functions.	                                                     */
/*===========================================================================*/
void do_djikstra(uint8_t going_back) {

		// This table is used in Dijkstra algorithm and is progressively updated. Closest nodes are the first nodes.
		int queue[NB_NODES] = {0};

		if(!going_back){
				dijkstra(queue,node_list,START_NODE);

				for(struct Noeud* p = node_list[END_NODE] ; p->parent != no_link ; p = node_list[p->parent]) {
					path[size_path] = p->uid;

					++size_path;
				}
				path[size_path] = node_list[START_NODE]->uid;
		}else if (going_back){
				dijkstra(queue,node_list,END_NODE);
				size_path =0;
				for(struct Noeud* p = node_list[START_NODE] ; p->parent != no_link ; p = node_list[p->parent]) {
					path[size_path] = p->uid;
					++size_path;
				}
				path[size_path] = node_list[END_NODE]->uid;

		}
}

unsigned int get_size_path(void){
	 return size_path;
}
unsigned int get_START_NODE(void){
	return START_NODE;
}
unsigned int get_END_NODE(void){
	return END_NODE;
}
void get_path(int copy_path[MAX_PATH_SIZE]){
	for(int i= 0 ; i < MAX_PATH_SIZE ;++i){
		copy_path[i]= path[i];
	}
}
