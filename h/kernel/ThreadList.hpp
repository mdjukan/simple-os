#pragma once
#include "../../lib/hw.h"
#include "PoolAllocator.hpp"

class TCB;

class ThreadListElement
{
    POOL_ALLOCATOR

public:
    ThreadListElement *next;
    ThreadListElement *prev;
    TCB *thread;

    ThreadListElement(TCB *thread) : next(nullptr), prev(nullptr), thread(thread) {}
    ThreadListElement() : next(nullptr), prev(nullptr), thread(nullptr) {}

    static ThreadListElement *create(TCB *thread);
};

class ThreadList
{
public:
    ThreadListElement sentinel;

    ThreadList()
    {
        sentinel.next = &sentinel;
        sentinel.prev = &sentinel;
    }

    TCB *get();
    virtual void put(TCB *thread);

    bool empty();
    void remove_thread(TCB *thread);

private:
    void remove_element(ThreadListElement *target);
};