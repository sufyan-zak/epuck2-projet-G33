/**
 * @file    noeud.c
 * @brief   Implementation of Dijkstra algorithm, as well as of subfunctions
 * 			used in it.
 */

// module headers
#include <math.h>
#include <noeud.h>

/*===========================================================================*/
/* Module functions.	                                                     */
/*===========================================================================*/
void init_queue(int *queue,  struct Noeud *tn[NB_NODES], unsigned int d)
{
	for(int j = 0 ; j < NB_NODES ; ++j) {
		tn[j]->access = infinite_time;
		tn[j]->parent = no_link;
		tn[j]->in = 1;
		queue[j] = j;
	}

	//d is the index of the start node
	tn[d]->access = 0;
	tn[d]->in = 1;
	tn[d]->parent = no_link;
	queue[d] = d;
}

void sort_queue(int *queue,  struct Noeud *tn[NB_NODES])
{
	float tab_access[NB_NODES];

	for(unsigned int k =0 ; k < NB_NODES ; ++k) {
		tab_access[k] = tn[queue[k]]->access;
	}

	_Bool swap = 1;
	float tmp = 0;
	float mem = 0;
	for(unsigned int k = 0 ; k < NB_NODES - 1 && swap; ++k) {
		if(tab_access[k+1] < tab_access[k]) {
			while(tab_access[k+1] < tab_access[k]) {
				tmp = tab_access[k+1];
				mem = queue[k+1];

				tab_access[k+1] = tab_access[k];
				queue[k+1] = queue[k];

				tab_access[k] = tmp;
				queue[k] = mem;
				if(k != 0 )  --k;
			}
			swap = 0;
		}
	}
}

unsigned int find_min_access(int *queue, struct Noeud *tn[NB_NODES])
{
	for(unsigned int k = 0 ; k < NB_NODES ; ++k) {
		if(tn[queue[k]]->in)  return queue[k];
	}
	return 0;
}

void dijkstra(int *queue, struct Noeud *tn[NB_NODES], unsigned int deb)
{
	init_queue(queue, tn, deb);
	sort_queue(queue, tn);

	unsigned int nd_min = 0;
	unsigned int cmt_tab = 0;
	while(cmt_tab < NB_NODES) {
		nd_min = find_min_access(queue, tn);

		tn[nd_min]->in = 0;
		cmt_tab++;
		unsigned int size_tab_liens = sizeof(tn[nd_min]->tab_liens)/sizeof(tn[nd_min]->tab_liens[0]);
		recherche_voisins(queue, tn, nd_min, tn[nd_min], size_tab_liens);
	}

}

//updates the values of neighbor nodes if they are closer
void recherche_voisins(int *queue, struct Noeud *tn[NB_NODES], unsigned int nd_min, struct Noeud* nd, unsigned int size_tab_liens)
{
	float alt = 0;

	//to get the real number of links
	for(unsigned int i = 0 ; i < size_tab_liens ; ++i) {
		if(nd->tab_liens[i] == no_link)
		{
			size_tab_liens = i;
			break;
		}
	}
	for(unsigned int i = 0 ; i < size_tab_liens ; ++i) {
		if(tn[nd->tab_liens[i]]->in)
		{
			alt = nd->access + temps_lien(nd, tn[nd->tab_liens[i]]);
			if(tn[nd->tab_liens[i]]->access > alt) {
				tn[nd->tab_liens[i]]->access = alt;
				tn[nd->tab_liens[i]]->parent = nd_min;
				sort_queue(queue, tn);
			}
		}
	}

}

//calculates the time it takes to cross a link
float temps_lien(struct Noeud* a, struct Noeud* b)
{
	_Bool stop = 0;
	int i = 0;
	float dist = 0;

	while(!stop && i < 5) {
		if(a->tab_liens[i] == b->uid) {
			stop = 1;
			dist = a->tab_liens_dist[i];
		}
		++i;
	}

	return dist/default_speed;
}
