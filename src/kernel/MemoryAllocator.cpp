#include "../../lib/hw.h"
#include "../../h/kernel/Utils.hpp"
#include "../../h/kernel/MemoryAllocator.hpp"

MemoryAllocator::MemoryAllocator()
{
    uint64 HEAP_SIZE = (uint8 *)HEAP_END_ADDR - (uint8 *)HEAP_START_ADDR;
    BLOCK_COUNT = HEAP_SIZE / MEM_BLOCK_SIZE;

    for (uint64 i = 0; i < BLOCK_COUNT; i++)
    {
        map[i] = 0;
    }

    // map size in blocks:262101
    uint64 map_size_bytes = BLOCK_COUNT * sizeof(long);
    uint64 MAP_SIZE = map_size_bytes / MEM_BLOCK_SIZE;
    if (map_size_bytes % MEM_BLOCK_SIZE != 0)
    {
        MAP_SIZE += 1;
    }

    map[0] = -MAP_SIZE; //- zauzet
    map[MAP_SIZE - 1] = -MAP_SIZE;
    map[MAP_SIZE] = BLOCK_COUNT - MAP_SIZE; // + slobodan
    map[BLOCK_COUNT - 1] = BLOCK_COUNT - MAP_SIZE;
}

void *MemoryAllocator::block_address(uint64 block)
{
    return ((uint8 *)HEAP_START_ADDR) + block * MEM_BLOCK_SIZE;
}

long MemoryAllocator::distnace_from_start(void *p)
{
    return (long)((uint8 *)p - (uint8 *)HEAP_START_ADDR);
}

void MemoryAllocator::try_join_next(uint64 block)
{
    uint64 size = map[block];
    uint64 next = block + size;
    if (next > BLOCK_COUNT || map[next] < 0)
    {
        return;
    }

    uint64 next_size = map[next];
    uint64 new_size = size + next_size;

    map[block] = new_size;
    map[block + (new_size - 1)] = new_size;

    if (size != 1)
    {
        map[block + (size - 1)] = 0;
    }

    if (next_size != 1)
    {
        map[next] = 0;
    }
}

void MemoryAllocator::try_join_prev(uint64 block)
{
    if (block == 0 || map[block - 1] < 0)
    {
        return;
    }

    uint64 prev = block - map[block - 1];
    try_join_next(prev);
}

void *MemoryAllocator::allocate(uint64 size_in_blocks)
{
    if (size_in_blocks == 0)
    {
        error("zero alloc");
    }

    uint64 block = 0;
    while (block < BLOCK_COUNT)
    {
        if (map[block] < 0)
        {
            block = block - map[block];
        }
        else if ((uint64)map[block] < size_in_blocks)
        {
            block = block + map[block];
        }
        else
        {
            if ((uint64)map[block] == size_in_blocks)
            {
                long size = map[block];
                map[block + (size - 1)] = -size;
                map[block] = -size;
            }
            else
            {
                uint64 chunk_size = map[block];
                uint64 leftover = chunk_size - size_in_blocks;
                map[block] = -size_in_blocks;
                map[block + (size_in_blocks - 1)] = -size_in_blocks;

                map[block + size_in_blocks] = leftover;
                map[block + (chunk_size - 1)] = leftover;
            }

            return block_address(block);
        }
    }

    return nullptr;
}

void *MemoryAllocator::allocate_bytes(uint64 size)
{
    uint64 size_in_blocks = size / MEM_BLOCK_SIZE;
    if (size % MEM_BLOCK_SIZE != 0)
    {
        size_in_blocks += 1;
    }

    return allocate(size_in_blocks);
}

#define SUCCESS 0
#define ERROR_ADDR_NOT_ALIGNED -1
#define ERROR_ADDR_NOT_ON_HEAP -2
#define ERROR_NOT_ALLOCATED -3

int MemoryAllocator::free(void *p)
{
    long distance = (long)((uint8 *)p - (uint8 *)HEAP_START_ADDR);
    if (distance % MEM_BLOCK_SIZE != 0)
    {
        return ERROR_ADDR_NOT_ALIGNED;
    }

    uint64 block = distance / MEM_BLOCK_SIZE;
    if (distance < 0 || block > BLOCK_COUNT)
    {
        return ERROR_ADDR_NOT_ON_HEAP;
    }

    if (map[block] >= 0)
    {
        return ERROR_NOT_ALLOCATED;
    }

    long size = -map[block];

    map[block + (size - 1)] = size;
    map[block] = size;

    try_join_next(block);
    try_join_prev(block);

    return SUCCESS;
}

void MemoryAllocator::print_map()
{
    for (uint64 i = 0; i < 80; i++)
    {
        print_num(map[262095 + i]);
        print(" ");
    }
    print("\n");
}