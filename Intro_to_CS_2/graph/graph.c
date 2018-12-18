#include "graph.h"

graph *graph_new(unsigned int n_vertices, char **labels)
{
  int i;
  graph *g = (graph*)malloc(sizeof(graph));
  g->n_vertices = n_vertices;
  g->vs = (vertex**)malloc(n_vertices*sizeof(vertex*));
  g->es = (il**)malloc(n_vertices*sizeof(il*));
  for (i=0; i<n_vertices; i++) {
    g->vs[i] = vertex_new(i,labels[i]);
    g->es[i] = NULL;
  }
  return g;
}

int graph_contains(graph *g, unsigned int k)
{
  /* This assumes the graph is well-formed. */
  return k<g->n_vertices;
}

void graph_e(graph *g, unsigned int src_key, unsigned int dst_key)
{
  if (!graph_contains(g,src_key)) {
    fprintf(stderr,"graph_e: no such source vertex %u\n", src_key);
    exit(1);
  }
  if (!graph_contains(g,dst_key)) {
    fprintf(stderr,"graph_e: no such dest vertex %u\n", dst_key);
    exit(1);
  }      
  g->es[src_key] = il_cons(dst_key,g->es[src_key]);    
}

il *graph_neighbors(graph *g, unsigned int key)
{
  return g->es[key];
}

void graph_free(graph *g)
{
  if (g==NULL) return;
  for (int i=0; i<g->n_vertices; i++) {
    vertex_free(g->vs[i]);
    il_free(g->es[i]);
  }
  free(g->vs);
  free(g->es);
  free(g);
}

void graph_show(graph *g)
{
  int i;
  printf("begin graph\n");
  for (i=0; i<g->n_vertices; i++) {
    putchar(' ');
    vertex_show(g->vs[i]);
    putchar(' ');
    il_show(g->es[i]);
  }
  printf("end graph\n");
}

void graph_show_labels(graph *g, il *keys)
{
  putchar('[');
  while (keys) {
    int k = keys->n;
    printf("%d:\"%s\"",k,g->vs[k]->label);
    if (keys->next)
      putchar(',');
    keys=keys->next;
  }
  putchar(']');
  putchar('\n');
}

/* === sample graphs === */

graph *sample_graph_0()
{
  /* a graph with one vertex and no edges */
  char **labels = (char**)malloc(sizeof(char*));
  labels[0] = "x";
  graph *g = graph_new(1,labels);
  free(labels);
  return g;
}

graph *sample_graph_1() 
{
  graph *g;
  char **labels = (char**)malloc(sizeof(char*)*5);
  labels[0] = "A";
  labels[1] = "B";
  labels[2] = "C";
  labels[3] = "X";
  labels[4] = "Y";
  g = graph_new(5,labels);
  graph_e(g,0,1);
  graph_e(g,0,2);
  graph_e(g,1,3);
  graph_e(g,3,4);
  free(labels);
  return g;
}

graph *sample_graph_2() 
{
  graph *g;
  char **labels = (char**)malloc(sizeof(char*)*5);
  labels[0] = "A";
  labels[1] = "B";
  labels[2] = "C";
  labels[3] = "D";
  labels[4] = "E";
  g = graph_new(5,labels);
  graph_e(g,0,1);
  graph_e(g,1,2);
  graph_e(g,0,3);
  graph_e(g,3,0);
  graph_e(g,3,4);
  free(labels);
  return g;
}

graph *sample_graph(unsigned int selection)
{
  switch (selection) {
  case 1  : return sample_graph_1();
  case 2  : return sample_graph_2();
  case 0  : 
  default : return sample_graph_0();
  }
}
