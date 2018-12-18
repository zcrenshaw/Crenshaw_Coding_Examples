#ifndef _IL_H_
#define _IL_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct il il;
struct il {
  int n;
  il *next;
};

il *il_cons(int n, il *ns);

il *il_snoc(il *ns, int n);

il *il_append(il *ns1, il *ns2);

il *il_singleton(int n);

unsigned int il_len(il *ns);

void il_free(il *ns);

void il_show(il *ns);

#endif /* _IL_H_ */
