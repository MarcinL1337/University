#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.
 * When you hand in, remove the #define DEBUG line. */
// #define DEBUG
#ifdef DEBUG
#define debug(fmt, ...) printf("%s: " fmt "\n", __func__, __VA_ARGS__)
#define msg(...) printf(__VA_ARGS__)
#else
#define debug(fmt, ...)
#define msg(...)
#endif

#define __unused __attribute__((unused))

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* !DRIVER */

/* Moje definicje */

typedef int32_t word_t; /* Heap is bascially an array of 4-byte words. */

#define WSIZE 4
#define DSIZE 8
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
#define PACK(size, alloc) ((size) | (alloc))
#define CHUNKSIZE 1 << 12

typedef struct {
  word_t header;
  /*
   * We don't know what the size of the payload will be, so we will
   * declare it as a zero-length array.  This allow us to obtain a
   * pointer to the start of the payload.
   */
  uint8_t payload[];
} block_t;

typedef enum {
  FREE = 0,     /* Block is free */
  USED = 1,     /* Block is used */
  PREVFREE = 2, /* Previous block is free (optimized boundary tags) */
} bt_flags;

static word_t *heap_start; /* Address of the first block */
static word_t *heap_end;   /* Address past last byte of last block */
static word_t *last;       /* Points at last block */
// static int counter = 0;

/* --=[ boundary tag handling ]=-------------------------------------------- */

static inline word_t bt_size(word_t *bt) {
  return *bt & ~(USED | PREVFREE);
}

static inline int bt_used(word_t *bt) {
  return *bt & USED;
}

static inline int bt_free(word_t *bt) {
  // msg("\npointer w bt_free = %p\n", bt);
  return !(*bt & USED);
}

/* Given boundary tag address calculate it's buddy address. */
static inline word_t *bt_footer(word_t *bt) {
  return (void *)bt + (bt_size(bt) - WSIZE);
}

/* Given payload pointer returns an address of boundary tag. */
static inline word_t *bt_fromptr(void *ptr) {
  return (word_t *)ptr - 1;
}

/* Creates boundary tag(s) for given block. */
static inline void bt_make(word_t *bt, size_t size, bt_flags flags) {
  PUT(bt, PACK(size, flags)); // header
  PUT(bt_footer(bt), PACK(size, flags)); // footer
}

/* Previous block free flag handling for optimized boundary tags. */
static inline bt_flags bt_get_prevfree(word_t *bt) {
  return *bt & PREVFREE;
}

static inline void bt_clr_prevfree(word_t *bt) {
  if (bt)
    *bt &= ~PREVFREE;
}

static inline void bt_set_prevfree(word_t *bt) {
  *bt |= PREVFREE;
}

/* Returns address of payload. */
static inline void *bt_payload(word_t *bt) {
  return bt + 1;
}

/* Returns address of next block or NULL. */
static inline word_t *bt_next(word_t *bt) {
  word_t *next = (void *)bt + bt_size(bt);
  return next <= heap_end ? next : NULL; // <?
}

/* Returns address of previous block or NULL. */
static inline word_t *bt_prev(word_t *bt) {
  word_t *prev = (void *)bt - bt_size(bt - 1);
  return prev >= heap_start ? prev : NULL;
}

/* --=[ miscellanous procedures ]=------------------------------------------ */

/* Calculates block size incl. header, footer & payload,
 * and aligns it to block boundary (ALIGNMENT). */

static size_t round_up(size_t size) {
  return (size + ALIGNMENT - 1) & -ALIGNMENT;
}

// static inline size_t blksz(size_t size) {
  
// }

static void *coalesce(void *ptr)
{
  size_t next_free = 0, prev_free = 0;
  // msg("\ncoalesce\n");
  word_t *prev = bt_prev(ptr);
  if(prev) prev_free = bt_free(prev);
  // msg("\nnext\n");
  word_t *next = bt_next(ptr);
  if(next) next_free = bt_free(next);
  size_t size = bt_size(ptr);

  if(prev_free && next_free)
  {
    // msg("\n1\n");
    size += bt_size(bt_prev(ptr)) + bt_size(bt_next(ptr));
    PUT(bt_footer(bt_next(ptr)), PACK(size, FREE));
    PUT(bt_prev(ptr), PACK(size, FREE));
    ptr = bt_prev(ptr);
  }
  
  else if(!prev_free && next_free)
  {
    // msg("\n2\n");
    size += bt_size(bt_next(ptr));
    PUT(ptr, PACK(size, FREE));
    PUT(bt_footer(ptr), PACK(size, FREE));
  }
  else if(prev_free && !next_free)
  {
    // msg("\n3\n");
    size += bt_size(bt_prev(ptr));
    PUT(bt_footer(ptr), PACK(size, FREE));
    PUT(bt_prev(ptr), PACK(size, FREE));
    ptr = bt_prev(ptr);
  }
  // msg("\n4\n");
  return ptr;
}

