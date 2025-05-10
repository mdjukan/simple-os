#pragma once
#include "Thread.hpp"
#include "../syscall_c.h"
#include "../defs.hpp"

class PeriodicThread : public Thread
{
public:
    void terminate() {
        done = true;
    }

protected:
    PeriodicThread(time_t period) : period(period) {}
    virtual void periodicActivation() {}

    void run() override
    {
        while (!done)
        {
            thread_t tid;
            thread_create(&tid, PeriodicThread::periodic_wrapper, this);
            time_sleep(period);
        }
    }

private:
    time_t period;
    bool done = false;

public:
    static void periodic_wrapper(void *arg)
    {
        PeriodicThread *pt = (PeriodicThread*)arg;
        pt->periodicActivation();
    }
};