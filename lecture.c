/**
 * @file    lecture.c
 * @brief   Calculates the shortest path when going and coming back from the end_node
 */
#include "ch.h"
#include "hal.h"
#include <usbcfg.h>
#include <chprintf.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <lecture.h>
#include <noeud.h>
#include <city_initialisation.h>
static unsigned int size_path = 0;
static unsigned int start_node = 0;
static unsigned int end_node =14;
static int path[MAX_PATH_SIZE]= {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void do_djikstra(uint8_t going_back) {

		// This table is used in Dijkstra algorithm and is progressively updated. Closest nodes are the first nodes.
		int queue[NB_NODES] = {0};

		if(!going_back){
				dijkstra(queue,node_list,start_node);

				for(struct Noeud* p = node_list[end_node] ; p->parent != no_link ; p = node_list[p->parent])
				{
					path[size_path] = p->uid;

					++size_path;
				}
				path[size_path] = node_list[start_node]->uid;
		}else if (going_back){
				dijkstra(queue,node_list,end_node);
				size_path =0;
				for(struct Noeud* p = node_list[start_node] ; p->parent != no_link ; p = node_list[p->parent])
				{
					path[size_path] = p->uid;
					++size_path;
				}
				path[size_path] = node_list[end_node]->uid;
				end_node = 0;

		}


}

unsigned int get_size_path(void){
	 return size_path;
}
unsigned int get_start_node(void){
	return start_node;
}
unsigned int get_end_node(void){
	return end_node;
}
void get_path(int copy_path[MAX_PATH_SIZE]){
	for(int i= 0 ; i < MAX_PATH_SIZE ;++i){
		copy_path[i]= path[i];
	}
}
