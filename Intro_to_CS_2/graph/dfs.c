#include <stdio.h>
#include <stdlib.h>

#include "is.h"
#include "graph.h"

il *dfs(graph *g, unsigned int start_key)
{
	is *s = is_new();
	il *list = calloc(1,sizeof(il));
	vertex **vs = g->vs;
	push(s,start_key);
	vs[start_key]->visited = 1;
	while (s->ns != NULL) {
		int curr = pop(s);
		if (!vs[curr]->visited)
			il_append(list,il_singleton(curr));
		vs[curr]->visited = 1;
		il *nbrs = graph_neighbors(g,curr);
		while (nbrs != NULL) {
			if (!vs[nbrs->n]->visited)
				push(s,vs[nbrs->n]->key);
			nbrs = nbrs->next;
		}
	}
	is_free(s);
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

  printf("--- depth-first search ---\n");
  il *ns = dfs(g,0);
  graph_show_labels(g,ns);
  putchar('\n');

  il_free(ns);
  graph_free(g);

  return 0;
}
