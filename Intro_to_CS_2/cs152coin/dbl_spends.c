#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dbl_spends.h"


dbl_spends_t* dbl_spends_new()
{
	dbl_spends_t *new = calloc(1,sizeof(dbl_spends_t));
	allocation_check(new,"dbl_spends_new");
	return new;
}

void dbl_spends_free(dbl_spends_t* ds)
{
	if (ds == NULL)
		return;
	while (ds->first != NULL) {
		spend_t *tmp = ds->first;
		ds->first = ds->first->next;
		free(tmp);
	}
	free(ds);
}

void add_dbl_spend(const char* trans_id, const char* addr, dbl_spends_t* ds)
{
	if (ds->first == NULL) {
		spend_t *new = spend_new(trans_id,addr);
		ds->first = new;
		ds->last = new;
	} else {
		spend_t *new = spend_new(trans_id,addr);
		ds->first->prev = new;
		new->next = ds->last;
		ds->first = new;
	}	
}

void dbl_spends_show(dbl_spends_t* ds, FILE* f)
{
	while (ds->last != NULL) 
		fprintf(stdout,"<%s>, <%s>",ds->last->id,ds->last->addr);
}

// =======================================================================
// Add any other functions you need in order to implement dbl_spends below
// =======================================================================

spend_t *spend_new(const char *id, const char *addr)
{
	spend_t *new = calloc(1,sizeof(spend_t));
	new->addr = strdup(addr);
	new->id = strdup(id);
	return new;
}

void allocation_check(void *item, char *fxn_name)
{
	if (item == NULL) {
		fprintf(stderr,"%s: memory allocation failed\n",fxn_name);
		exit(1);
	}
}

