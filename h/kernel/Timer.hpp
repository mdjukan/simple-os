#pragma once
#include "ThreadList.hpp"
#include "__Thread.hpp"
#include "Scheduler.hpp"
#include "../defs.hpp"

class Timer
{
public:
    static Timer *instance()
    {
        static Timer timer;
        return &timer;
    }

private:
    SleepingThreadList sleeping;
    TimedwaitThreadList timedwait;

public:
    void put_to_sleep(time_t time)
    {
        __Thread *running = Scheduler::instance()->running;
        running->suspended = true;
        running->sleep_ticks = time;
        sleeping.put(running);
        yield();
    }

    void add_timedwait(__Thread *thread, time_t time) {
        thread->sleep_ticks = time;
        timedwait.put(thread);
    }

    void handle_interrupt() {
        sleeping.update_ticks();
        timedwait.update_ticks();

        __Thread *running = Scheduler::instance()->running;
        running->running_ticks += 1;
        if (running->running_ticks >= DEFAULT_TIME_SLICE) {
            yield();
        }
    }

    void remove_timedwait(__Thread *thread) {
        timedwait.remove_thread(thread);
    }
};