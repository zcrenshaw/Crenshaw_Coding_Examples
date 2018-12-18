#include "iq.h"

int main(int argc, char *argv[])
{
  printf("=== testing iq (integer queues) ===\n");

  int i;
  iq *q = iq_new();
  iq_show(q);
  putchar('\n');
  for (i=2;i<7;i++) {
    enqueue(q,i);
    printf("enqueued %d\n",i);
    iq_show(q);
    putchar('\n');
  }
  while (q->n>0) {
    int d = dequeue(q);
    printf("dequeued %d\n",d);
    iq_show(q);
    putchar('\n');
  }
  free(q);
  return 0;
}
