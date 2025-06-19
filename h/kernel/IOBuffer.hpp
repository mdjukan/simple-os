#pragma once
#include "../../lib/hw.h"
#include "SCB.hpp"

template <uint64 IOBUF_SIZE>
class IOBuffer
{
    char data[IOBUF_SIZE];

    uint64 count = 0;
    uint64 r = 0;
    uint64 w = 0;

public:
    SCB items;
    SCB slots;

    IOBuffer() : items(0), slots(IOBUF_SIZE) {}

    bool empty()
    {
        return count == 0;
    }

    bool full()
    {
        return count == IOBUF_SIZE;
    }

    void put(char c)
    {
        slots.wait();

        data[w] = c;
        w = (w + 1) % IOBUF_SIZE;
        count += 1;

        items.signal();
    }

    char get()
    {
        items.wait();

        char c = data[r];
        r = (r + 1) % IOBUF_SIZE;
        count -= 1;

        slots.signal();
        return c;
    }
};