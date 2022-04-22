/*
 * noeud.c
 *
 *  Created on: 21 avr. 2022
 *      Author: admin
 */

#include <main.h>
#include <noeud.h>
#include <math.h>

void init_queue(int *queue,  struct Noeud *tn[size], unsigned int d)
{
	for(int j = 0 ; j < size ; ++j)
	{
		tn[j]->access = infinite_time;
		tn[j]->parent = no_link;
		tn[j]->in = true;
		queue[j] = j;
	}

	// i est l'indice du noeud de départ
	tn[d]->access = 0;
	tn[d]->in = true;
	tn[d]->parent = no_link;
	queue[d] = d;
}

void sort_queue(int *queue,  struct Noeud *tn[size])
{
	float tab_access[size];

	for(unsigned int k =0 ; k < size ; ++k)
	{
		tab_access[k] = tn[queue[k]]->access;
	}

	_Bool swap = 1;
	float tmp = 0;
	float mem = 0;
	for(unsigned int k = 0 ; k < size - 1 && swap; ++k)
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

unsigned int find_min_access(int *queue, struct Noeud *tn[size])
{
	for(unsigned int k = 0 ; k < size ; ++k)
	{
		if(tn[queue[k]]->in)  return queue[k];
	}
}

void djikstra(int *queue, struct Noeud *tn[size], unsigned int deb)
{
	init_queue(queue, tn, deb);
	sort_queue(queue, tn);

	unsigned int nd_min = 0;
	unsigned int cmt_tab = 0;
	while(cmt_tab < size)
	{
		nd_min = find_min_access(queue, tn);

		tn[nd_min]->in = 0;
		cmt_tab++;
		unsigned int size_tab_liens = sizeof(tn[nd_min]->tab_liens)/sizeof(tn[nd_min]->tab_liens[0]);
		recherche_voisins(queue, tn, nd_min, tn[nd_min], size_tab_liens);
	}
}

// update les valeurs des voisins du noeud, s'ils sont plus proches
void recherche_voisins(int *queue, struct Noeud *tn[size], unsigned int nd_min, struct Noeud* nd, unsigned int size_tab_liens)
{
	float alt = 0;

	// to get the real number of links
	for(unsigned int i = 0 ; i < size_tab_liens ; ++i)
	{
		if(nd->tab_liens[i] == 0)
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
	float dist = norme(a->x - b->x, a->y - b->y);

	return dist/default_speed;
}

float norme(float x, float y)
{
	return sqrt(x*x + y*y);
}
