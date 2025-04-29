#include "../../lib/hw.h"
#include "../../h/kernel/MemoryAllocator.hpp"
#include "../../h/kernel/PoolAllocator.hpp"

//TODO neko bolje mesto za ovaj makro
#define ROUNDUP(num, denom) (num) % (denom) == 0 ? (num) / (denom) : (num) / (denom) + 1

PoolAllocator::PoolAllocator(uint64 elem_size, uint64 elem_count)
    : elem_size(elem_size), elem_count(elem_count)
{
    data = MemoryAllocator::instance()->allocate(ROUNDUP(elem_count * elem_size, MEM_BLOCK_SIZE));
    free = (bool *)MemoryAllocator::instance()->allocate(ROUNDUP(elem_count * sizeof(bool), MEM_BLOCK_SIZE));
    // TODO error handeling
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
            return (uint8 *)data + i * elem_size;
        }
    }

    return nullptr;
}

void PoolAllocator::release(void *p)
{
    free[((uint8 *)p - (uint8 *)data) / elem_size] = true;
}
