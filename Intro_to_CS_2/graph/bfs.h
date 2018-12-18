#ifndef _BFS_H_
#define _BFS_H_

#include "graph.h"

/* bfs is breadth-first search. */
/* Traverse the graph from the start key using an auxiliary queue data structure. */
/* Build a list of the vertex keys in order of visiting them. */
il *bfs(graph *g, unsigned int start_key);

#endif /* _BFS_H_ */