static void *morecore(size_t size) {
  // msg("\nmorecore\n");
  void *ptr = mem_sbrk(size);
  
  if(ptr == (void *)-1)
    return NULL;
  
  // ptr -= 4;
  ptr = heap_end;

  bt_make(ptr, size, FREE);
  PUT((void *)bt_footer(ptr) + 4, PACK(0, USED)); // new epilogue

  last = heap_end;
  heap_end = bt_footer(ptr) + 1;

  return ptr; //return coalesce(ptr)?
}

/* --=[ mm_init ]=---------------------------------------------------------- */

int mm_init(void) {
  // msg("\ninit\n");
  if((heap_start = mem_sbrk(4 * DSIZE)) == (void *)-1)
    return -1;
  // msg("\n%p\n", heap_start);
  // PUT(heap_start, 0);                       // Alignment padding
  bt_make(heap_start + 3, ALIGNMENT, USED); // prologue
  PUT(heap_start + 7, PACK(0, USED));       // epilogue
  heap_start += 7;
  heap_end = heap_start;
  // msg("\n%p\n", heap_start);

  // if(morecore(CHUNKSIZE) == NULL)
  //   return -1;

  return 0;
}

/* --=[ malloc ]=----------------------------------------------------------- */

#if 1
/* First fit startegy. */
static void *find_fit(size_t reqsz) {
  // msg("\nfind_fit\n");
  // msg("\nszukam miejsca dla %ld\n", reqsz);
  void *ptr;

  for(ptr = heap_start; bt_size(ptr) > 0; ptr = bt_next(ptr))
  {
    // msg("\nptr = %p\n", ptr);
    if(bt_free(ptr) && reqsz <= bt_size(ptr)) // bt_size(ptr) + DSIZE?
      return ptr;
  }
  return NULL; // No fit
}
#else
/* Best fit startegy. */
static word_t *find_fit(size_t reqsz) {

}
#endif

static void place(void *ptr, size_t asize)
{
  // msg("\nplace\n");
  size_t csize = bt_size(ptr);

  if((csize - asize) >= ALIGNMENT)
  {
    bt_make(ptr, asize, USED);
    // ptr = bt_next(ptr);??
    bt_make(bt_next(ptr), csize - asize, FREE);
    // coalesce(bt_next(ptr));
  }
  else
    bt_make(ptr, csize, USED);

  // msg("\nsize = %ld\n", asize);
  // msg("\npoczątek bloku = %p\n", ptr);
  // msg("\nblock footer = %p\n", bt_footer(ptr));
}

void *malloc(size_t size) {
  // msg("\nmalloc\n");
  if(size == 0)
    return NULL;

  size_t asize; // Adjusted block size
  asize = round_up(size + DSIZE);

  void *ptr = find_fit(asize);

  if(!ptr)
  {
    if((ptr = morecore(asize)) == NULL) //morecore(CHUNKSIZE)?
      return NULL;
  }
  
  place(ptr, asize);

  return bt_payload(ptr);
}


/* --=[ free ]=------------------------------------------------------------- */

void free(void *ptr) {
  msg("\nfree\n");
  word_t *block_ptr = bt_fromptr(ptr);
  
  if(block_ptr < heap_start || block_ptr > heap_end)
    return;

  size_t size = bt_size(block_ptr);
  // msg("\nblock_ptr = %p\n", block_ptr);
  bt_make(block_ptr, size, FREE);
  coalesce(block_ptr);
}

/* --=[ realloc ]=---------------------------------------------------------- */

