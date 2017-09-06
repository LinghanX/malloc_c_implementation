#ifndef HELPERS_H
#define HELPERS_H
#include "malloc.h"

struct block_meta *get_block(void *p);
int valid_addr(void *ptr);
int is_end_of_heap(struct block_meta *block);
void release(struct block_meta *block);
void copy_block(struct block_meta *src, struct block_meta *dst);
size_t align_mem(size_t size);
void *find_bin(void *base, size_t s);
void *search_arena();
struct block_meta *search_available_block(struct block_meta **last, size_t size, struct block_meta *base);
struct block_meta *ask_for_more_space(struct block_meta *last, size_t s);
struct block_meta *bin_init(struct block_meta *b, size_t s);
void mem_init();
int free_bins();

#endif
