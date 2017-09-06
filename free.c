#include "malloc.h"
#include "helpers.h"

void free(void *p) {
    struct block_meta * block; 
    struct block_meta *last;
    if(valid_addr(p)){
	pthread_mutex_lock(&lock1);
	block = get_block(p);
	block->free = 1;
	pthread_mutex_unlock(&lock1);
    } else {
	;
    }

    free_requests++;
}
