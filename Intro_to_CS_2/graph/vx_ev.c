#include "vx.h"

int main(int argc, char *argv[])
{
  printf("=== testing vertex ===\n");
  vertex *v = vertex_new(0,"v");
  vertex_show(v);
  putchar('\n');
  vertex_free(v);
  return 0;
}
