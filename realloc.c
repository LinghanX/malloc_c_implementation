#include "malloc.h"
#include "helpers.h"
#include <string.h>

void *realloc(void *ptr, size_t size){
    size_t s;
    struct block_meta * b;
    struct block_meta *new;
    void *newp;
    if(!ptr)
	return (malloc(size));
    if(valid_addr(ptr)){
	s = align8(size);
	b = get_block(ptr);
	if(b->size >=s) {
	    ;
	}
	else {
	    newp= malloc(s);
	    if(!newp)
		return (NULL);
	    new = get_block(newp);
	    copy_block(b, new);
	    free(ptr);
	    free_requests++;
	    return (newp);
	}
	return (ptr);
    }
    return (NULL);
}

