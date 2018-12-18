/* Zack Crenshaw
   CS 152 Winter 2018
   Homework 4
   1/5/18
*/

#include "sdll.h"

/* helper functions */

void malloc_check(void *x, char *fxn_name)
{
	if (x == NULL) {
		fprintf(stderr, "%s: malloc failed",fxn_name);
		exit (1);
	}
}

node *node_new(char *s, node *next, node *prev)
{
	node *nd = malloc(sizeof(node));
	malloc_check(nd,"node_new");
	nd->value = strdup(s);
	nd->next = next;
	nd->prev = prev;
	
	return nd;
}


/* functions for hw */

sdll *list_new()
{
	sdll *nlist = malloc(sizeof(sdll));
	malloc_check(nlist,"list_new");
	nlist -> first = NULL;
	nlist -> last = NULL;

	return nlist;

}

void list_free(sdll *list)
{
	node *nd = list->first;
	while (nd != NULL) {
		node *e = nd;
		nd = nd->next;
		free(e->value);
		free(e);
	}
	free(list);
}	

void node_show(FILE *f, node *nd)
{
	if (nd != NULL) {
		fprintf(f,"%s",nd->value);
	} else {
		fprintf(f,"NULL NODE");
	}
}

void list_show(FILE *f, sdll *list, char sep)
{
	if (list != NULL && list->first != NULL) {
		node *nd = list->first;
		node_show(f,nd);
		nd = nd->next;
		while (nd != NULL) {
			fprintf(f,"%c",sep);
			node_show(f,nd);
			nd = nd->next;
		}
		fprintf(f,"\n");
	} else if (list == NULL) { 
		fprintf(f,"NULL LIST\n");
	} else if (list->first == NULL && list->last == NULL) {
		fprintf(f,"EMPTY LIST");
	} else {
		fprintf(stderr,"list_show: invalid list");
		exit(1);
	}
}	

node *find_first(sdll *list, char *val)
{
	if (list == NULL) 
		return NULL;
	node *nd = list->first;
	while (nd != NULL) {
		if (strcmp(nd->value,val) == 0) 
			return nd;
		nd = nd->next;
	}
	return NULL;
}	

node *get_first(sdll *list)
{
	if (list == NULL) 
		return NULL;
	return list->first;
}

node *get_last(sdll *list)
{
	if (list == NULL) 
		return NULL;
	return list->last;
}

node *get_next(sdll *list, node *nd)
{
	if (list == NULL) 
		return NULL;
	node *spot = find_first(list,nd->value);
	if (spot == NULL) 
		return NULL;
	return spot -> next;
}	

node *get_previous(sdll *list, node *nd)
{
	if (list == NULL) 
		return NULL;
	node *spot = find_first(list,nd->value);
	if (spot == NULL) 
		return NULL;
	return spot -> prev;
}

node *get_at_index(sdll *list, size_t index)
{
	if (list == NULL) 
		return NULL;
	node *nd = list->first;
	for (int i = 0; i < index; i++) {
		if (nd == NULL) {
			return NULL;
		}
		nd = nd->next;
	}
	return nd;
}

sdll *insert_after(sdll *list, char *sought_val, char *new_val)
{
	node *target = find_first(list, sought_val);
	if (target == NULL) 
		return NULL;
	node *newnd = malloc(sizeof(node));
	newnd->value = strdup(new_val);
	newnd->next = target->next;
	newnd->prev = target;
	if (target->next == NULL) {
		list->last = newnd;
	} else {
		target->next->prev = newnd;
	}
	target->next = newnd;
	return list;

}

sdll *insert_before_stitching(sdll *list, node *target, char *new_val)
{
	node *newnd = malloc(sizeof(node));
	malloc_check(newnd,"insert_before");
	newnd->value = strdup(new_val);
	newnd->next = target;
	newnd->prev = target->prev;
	if (target->prev == NULL) {
		list->first = newnd;
	} else {
		target->prev->next = newnd;
	}
	target->prev = newnd;
	return list;
}

sdll *insert_before(sdll *list, char *sought_val, char *new_val)
{
	node *target = find_first(list, sought_val);
	if (target == NULL) 
		return NULL;
	return insert_before_stitching(list,target,new_val);
}	

sdll *insert_at_index(sdll *list, size_t index, char *new_val)
{
	node *target = get_at_index(list,index);
	if (target == NULL) 
		return NULL;
	return insert_before_stitching(list,target, new_val);
}

sdll *remove_item(sdll *list, node *target)
{
	if (target->prev != NULL) 
		target->prev->next = target->next;
	if (target->next != NULL) 
		target->next->prev = target->prev;
	free(target);
	return list;
}

sdll *remove_first(sdll *list, char *sought_val)
{
	node *target = find_first(list,sought_val);
	if (target == NULL) 
		return NULL;
	return remove_item(list,target);

}	

sdll *remove_all(sdll *list, char *sought_val)
{
	node *target = find_first(list,sought_val);
	if (target == NULL)
		return NULL;
	while (target != NULL) {
		list = remove_item(list,target);
		target = find_first(list,sought_val);
	}
	return list;
}
