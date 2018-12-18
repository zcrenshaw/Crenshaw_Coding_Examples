#ifndef _IQ_H_
#define _IQ_H_

#include <stdlib.h>
#include <stdio.h>
#include "il.h"

struct iq {
  unsigned int n;
  il *front;
  il *back;
};
typedef struct iq iq;

iq *iq_new();

void enqueue(iq *q, int n);

int dequeue(iq *q);

void iq_show(iq *q);

void iq_free(iq *q);

#endif /* _IQ_H_ */
