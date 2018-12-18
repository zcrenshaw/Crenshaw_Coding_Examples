#include "graph.h"

int main(int argc, char *argv[]) 
{
  printf("=== testing graphs ===\n");
  char *labels[] = {"a","b","c","d","e"};
  graph *g = graph_new(5,labels);
  graph_show(g);
  graph_free(g);
  return 0;
}

