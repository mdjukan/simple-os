#pragma once
#include "../../lib/hw.h"
#include "../defs.hpp"
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
    bool full();
    bool valid_ptr(void *p);

    static void init_allocators();
    static void realse_allocators();

private:
    uint64 elem_size;
    uint64 elem_count;

    uint64 free_count;

    void *data;
    bool *free;
};

#define POOL_ALLOCATOR                          \
public:                                         \
    static PoolAllocator *allocator;            \
    void *operator new(size_t size)             \
    {                                           \
        void *p = allocator->get();             \
        return p;                               \
    }                                           \
    void *operator new[](size_t size) = delete; \
    void operator delete(void *p) noexcept      \
    {                                           \
        allocator->release(p);                  \
    }                                           \
    void operator delete[](void *p) = delete;
