#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* "sdll" is an abbreviation for string doubly-linked list. */

#ifndef SDLL_H
#define SDLL_H

typedef struct sdll_node {
	  char *value;
	    struct sdll_node *next;
	      struct sdll_node *prev;
} node;

typedef struct sdll_container {
	  node *first;
	    node *last;
} sdll;

/* Create a new empty list. It is a container struct with two NULL pointers. */
sdll *list_new();

/* Free the whole list, including all strings and the container. */
void list_free(sdll *list);

/* Display a reasonable textual version of the node to f. */
void node_show(FILE *f, node *nd);

/* Display a reasonable textual version of the list to f, with sep between nodes. */
void list_show(FILE *f, sdll *list, char sep);

/* Return the first occurrence of the node containing the string, or NULL. */
node *find_first(sdll *list, char *val);

/* Return the first node in the list. */
node *get_first(sdll *list);

/* Return the last node in the list. */
node *get_last(sdll *list);

/* Return a pointer to the next node, or NULL. 
 * Note: the list pointer need not be used in the implementation.
 */
node *get_next(sdll *list, node *nd);

/* Return a pointer to the previous node, or NULL. 
 * Note: the list pointer need not be used in the implementation.
 */
node *get_previous(sdll *list, node *nd);

/* Return a pointer to the node at the given index, starting at 0. */
node *get_at_index(sdll *list, size_t index);

/* Insert a new node pointing to a duplicate of new_val after the first 
 * occurrence of the sought_val, if sought_val occurs in the list;
 * otherwise return NULL.
 */
sdll *insert_after(sdll *list, char *sought_val, char *new_val);

/* Insert a new node pointing to a duplicate of new_val before the first 
 * occurrence of the sought_val, if sought_val occurs in the list;
 * otherwise return NULL.
 */
sdll *insert_before(sdll *list, char *sought_val, char *new_val);

/* Insert a new node pointing to a duplicate of new_val in the given position.
 * Return NULL if the list has no such position.
 */
sdll *insert_at_index(sdll *list, size_t index, char *new_val);

/* Remove the first node containing an occurrence of sought_val,
 * if one exists, freeing it and its string.  If there is no such occurrence, 
 * return NULL.
 */
sdll *remove_first(sdll *list, char *sought_val);

/* Remove all nodes containing an occurrence of sought_val,
 * if they exist, freeing them and their strings. If there is no such occurrence, 
 * return NULL.
 */
sdll *remove_all(sdll *list, char *sought_val);

/* Remove all nodes containing an occurrence of sought_val,
 * if they exist, freeing them and their strings. If there is no such occurrence, 
 * return NULL.
 */
sdll *remove_all(sdll *list, char *sought_val);

/* --- HELPER FUNCTIONS USED IN EVIDENCE FILE AND IMPLEMENTATION FILE  --- */

/* Creates a new heap allocated node */
node *node_new(char *s, node *next, node *prev);


#endif
