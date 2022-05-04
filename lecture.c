#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <lecture.h>
#include <noeud.h>


static unsigned int size_path = 0;
static unsigned int start_node = 0;
static unsigned int end_node = 6;
static int path[10]= {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void do_djikstra(void) {
	
	struct Noeud nd1 = {.uid=0, .x = 0, .y = 0, .tab_liens={1, no_link}};				// ON MET L'UID DES NOEUDS DANS TAB_LIENS. CET UID CORRESPOND AUSSI A L'INDICE DANS TN
		struct Noeud nd2 = {.uid=1, .x = 30, .y = 0, .tab_liens={0,2,4, no_link}};
		struct Noeud nd3 = {.uid=2, .x = 50, .y = 0, .tab_liens={1,5, no_link} };
		struct Noeud nd4 = {.uid=3, .x = 0, .y = 4, .tab_liens={4,6, no_link} };
		struct Noeud nd5 = {.uid=4, .x = 7, .y = 4, .tab_liens={1,3, 5, no_link} };
		struct Noeud nd6 = {.uid=5, .x = 10, .y = 4, .tab_liens={2,4,no_link} };
		struct Noeud nd7 = {.uid=6, .x = 0, .y = 100, .tab_liens={3, no_link} };


		struct Noeud *tn[7] = {&nd1, &nd2, &nd3, &nd4,&nd5, &nd6, &nd7};


			/*for(int i = 0; i <  7 ; ++i) {
				printf("uid of nd : %d\n", tn[i]->uid);
				printf("x of nd : %d\n", tn[i]->x);
				printf("y of nd : %d\n", tn[i]->y);
				for(int j = 0 ; tn[i]->tab_liens[j] != no_link ; ++j) {
					printf("link of nd : %d\n", tn[i]->tab_liens[j]);
				}
			}*/
		int test[SIZE] = {0};

		dijkstra(test,tn,start_node);
		//waits 1 second
		for(struct Noeud* p = tn[end_node] ; p->parent != no_link ; p = tn[p->parent])
		{
			path[size_path] = p->uid;
			++size_path;
		}
		path[size_path] = tn[start_node]->uid;

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
void get_path(int copy_path[10]){
	for(int i= 0 ; i < 10 ;++i){
		copy_path[i]= path[i];
	}

}
