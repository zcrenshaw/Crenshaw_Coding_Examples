/* Zack Crenshaw
   CS 152 Win 18
   HW5
   1/12/18
   */

#include "sal.h"

/* Helper Functions */

void malloc_check(void *x, char *fxn_name)
{
	if (x == NULL) {
		fprintf(stderr, "%s: malloc failed",fxn_name);
		exit (1);
	}
}

/* Functions in header file */
sal *list_new()
{
	sal *list = malloc(sizeof(sal));
	malloc_check(list,"list_new");
	list->values = NULL;
	list->num_items = 0;
	return list;
}

void list_free(sal *list)
{
	for (int i = 0; i<list->num_items; i++)
		free(list->values[i]);
	free(list);

}

void list_show(FILE *f, sal *list, char sep)
{
	if (list == NULL) {
		fprintf(f,"NULL LIST");
	} else if (list->num_items == 0) {
		fprintf(f,"EMPTY LIST");
	} else {
		for (int i = 0; i<list->num_items; i++) 
			fprintf(f,"%s%c",list->values[i],sep);
	}
	fprintf(f,"\n");
}

sal *add_to_front(sal *list, char *val)
{
	char **a = malloc(sizeof(char*)*(++list->num_items));
	malloc_check(a,"add_to_front");
	a[0] = strdup(val);
	for (int i = 0; i<list->num_items-1; i++) {
		a[i+1] = list->values[i];
	}
	free(list->values);
	list->values = a;
	return list;
}

int find_first(sal *list, char *val)
{
	for (int i = 0; i<list->num_items; i++) {
		if (strcmp(list->values[i],val) == 0)
			return i;
	}
	return -1;
}

char *get_first(sal *list)
{
	if (list != NULL && list->values != NULL)
		return list->values[0];
	return NULL;
}

char *get_last(sal *list)
{
	if (list->values != NULL)
		return list->values[list->num_items-1];
	return NULL;
}

char *get_at_index(sal *list, size_t index)
{
	if (list->values != NULL && index < list->num_items) 
		return list->values[index];
	return NULL;
}

sal *insert_after(sal *list, char *sought_val, char *new_val)
{
	int index = find_first(list,sought_val);
	if (index >= 0) {
		char **a = malloc(sizeof(char*)*(++list->num_items));
		malloc_check(a,"insert_after");
		for (int i = 0; i<=index; i++) { 
			a[i] = list->values[i];
		}
		a[index+1] = strdup(new_val);
		for (int i = index+2; i<list->num_items; i++) {
			a[i] = list->values[i-1];
		}
		free(list->values);
		list->values = a;
		return list;
	}
	return NULL;
}
		
sal *insert_before(sal *list, char *sought_val, char *new_val)
{
	int index = find_first(list,sought_val);
	if (index >= 0) {
		list = insert_at_index(list, index, new_val);
		return list;
	}
	return NULL;
}

sal *insert_at_index(sal *list, size_t index, char *new_val)
{
	if (index < list->num_items) {
		char **a = malloc(sizeof(char*)*++list->num_items);
		malloc_check(a,"insert_at_index");
		for (int i = 0; i<index; i++) { 
			a[i] = list->values[i];
		}
		a[index] = strdup(new_val);
		for (int i = index+1; i<list->num_items; i++) {
			a[i] = list->values[i-1];
		}
		free(list->values);
		list->values = a;
		return list;
	}
	return NULL;
}

sal *remove_first(sal *list, char *sought_val)
{
	int index = find_first(list, sought_val);
       	if (index >= 0) {	
		char **a = malloc(sizeof(char*)*(list->num_items-1));
		malloc_check(a,"remove_first");
		for (int i = 0; i<index; i++) { 
			a[i] = list->values[i];
		}
		free(list->values[index]);
		for (int j = index+1; j<list->num_items; j++) {
			a[j-1] = list->values[j];
		}
		free(list->values);
		list->values = a;
		list->num_items--;
		return list;
	}
	return NULL;
}

sal *remove_all(sal *list, char *sought_val)
{
	size_t last = list->num_items;
	char **a = malloc(sizeof(char*)*list->num_items);
	malloc_check(a,"filter");
	int delta = 0;
	for (int j = 0; j<list->num_items; j++) {
		if (strcmp(list->values[j],sought_val) == 0) {
			delta++;
			last--;
			free(list->values[j]);
		} else {
			a[j-delta] = list->values[j];
		}
	}
	if (last == list->num_items) {
		free(a);
		return NULL;
	} else {
		
		while (last<list->num_items) {
			free(a[last]);
			list->num_items--;
		}
		free(list->values);
		list->values = a;
		return list;
	}

}	

sal *act_on_strings(sal *list, void(*f)(char*))
{
	for (int i = 0; i<list->num_items; i++) {
		f(list->values[i]);
	}
	return list;
}

sal *filter(sal *list, int(*test)(char*))
{
	size_t last = list->num_items;
	char **a = malloc(sizeof(char*)*list->num_items);
	malloc_check(a,"filter");
	int delta = 0;
	for (int j = 0; j<list->num_items; j++) {
		if (test(list->values[j]) == 0) {
			delta++;
			last--;
			free(list->values[j]);
		} else {
			a[j-delta] = list->values[j];
		}
	}
	if (last == list->num_items) {
		free(a);
		return NULL;
	} else {
		
		while (last<list->num_items) {
			free(a[last]);
			list->num_items--;
		}
		free(list->values);
		list->values = a;
		return list;
	}
}
	

			
		