void *realloc(void *old_ptr, size_t size) {
  // msg("\nrealloc\n");
  if(!old_ptr)
  {
    msg("\nold_ptr = NULL realloc\n");
    return malloc(size);
  }

  if(!size)
  {
    msg("\nSize = 0 realloc\n");
    free(old_ptr);
    return NULL;
  }

  size_t asize = round_up(size + DSIZE); // Adjusted size
  word_t *block_ptr = bt_fromptr(old_ptr);
  msg("Realloc address: %p size: %ld\n", block_ptr, asize);
  // word_t *next = bt_next(block_ptr);
  
  // Case dla asize < bt_size(old_ptr)

  int csize = bt_size(block_ptr) - asize;
  if(csize >= ALIGNMENT)
  {
    msg("\ncsize realloc\n");
    msg("\ncsize = %d\n", csize);
    bt_make(block_ptr, asize, USED);
    // csize = ((csize / ALIGNMENT) - (csize % ALIGNMENT)) * ALIGNMENT;
    bt_make(bt_next(block_ptr), csize, FREE);
    return old_ptr;
  }
  else if(csize > 0)
  {
    msg("\ncsize realloc2\n");
    return old_ptr;
  }

  // if(bt_free(next) && (bt_size(block_ptr) + bt_size(next) - 2*WSIZE) >= size)
  // {
  //   msg("\nNeighbour realloc\n");
  //   if(((bt_size(block_ptr) + bt_size(next) - 2*WSIZE) - size) < ALIGNMENT) 
  //     bt_make(block_ptr, (bt_size(block_ptr) + bt_size(next) - 2*WSIZE), USED);
  //   else
  //   {
  //     bt_make(block_ptr, size, USED);
  //     bt_make(bt_next(block_ptr), (bt_size(block_ptr) + bt_size(next) - 2*WSIZE) - size, FREE);
  //   }
  //   return bt_payload(block_ptr);
  // }

  // word_t *new_ptr; //sam morecore
  // if((new_ptr = morecore(asize)) == NULL)
  //       return NULL;
  //     place(new_ptr, asize);
  //     return bt_payload(new_ptr);

  word_t *new_ptr = find_fit(asize);
  msg("Realloc new pointer: %p\n", new_ptr);
  if(!new_ptr)
  {
    msg("\nMorecore realloc\n");
    if((new_ptr = morecore(asize)) == NULL)
      return NULL;
    msg("Realloc new pointer: %p\n", new_ptr);
    place(new_ptr, asize);
    memcpy(bt_payload(new_ptr), old_ptr, bt_size(block_ptr) - DSIZE); //asize?
    free(old_ptr);
    return bt_payload(new_ptr);
  }
  else
  {
    msg("\nFindfit realloc\n");
    place(new_ptr, asize);
    memcpy(bt_payload(new_ptr), old_ptr, bt_size(block_ptr) - DSIZE); //asize?
    free(old_ptr);
    return bt_payload(new_ptr);
  }
}

/* --=[ calloc ]=----------------------------------------------------------- */

void *calloc(size_t nmemb, size_t size) {
  // msg("\ncalloc\n");
  size_t bytes = nmemb * size;
  void *new_ptr = malloc(bytes);
  if (new_ptr)
    memset(new_ptr, 0, bytes);
  return new_ptr;
}

/* --=[ mm_checkheap ]=----------------------------------------------------- */

void mm_checkheap(int verbose) {
  // printf("\nCHECKHEAP---------------------------\n");
  // void *ptr;

  // for(ptr = heap_start; bt_size(ptr) > 0; ptr = bt_next(ptr))
  // {
  //   printf("\nblock header address = %p\n", ptr);
  //   printf("\nblock size = %x\n", bt_size(ptr));
  //   printf("\nblock footer address = %p\n", bt_footer(ptr));
  //   printf("\nblock header value = %d\n", *(word_t *)ptr);
  //   printf("\nblock footer value = %d\n", *bt_footer(ptr));
  // }

  // void *bt;
  // printf("Check Heap \n");
  // for (bt = heap_start; bt && bt_size(bt) > 0; bt = bt_next(bt)) {
  //   printf("Block Address: %p Block Header Size: %d Block Header type: %d Block ends at: %p Block Footer Size: %d Block Footer Type: %d\n", bt, bt_size(bt), bt_used(bt), bt_next(bt), bt_size(bt_footer(bt)), bt_used(bt_footer(bt)));
  //   if ((*(word_t *)bt) != (*(word_t*)bt_footer(bt))){
  //     printf("\nBLOCK CORRUPTED \n\n");
  //     exit(-1);
  //   }
  //   // counter++;
  // }
  // printf("Heap start: %p Heap end: %p last: %p \n", heap_start, heap_end, last);
  // printf("Check Heap End\n\n");
  // if (counter > 10) exit(-1);
}