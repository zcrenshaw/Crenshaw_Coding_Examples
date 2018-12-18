#ifndef _HSET_H
#define _HSET_H

#include <stddef.h>

/* a bucket is a linked list, each of whose entries contains a string and its hash */

typedef struct bucket bucket;
struct bucket {
  char* string;
  unsigned long int hash;
  bucket* next;
};
/* By convention, the empty bucket is NULL. */

typedef struct hash_set hset;
struct hash_set {
  unsigned long int(*hash_func)(const char*); /* a pointer to the hash function being used */
  bucket** buckets; /* an array of pointers to buckets */
  size_t n_buckets;
};

/* === bucket operations === */

/* bucket_cons : (char*, unsigned long, bucket*) -> bucket* */
/* build new list with given string at the head */
/* note: copy the given string to the list (deep copy) */
bucket* bucket_cons(const char* s, unsigned long int hash, bucket* prev_head);

/* bucket_size : bucket* -> size_t */
/* return the number of strings in the given bucket */
size_t bucket_size(bucket* b);

/* bucket_contains : (char*, unsigned long, bucket*) -> int */
/* test membership of given string in given bucket */
/* note: use hashes for fast comparison; see comments on piazza */
int bucket_contains(const char* s, unsigned long int hash, bucket* b);

/* bucket_show : bucket* -> <void> */
/* print a representation of the linked list to stdout */
void bucket_show(bucket* b);

/* bucket_free : bucket* -> <void> */
/* free the list nodes and all strings pointed to as well (deep free) */
void bucket_free(bucket* b);

/* === hashing === */

/* good_hash : char -> unsigned long int */
/* compute hash code for given string */
/* see piazza for algorithm */
unsigned long int good_hash(const char* s);

/* bad_hash : char -> unsigned long int */
/* implement this however you like, as long as it's bad */
unsigned long int bad_hash(const char* s);

/* === hash set operations === */

/* hset_new : (string -> unsigned long) int -> hset* */
/* allocate space for a new hash set of given size */
/* all buckets initially empty */
/* first parameter is a pointer to the hash function to use */
hset* hset_new(unsigned long int(*hash_func)(const char*), size_t sz);

/* hset_size : hset* -> size_t */
/* return the total number of entries in all buckets */
size_t hset_size(hset* hs);

/* hset_load_factor : hset* -> double */
/* The load factor is the mean number of elements per bucket. */
double hset_load_factor(hset* hs);

/* hset_max_bucket : hset* -> unsigned int */
/* Report the number of items in the fullest bucket. */
/* (Ideally, the max bucket has 1 item in it.) */
size_t hset_max_bucket(hset* hs);

/* hset_ins : (char*, hset*) -> int */ 
/* add string s to hash set hs */
/* no special treatment for duplicates, just insert them again */
/* return the number of strings in s's bucket after inserting it */
size_t hset_insert(const char* s, hset* hs);

/* hset_contains : (char*, hset*) -> int */
/* test membership of given string in given set */
int hset_contains(const char* s, hset* hs);

/* hset_show : hset* -> <void> */
/* print a representation of the hash set to stdout */
void hset_show(hset* hs);

/* hset_free : hset* -> <void> */
void hset_free(hset* hs);

#endif /* _HSET_H */
