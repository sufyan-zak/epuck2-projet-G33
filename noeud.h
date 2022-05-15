/**
 * @file    noeud.h
 * @brief   Constants relative to nodes or to links, structure declaration of a Node
 * 			Prototype function of Dijkstra algorithm, as well as of the functions used in
 * 			the Dijkstra algorithm
 */

#ifndef NOEUD_H_
#define NOEUD_H_

// module headers
#include <main.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/
#define no_link  -1
#define infinite_time  100000000000
#define default_speed 5.0

struct Noeud {
	int uid;

	int tab_liens[5];			// if the number is != -1, it is a link. The number corresponds to the index in the matrix tn
	float tab_liens_dist[5]; 	// corresponds to the distance to the correspondant node in tab_liens

	// variables used in Dijkstra Algorithm
	float access;
	int in;					// Boolean value, either 0 (false) or 1 (true)
	int parent;
};

/*===========================================================================*/
/* Functions declaration.                                                    */
/*===========================================================================*/

/**
 * @brief :		Dijkstra algorithm that calculates the shortest path.
 * 				Starts from start_nodes to iteratively explores the distance
 * 				of each neighbours.
 * 				At the end of the algorithm, every node will have a "parent" node
 * 				(cf. "Noeud" struct) that is the neighbour node from which to go in
 * 				order to follow the shortest path till start_node. The
 * 				"access" variable (in "Noeud" struct) is successively updated and
 * 				represents the time taken in order to arrive to the start_node.
 *
 *
 * @return :	None
 */
void dijkstra(int *queue,struct Noeud *tn[NB_NODES], unsigned int deb);



/**
 * @brief :		queue is a table with the UID of the nodes in an increasing
 * 				order relatively to the "access" time. "init_queue" initializes
 * 				this table at the beginning of the Dijkstra algorithm.
 *
 *
 * @return :	None
 */
void init_queue(int *queue,  struct Noeud *tn[NB_NODES], unsigned int deb);



/**
 * @brief :		sorts the queue table in an increasing order relatively to
 * 				the "access" time.
 *
 *
 * @return :	None
 */
void sort_queue(int *queue,  struct Noeud *tn[NB_NODES]);



/**
 * @brief :		Finds the closest next node to be studied. It is memorized in
 * 				the nd_min variable.
 *
 *
 * @return :	The corresponding node closest node that hasn't still been studied.
 */
unsigned int find_min_access(int *queue, struct Noeud *tn[NB_NODES]);



/**
 * @brief :		Updates the "parent" and "access" value of nodes that are neighbour
 * 				to the the current node being studied (that is tn[nd_min]).
 *
 *
 * @return :	None
 */
void recherche_voisins(int *queue, struct Noeud *tn[NB_NODES], unsigned int nd_min, struct Noeud* nd, unsigned int size_tab_liens);



/**
 * @brief :		Calculates the time it takes to travel through a link. The
 * 				corresponding distance is taken in the "tab_liens_dist" table
 * 				of the node. Time is then obtained by dividing this distance by
 * 				a default_speed.
 *
 *
 * @return :	A float representing the time it takes to travel a link.
 */
float temps_lien(struct Noeud* a, struct Noeud* b);

#endif /* NOEUD_H_ */
