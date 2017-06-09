#include <stdio.h>
#include <stdlib.h>


void *emalloc(int size) {
    void *ptr = malloc(size);
    if (!ptr) {
        perror("Out of memory");
        exit(1);
    }

    return ptr;
}
