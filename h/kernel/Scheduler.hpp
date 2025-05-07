#pragma once
#include "../../lib/hw.h"
#include "ThreadList.hpp"

class __Thread;

extern "C" uint64 sched(uint64 old_sp);

class Scheduler
{
public:
    static Scheduler *instance()
    {
        static Scheduler scheduler;
        return &scheduler;
    }

private:
    ThreadList ready;

public:
    uint64 active_threads = 0;

    __Thread *idle_thread = nullptr;
    __Thread *main_thread = nullptr;
    __Thread *running = nullptr;

    void add_new_thread(__Thread *thread);
    void put(__Thread *thread);
    __Thread *get();
};
