/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* Basic constants and macros */
#define WSIZE       4       /* Word and header/footer size (bytes) */
#define DSIZE       8       /* Double word size (bytes) */
#define CHUNKSIZE   (1<<12) /* Extend heap by this amount (bytes) */

#define MINBLOCKSIZE  (4 * WSIZE) // minimum block size

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

#define SUCC(bp) (*(char **)(bp))                        // Successor pointer (next free block)
#define PRED(bp) (*(char **)((char *)(bp) + WSIZE))      // Predecessor pointer (previous free block)

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your information in the following struct.
 ********************************************************/
team_t team = {
    /* Your student ID */
    "20211522",
    /* Your full name*/
    "Junghwan Kim",
    /* Your email address */
    "jh0814@sogang.ac.kr",
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

static char *heap_list = NULL; // heap_list (was heap_listp)
static char *free_list = NULL; // free list (was free_listp)

static void *extend_heap(size_t words); // heap을 expand
static void *find_fit(size_t asize); // fit한 block 찾는 function
static void place(void* p, size_t size); // block을 크기만큼 allocate
static void* coalesce(void* bp); // free node를 합치는 작업 수행
static void insert_free_block(void *bp); // free block의 list에 삽입하는 function
static void remove_free_block(void *bp); // free block의 list에서 제거하는 function

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void){
    /* Create the initial empty heap */
    if ((heap_list = mem_sbrk(4*WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_list, 0);                             /* Alignment padding */
    PUT(heap_list + (1*WSIZE), PACK(DSIZE, 1));    /* Prologue header */
    PUT(heap_list + (2*WSIZE), PACK(DSIZE, 1));    /* Prologue footer */
    PUT(heap_list + (3*WSIZE), PACK(0, 1));        /* Epilogue header */
    heap_list += (2*WSIZE);

    free_list = NULL; // initialize free_list

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}

static void *extend_heap(size_t words){
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = MAX((words % 2 ? (words + 1) : words) * WSIZE, MINBLOCKSIZE);
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));              /* Free block header */
    PUT(FTRP(bp), PACK(size, 0));              /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));      /* New epilogue header */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

static void insert_free_block(void *bp){
    SUCC(bp) = free_list;
    PRED(bp) = NULL;
    if (free_list != NULL)
        PRED(free_list) = bp;
    free_list = bp;
}

static void remove_free_block(void *bp) {
    if (PRED(bp))
        SUCC(PRED(bp)) = SUCC(bp);
    else
        free_list = SUCC(bp);
    if (SUCC(bp))
        PRED(SUCC(bp)) = PRED(bp);
}

static void *find_fit(size_t asize){
     // Best-fit: 가장 낭비가 적은 블록 선택
    void *ptr = free_list; // search위한 initialize
    void *best_fit = NULL; //return할 value
    size_t min_diff = (size_t)-1; // 최대값으로 초기화

    while (ptr != NULL) {
        size_t block_size = GET_SIZE(HDRP(ptr));
        if (block_size >= asize) {
            size_t diff = block_size - asize;
            if (diff < min_diff) {
                min_diff = diff;
                best_fit = ptr;

                // perfect match면 바로 리턴
                if (diff == 0) break;
            }
        }
        ptr = SUCC(ptr); // move to successor
    }

    return best_fit; // NULL이면 no-fit
}

static void place(void *bp, size_t asize){
    size_t csize = GET_SIZE(HDRP(bp));
    remove_free_block(bp); // 가용 리스트에서 제거

    // 블록을 나눌 수 있을 만큼 크다면 분할
    if ((csize - asize) >= MINBLOCKSIZE) {
        // 현재 블록을 asize만큼 할당
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));

        // 남은 부분을 새 free block으로 설정
        void *next_bp = NEXT_BLKP(bp);
        size_t remaining = csize - asize;

        PUT(HDRP(next_bp), PACK(remaining, 0));
        PUT(FTRP(next_bp), PACK(remaining, 0));
        insert_free_block(next_bp);
    } else {
        // 블록이 너무 작아서 분할하지 않고 통째로 할당
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}

static void *coalesce(void *bp){
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (!prev_alloc) remove_free_block(PREV_BLKP(bp)); // 이전 block이 free block이면 우선 free_list에서 제거
    if (!next_alloc) remove_free_block(NEXT_BLKP(bp)); // 다음 block이 free block이면 우선 free_list에서 제거

    if (prev_alloc && next_alloc) { /* Case 1 */ // prev and next are both allocated
        insert_free_block(bp);
        return bp;
    }
    else if (prev_alloc && !next_alloc) { /* Case 2 */ // only next block is free
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size,0));
    }
    else if (!prev_alloc && next_alloc) { /* Case 3 */ // only previous block is free
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    else { /* Case 4 */ // prev and next are both free
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    insert_free_block(bp); // insert free block to free block list

    return bp;
}

void *mm_malloc(size_t size){
    size_t asize; /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;
    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

     /* Adjust block size to include overhead and alignment reqs. */
     if (size <= DSIZE)
        asize = MINBLOCKSIZE;
     else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

    if (asize < MINBLOCKSIZE)
        asize = MINBLOCKSIZE;

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

void mm_free(void *ptr){
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
}

void *mm_realloc(void *ptr, size_t size){
    if (ptr == NULL) return mm_malloc(size);
    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    size_t oldsize = GET_SIZE(HDRP(ptr));
    size_t newsize;

    if (size <= DSIZE)
        newsize = 2 * DSIZE;
    else
        newsize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

    if (newsize <= oldsize) {
        return ptr;  // 기존 블록 크기로 충분히 커버 가능한 case
    }

    void *next_bp = NEXT_BLKP(ptr);
    if (!GET_ALLOC(HDRP(next_bp))) {
        size_t next_size = GET_SIZE(HDRP(next_bp));
        if (oldsize + next_size >= newsize) {
            size_t total_size = oldsize + next_size;
            remove_free_block(next_bp);
            PUT(HDRP(ptr), PACK(total_size, 1));
            PUT(FTRP(ptr), PACK(total_size, 1));
            return ptr;
        }
    }

    void *newptr = mm_malloc(size);
    if (newptr == NULL) // malloc이 안 된 경우
        return NULL;

    size_t copySize = oldsize - DSIZE; // copy할 size
    if (size < copySize)
        copySize = size;

    memcpy(newptr, ptr, copySize);
    mm_free(ptr);
    return newptr;
}
