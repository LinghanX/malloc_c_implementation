#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int a = 1;
    printf("current mem address is %p\n", &a);
    size_t size = 12;
    void *mem = malloc(size);
    printf("successfully malloc'd %zu bytes at addr %p\n", size, mem);
    void *mem2 = malloc(size);
    printf("current mem address of mem2 is %p\n", mem2);
    assert(mem!=NULL);
    free(mem);

    return 0;
}

