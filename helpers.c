#include "malloc.h"
#include "helpers.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

struct block_meta *search_available_block(struct block_meta **last, size_t size, struct block_meta * base){
    struct block_meta *block = base;
    while (block && ! (block->free && block-> size >= size)){
	*last = block;
	block = block->next;
    }
    return (block);
}

struct block_meta * get_block(void *p) {
    char *tmp;
    tmp = p;
    return (p = tmp -= BLOCK_SIZE);
}

int valid_addr(void *ptr) {
    if(ptr> heap_start && ptr < sbrk(0)){
	return (ptr == (get_block(ptr))->ptr);
    }
    return (0);
}

int is_end_of_heap(struct block_meta * block) {
    return (((char*)block + BLOCK_SIZE + block->size) == sbrk(0));
}

void release(struct block_meta * block) {
    struct block_meta *prev = block->prev;
    if(is_end_of_heap(block)){
	if(block->prev != NULL) {
	    block->prev->next = block->next;
	}
	brk(block);
    }
    if(prev->free) {
	release(prev);
    }
}


void copy_block(struct block_meta * src, struct block_meta * dst) {
    int *sdata, *ddata;
    sdata = src->ptr;
    ddata = dst->ptr;
    memcpy(ddata, sdata, src->size);
}

size_t align_mem(size_t size){
    if(size <= sysconf(_SC_PAGESIZE)){
	return sysconf(_SC_PAGESIZE);
    } else {
	int num_page = size / sysconf(_SC_PAGESIZE);
	return (num_page + 1) * sysconf(_SC_PAGESIZE);
    }
}

struct block_meta * ask_for_more_space(struct block_meta * last, size_t s){
    struct block_meta *b;
    b = sbrk(0);
    if(sbrk(BLOCK_SIZE + s) == (void*) -1) {
	perror("sbrk");
	return (NULL);
    }

    b-> size = s;
    b->next = NULL;
    b-> prev = last;
    b->ptr = b->data;
    if(last)
	last->next = b;
    b->free = 0;
    blocks += 1;
    return (b);
}

void *find_bin(void *base, size_t s) {
    if(heap_start == NULL) heap_start = sbrk(0); struct block_meta * b;
    struct block_meta *last;

    if(base){
	last = base;
	b = search_available_block(&last, s, base);
	if(b){
	    b->free = 0;
	} else {
	    b = ask_for_more_space(last, s);
	    if(!b)
		return (NULL);
	}
    } else {
	b = ask_for_more_space(NULL, s);
	if(!b)
	    return (NULL);
    }
    return (b->data);
}

void * search_arena(){
    struct arena_meta *current_arena;
    current_arena = main_arena;
    while(current_arena != NULL){
	if(current_arena->pid == getpid()){
	    return main_arena->start;
	} 
	current_arena = current_arena->next;
    }

    struct arena_meta *last_arena;
    last_arena = main_arena;
    while(last_arena->next != NULL){
	last_arena = last_arena->next;
    }

    struct arena_meta *new_arena;
    last_arena->next = new_arena;
    new_arena -> prev = last_arena;
    size_t arena_size = 10 * sysconf(_SC_PAGESIZE);
    new_arena = mmap(0, arena_size, PROT_READ|PROT_WRITE, 
	    MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    new_arena->pid = getpid();
    new_arena->start = (void *)new_arena + sizeof(struct arena_meta);
    new_arena->size = arena_size;
    new_arena->next = NULL;
    arena_number++;

    return new_arena->start;
}


struct block_meta * bin_init(struct block_meta * b, size_t s) {
    b->next = NULL;
    b->prev = NULL;
    b->size = s;
    b-> ptr = NULL;
    return b;
}

void mem_init(){
    size_t arena_size = 10 * sysconf(_SC_PAGESIZE);
    main_arena = mmap(0, arena_size, PROT_READ|PROT_WRITE, 
	    MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    main_arena->pid = getpid();
    main_arena->start = (void *)main_arena + sizeof(struct arena_meta);
    main_arena->size = arena_size;
    main_arena->prev = NULL;
    main_arena->next = NULL;
    
    void *base = main_arena->start;
    sbrk(BLOCK_SIZE + BIN_SIZE_1);
    base_1 = bin_init(base, BIN_SIZE_1);

    base = sbrk(0);
    sbrk(BLOCK_SIZE + BIN_SIZE_2);
    base_2 = bin_init(base, BIN_SIZE_2);

    base = sbrk(0);
    sbrk(BLOCK_SIZE + BIN_SIZE_2);
    base_3 = bin_init(base, BIN_SIZE_3);

    base = sbrk(0);
    sbrk(BLOCK_SIZE + BIN_SIZE_2);
    base_other = bin_init(base, BIN_SIZE_3);

    initialized = 1;

    arena_number++;
    blocks += 4;
}

int free_bins(){
    int i = 0;
    while(base_1 != NULL){
	if(base_1 -> free){
	    i++;
	}
	base_1 = base_1 -> next;
    }
    while(base_2 != NULL){
	if(base_2 -> free){
	    i++;
	}
	base_2 = base_2 -> next;
    }
    while(base_3 != NULL){
	if(base_3 -> free){
	    i++;
	}
	base_3 = base_3 -> next;
    }
    while(base_other != NULL){
	if(base_other -> free){
	    i++;
	}
	base_other = base_other -> next;
    }

    return i;
}
