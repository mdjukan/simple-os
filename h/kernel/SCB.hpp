#pragma once
#include "../../lib/hw.h"
#include "PoolAllocator.hpp"
#include "ThreadList.hpp"

// SCB - Semaphore Control Block
class SCB
{
    POOL_ALLOCATOR

private:
    uint64 val;
    ThreadList blocked;

public:
    SCB(int val = 1) : val(val) {}
    void signal();
    int wait();
    int try_wait();
    void close();
    int timedwait(time_t time);

    void remove_blocked(TCB *thread);

public:
    static SCB *create(int val = 1);
};