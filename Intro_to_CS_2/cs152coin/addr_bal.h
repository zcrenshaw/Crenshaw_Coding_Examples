#ifndef _ADDR_BAL_H
#define _ADDR_BAL_H

typedef struct bucket bucket;
struct bucket {
	char* addr;
	double balance;
	unsigned long int hash;
	bucket* next;
};
/* By convention, the empty bucket is NULL. */

typedef struct addr_bal {
	bucket** buckets;
	size_t n_buckets;
} addr_bal_t;


// Returns a new heap-allocated data structure for tracking the balance of each address.
addr_bal_t* addr_bal_new();

// Frees a heap-allocated data structure for tracking the balance of each address.
void addr_bal_free(addr_bal_t* ab);

// Get the balance associated with address addr and store the result in the double
// pointed to by out_bal.
// Returns 0 if the address couldn't be found and nonzero otherwise.
int get_balance(const char* addr, double* out_bal, addr_bal_t* ab);

// Set the balance associated with address addr to bal.
void set_balance(const char* addr, double bal, addr_bal_t* ab);


// =========================================================================
// Declare any other functions you need in order to implement addr_bal below
// =========================================================================

/* === bucket operations === */

/* build new list with given string at the head */
bucket* bucket_cons(const char* s, double amount, unsigned long int hash, bucket* prev_head);

/* return the number of strings in the given bucket */
size_t bucket_size(bucket* b);

/* test membership of given address in given bucket */
int bucket_contains(const char* addr, unsigned long int hash, bucket* b);

/* print a representation of the linked list to stdout */
void bucket_show(bucket* b);

/* free the list nodes and all strings pointed to as well (deep free) */
void bucket_free(bucket* b);

/* === hashing === */

/* compute hash code for given string */
unsigned long int hash(const char* s);


/* === hash set operations === */
/*
// return the total number of entries in all buckets 
size_t hset_size(hset* hs);

// The load factor is the mean number of elements per bucket. 
double hset_load_factor(hset* hs);

// Report the number of items in the fullest bucket. 
// (Ideally, the max bucket has 1 item in it.) 
size_t hset_max_bucket(hset* hs);
*/
/* add address and accompanying balance to data set */
/* if duplicate, ignore */
void addr_bal_insert(const char* addr, double amount, addr_bal_t* ab);

/* test membership of given address in given set */
int addr_bal_contains(const char* addr, addr_bal_t* ab);

// update the balance of a given account based on deposit or withdrawl
void update_balance(const char *addr, double amount, int type, addr_bal_t *ab);


/*
// print a representation of the hash set to stdout 
void hset_show(hset* hs);
*/
// checks to make sure memory allocation was successful 
// returns an error to stderr, naming the function in which allocation failed 
void allocate_check(void *item, char *fxn_name);


#endif /* _HSET_H */
