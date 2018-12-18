/* 
 * mm-.c - This is the only file you should modify
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mm.h"
#include "memlib.h"

/*
 * If NEXT_FIT defined use next fit search, else use first fit search 
 */
//#define NEXT_FIT

/* $begin mallocmacros */
/* Basic constants and macros */
#define WSIZE       4       /* word size (bytes) */  
#define DSIZE       8        /* doubleword size (bytes) */
#define CHUNKSIZE  (1<<16)  /* initial heap size (bytes) */
#define OVERHEAD    8       /* overhead of header and footer (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))  

//MINE
#define PUT_PNT(p, pnt) (*(unsigned int**)(p) = (pnt)) //CHANGED!!!!
#define GET_LONG(p) (*(size_t *)(p))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)  
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))


//MINE
#define SUCC(bp)	(*(void**)(bp))
#define PRED(bp)	(*(void**)(bp + DSIZE))

#define PUT_SUCC(bp,val)	(SUCC(bp) = val)
#define PUT_PRED(bp,val)	(PRED(bp) = val)


/* Global variables */
static char *heap_listp = 0;  /* pointer to first block */  
#ifdef NEXT_FIT
static char *rover;       /* next fit rover */
#endif

static char *head;

/* function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp); 
static void checkblock(void *bp);

/* 
 * mm_init - Initialize the memory manager 
 */
int mm_init(void) 
{
//	printf("\n\nINITIATE\n");
//	printf("heap size at first: %zu\n",mem_heapsize());
  /* create the initial empty heap */
  if ((heap_listp = mem_sbrk(4*WSIZE)) == NULL)
    return -1;
 // printf("HEAP EXISTS\n");
  PUT(heap_listp, 0);                        /* alignment padding */
  PUT(heap_listp+WSIZE, PACK(OVERHEAD, 1));  /* prologue header */ 
  PUT(heap_listp+DSIZE, PACK(OVERHEAD, 1));  /* prologue footer */ 
  PUT(heap_listp+WSIZE+DSIZE, PACK(0, 1));   /* epilogue header */
  heap_listp += DSIZE;

  head = NULL;

 // printf("heap size after pro/epilogue: %zu\n",mem_heapsize());
  /* Extend the empty heap with a free block of CHUNKSIZE bytes */
  if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
    return -1;
	#ifdef NEXT_FIT
 	 rover = head;
	#endif
	// printf("%p =? %p \n",rover,head);
  //	printf("I'M EXTENDED\n");
  return 0;
}

/*
 * malloc - Allocate a block with at least size bytes of payload 
 */
void *mm_malloc(size_t size)
{
  size_t asize;      /* adjusted block size */
  size_t extendsize; /* amount to extend heap if no fit */
  char *bp;      
  if (heap_listp == 0){
    mm_init();
  }

  /* Ignore spurious requests */
  if (size <= 0)
    return NULL;

  /* Adjust block size to include overhead and alignment reqs. */
  if (size <= DSIZE*2)
    asize = DSIZE*2 + OVERHEAD;
  else
    asize = DSIZE * ((size + (OVERHEAD) + (DSIZE-1)) / DSIZE);

  /* Search the free list for a fit */
  if ((bp = find_fit(asize)) != NULL) {
    place(bp, asize);
    return bp;
  }

  /* No fit found. Get more memory and place the block */
  extendsize = MAX(asize,CHUNKSIZE);
  if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
    return NULL;
  place(bp, asize);
  return bp;
} 

/* 
 * free - Free a block 
 */
void mm_free(void *bp)
{
  if(bp == 0) return;

  size_t size = GET_SIZE(HDRP(bp));
  if (heap_listp == 0){
    mm_init();
  }
//	printf("freeing: %p \n",bp);
  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  coalesce(bp);
  //mm_checkheap(0);    // TURNED CHECKING OFFFFFFFFFF!!!!!!
}


/*
 * realloc - naive implementation of realloc
 */
