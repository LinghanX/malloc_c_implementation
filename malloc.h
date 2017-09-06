#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

struct arena_meta {
    pid_t pid; 
    size_t size;
    void * start;
    struct arena_meta *next;
    struct arena_meta *prev;
};

struct block_meta {
    size_t size;
    struct block_meta *next;
    struct block_meta *prev;
    int free;
    void *ptr;
    char data[1];
};

#define BLOCK_SIZE sizeof(struct block_meta) - sizeof(char *)
#define align8(x) (((((x) - 1) >> 4) << 4) + 8)
#define BIN_SIZE_1 64
#define BIN_SIZE_2 512
#define BIN_SIZE_3 4096

void *malloc(size_t) __attribute__ ((malloc));
void *calloc (size_t, size_t) __attribute__ ((malloc));
void *realloc (void *, size_t);
void free (void *);

extern int free_requests;
extern int arena_number;
extern int allocation_requests;
extern int blocks;
extern int used_blocks;
extern void *heap_start;
extern int initialized;
extern struct arena_meta *main_arena;
extern struct block_meta *base_1;
extern struct block_meta *base_2;
extern struct block_meta *base_3;
extern struct block_meta *base_other;
extern pthread_mutex_t lock1;
extern pthread_mutex_t lock2;


#endif
