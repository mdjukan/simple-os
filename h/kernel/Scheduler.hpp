#pragma once
#include "../../lib/hw.h"
#include "ThreadList.hpp"

class TCB;

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
    uint64 active_user_threads = 0;

    TCB *idle_thread = nullptr;
    TCB *main_thread = nullptr;
    TCB *running = nullptr;

    void add_new_thread(TCB *thread);
    void put(TCB *thread);
    TCB *get();
};
