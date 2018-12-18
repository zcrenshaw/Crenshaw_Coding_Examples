#include "il.h"

il *il_cons(int n, il *ns)
{
  il *l = (il*)malloc(sizeof(il));
  if (l==NULL) {
    fprintf(stderr,"il_cons: malloc failed\n");
    exit(1);
  }
  l->n = n;
  l->next = ns;
  return l;
}

il *il_snoc(il *ns, int n)
{
  if (ns==NULL)
    return il_singleton(n);
  il *curr = ns;
  while (curr->next)
    curr=curr->next;
  curr->next = il_singleton(n);
  return ns;
}

il *il_singleton(int n)
{
  return il_cons(n,NULL);
}

il *il_append(il *ns1, il *ns2)
{
  if (ns1==NULL)
    return ns2;
  else {
    ns1->next = il_append(ns1->next,ns2);
    return ns1;
  }
}

unsigned int il_len(il *ns)
{
  unsigned int n = 0;
  while (ns) {
    n++;
    ns=ns->next;
  }
  return n;
}

void il_free(il *ns)
{
  if (ns) {
    il_free(ns->next);
    free(ns);
  }
}

void il_show(il *ns)
{
  putchar('[');
  while (ns) {
    printf(ns->next?"%d,":"%d",ns->n);
    ns=ns->next;
  }
  putchar(']');
  putchar('\n');
}
