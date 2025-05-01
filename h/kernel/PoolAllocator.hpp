#pragma once
#include "../../lib/hw.h"
#include "Utils.hpp"
#include "MemoryAllocator.hpp"

class PoolAllocator
{
public:
    void *operator new(size_t size)
    {
        return MemoryAllocator::instance()->allocate_bytes(size);
    }
    void *operator new[](size_t size) = delete;
    void operator delete(void *p) noexcept
    {
        MemoryAllocator::instance()->free(p);
    }
    void operator delete[](void *p) = delete;

    PoolAllocator(uint64 elem_size, uint64 elem_count);
    ~PoolAllocator();
    void *get();
    void release(void *p);
    bool has_free();

private:
    uint64 elem_size;
    uint64 elem_count;

    void *data;
    bool *free;
};