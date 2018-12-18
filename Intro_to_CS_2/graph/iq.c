#include "iq.h"

iq *iq_new()
{
  iq *q = (iq*)malloc(sizeof(iq));
  if (q==NULL) {
    fprintf(stderr,"iq_new: malloc failed\n");
    exit(1);
  }
  q->n = 0;
  q->front = q->back = NULL;
  return q;
}

void enqueue(iq *q, int n)
{
	il *new_item = il_singleton(n);
	if (q->front == NULL) {
		q->front = new_item;
		q->back = new_item;
	} else {
		q->back->next = new_item;
		q->back = new_item;
	}
	q->n++;
}

int dequeue(iq *q)
{
	if (q == NULL || q->front == NULL) {
		fprintf(stderr,"dequeue: dequeueing non-existent item");
		exit(1);
	}
	int n = q->front->n;
	il *tmp = q->front;
	q->front = q->front->next;
	free(tmp);
	q->n--;
	return n;	
}

void iq_show(iq *q)
{
  printf("begin queue (%d items, front shown at left))\n",q->n);
  printf("  ");
  il_show(q->front);
  printf("end queue\n");
}

void iq_free(iq *q)
{
  if (q!=NULL) {
    il_free(q->front);
    free(q);
  }
}
