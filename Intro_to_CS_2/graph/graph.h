#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include "vx.h"
#include "il.h"

typedef struct {
  unsigned int n_vertices; /* number of vertices */
  vertex **vs;
  il     **es;
} graph;

graph *graph_new(unsigned int n_vertices, char **labels);

int graph_contains(graph *g, unsigned int key);

/* add edge to graph */
/* no check against self-loops or redundant edges */
void graph_e(graph *g, unsigned int src_key, unsigned int dst_key);

il *graph_neighbors(graph *g, unsigned int key);

void graph_free(graph *g);

void graph_show(graph *g);

void graph_show_labels(graph *g, il *keys);

graph *sample_graph(unsigned int selection);

#endif /* _GRAPH_H_ */
