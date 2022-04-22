/*
 * noeud.h
 *
 *  Created on: 21 avr. 2022
 *      Author: admin
 */

#ifndef NOEUD_H_
#define NOEUD_H_

const int no_link = -1;
const float infinite_time = 1e10;
const unsigned int size = 40;
const int default_speed = 5.0;

struct Noeud {
	int tab_liens[20];		// if the number is != 0, it is a link. The number corresponds to the index in the matrix tn
	float access;
	int in;					// Boolean value, either 0 (false) or 1 (true)
	unsigned int parent;

	// spatial position
	int x;
	int y;
};

void dijkstra(int *queue,  struct Noeud *tn[size], unsigned int deb);
void init_queue(int *queue,  struct Noeud *tn[size], unsigned int deb);
void sort_queue(int *queue,  struct Noeud *tn[size]);
unsigned int find_min_access(int *queue, struct Noeud *tn[size]);
void recherche_voisins(int *queue, struct Noeud *tn[size], unsigned int nd_min, struct Noeud* nd, unsigned int size_tab_liens);
float temps_lien(struct Noeud* a, struct Noeud* b);
float norme(float x, float y);


#endif /* NOEUD_H_ */
