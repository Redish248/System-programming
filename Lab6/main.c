#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main() {

   heap_init(5000);
    memalloc_debug_heap(stdout, HEAP_START);

        void* mem1 = _malloc(1000);
        void* mem2 = _malloc(2000);
        void* mem3 = _malloc(400);
        void* mem4 = _malloc(3000);
        memalloc_debug_heap(stdout, HEAP_START);

        _free(mem3);
        printf("mem3 free!\n");
        memalloc_debug_heap(stdout, HEAP_START);

        _free(mem1);
        printf("mem1 free!\n");
        memalloc_debug_heap(stdout,HEAP_START);

       _free(mem2);
        printf("mem2 free!\n");
        memalloc_debug_heap(stdout,HEAP_START);

        _free(mem4);
        printf("mem4 free!\n");
        memalloc_debug_heap(stdout,HEAP_START);


    return 0;
}