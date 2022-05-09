/*
 * noeud.h
 *
 *  Created on: 21 avr. 2022
 *      Author: admin
 */

#ifndef NOEUD_H_
#define NOEUD_H_

#define no_link  -1
#define infinite_time  100000000000
#define SIZE 25
#define default_speed 5.0

struct Noeud {
	int tab_liens[5];		// if the number is != 0, it is a link. The number corresponds to the index in the matrix tn
	float access;
	int in;					// Boolean value, either 0 (false) or 1 (true)
	unsigned int parent;
	int uid;
	// spatial position
	int x;
	int y;
};

void dijkstra(int *queue,struct Noeud *tn[SIZE], unsigned int deb);
void init_queue(int *queue,  struct Noeud *tn[SIZE], unsigned int deb);
void sort_queue(int *queue,  struct Noeud *tn[SIZE]);
unsigned int find_min_access(int *queue, struct Noeud *tn[SIZE]);
void recherche_voisins(int *queue, struct Noeud *tn[SIZE], unsigned int nd_min, struct Noeud* nd, unsigned int size_tab_liens);
float temps_lien(struct Noeud* a, struct Noeud* b);
float norme(float x, float y);


#endif /* NOEUD_H_ */
