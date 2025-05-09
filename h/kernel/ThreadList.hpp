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
    virtual void put(__Thread *thread);

    bool empty();
    void remove_thread(__Thread *thread);
};

class SleepingThreadList : public ThreadList
{
public:
    void put(__Thread *thread) override;
    virtual void update_ticks();
};

class TimedwaitThreadList : public SleepingThreadList {
public:
    void update_ticks() override;
};