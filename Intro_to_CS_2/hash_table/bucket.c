#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hset.h"

bucket* bucket_cons(const char* s, unsigned long int hash, bucket* prev_head)
{
	bucket *nbucket = malloc(sizeof(bucket));
	nbucket->string = strdup(s);
	nbucket->hash = hash;
	nbucket->next = prev_head;
	return nbucket;
}

size_t bucket_size(bucket* b)
{
	size_t size = 0;
	while (b != NULL) {
		++size;
		b = b->next;
	}
	return size;
}

int bucket_contains(const char* s, unsigned long int hash, bucket* b)
{
	while (b != NULL) {
		if (hash == b->hash && strcmp(b->string,s) == 0)
			return 1;
		b = b->next;
	}
	return 0;
}

void bucket_show(bucket* b)
{
	if (b != NULL) { //purely cosmetic (to avoid a leading comma on the first item)
		printf("('%s', %ld)",b->string,b->hash);
		b = b->next;
	}
	while (b != NULL) {
		printf(", ('%s', %ld)",b->string,b->hash);
		b = b->next;
	}
}

void bucket_free(bucket* b)
{
	while (b != NULL) {
		bucket *current = b;
		b = b->next;
		free(current->string);
		free(current);
	}
}

