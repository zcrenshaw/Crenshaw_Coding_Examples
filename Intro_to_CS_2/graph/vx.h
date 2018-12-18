#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int key;
  char *label;
  int visited;
} vertex;

vertex *vertex_new(unsigned int key, char *label);

void vertex_free(vertex *v);

void vertex_show(vertex *v);

#endif /* _VERTEX_H_ */
