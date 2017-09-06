#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>
#include "malloc.h"
#include "helpers.h"
#include "free.h"
#include "calloc.h"
#include "realloc.h"

struct arena_meta;
struct block_meta;

int free_requests = 0;
int arena_number = 0;
int allocation_requests = 0;
int blocks = 0;
int used_blocks = 0;

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void *heap_start = NULL;



int initialized = 0;

struct arena_meta *main_arena = NULL;
struct block_meta *base_1 = NULL;
struct block_meta *base_2 = NULL;
struct block_meta *base_3 = NULL;
struct block_meta *base_other = NULL;

void *malloc(size_t size){
    pthread_mutex_lock(&lock1);
    if(!initialized) mem_init();
    if(heap_start == NULL) heap_start = sbrk(0);
    void* p = search_arena();
    if(size <= BIN_SIZE_1){
	p = find_bin(base_1, BIN_SIZE_1);
    } else if ( size <= BIN_SIZE_2) {
	p = find_bin(base_2, BIN_SIZE_2);
    } else if ( size <= BIN_SIZE_3) {
	p = find_bin(base_3, BIN_SIZE_3);
    } else if (size > BIN_SIZE_3) {
	p = find_bin(base_other, align8(size));
    }

    pthread_mutex_unlock(&lock1);
    if(p == NULL){
	printf("empty");
	exit(1);
    }
    allocation_requests++;
    return p;
}


void malloc_stats(){
    printf("Total size of arena allocated: %d\n", arena_number);
    printf("Total number of bins: %d\n", arena_number * 4);
    printf("Total number of blocks allocated: %d\n", blocks);
    printf("Total number of free blocks:  %d\n", free_bins());
    printf("Total number of allocation requests : %d\n", allocation_requests);
    printf("Total number of free requests : %d\n", free_requests);
}
