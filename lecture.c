#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <lecture.h>
#include <noeud.h>


static unsigned int size_path = 0;
static unsigned int start_node = 0;
static unsigned int end_node = 15;
static int path[10]= {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void do_djikstra(void) {
	
	     struct Noeud nd1 = {.uid=0, .tab_liens={1, 5, no_link}, .tab_liens_dist = {40.4, 40.4, no_link}};				// ON MET L'UID DES NOEUDS DANS TAB_LIENS. CET UID CORRESPOND AUSSI A L'INDICE DANS TN
	     struct Noeud nd2 = {.uid=1, .tab_liens={0,2,6, no_link}, .tab_liens_dist = {40.4, 9, 58, no_link}};
	     struct Noeud nd3 = {.uid=2, .tab_liens={1,3, 7, no_link}, .tab_liens_dist = {9, 10.4, 37.28, no_link}};
	     struct Noeud nd4 = {.uid=3,  .tab_liens={2,8, no_link}, .tab_liens_dist = {10.4, 15.5, no_link} };
	     struct Noeud nd5 = {.uid=4,  .tab_liens={no_link}, .tab_liens_dist = {no_link} };
	     struct Noeud nd6 = {.uid=5, .tab_liens={0, 6, 10,no_link}, .tab_liens_dist = {40.4, 69.56, 7, no_link} };
	     struct Noeud nd7 = {.uid=6, .tab_liens={1, 5, 7, 11, no_link}, .tab_liens_dist = {58, 69.56, 6.5, 8.5, no_link} };
	     struct Noeud nd8 = {.uid=7, .tab_liens={2, 6, no_link}, .tab_liens_dist = {37.28, 6.5, no_link} };
	     struct Noeud nd9 = {.uid=8, .tab_liens={3, 13, no_link}, .tab_liens_dist = {15.5, 5.3, no_link} };
	     struct Noeud nd10 = {.uid=9, .tab_liens={no_link}, .tab_liens_dist = {no_link} };
	     struct Noeud nd11 = {.uid=10, .tab_liens={5, 11, 15, no_link}, .tab_liens_dist = {7, 48.4, 7, no_link} };
	     struct Noeud nd12 = {.uid=11,  .tab_liens={6, 10, 16, no_link}, .tab_liens_dist = {8.5, 48.4, 6.9, no_link} };
	     struct Noeud nd13 = {.uid=12, .tab_liens={ no_link}, .tab_liens_dist = {no_link} };
	     struct Noeud nd14 = {.uid=13, .tab_liens={8, 14, 18, no_link}, .tab_liens_dist = {24.3, 8.1, 5.3, no_link} };
	     struct Noeud nd15 = {.uid=14, .tab_liens={13, no_link}, .tab_liens_dist = {8.1, no_link} };
	     struct Noeud nd16 = {.uid=15, .tab_liens={10, 16, no_link}, .tab_liens_dist = {7, 41.2, no_link} };
	     struct Noeud nd17 = {.uid=16, .tab_liens={11, 15, 17, no_link}, .tab_liens_dist = {6.9, 41.2, 7, no_link} };
	     struct Noeud nd18 = {.uid=17, .tab_liens={16, 18, no_link}, .tab_liens_dist = {7, 7.1, no_link}};
	     struct Noeud nd19 = {.uid=18, .tab_liens={13, 17, no_link}, .tab_liens_dist = {5.3, 7.1, no_link} };
	     struct Noeud nd20 = {.uid=19, .tab_liens={ no_link}, .tab_liens_dist = {no_link} };
	     struct Noeud nd21 = {.uid=20, .tab_liens={ no_link}, .tab_liens_dist = {no_link} };
	     struct Noeud nd22 = {.uid=21, .tab_liens={ no_link}, .tab_liens_dist = {no_link} };
	     struct Noeud nd23 = {.uid=22, .tab_liens={ no_link}, .tab_liens_dist = {no_link} };
	     struct Noeud nd24 = {.uid=23, .tab_liens={ no_link}, .tab_liens_dist = {no_link} };
	     struct Noeud nd25 = {.uid=24, .tab_liens={ no_link}, .tab_liens_dist = {no_link} };



		 struct Noeud *tn[25] = {&nd1, &nd2, &nd3, &nd4,&nd5, &nd6, &nd7,&nd8,&nd9,&nd10,&nd11,&nd12,&nd13,&nd14,&nd15,&nd16,&nd17,&nd18,&nd19,&nd20,&nd21,&nd22,&nd23,&nd24,&nd25};


			/*for(int i = 0; i <  7 ; ++i) {
				printf("uid of nd : %d\n", tn[i]->uid);
				printf("x of nd : %d\n", tn[i]->x);
				printf("y of nd : %d\n", tn[i]->y);
				for(int j = 0 ; tn[i]->tab_liens[j] != no_link ; ++j) {
					printf("link of nd : %d\n", tn[i]->tab_liens[j]);
				}
			}*/
		int queue[SIZE] = {0};

		dijkstra(queue,tn,start_node);
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
