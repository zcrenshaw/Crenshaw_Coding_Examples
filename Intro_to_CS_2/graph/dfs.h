#ifndef _DFS_H_
#define _DFS_H_

#include "graph.h"

/* dfs is depth-first search. */
/* Traverse the graph from the start key using an auxiliary stack data structure. */
/* Build a list of the vertex keys in order of visiting them. */
il *dfs(graph *g, unsigned int start_key);

#endif /* _DFS_H_ */
