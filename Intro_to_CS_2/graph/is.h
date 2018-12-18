#ifndef _I_STACK_H_
#define _I_STACK_H_

#include <stdio.h>
#include <stdlib.h>

#include "il.h"

typedef struct {
  il *ns;
} is;

is *is_new();

void push(is *s, int n);

int pop(is *s);

void is_show(is *s);

void is_free(is *s);

#endif /* _I_STACK_H_ */
