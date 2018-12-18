#include "is.h"

is *is_new() 
{
  is *s = (is*)malloc(sizeof(is));
  if (s==NULL) {
    fprintf(stderr,"is_new: malloc failed\n");
    exit(1);
  }
  s->ns = NULL;
  return s;
}

void push(is *s, int n)
{
	s->ns = il_cons(n,s->ns);
}

int pop(is *s)
{
	if (s->ns == NULL) {
		fprintf(stderr,"pop: popping non-existent item");
		exit(1);
	}
	int n = s->ns->n;
	il *tmp = s->ns;
	s->ns = s->ns->next;
	free(tmp);
	return n;
}

void is_show(is *s)
{  
  printf("begin stack (%d items, top shown at left)\n",il_len(s->ns));
  printf("  ");
  il_show(s->ns);
  printf("end stack\n");
}

void is_free(is*s)
{
  if (s!=NULL) {
    il_free(s->ns);
    free(s);
  }
}