void *mm_realloc(void *ptr, size_t size)
{
  size_t oldsize;
  void *newptr;

  /* If size == 0 then this is just free, and we return NULL. */
  if(size == 0) {
    mm_free(ptr);
    return 0;
  }

  /* If oldptr is NULL, then this is just malloc. */
  if(ptr == NULL) {
    return mm_malloc(size);
  }

  newptr = mm_malloc(size);

  /* If realloc() fails the original block is left untouched  */
  if(!newptr) {
    return 0;
  }

  /* Copy the old data. */
  oldsize = GET_SIZE(HDRP(ptr));
  if(size < oldsize) oldsize = size;
  memcpy(newptr, ptr, oldsize);

  /* Free the old block. */
  mm_free(ptr);

  return newptr;
}

/* 
 * checkheap - Minimal check of the heap for consistency 
 */


/* The remaining routines are internal helper routines */

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 */
/*
static void print_list()
{
	
	int i = 0;
	char* tmp = head;
	if (tmp != NULL) {
		printf("\nFree list: \n");
	}

	while (tmp != NULL && i < 10) {
	//	printf("%d: %p\n",i++,tmp);
		printf("%d: p: %p\t succ: %p\t pred: %p \n",i++,tmp,SUCC(tmp),PRED(tmp));
		tmp = SUCC(tmp);
	}
	printf("\n");
}
*/
static void init_free(void *p)
{
	if (head != p) {
		head = p;
		PUT_SUCC(p,NULL);
		PUT_PRED(p,NULL);
	}
}

static void add_free(void *p)
{
//	print_list();
//	printf("ADDING: %p\n",p);
	
	if (head == NULL) {
//		printf("init list\n");
		init_free(p);
	} else {
		if (head == p || GET_SIZE(HDRP(p)) == 0) {
			return;
		}
			PUT_PRED(head,p);
			PUT_SUCC(p,head);
			PUT_PRED(p,NULL);
			head = p;
	}
//	printf(" head ; succ -- %p ; %p\n",head,SUCC(head));
	//printf("added %p\n",p);
//	printf("%p succ: %p, pred: %p\n",p,SUCC(p),PRED(p));
}
		
static void rm_free(void *p)
{
	
	void *prev = PRED(p);
	void *next = SUCC(p);

//	printf("FREE ACTIVATED!  free: %p - succ: %p - pred: %p\n",p,next,prev);

//	printf("REMOVING: %p %d\n",p,GET_ALLOC(HDRP(p)));
	if (prev == NULL && next == NULL) {
		head = NULL;
	} else if (prev != NULL && next != NULL) {
		PUT_PRED(next,prev);
		PUT_SUCC(prev,next);
	} else if (prev != NULL) {
		PUT_SUCC(prev, NULL);
	} else if (next != NULL) {
		head = next;
		PUT_PRED(next, NULL);
	}

}


static void *extend_heap(size_t words) 
{
//	printf("EXTEND ME\n");
  char *bp;
  size_t size;
  void *return_ptr;

  /* Allocate an even number of words to maintain alignment */
  size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;


//  printf("extending by %zu\n",size);


  if ((long)(bp = mem_sbrk(size)) < 0) 
    return NULL;

  /* Initialize free block header/footer and the epilogue header */
  PUT(HDRP(bp), PACK(size, 0));         /* free block header */
  PUT(FTRP(bp), PACK(size, 0));         /* free block footer */
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* new epilogue header */

	//	add_free(bp);
  /* Coalesce if the previous block was free */
  return_ptr = coalesce(bp); 
  //mm_checkheap(0);
  return return_ptr;
}

/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
static void place(void *bp, size_t asize)
{
  size_t csize = GET_SIZE(HDRP(bp));   


  if ((csize - asize) >= (DSIZE*2 + OVERHEAD)) { 
	  rm_free(bp);
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
  	    bp = NEXT_BLKP(bp);
    PUT(HDRP(bp), PACK(csize-asize, 0));
    PUT(FTRP(bp), PACK(csize-asize, 0));
	add_free(bp);

  }
  else { 
	  rm_free(bp);
    PUT(HDRP(bp), PACK(csize, 1));
    PUT(FTRP(bp), PACK(csize, 1)); 
  }
}

/* 
 * find_fit - Find a fit for a block with asize bytes 
 */
