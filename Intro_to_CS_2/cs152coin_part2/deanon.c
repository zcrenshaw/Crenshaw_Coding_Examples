#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "deanon.h"
#include "cs152coin.h"
#include "hmap.h"

deanon_t* deanon_new()
{
    
	deanon_t *d = calloc(1,sizeof(deanon_t));
	if (d == NULL) {
		fprintf(stderr,"deanon_new: calloc failed");
		exit(1);
	}
	d->ht = hmap_new();

	return d;
}

void value_free(const char* key, void *c)
{
	cluster_free(c);
}

void deanon_free(deanon_t* d)
{
	hmap_foreach(value_free,d->ht);
	hmap_free(d->ht);
	free(d);
}

void add_address(const char* addr, deanon_t* d)
{
	hmap_put(addr,cluster_new(addr),d->ht);

}

void add_associations(dep_wd_t* trans_inputs[], unsigned int num_inputs, deanon_t* d)
{
	for (int i = 0; i<num_inputs; i++)
		add_address(trans_inputs[i]->addr,d);
	
	cluster_t *combined = hmap_get(trans_inputs[0]->addr,d->ht);

	for (int i = 1; i<num_inputs; i++) {
		cluster_t *curr = hmap_get(trans_inputs[i]->addr,d->ht);
		cluster_t *tmp = curr;
		while (tmp != NULL) {
			if (!cluster_contains(combined,tmp)) { 

				cluster_snoc(tmp->addr,combined);
			}
			tmp = tmp->next;
		}
	}
	for (cluster_t *tmp_c = combined; tmp_c!= NULL; tmp_c = tmp_c->next) {
		cluster_t *val = hmap_get(tmp_c->addr,d->ht);
		val = combined;
	}
}


cluster_t* get_cluster(const char* addr, deanon_t* d)
{
	return hmap_get(addr, d->ht);
    
}

cluster_t* cluster_new(const char* addr)
{
	cluster_t *c = calloc(1,sizeof(cluster_t));
	if (c == NULL) {
		fprintf(stderr,"cluster_new: calloc failed");
		exit(1);
	}

	c->addr = strdup(addr);
	c->next = NULL;

	return c;
}


void cluster_free(cluster_t* c)
{
	while (c != NULL) {
		cluster_t *tmp = c;
		c = c->next;
		free(tmp->addr);
		free(tmp);
	}
}


// ===================================================================
// Add any other functions you need in order to implement deanon below
// ===================================================================


void cluster_snoc(const char* addr, cluster_t* c)
{
	cluster_t *tmp = c;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next = cluster_new(addr);	
}

int cluster_contains(cluster_t *c1, cluster_t* c2)
{
	cluster_t* tmp = c1;
	while (tmp != NULL) {
		if (strcmp(tmp->addr,c2->addr)==0)
			return 1;
		tmp = tmp->next;
	}
	return 0;	
}


