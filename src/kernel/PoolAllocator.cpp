#include "../../lib/hw.h"
#include "../../h/kernel/MemoryAllocator.hpp"
#include "../../h/kernel/PoolAllocator.hpp"
#include "../../h/kernel/Utils.hpp"

PoolAllocator::PoolAllocator(uint64 elem_size, uint64 elem_count)
    : elem_size(elem_size), elem_count(elem_count), free_count(elem_count)
{
    data = MemoryAllocator::instance()->allocate_bytes(elem_size * elem_count);
    if (data == nullptr)
    {
        error("PoolAllocator constructor failed");
    }

    free = (bool *)MemoryAllocator::instance()->allocate_bytes(sizeof(bool) * elem_count);
    if (free == nullptr)
    {
        error("PoolAllocator constructor failed");
    }

    for (uint64 i = 0; i < elem_count; i++)
    {
        free[i] = true;
    }
}

PoolAllocator::~PoolAllocator()
{
    MemoryAllocator::instance()->free(data);
    MemoryAllocator::instance()->free(free);
}

void *PoolAllocator::get()
{
    for (uint64 i = 0; i < elem_count; i++)
    {
        if (free[i])
        {
            free[i] = false;
            free_count -= 1;
            return (uint8 *)data + i * elem_size;
        }
    }

    return nullptr;
}

void PoolAllocator::release(void *p)
{
    long idx = ((uint8 *)p - (uint8 *)data) / elem_size;
    if (idx < 0 || idx >= (long)elem_count)
    {
        error("PoolAllocator invalid free");
    }

    free_count += 1;
    free[idx] = true;
}

bool PoolAllocator::full()
{
    return free_count == 0;
}
