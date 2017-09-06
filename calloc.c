#include "malloc.h"
#include "helpers.h"
#include <string.h>

void *calloc(size_t n_elem, size_t size){
    size_t total_size = n_elem * size;
    pthread_mutex_lock(&lock2);

    void *p = malloc(total_size);
    if(!p) return NULL;
    pthread_mutex_unlock(&lock2);

    return memset(p, 0, total_size);
}

