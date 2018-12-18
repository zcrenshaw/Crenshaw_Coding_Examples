#ifndef _ADRBOOK_H_
#define _ADRBOOK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char* cnet;
  char* email;
  char* fname;
  char* lname;
  char* tel;
} vcard;

typedef struct bst bst;
struct bst {
  vcard* vc;
  bst* lsub;
  bst* rsub;
};

/* ============================ */
/* ===== vcard operations ===== */
/* ============================ */

/* Construct a new vcard */
/* Copy each string with strdup during construction. */
/* (This is a deep-copy constructor.) */
vcard* vcard_new(const char* cnet, const char* email, const char* fname, const char* lname, const char* tel);

/* Display vcard to stdout. */
/* The display of a vcard need not match any exact specification, */
/* but it must include all five of the vcard's component strings. */
void vcard_show(FILE *f, vcard* vc);

/* Free the vcard and all strings in it. */
void vcard_free(vcard* vc);

/* ============================ */
/* =====  bst operations  ===== */
/* ============================ */

/* Create a bst consisting of one leaf node. */
/* Do not copy the vcard, just point to it. */
/* (This is a shallow-copy constructor.) */
bst* bst_singleton(vcard* vc);

/* Insert a vcard into a non-empty BST. */
/* Raise an error if given an empty BST. */
/* Use cnet alphabetical order (per strcmp). */
/* If the cnet is already present in the tree, insert nothing. */
/* Do not copy the vcard (this is shallow-copy insert). */
/* Return the number of vcards inserted (either 1 or 0). */
int bst_insert(bst* t, vcard* vc);

/* Compute the total number of vcards in the tree. */
unsigned int bst_num_entries(bst* t);

/* The empty bst has height 0. */
/* A singleton tree has height 1, etc. */
unsigned int bst_height(bst* t);

/* Return NULL is nothing is found. */
/* n_comparisons is an out parameter to count the number of */
/* comparisons made during the search. */
/* The program must not assume that *n_comparisons is 0. */
vcard* bst_search(bst* t, const char* cnet, int* n_comparisons);

/* Show all cnets that start with given prefix. */
/* Show cnets one per line in alphabetical order. */
/* Do not traverse parts of the tree you know to be irrelevant. */
/* Return the number of cnets displayed. */
unsigned int bst_prefix_show(FILE* f, bst* t, const char* prefix);

/* Free the bst and all vcards as well. */
void bst_free(bst* t);

/* --- HELPER FUNCTIONS ADDED --- */

//compares only a prefix to the beginning of a string
int precmp(const char *pre, const char *ts);

#endif /* _ADRBOOK_H_ */
