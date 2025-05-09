#pragma once
#include "../../lib/hw.h"
#include "ThreadList.hpp"
#include "PoolAllocator.hpp"

class __Semaphore
{
private:
    uint64 val;
    ThreadList blocked;

public:
    __Semaphore(int val = 1) : val(val) {}
    void signal();
    int wait();
    int try_wait();
    void close();
    int timedwait(time_t time);

    void remove_blocked(__Thread *thread);

public:
    static PoolAllocator *allocator;
    static __Semaphore *create(int val = 1);

    void *operator new(size_t size)
    {
        void *p = allocator->get();

        return p;
    }

    void *operator new[](size_t size) = delete;

    void operator delete(void *p) noexcept
    {
        allocator->release(p);
    }

    void operator delete[](void *p) = delete;
};