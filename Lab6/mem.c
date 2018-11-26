#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "block.h"


mem_t* allocate(mem_t* lastBlock, size_t query) {
    mem_t* newBlock;
    void* mem = (void*)((char*)lastBlock + sizeof(mem_t) + lastBlock->capacity);
    query += sizeof(mem_t);
    newBlock = mmap(mem, query, PROT_EXEC | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (newBlock == MAP_FAILED) {
        newBlock = mmap(mem,query,PROT_EXEC | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (newBlock == MAP_FAILED) {
            return NULL;
        }
    }
    if (lastBlock->is_free == 0) {
        newBlock->next = NULL;
        newBlock->capacity = query - sizeof(mem_t);
        newBlock->is_free = 1;
        lastBlock->next = newBlock;
    } else {
        if (mem == (void*)newBlock) {
            lastBlock->capacity += query;
            return lastBlock;
        } else {
            newBlock->next = NULL;
            newBlock->capacity = query - sizeof(mem_t);
            newBlock->is_free = 1;
            lastBlock->next = newBlock;
        }

    }
    return newBlock;
}

/**
 * This function initializes memory for a new heap
 * @param initial_size new heap size
 * @return pointer to a new part of memory
 */
void* heap_init( size_t initial_size ) {
    if ((initial_size <= 0)) {
        return  NULL;
    }

    mem_t* heaps = (mem_t*)mmap(HEAP_START,initial_size, PROT_EXEC | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (heaps == MAP_FAILED) {
        return NULL;
    }


    heaps->next = NULL;
    heaps->capacity = initial_size;
    heaps->is_free = 1;

    return heaps;
}


/**
 * This function allocates the memory
 * @param query size to be allocated
 * @return pointer to the new part of memory
 */
void* _malloc( size_t query ) {
    if (query <= 0) {
        return NULL;
    }

    mem_t* block = findBlock(HEAP_START, query);

    if (block == NULL) {
        block = getLastBlock(HEAP_START);
        block = allocate(block, query);
    }

    if (block->capacity - sizeof(mem_t) > query) {
        mem_t *new = (mem_t*) ((char *) block + sizeof(mem_t) + query);
        new->capacity = block->capacity  - query;
        new->is_free = 1;
        new->next = block->next;
        block->capacity = query;
        block->next = new;
    }

    block->is_free = 0;
    return (char*)block + sizeof(mem_t);
}


/**
 * This function cleans the memory
 * @param mem pointer to the part of memory to be cleaned
 */
void _free(void* mem ) {
    mem_t* block = getBlock(HEAP_START, mem);
    mem_t* prevBlock = getPrevBlock(HEAP_START, block);

    if (block) {
        //clean all next free blocks
        block->is_free = 1;
            if (block->next) {
                if (block->next->is_free == 1) {
                    if (block + block->capacity >= block->next) {
                        block->capacity += block->next->capacity;
                    }
                    block->next = block->next->next;
                }
            }
            //clean all previous blocks
            if (prevBlock) {
                if (prevBlock->is_free == 1) {
                    if (block - prevBlock->capacity < prevBlock) {
                        prevBlock->capacity += block->capacity;
                    }
                    prevBlock->next = block->next;
                }
            }

    }
}


