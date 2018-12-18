#include "is.h"

int main(int argc, char *argv[])
{
  printf("=== testing is (integer stacks) ===\n");
  int i;
  is *s = is_new();
  is_show(s);
  putchar('\n');
  for (i=2;i<7;i++) {
    push(s,i);
    printf("pushed %d\n",i);
    is_show(s);
    putchar('\n');
  }
  while (s->ns!=NULL) {
    int p = pop(s);
    printf("popped %d\n",p);
    is_show(s);
    putchar('\n');
  }
  free(s);
  return 0;
}
