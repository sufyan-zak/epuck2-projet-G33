/*
 * noeud.c
 *
 *  Created on: 21 avr. 2022
 *      Author: admin
 */



#include <math.h>
#include <noeud.h>


void init_queue(int *queue,  struct Noeud *tn[SIZE], unsigned int d)
{
	for(int j = 0 ; j < SIZE ; ++j)
	{
		tn[j]->access = infinite_time;
		tn[j]->parent = no_link;
		tn[j]->in = 1;
		queue[j] = j;
	}

	// i est l'indice du noeud de départ
	tn[d]->access = 0;
	tn[d]->in = 1;
	tn[d]->parent = no_link;
	queue[d] = d;
}

void sort_queue(int *queue,  struct Noeud *tn[SIZE])
{
	float tab_access[SIZE];

	for(unsigned int k =0 ; k < SIZE ; ++k)
	{
		tab_access[k] = tn[queue[k]]->access;
	}

	_Bool swap = 1;
	float tmp = 0;
	float mem = 0;
	for(unsigned int k = 0 ; k < SIZE - 1 && swap; ++k)
	{
		if(tab_access[k+1] < tab_access[k])
		{
			while(tab_access[k+1] < tab_access[k])
			{
				tmp = tab_access[k+1];
				mem = queue[k+1];

				tab_access[k+1] = tab_access[k];
				queue[k+1] = queue[k];

				tab_access[k] = tmp;
				queue[k] = mem;
				if(k != 0 )  --k;
			}
			//swap = 0;				Pourquoi y avait ça ?! L'algo marche bien si on enlève cette ligne. De l'opti ? Surement.
		}
	}
}

unsigned int find_min_access(int *queue, struct Noeud *tn[SIZE])
{
	for(unsigned int k = 0 ; k < SIZE ; ++k)
	{
		if(tn[queue[k]]->in)  return queue[k];
	}
	return 0;
}

void dijkstra(int *queue, struct Noeud *tn[SIZE], unsigned int deb)
{
	init_queue(queue, tn, deb);
	sort_queue(queue, tn);

	unsigned int nd_min = 0;
	unsigned int cmt_tab = 0;
	while(cmt_tab < SIZE)
	{
		nd_min = find_min_access(queue, tn);

		tn[nd_min]->in = 0;
		cmt_tab++;
		unsigned int size_tab_liens = sizeof(tn[nd_min]->tab_liens)/sizeof(tn[nd_min]->tab_liens[0]);
		recherche_voisins(queue, tn, nd_min, tn[nd_min], size_tab_liens);
	}

}

// update les valeurs des voisins du noeud, s'ils sont plus proches
void recherche_voisins(int *queue, struct Noeud *tn[SIZE], unsigned int nd_min, struct Noeud* nd, unsigned int size_tab_liens)
{
	float alt = 0;

	// to get the real number of links
	for(unsigned int i = 0 ; i < size_tab_liens ; ++i)
	{
		if(nd->tab_liens[i] == no_link)
		{
			size_tab_liens = i;
			break;
		}
	}
	for(unsigned int i = 0 ; i < size_tab_liens ; ++i)
	{
		if(tn[nd->tab_liens[i]]->in)
		{
			alt = nd->access + temps_lien(nd, tn[nd->tab_liens[i]]);
			if(tn[nd->tab_liens[i]]->access > alt)
			{
				tn[nd->tab_liens[i]]->access = alt;
				tn[nd->tab_liens[i]]->parent = nd_min;
				sort_queue(queue, tn);
			}
		}
	}

}

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
