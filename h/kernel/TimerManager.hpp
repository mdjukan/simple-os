#pragma once
#include "ThreadList.hpp"
#include "TCB.hpp"
#include "Scheduler.hpp"
#include "../defs.hpp"

class TimerManager
{
public:
    static TimerManager *instance()
    {
        static TimerManager timer;
        return &timer;
    }

private:
    ThreadList sleeping;

public:
    void put_to_sleep(time_t time)
    {
        TCB *running = Scheduler::instance()->running;
        running->suspended = true;
        running->sleep_ticks = time;
        sleeping.put(running);
        yield();
    }

    void handle_interrupt()
    {
        ThreadListElement *current = sleeping.sentinel.next;
        while (current != &(sleeping.sentinel))
        {
            current->thread->sleep_ticks -= 1;

            if (current->thread->sleep_ticks == 0)
            {
                TCB *thread = current->thread;
                current = current->next;

                sleeping.remove_thread(thread);
                thread->suspended = false;

                if (thread->timed_wait)
                {
                    thread->wait_failed = true;
                    thread->blocked_on->remove_blocked(thread);
                }

                Scheduler::instance()->put(thread);
            }
            else
            {
                current = current->next;
            }
        }

        TCB *running = Scheduler::instance()->running;
        running->running_ticks += 1;
        if (running->running_ticks >= running->time_slice)
        {
            yield();
        }
    }

    void remove_sleeping(TCB *thread)
    {
        sleeping.remove_thread(thread);
    }
};