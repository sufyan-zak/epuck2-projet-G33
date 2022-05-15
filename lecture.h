/**
 * @file    lecture.h
 * @brief   Creates the city and calculates the shortest path
 */

#ifndef LECTURE_H_
#define LECTURE_H_




/**
 * @brief :		Creates the city by initializing every node and its links with
 * 				other nodes.
 * 				The Dijkstra algorithm is then directly called
 * 				from a start_node.
 *				Considering an end_node, the shortest path (i.e the corresponding UID
 *				nodes) is memorized in a table.
 *
 *
 * @return :	None
 */
void do_djikstra(uint8_t going_back);



/**
 * @brief :		Returns the number of nodes in the shortest path
 *
 *
 * @return :	Number of nodes in the shortest path (unsigned int)
 */
unsigned int get_size_path(void);



/**
 * @brief :		Copies the table with the shortest path nodes in
 * 				the table given in parameter.
 *
 *
 * @return :	None
 */
void get_path(int copy_path[10]);


#endif /* LECTURE_H_ */
