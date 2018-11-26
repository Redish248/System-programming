#include <stdio.h>
#include "mem.h"

mem_t* getPrevBlock(mem_t* block,mem_t* mem);
mem_t* getBlock(mem_t* mem, int8_t * size);
mem_t* getLastBlock(mem_t* mem);
mem_t* findBlock(mem_t* block, size_t capacity);