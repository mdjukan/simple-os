#pragma once
#include "../../lib/hw.h"

class __Thread;

class Scheduler
{
public:
    static Scheduler *instance()
    {
        static Scheduler scheduler;
        return &scheduler;
    }

private:
    __Thread *ready_head = nullptr;
    __Thread *ready_tail = nullptr;

public:
    __Thread *running = nullptr;
    void put(__Thread *thread);
    __Thread *get();
};

extern "C" uint64 sched(uint64 old_sp);
