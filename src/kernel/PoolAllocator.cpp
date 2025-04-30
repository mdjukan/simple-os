#include "../../lib/hw.h"
#include "../../h/kernel/MemoryAllocator.hpp"
#include "../../h/kernel/PoolAllocator.hpp"
#include "../../h/kernel/Utils.hpp"

PoolAllocator::PoolAllocator(uint64 elem_size, uint64 elem_count)
    : elem_size(elem_size), elem_count(elem_count)
{
    uint64 data_size = elem_size * elem_count;
    data = MemoryAllocator::instance()->allocate(div_roundup(data_size, MEM_BLOCK_SIZE));
    if (data==nullptr) {
        panic("PoolAllocator constructor");
    }

    uint64 free_size = sizeof(bool) * elem_count;
    free = (bool *)MemoryAllocator::instance()->allocate(div_roundup(free_size, MEM_BLOCK_SIZE));
    if (free==nullptr) {
        panic("PoolAllocator constructor");
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
            return (uint8 *)data + i * elem_size;
        }
    }

    return nullptr;
}

void PoolAllocator::release(void *p)
{
    free[((uint8 *)p - (uint8 *)data) / elem_size] = true;
}
