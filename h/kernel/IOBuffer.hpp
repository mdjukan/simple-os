#pragma once
#include "../../lib/hw.h"

#define IOBUFFER_SIZE 128
class IOBuffer
{
    char data[IOBUFFER_SIZE];

    uint64 count = 0;
    uint64 r = 0;
    uint64 w = 0;

public:
    bool empty();
    bool full();
    void put(char c);
    char get();
};