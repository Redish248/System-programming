#include "block.h"


mem_t* getPrevBlock(mem_t* block,mem_t* mem) {

    while (block != NULL) {
        if (block->next == mem) {
            return block;
        }
        block = block->next;
    }
    return NULL;
}


mem_t* getBlock(mem_t* mem, int8_t * size) {
    while (mem != NULL) {
        if (mem == (mem_t*)(size - sizeof(mem_t))) {
            return mem;
        }
        mem = mem ->next;
    }
    return NULL;
}


mem_t* getLastBlock(mem_t* mem) {
    while ( mem->next != NULL) {
        mem = mem->next;
    }
    return  mem;
}

mem_t* findBlock(mem_t* block, size_t capacity) {
    mem_t* mem = NULL;
    while (block != NULL) {
        if ( block->capacity >= capacity && block->is_free == 1) {
            mem = block;
            break;
        }
        block = block->next;
    }
    return  mem;
}