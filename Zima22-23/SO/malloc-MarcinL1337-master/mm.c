// Marcin Linkiewicz, nr indeksu: 323853

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
#define DEBUG
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
#define GET(p)                 (*(unsigned int *)(p))
#define PUT(p, val)            (*(unsigned int *)(p) = (val))
#define PACK(size, alloc)      ((size) | (alloc))
#define GET_PREV_FREE(p)       *(p + 1)
#define GET_NEXT_FREE(p)       *(p + 2)
#define PUT_PREV_FREE(p, prev) (*(word_t *)(p+1) = (word_t)((unsigned long)prev))
#define PUT_NEXT_FREE(p, next) (*(word_t *)(p+2) = (word_t)((unsigned long)next))
#define WORD_TO_POINTER(word)  (word_t *)((unsigned long)word | 0x800000000)
#define CHUNKSIZE              1 << 12
#define NR_OF_SEG_LISTS        10

static word_t **segregated_list;


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
static int counter = 0; // counter do checkheap

/* --=[ boundary tag handling ]=-------------------------------------------- */

static inline word_t bt_size(word_t *bt) {
  return *bt & ~(USED | PREVFREE);
}

static inline int bt_used(word_t *bt) {
  return *bt & USED;
}

static inline int bt_free(word_t *bt) {
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
  PUT(bt, PACK(size, flags));            // header
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

static size_t calc_class(size_t asize){
  size_t res = 27 - __builtin_clz(asize);
  return res < NR_OF_SEG_LISTS ? res : 9;
}

static void add_to_seg_list(word_t *ptr, size_t class){
  msg("\nadd_to_seg_list ptr = %p, class = %ld\n", ptr, class);
  if(segregated_list[class]){
    PUT_NEXT_FREE(ptr, segregated_list[class]);
    PUT_PREV_FREE(ptr, NULL);
    PUT_PREV_FREE(segregated_list[class], ptr);
    segregated_list[class] = ptr;
  }
  else{
    PUT_NEXT_FREE(ptr, NULL);
    PUT_PREV_FREE(ptr, NULL);
    segregated_list[class] = ptr;
  }
}

static void del_from_seg_list(word_t *ptr, size_t class){
  msg("\ndel_from_seg_list ptr = %p, class = %ld\n", ptr, class);
  word_t *class_head = segregated_list[class];
  msg("class_head = %p\n", class_head);
  msg("head_prev = %x, head_next = %x\n",GET_PREV_FREE(class_head), GET_NEXT_FREE(class_head));
  if(!GET_NEXT_FREE(ptr) && !GET_PREV_FREE(ptr))
    segregated_list[class] = NULL;
  else{
    if(ptr == class_head){
      PUT_PREV_FREE(WORD_TO_POINTER(GET_NEXT_FREE(class_head)), NULL);
      segregated_list[class] = WORD_TO_POINTER(GET_NEXT_FREE(ptr));
    }
    else if(!GET_NEXT_FREE(ptr)) // == NULL?
      PUT_NEXT_FREE(WORD_TO_POINTER(GET_PREV_FREE(ptr)), NULL);
    else{
      PUT_NEXT_FREE(WORD_TO_POINTER(GET_PREV_FREE(ptr)), GET_NEXT_FREE(ptr));
      PUT_PREV_FREE(WORD_TO_POINTER(GET_NEXT_FREE(ptr)), GET_PREV_FREE(ptr));
    }
  }
}


static void *coalesce(void *ptr) {
  msg("\nCoalesce\n");
  size_t next_free = 0, prev_free = 0;

  word_t *prev = bt_prev(ptr);
  if (prev) prev_free = bt_free(prev);

  word_t *next = bt_next(ptr);
  if (next) next_free = bt_free(next);

  size_t size = bt_size(ptr);
  // ZMIENIĆ PUTY NA BT_MAKE
  if (prev_free && next_free) {
    size += bt_size(bt_prev(ptr)) + bt_size(next);
    del_from_seg_list(prev, calc_class(bt_size(prev)));
    del_from_seg_list(next, calc_class(bt_size(next)));
    PUT(bt_footer(next), PACK(size, FREE));
    PUT(prev, PACK(size, FREE));
    ptr = prev;
    add_to_seg_list(ptr, calc_class(size));
  } else if (!prev_free && next_free) {
    size += bt_size(next);
    del_from_seg_list(next, calc_class(bt_size(next)));
    PUT(ptr, PACK(size, FREE));
    PUT(bt_footer(ptr), PACK(size, FREE));
    add_to_seg_list(ptr, calc_class(size));
  } else if (prev_free && !next_free) {
    size += bt_size(prev);
    del_from_seg_list(prev, calc_class(bt_size(prev)));
    PUT(bt_footer(ptr), PACK(size, FREE));
    PUT(prev, PACK(size, FREE));
    ptr = prev;
    add_to_seg_list(ptr, calc_class(size));
  }
  else
    add_to_seg_list(ptr, calc_class(size));

  return ptr;
}

static void *morecore(size_t size) {
  void *ptr = mem_sbrk(size);

  if (ptr == (void *)-1)
    return NULL;

  ptr = heap_end;

  bt_make(ptr, size, FREE);
  msg("\nmorecore size = %ld, calc_class = %ld\n", size, calc_class(size));
  // add_to_seg_list(ptr, calc_class(size));
  PUT((void *)bt_footer(ptr) + 4, PACK(0, USED)); // new epilogue

  last = heap_end;
  heap_end = bt_footer(ptr) + 1;

  // return ptr;
  return coalesce(ptr);
}

/* --=[ mm_init ]=---------------------------------------------------------- */

int mm_init(void) {
  segregated_list = mem_sbrk(NR_OF_SEG_LISTS * DSIZE);

  for(int i = 0; i < NR_OF_SEG_LISTS; i++)
    segregated_list[i] = NULL;

  if ((heap_start = mem_sbrk(4 * DSIZE)) == (void *)-1)
    return -1;
  // PUT(heap_start, 0);                       // Alignment padding
  bt_make(heap_start + 3, ALIGNMENT, USED); // prologue
  PUT(heap_start + 7, PACK(0, USED));       // epilogue
  heap_start += 7;
  heap_end = heap_start;

  return 0;
}

/* --=[ malloc ]=----------------------------------------------------------- */

#if 1
/* First fit startegy. */
static word_t *find_fit(size_t reqsz) {
  short class = calc_class(reqsz);
  word_t *ptr = segregated_list[class];
  msg("\nfindfit ptr = %p, class = %d\n", ptr, class);
  // if(!ptr) return NULL;

  while(class < NR_OF_SEG_LISTS){
    // printf("\nfindfit outer while, class = %d\n", class);
    while(ptr != (word_t *)0x800000000 && ptr != NULL){
      msg("findfit inner while ptr = %p\n", ptr);
      if(reqsz <= bt_size(ptr)){
        msg("findfit return = %p\n", ptr);
        return ptr;
      }
      ptr = WORD_TO_POINTER(GET_NEXT_FREE(ptr));
    }
    class++;
    ptr = segregated_list[class];
  }
  msg("findfit return = NULL\n");
  return NULL; // No fit
}
#else
/* Best fit startegy. */
static word_t *find_fit(size_t reqsz) {
  word_t *ptr = free_list_head;
  if(!ptr) return NULL;
  word_t *min_ptr;
  size_t min = -1;

  while(1){
    if(bt_free(ptr) && reqsz < bt_size(ptr)){
      if(bt_size(ptr) < min){
        min = bt_size(ptr);
        min_ptr = ptr;
      }
    }
    else if(reqsz == bt_size(ptr))
      return ptr;
      
    if((ptr = WORD_TO_POINTER(GET_NEXT_FREE(ptr))) == (word_t *)0x800000000)
      break;
  }
  return min < -1 ? min_ptr : NULL;
}
#endif

static void place(void *ptr, size_t asize) {
  msg("\nplace\n");
  size_t csize = bt_size(ptr);
  msg("place csize = %ld\n", csize);

  del_from_seg_list(ptr, calc_class(bt_size(ptr)));
  if ((csize - asize) >= ALIGNMENT) {
    msg("place if\n");
    bt_make(ptr, asize, USED);
    bt_make(bt_next(ptr), csize - asize, FREE);
    add_to_seg_list(bt_next(ptr), calc_class(csize - asize));
    // coalesce(bt_next(ptr));
  } else{
    msg("place else\n");
    bt_make(ptr, csize, USED);
  }
}

// static size_t maks(size_t x, size_t y){
//   return x > y ? x : y;
// }

void *malloc(size_t size) {
  if (size == 0)
    return NULL;

  size_t asize; // Adjusted block size
  asize = round_up(size + DSIZE);

  msg("\nMalloc, size = %ld\n", asize);

  void *ptr = find_fit(asize);

  if (!ptr) {
    msg("\nMalloc morecore\n");
    if ((ptr = morecore(asize)) == NULL) // morecore(CHUNKSIZE)?
      return NULL;
  }
  msg("\nplace asize = %ld\n", asize);
  msg("\nplace ptr = %p\n", ptr);
  place(ptr, asize);

  return bt_payload(ptr);
}

/* --=[ free ]=------------------------------------------------------------- */

void free(void *ptr) {
  word_t *block_ptr = bt_fromptr(ptr);
  msg("\nfree, address = %p\n", block_ptr);

  if (block_ptr < heap_start || block_ptr > heap_end)
    return;

  size_t size = bt_size(block_ptr);
  bt_make(block_ptr, size, FREE);
  // add_to_free_list(block_ptr);
  coalesce(block_ptr);
}

/* --=[ realloc ]=---------------------------------------------------------- */

void *realloc(void *old_ptr, size_t size) {
  if (!old_ptr)
    return malloc(size);

  if (!size) {
    free(old_ptr);
    return NULL;
  }

  size_t asize = round_up(size + DSIZE); // Adjusted size
  word_t *block_ptr = bt_fromptr(old_ptr);
  msg("Realloc address: %p size: %ld\n", block_ptr, asize);
  

  // Case dla asize < bt_size(old_ptr)
  int csize = bt_size(block_ptr) - asize;
  if (csize >= ALIGNMENT) {
    msg("\nCsize realloc\n");
    bt_make(block_ptr, asize, USED);
    word_t *next = bt_next(block_ptr);
    bt_make(next, csize, FREE);
    add_to_seg_list(next, calc_class(csize));
    // coalesce(next);
    return old_ptr;
  } else if (csize >= 0) {
    return old_ptr;
  }

  word_t *next = block_ptr < heap_end ? bt_next(block_ptr) : NULL;
  word_t *prev = block_ptr > heap_start ? bt_prev(block_ptr) : NULL;
  
  size_t prev_size = prev ? bt_size(prev) : 0;
  size_t curr_size = bt_size(block_ptr);
  size_t next_size = bt_size(next);

  size_t new_size = curr_size;

  if(prev && bt_free(prev)) new_size += prev_size;
  msg("\ntest\n");
  if(bt_free(next)) new_size += next_size;
  if(new_size >= asize){
    msg("\nNeighbour realloc\n");
    if(bt_free(next)) del_from_seg_list(next, calc_class(next_size));
    if(prev){
      if(bt_free(prev)){
        del_from_seg_list(prev, calc_class(prev_size));
        block_ptr = prev;
      }
      memcpy(bt_payload(block_ptr), old_ptr, curr_size - DSIZE);
    }
    
    if(new_size - asize < ALIGNMENT){ // -DSIZE??
      bt_make(block_ptr, new_size, USED);
    }
    else{
      bt_make(block_ptr, asize, USED);
      bt_make(bt_next(block_ptr), new_size - asize, FREE);
      add_to_seg_list(bt_next(block_ptr), calc_class(new_size - asize));
    }
    return bt_payload(block_ptr);
  }

  word_t *new_ptr = find_fit(asize);
  msg("Realloc new pointer: %p\n", new_ptr);
  if (!new_ptr) {
    msg("\nMorecore realloc\n");
    if ((new_ptr = morecore(asize)) == NULL)
      return NULL;
    msg("Realloc new pointer: %p\n", new_ptr);
    place(new_ptr, asize);
    memcpy(bt_payload(new_ptr), old_ptr, bt_size(block_ptr) - DSIZE); // asize?
    free(old_ptr);
    return bt_payload(new_ptr);
  } else {
    msg("\nFindfit realloc\n");
    place(new_ptr, asize);
    memcpy(bt_payload(new_ptr), old_ptr, bt_size(block_ptr) - DSIZE); // asize?
    free(old_ptr);
    return bt_payload(new_ptr);
  }
}

/* --=[ calloc ]=----------------------------------------------------------- */

void *calloc(size_t nmemb, size_t size) {
  size_t bytes = nmemb * size;
  void *new_ptr = malloc(bytes);
  if (new_ptr)
    memset(new_ptr, 0, bytes);
  return new_ptr;
}

/* --=[ mm_checkheap ]=----------------------------------------------------- */

void mm_checkheap(int verbose) {
  void *bt;
  printf("Check Heap \n");
  for (bt = heap_start; bt && bt_size(bt) > 0; bt = bt_next(bt)) {
    if(counter > -1) { 
      printf("\n\nBlock Address: %p Block Header Size: %d Block Header type: %d Block ends at: %p Block Footer Size: %d Block Footer Type: %d\n", bt,
      bt_size(bt), bt_used(bt), bt_next(bt), bt_size(bt_footer(bt)),
    bt_used(bt_footer(bt)));
    }
    if ((*(word_t *)bt) !=
    (*(word_t*)bt_footer(bt))){
      printf("\nBLOCK CORRUPTED \n\n");
      exit(-1);
    }
  }
  counter++;
  printf("Heap start: %p Heap end: %p last: %p \n", heap_start, heap_end,
  last); printf("Check Heap End\n\n");

  printf("Seglists check: \n");

  for(int i = 0; i < NR_OF_SEG_LISTS; i++){
    if(!segregated_list[i])
      printf("%d: NULL\n", i);
    else{
      word_t *ptr = segregated_list[i];
      printf("%d: ", i);
      while(1){
        printf("%p -> ", ptr);
        ptr = WORD_TO_POINTER(GET_NEXT_FREE(ptr));
        if(ptr == (word_t *)0x800000000){
          printf("NULL\n");
          break;
        }
      }
    }
  }
  if(counter > 100) exit(0);

  // printf("\nfree_list_head = %p", free_list_head);
  // printf("\nfree_list_tail = %p\n", free_list_tail);
 
  // word_t *ptr = free_list_head;
  // msg("\nFree list check findfit ptr = %p\n", ptr);
  // if(!ptr) 
  //   printf("\nChechheap free_list ptr = NULL");
  // else{
  //   printf("\ncheckheap free_list: ");
  //   while(1){
  //     printf("%p -> ", ptr);
  //     if(ptr == (word_t *)0x800000000){
  //       break;
  //     }
  //     ptr = (word_t *)((unsigned long)GET_NEXT_FREE(ptr) | 0x800000000);
  //   }
  // }
  // printf("\nFree List check END\n");
}
