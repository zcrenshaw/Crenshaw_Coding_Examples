#include <stdlib.h>
#include <stdio.h>
#include "adrbook.h"

const char *_bst_todo_format = "TODO [bst]: %s\nhalting\n";

bst* bst_singleton(vcard* vc)
{
	bst *t = calloc(1,sizeof(bst));
	t->vc = vc;
	return t; 
}

int bst_insert(bst* t, vcard* vc)
{
	if (t == NULL) {
		fprintf(stderr,"bst_insert: tree empty\n");
		exit(1);
	}
	
	if (strcmp(vc->cnet,t->vc->cnet) > 0) {
		if (t->rsub == NULL) {
			t->rsub = bst_singleton(vc);
			return 1;
		}
		else
			return bst_insert(t->rsub,vc);
	}
	
	if (strcmp(vc->cnet,t->vc->cnet) < 0) {
		if (t->lsub == NULL) {
			t->lsub = bst_singleton(vc);
			return 1;
		}
		else
			return bst_insert(t->lsub,vc);
	}
	else
		return 0;	
}

unsigned int bst_num_entries(bst* t)
{
	if (t == NULL)
		return 0;
	return bst_num_entries(t->lsub) + bst_num_entries(t->rsub) + 1;
}

unsigned int bst_height(bst* t)
{
	if (t == NULL)
		return 0;
	unsigned int hl = bst_height(t->lsub);
	unsigned int hr = bst_height(t->rsub);

	if (hl > hr)
		return hl + 1;
	else
		return hr + 1;
}

vcard* bst_search(bst* t, const char* cnet, int* n_comparisons)
{
	if (t == NULL)
		return NULL;
	int cmp = (strcmp(cnet,t->vc->cnet));
	if (cmp > 0) {
		*n_comparisons+=1;
		return bst_search(t->rsub,cnet,n_comparisons);
	}
	if (cmp < 0) {
		*n_comparisons+=1;
		return bst_search(t->lsub,cnet,n_comparisons);
	}
	else { 
		*n_comparisons+=1;
		return t->vc;
	}
}

unsigned int bst_prefix_show(FILE* f, bst* t, const char* prefix)
{
	if (t == NULL)
		return 0;

	if (precmp(prefix,t->vc->cnet) > 0)
	       return bst_prefix_show(f,t->rsub,prefix);
	if (precmp(prefix,t->vc->cnet) < 0)
		return bst_prefix_show(f,t->lsub,prefix);
	else {
		unsigned int lsub = bst_prefix_show(f,t->lsub,prefix);
		fprintf(f,"%s\n",t->vc->cnet);
		unsigned int rsub = bst_prefix_show(f,t->rsub,prefix);
		return lsub + rsub + 1;	
	}
}	

void bst_free(bst* t)
{
	if (t == NULL) 
		return;
	vcard_free(t->vc);
	bst_free(t->lsub);
	bst_free(t->rsub);
	free(t);
}

/*  --- HELPER FUNCTIONS --- */

//compares only a prefix to the beginning of a string
int precmp(const char *pre, const char *ts)
{
	while (*pre != '\0') {	
		if (*ts == '\0') {
			fprintf(stderr,"prefix is longer than cnet");
			exit(1);
		}
		if (*pre < *ts)
			return -1;
		if (*pre > *ts)
			return 1;
		pre++;
		ts++;
	}
	return 0;
}
