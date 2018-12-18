#include "il.h"

int main(int argc, char *argv[])
{
  int i=0;
  il *ns = NULL;
  printf("=== testing il (integer lists) ===\n");
  il_show(ns);
  for (;i<10;i++) {
    ns = il_cons(i,ns);
    il_show(ns);
  }
  il_free(ns);
  return 0;
}