static void *find_fit(size_t asize)
{
#ifdef NEXT_FIT 
  /* next fit search */
  char *oldrover = rover;

  /* search from the rover to the end of list */
  for ( ; rover != NULL && GET_SIZE(HDRP(rover)) > DSIZE*2 + OVERHEAD; rover = SUCC(rover))
    if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
      return rover;

  /* search from start of list to old rover */
  for (rover = head; rover != NULL && rover != oldrover; rover = SUCC(rover))
    if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
      return rover;

  return NULL;  /* no fit found */
#else 
  /* first fit search */
  char *bp;

  for (bp = head; bp != NULL ; bp = SUCC(bp)) {  
//	  printf("SEARCHING at %p\n",bp);
    if (asize <= GET_SIZE(HDRP(bp))) {
//	    printf("return me!\n");
      return bp;
    }

  }
  return NULL; /* no fit */
#endif
}


/*
 * coalesce - boundary tag coalescing. Return ptr to coalesced block
 */
static void *coalesce(void *bp) 
{
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  if (prev_alloc && next_alloc) {            /* Case 1 */
	  //printf("CASE 1\n");
	  add_free(bp);
	return bp;
  }
  else if (prev_alloc && !next_alloc) {      /* Case 2 */
	 // printf("CASE 2\n");
/*
	PUT_SUCC(bp,SUCC(NEXT_BLKP(bp)));
	PUT_PRED(bp,NULL);
*/

	  rm_free(NEXT_BLKP(bp));
	size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size,0));
	add_free(bp); 
/*
     void* next = NEXT_BLKP(bp);
	  void* last = SUCC(next);
	  void* first = PRED(next);

	size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size,0));

	PUT_SUCC(bp,last);
	PUT_PRED(last,bp);
	PUT_SUCC(first,bp);
	PUT_PRED(bp,first);	

*/
  }


  else if (!prev_alloc && next_alloc) {      /* Case 3 */ 
	// printf("CASE 3\n");

	 //rm_free(PREV_BLKP(bp));

    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
//	add_free(bp);
/*
	PUT_SUCC(bp,SUCC(PREV_BLKP(oldbp)));
	PUT_PRED(bp,PRED(PREV_BLKP(oldbp)));
*/

   }

  else {                                     /* Case 4 */
	 // printf("CASE 4\n");
/*
	PUT_SUCC(bp,SUCC(NEXT_BLKP(bp)));
	PUT_PRED(bp,PRED(PREV_BLKP(bp)));

*/

	rm_free(PREV_BLKP(bp));
	rm_free(NEXT_BLKP(bp));
	size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
      GET_SIZE(FTRP(NEXT_BLKP(bp)));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);

    add_free(bp);
     }

#ifdef NEXT_FIT
  /* Make sure the rover isn't pointing into the free block */
  /* that we just coalesced */
    rover = bp;
#endif

  return bp;
}






void mm_checkheap(int verbose)
{
  char *bp = heap_listp;

  if (verbose)
    printf("Heap (%p):\n", heap_listp);
 

 // print_list();

  if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
    printf("Bad prologue header\n");
  checkblock(heap_listp);

  for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
    if (verbose) 
      printblock(bp);
    checkblock(bp);
  }

  if (verbose)
    printblock(bp);
  if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
    printf("Bad epilogue header\n");
}


static void printblock(void *bp) 
{
  size_t hsize, halloc, fsize, falloc;

  hsize = GET_SIZE(HDRP(bp));
  halloc = GET_ALLOC(HDRP(bp));  
  fsize = GET_SIZE(FTRP(bp));
  falloc = GET_ALLOC(FTRP(bp));  

  if (hsize == 0) {
    printf("%p: EOL\n", bp);
    return;
  }
//	printf("size of block %p: %zu\n",bp,hsize);
//	printf("succ: %p - pred: %p\n",SUCC(bp),PRED(bp));

    printf("%p: header: [%zu:%c] footer: [%zu:%c]\n", bp, 
      hsize, (halloc ? 'a' : 'f'), 
      fsize, (falloc ? 'a' : 'f')); 
}

static void checkblock(void *bp) 
{
  if ((size_t)bp % 8)
    printf("Error: %p is not doubleword aligned\n", bp);
  if (GET(HDRP(bp)) != GET(FTRP(bp)))
    printf("Error: header does not match footer\n");
}

void *mm_calloc (size_t nmemb, size_t size)
{
  void *ptr;
  if (heap_listp == 0){
    mm_init();
  }

  ptr = mm_malloc(nmemb*size);
  bzero(ptr, nmemb*size);


  return ptr;
}
