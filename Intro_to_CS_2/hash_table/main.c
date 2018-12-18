#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <ctype.h>
#include "hset.h"

#define DEFAULT_SIZE   10 /* default hash table size */
#define MAX_LINE_LEN 1024 /* maximum length for lines read from stdin */

/* display usage message */
void usage(char* exec_name)
{
  fprintf(stderr, "Usage: %s [OPTIONS]\n", exec_name);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -b      \tuse lousy hash function\n");
  fprintf(stderr, "  -s <num>\tuse <num> (positive int) table size\n");
}

/* zero out trailing newlines in s */
void clip_newlines(char* s)
{
  int n = strlen(s)-1;
  while (n>=0 && s[n]=='\n') {
      s[n--]='\0';
  }
}

/* set options in the two out parameters */
void parse_opts(int argc, char* argv[], int *bad_hash, int *sz)
{
  char c;
  while ((c=getopt(argc, argv, "bs:"))!=-1) {
    switch (c) {
    case 'b':
      *bad_hash = 1;
      break;
    case 's':
      *sz = atoi(optarg);
      if (*sz<1) {
	      fprintf(stderr, "%s error: nonpositive table size %d\n", *argv, *sz);
	      exit(1);
      }
      break;
    case '?':
      usage(*argv);
      exit(1);
    }
  }
}

void hset_info(hset* t)
{
  printf("\nThe hash table has %zu buckets with %zu entries (load factor %lg).\n", 
	 t->n_buckets, 
	 hset_size(t), 
	 hset_load_factor(t));
  printf("\nThe bucket with the most items in it contains %zu items.\n", 
	 hset_max_bucket(t));
  
  /* test hash set membership */
  printf("\ntesting hset_contains:\n");
  const char* names[] = {"Aaron", "Victoria", "Blender", "Xylophone"};
  for (size_t i = 0; i < sizeof(names)/sizeof(const char*); i++) {
      printf("membership of cnet \"%s\" : %i\n", names[i], hset_contains(names[i], t));
  }
}

/* main:
 * read in strings from standard input, 
 * build hash table, 
 * display hash table and a few statistics */
int main(int argc, char* argv[])
{
  char buffer[MAX_LINE_LEN];
  int bad = 0;
  int sz  = DEFAULT_SIZE;
  parse_opts(argc, argv, &bad, &sz);
  
  hset* t = hset_new(bad ? &bad_hash : &good_hash, (size_t)sz);
  
  fgets(buffer, MAX_LINE_LEN, stdin);
  while (!feof(stdin)) {
    clip_newlines(buffer);
    if (strlen(buffer) > 0) {
        hset_insert(buffer, t);
    }
    fgets(buffer, MAX_LINE_LEN, stdin);
  }
  
  hset_show(t);
  hset_info(t);
  hset_free(t);
  return 0;
}
