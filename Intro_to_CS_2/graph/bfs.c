#include <stdio.h>
#include <stdlib.h>

#include "iq.h"
#include "graph.h"

il *bfs(graph *g, unsigned int start_key)
{
	iq *q = iq_new();
	il *list = calloc(1,sizeof(il));
	vertex **vs = g->vs;
	enqueue(q,start_key);
	vs[start_key]->visited = 1;
	while (q->front != NULL) {
		int curr = dequeue(q);
		if (!vs[curr]->visited)
			list = il_append(list,il_singleton(curr));
		vs[curr]->visited = 1;
		il *nbrs = graph_neighbors(g,curr);
		while (nbrs != NULL) {
			if (!vs[nbrs->n]->visited)
				enqueue(q,vs[nbrs->n]->key);
			nbrs = nbrs->next;
		}
	}
	iq_free(q);
	return list;
}

int main(int argc, char *argv[])
{
  graph *g;
  if (argc>1)
    g = sample_graph(atoi(argv[1]));
  else
    g = sample_graph(0);
  
  printf("--- graph ---\n");
  graph_show(g);
  putchar('\n');

  printf("--- breadth-first search ---\n");
  il *ns = bfs(g,0);
  graph_show_labels(g,ns);
  putchar('\n');

  il_free(ns);
  graph_free(g);
  
  return 0;
}
