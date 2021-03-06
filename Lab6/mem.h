#ifndef _MEM_H_
#define _MEM_H_

#include <stddef.h>
#include  <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdbool.h>

#define  HEAP_START  ((void*)0x04040000)


#pragma pack(push, 1)
typedef struct mem_t  {
    struct mem_t* next;
    size_t capacity;
    bool is_free;
} mem_t;
#pragma pack(pop)

void* _malloc( size_t query );

void  _free( void* mem );

void* heap_init( size_t initial_size );

#define DEBUG_FIRST_BYTES 4

void memalloc_debug_struct_info( FILE* f, mem_t const * const address );

void memalloc_debug_heap( FILE* f, mem_t const * ptr );

#endif