#include "../../h/kernel/IOBuffer.hpp"
#include "../../h/kernel/Utils.hpp"

bool IOBuffer::empty()
{
    return count == 0;
}

bool IOBuffer::full()
{
    return count == IOBUFFER_SIZE;
}

void IOBuffer::put(char c)
{
    if (full())
    {
        error("put on full buffer");
    }

    data[w] = c;
    w = (w + 1) % IOBUFFER_SIZE;
    count += 1;
}

char IOBuffer::get()
{
    if (empty())
    {
        error("get on empty buffer");
    }

    char c = data[r];
    r = (r + 1) % IOBUFFER_SIZE;
    count -= 1;
    return c;
}
