#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hset.h"

unsigned long int good_hash(const char* s)
{
	unsigned long int n = 17;
	unsigned int slen = strlen(s);
	for (int i = 0; i<slen; i++) {
		n *= 37;
		n += s[i];
	}
	return n;
}

unsigned long int bad_hash(const char* s)
{
	unsigned long int n = 0;
	unsigned int slen = strlen(s);
	for (int i = 0; i<slen; i++) {
		n += s[i];
	}
	return n;
}

hset* hset_new(unsigned long int(*hash_func)(const char*), size_t sz)
{
	hset *nhset = malloc(sizeof(hset));
	nhset->hash_func = hash_func;
	nhset->n_buckets = sz;
	bucket** buckets = malloc(sizeof(bucket*)*sz); 
	for (int i = 0; i<sz; i++) {
		buckets[i] = NULL;
	}
	nhset->buckets = buckets;
	return nhset;
}

size_t hset_size(hset* hs)
{
	size_t elts = 0;
	for (int i = 0; i<hs->n_buckets; i++) 
		elts += bucket_size(hs->buckets[i]);
	return elts;
}

double hset_load_factor(hset* hs)
{
	double lf = hset_size(hs)/hs->n_buckets; 
	return lf;
}

size_t hset_max_bucket(hset* hs)
{
	size_t max = 0;
	for (int i = 0; i<hs->n_buckets; i++) {
		size_t bsize = bucket_size(hs->buckets[i]);
		if (bsize > max)
			max = bsize;
	}
	return max;
}	

size_t hset_insert(const char* s, hset* hs)
{
	unsigned long int hash = hs->hash_func(s);
	int i = hash%hs->n_buckets; 
	hs->buckets[i] = bucket_cons(s,hash,hs->buckets[i]);
	size_t size = bucket_size(hs->buckets[i]);
	return size;

}

int hset_contains(const char* s, hset* hs)
{
	unsigned long int hash = hs->hash_func(s);
	return bucket_contains(s,hash,hs->buckets[hash%hs->n_buckets]);
}

void hset_show(hset* hs)
{
	for (int i = 0; i<hs->n_buckets; i++) {
		printf("\n%d : ",i);
		bucket_show(hs->buckets[i]);
	}
}

void hset_free(hset* hs)
{
	for (int i = 0; i<hs->n_buckets; i++)
		bucket_free(hs->buckets[i]);
	free(hs->buckets);
	free(hs);
}
	
