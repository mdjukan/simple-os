#pragma once
#include "../../lib/hw.h"
#include "./printing.hpp"

class MemoryAllocator {
    private:

    long *map = (long*)HEAP_START_ADDR;
    uint64 BLOCK_COUNT;

    MemoryAllocator() {
        uint64 HEAP_SIZE = (uint8*)HEAP_END_ADDR - (uint8*)HEAP_START_ADDR;
        BLOCK_COUNT = HEAP_SIZE / MEM_BLOCK_SIZE;

        for (uint64 i=0; i<BLOCK_COUNT; i++) {
            map[i] = 0;
        }

        //map size in blocks:262101
        uint64 MAP_SIZE = (BLOCK_COUNT * sizeof(long)) / MEM_BLOCK_SIZE;


        map[0] = -MAP_SIZE; //- zauzet
        map[MAP_SIZE-1] = -MAP_SIZE;
        map[MAP_SIZE] = BLOCK_COUNT - MAP_SIZE; // + slobodan
        map[BLOCK_COUNT-1] = BLOCK_COUNT - MAP_SIZE;
    }

    void *block_address(uint64 block) {
        return (uint8*)HEAP_START_ADDR + block * MEM_BLOCK_SIZE;
    }

    long distnace_from_start(void *p) {
        return (long)((uint8*)p - (uint8*)HEAP_START_ADDR);
    }

    void try_join_next(uint64 block) {
        uint64 size = map[block];
        uint64 next = block + size;
        if (next > BLOCK_COUNT || map[next] < 0) {
            return;
        }

        uint64 next_size = map[next];
        print_num(next_size);
        print_string("\n");

        uint64 new_size = size + next_size;
        print_num(new_size);
        print_string("\n");

        map[block] = new_size;
        map[block+(new_size-1)] = new_size;

        if (size!=1) {
            map[block + (size-1)] = 0;
        }

        if (next_size!=1) {
            map[next] = 0;
        }
    }

    void try_join_prev(uint64 block) {
        if (block==0 || map[block-1] < 0) {
            return;
        }

        uint64 prev = block - map[block-1];
        try_join_next(prev);
    }

    public:
    void *allocate(uint64 size_in_blocks) {
        uint64 block = 0;
        while (block < BLOCK_COUNT) {
            if (map[block] < 0) {
                block = block - map[block];
            } else if ((uint64)map[block] < size_in_blocks) {
                block = block + map[block];
            } else {
                if ((uint64)map[block]==size_in_blocks) {
                    map[block + (map[block]-1)] *= -1;
                    map[block] *= 1;
                } else {
                    uint64 chunk_size = map[block];
                    uint64 leftover = map[block] - size_in_blocks;
                    map[block] = -size_in_blocks;
                    map[block+(size_in_blocks-1)] = -size_in_blocks;
                    map[block+size_in_blocks] = leftover;
                    map[block+(chunk_size-1)] = leftover;
                }

                return block_address(block);
            }
        }

        return nullptr;
    }

    void free(void *p) {
        long distance = (long)((uint8*)p - (uint8*)HEAP_START_ADDR);
        if (distance % MEM_BLOCK_SIZE != 0 || distance < 0) {
            return;
        }

        uint64 block = distance / MEM_BLOCK_SIZE;
        if (block > BLOCK_COUNT) {
            return;
        }

        if (map[block] >= 0) {
            return;
        }

        long size = -map[block];

        map[block+(size-1)] = size;
        map[block] = size;

        try_join_next(block);
        try_join_prev(block);
    }

    void print_map() {
        for (uint64 i=0; i<30; i++) {
            print_num(map[262099 + i]);
            print_string(" ");
        }
        print_string("\n");
    }

    static MemoryAllocator* instance() {
        static MemoryAllocator allocator;
        return &allocator;
    }

};