#include "../../h/kernel/SCB.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/TCB.hpp"
#include "../../h/kernel/TimerManager.hpp"

PoolAllocator *SCB::allocator = nullptr;

SCB *SCB::create(int val)
{
    if (allocator->full())
    {
        return nullptr;
    }
    else
    {
        return new SCB(val);
    }
}

void SCB::signal()
{
    if (blocked.empty())
    {
        val += 1;
    }
    else
    {
        TCB *thread = blocked.get();
        thread->suspended = false;
        thread->wait_failed = false;
        if (thread->timed_wait)
        {
            TimerManager::instance()->remove_sleeping(thread);
        }

        Scheduler::instance()->put(thread);
    }
}

#define SEM_WAIT_SUCCEEDED 0
#define SEM_WAIT_FAILED 1

int SCB::wait()
{
    if (val > 0)
    {
        val -= 1;
    }
    else
    {
        TCB *thread = Scheduler::instance()->running;
        thread->suspended = true;
        blocked.put(thread);
        yield();

        if (thread->wait_failed)
        {
            return SEM_WAIT_FAILED;
        }
    }

    return SEM_WAIT_SUCCEEDED;
}

int SCB::try_wait()
{
    if (val > 0)
    {
        val -= 1;
        return SEM_WAIT_SUCCEEDED;
    }
    else
    {
        return SEM_WAIT_FAILED;
    }
}

void SCB::close()
{
    while (!blocked.empty())
    {
        TCB *thread = blocked.get();
        if (thread->timed_wait)
        {
            TimerManager::instance()->remove_sleeping(thread);
        }

        thread->wait_failed = true;
        Scheduler::instance()->put(thread);
    }
}

int SCB::timedwait(time_t time)
{
    if (val > 0)
    {
        val -= 1;
    }
    else
    {
        TCB *running = Scheduler::instance()->running;
        running->suspended = true;
        running->timed_wait = true;
        running->blocked_on = this;
        blocked.put(running);

        TimerManager::instance()->put_to_sleep(time);

        running->blocked_on = nullptr;
        running->timed_wait = false;

        if (running->wait_failed)
        {
            return SEM_WAIT_FAILED;
        }
    }

    return SEM_WAIT_SUCCEEDED;
}

void SCB::remove_blocked(TCB *thread)
{
    blocked.remove_thread(thread);
}
