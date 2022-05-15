/**
 * @file	city_initialisation.h
 *
 * @brief	Initializes the city, the user enters the nodes, the link table and  the link distance table
 *  		for every node so that we can calculate the shortest path to the end node
 */

#ifndef CITY_INITIALISATION_H_
#define CITY_INITIALISATION_H_

// module headers
#include <noeud.h>
#include <main.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/
#define START_NODE 0
#define END_NODE 6


// Initialization of the city
struct Noeud nd1 = {.uid=0, .tab_liens={1,4,  no_link}, .tab_liens_dist = {35.52,32.57, no_link}};
struct Noeud nd2 = {.uid=1, .tab_liens={0,2, 5, no_link}, .tab_liens_dist = {35.52, 17.85, 35.71, no_link}};
struct Noeud nd3 = {.uid=2, .tab_liens={1,3, 6, no_link}, .tab_liens_dist = {17.85,15.61, 40.48, no_link}};
struct Noeud nd4 = {.uid=3, .tab_liens={2,7, no_link}, .tab_liens_dist = {15.61, 15.69, no_link} };
struct Noeud nd5 = {.uid=4, .tab_liens={0,5,8,no_link}, .tab_liens_dist = {32.57,62.87,8.77,no_link} };
struct Noeud nd6 = {.uid=5, .tab_liens={1, 4, 6, 9, no_link}, .tab_liens_dist = {35.71, 62.87, 23.93,8.73, no_link} };
struct Noeud nd7 = {.uid=6, .tab_liens={2,  5, no_link}, .tab_liens_dist = {40.48, 23.93, no_link} };
struct Noeud nd8 = {.uid=7, .tab_liens={3, 11, no_link}, .tab_liens_dist = { 15.69,14.32, no_link} };
struct Noeud nd9 = {.uid=8, .tab_liens={4, 9,12, no_link}, .tab_liens_dist = {8.77, 47.8, 10, no_link} };
struct Noeud nd10 = {.uid=9, .tab_liens={5,8,no_link}, .tab_liens_dist = {8.73,47.8, no_link} };
struct Noeud nd11 = {.uid=10, .tab_liens={11, no_link}, .tab_liens_dist = {13.28, no_link} };
struct Noeud nd12 = {.uid=11, .tab_liens={7,  10, 15,no_link}, .tab_liens_dist = {14.32, 13.28, 10.93 no_link} };
struct Noeud nd13 = {.uid=12, .tab_liens={ 8, 13,no_link}, .tab_liens_dist = {10,14.58, no_link} };
struct Noeud nd14 = {.uid=13, .tab_liens={ 12, 14, no_link}, .tab_liens_dist = {14.58, 52.42, no_link} };
struct Noeud nd15 = {.uid=14, .tab_liens={13,15, no_link}, .tab_liens_dist = { 52.42,15.2,no_link} };
struct Noeud nd16 = {.uid=15, .tab_liens={11, 14, no_link}, .tab_liens_dist = {10.93,15.2,  no_link} };


struct Noeud *node_list[16] = {&nd1, &nd2, &nd3, &nd4,&nd5, &nd6, &nd7,&nd8,
								&nd9,&nd10,&nd11,&nd12, &nd13,&nd14,&nd15,&nd16};


#endif /* CITY_INITIALISATION_H_ */
