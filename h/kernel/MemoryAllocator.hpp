#pragma once
#include "../../lib/hw.h"

#define HEAP_START_ADDR_FIX (void*)((uint8*)HEAP_START_ADDR + 10 * 1024)

class MemoryAllocator
{
public:
    static MemoryAllocator *instance()
    {
        static MemoryAllocator allocator;
        return &allocator;
    }

private:
    long *map = (long *)HEAP_START_ADDR_FIX;
    uint64 BLOCK_COUNT;

    MemoryAllocator();
    void *block_address(uint64 block);
    long distnace_from_start(void *p);
    void try_join_next(uint64 block);
    void try_join_prev(uint64 block);

public:
    void *allocate(uint64 size_in_blocks);
    void *allocate_bytes(uint64 size);
    void free(void *p);

    //debug
    void print_map();
};