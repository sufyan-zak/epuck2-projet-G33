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
	     struct Noeud nd8 = {.uid=7, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd9 = {.uid=8, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd10 = {.uid=9, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd11 = {.uid=10, .x = 0, .y = 100, .tab_liens={no_link} };
	     struct Noeud nd12 = {.uid=11, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd13 = {.uid=12, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd14 = {.uid=13, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd15 = {.uid=14, .x = 0, .y = 100, .tab_liens={no_link} };
	     struct Noeud nd16 = {.uid=15, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd17 = {.uid=16, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd18 = {.uid=17, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd19 = {.uid=18, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd20 = {.uid=19, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd21 = {.uid=20, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd22 = {.uid=21, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd23 = {.uid=22, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd24 = {.uid=23, .x = 0, .y = 100, .tab_liens={ no_link} };
	     struct Noeud nd25 = {.uid=24, .x = 0, .y = 100, .tab_liens={ no_link} };



		 struct Noeud *tn[25] = {&nd1, &nd2, &nd3, &nd4,&nd5, &nd6, &nd7,&nd8,&nd9,&nd10,&nd11,&nd12,&nd13,&nd14,&nd15,&nd16,&nd17,&nd18,&nd19,&nd20,&nd21,&nd22,&nd23,&nd24,&nd25};


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
