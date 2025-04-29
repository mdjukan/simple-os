#pragma once
#include "../../lib/hw.h"
#include "MemoryAllocator.hpp"

class PoolAllocator
{
public:
    PoolAllocator(uint64 elem_size, uint64 elem_count);
    ~PoolAllocator();
    void *get();
    void release(void *p);

private:
    uint64 elem_size;
    uint64 elem_count;

    void *data;
    bool *free;
};