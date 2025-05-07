#pragma once
#include "../../lib/hw.h"

class __Thread;

class ThreadList
{
public:
    __Thread *head = nullptr;
    __Thread *tail = nullptr;
    uint64 thread_count = 0;

    __Thread *get();
    void put(__Thread *thread);

    bool empty();
};