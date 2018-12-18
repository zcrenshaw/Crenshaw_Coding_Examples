#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "addr_bal.h"

addr_bal_t* addr_bal_new()
{
	addr_bal_t *new = malloc(sizeof(addr_bal_t));
	new->n_buckets = 1000;
	bucket** buckets = calloc(new->n_buckets,sizeof(bucket*)); 
	allocate_check(buckets,"addr_bal_new");
	new->buckets = buckets;
	return new;
}

void addr_bal_free(addr_bal_t* ab)
{
	if (ab == NULL)
		return;
	for (int i = 0; i<ab->n_buckets; i++)
		bucket_free(ab->buckets[i]);
	free(ab->buckets);
	free(ab);
}

int get_balance(const char* addr, double* out_bal, addr_bal_t* ab)
{
	unsigned long int h = hash(addr);
	bucket *b = ab->buckets[h%ab->n_buckets];
	while (b != NULL) {
		if (b->hash == h && strcmp(b->addr,addr) == 0) {
			out_bal = &b->balance;
			return 1;
		}
	}
	return 0;
}

void set_balance(const char* addr, double bal, addr_bal_t* ab)
{
	unsigned long int h = hash(addr);
	bucket *b = ab->buckets[h%ab->n_buckets];
	if (b != NULL) {
		if (b->hash == h && strcmp(b->addr,addr) == 0)
			b->balance = bal;
	}
}


// =====================================================================
// Add any other functions you need in order to implement addr_bal below
// =====================================================================


unsigned long int hash(const char* s)
{
	unsigned long int n = 17;
	unsigned int slen = strlen(s);
	for (int i = 0; i<slen; i++) {
		n *= 37;
		n += s[i];
	}
	return n;
}
/*
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
*/
void addr_bal_insert(const char* addr, double amount, addr_bal_t* ab)
{
	unsigned long int h = hash(addr);
	int i = h%ab->n_buckets;
       	if (!bucket_contains(addr,h,ab->buckets[i])) 	
		ab->buckets[i] = bucket_cons(addr,amount,h,ab->buckets[i]);

}

int addr_bal_contains(const char* addr, addr_bal_t* ab)
{
	unsigned long int h = hash(addr);
	return bucket_contains(addr,h,ab->buckets[h%ab->n_buckets]);
}

void update_balance(const char *addr, double amount, int type, addr_bal_t *ab)
{
	double *b = calloc(1,sizeof(double));
	int check = get_balance(addr,b,ab);
	if (!check) 
		addr_bal_insert(addr,*b,ab);
	else {
		switch (type) {
			case 0 : set_balance(addr,*b+amount,ab);
				       return;
			case 1 : set_balance(addr,*b-amount,ab);
					 return;
		}
	}
}



void hset_show(addr_bal_t* ab)
{
	for (int i = 0; i<ab->n_buckets; i++) {
		printf("\n%d : ",i);
		bucket_show(ab->buckets[i]);
								}
}




bucket* bucket_cons(const char* addr, double amount, unsigned long int hash, bucket* prev_head)
{
	bucket *nbucket = malloc(sizeof(bucket));
	allocate_check(nbucket,"bucket_cons");
	nbucket->addr = strdup(addr);
	nbucket->hash = hash;
	nbucket->balance = amount;
	nbucket->next = prev_head;
	return nbucket;
}
/*
size_t bucket_size(bucket* b)
{
	size_t size = 0;
	while (b != NULL) {
		++size;
		b = b->next;
	}
	return size;
}
*/
int bucket_contains(const char* addr, unsigned long int hash, bucket* b)
{
	while (b != NULL) {
		if (hash == b->hash && strcmp(b->addr,addr) == 0)
			return 1;
		b = b->next;
	}
	return 0;
}

void bucket_show(bucket* b)
{
	if (b != NULL) { //purely cosmetic (to avoid a leading comma on the first item)
		printf("('%s', %f)",b->addr,b->balance);
		b = b->next;
	}
	while (b != NULL) {
		printf(", ('%s', %f)",b->addr,b->balance);
		b = b->next;
	}
}

void bucket_free(bucket* b)
{
	while (b != NULL) {
		bucket *current = b;
		b = b->next;
		free(current->addr);
		free(current);
	}
}

void allocate_check(void *item, char *fxn_name)
{
	if (item == NULL) {
		fprintf(stderr,"%s: memory allocation failed\n",fxn_name);
		exit(1);
	}
}
