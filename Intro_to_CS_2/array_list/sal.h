#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* "sal" is an abbreviation for string array list. */

#ifndef SAL_H
#define SAL_H

typedef struct {
  char **values;
  size_t num_items;
} sal;

/* Create a new empty list. */
sal *list_new();

/* Free the whole list, including all strings. */
void list_free(sal *list);

/* Display a reasonable textual version of the list to f, with sep between strings. */
void list_show(FILE *f, sal *list, char sep);

/* Add (duplicate of) string to position 0, shifting any existing items to the right. */
sal *add_to_front(sal *list, char *val);

/* Return the index of the first occurrence of the node containing the string, or -1. */
int find_first(sal *list, char *val);

/* Return a pointer to the first string in the list. Return NULL if there isn't one. */
char *get_first(sal *list);

/* Return a pointer to the last string in the list. Return NULL if there isn't one. */
char *get_last(sal *list);

/* Return a pointer to the string at the given index, starting at 0.
 * Return NULL if there isn't one.
 */
char *get_at_index(sal *list, size_t index);

/* Insert a pointer to a duplicate of new_val after the first 
 * occurrence of the sought_val, if sought_val occurs in the list;
 * otherwise return NULL.
 */
sal *insert_after(sal *list, char *sought_val, char *new_val);

/* Insert a pointer to a duplicate of new_val before the first 
 * occurrence of the sought_val, if sought_val occurs in the list;
 * otherwise return NULL.
 */
sal *insert_before(sal *list, char *sought_val, char *new_val);

/* Insert a pointer to a duplicate of new_val in the given position.
 * Return NULL if the list has no such position.
 */
sal *insert_at_index(sal *list, size_t index, char *new_val);

/* Remove the first pointer containing an occurrence of sought_val,
 * if one exists, freeing its string.  If there is no such occurrence, 
 * return NULL.
 */
sal *remove_first(sal *list, char *sought_val);

/* Remove occurrences of sought_val, freeing the strings. If there is 
 * no such occurrence, return NULL.
 */
sal *remove_all(sal *list, char *sought_val);

/* Perform the given action on every item in the list.
 * Act on the strings in place; this should not build a new list.
 */
sal *act_on_strings(sal *list, void(*f)(char*));

/* Remove from the list and free every string that does not pass the test.
 * Do not allocate a new array list. 
 */
sal *filter(sal *list, int(*test)(char*));

#endif
