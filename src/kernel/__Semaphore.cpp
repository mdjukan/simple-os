#include "../../h/kernel/__Semaphore.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/Timer.hpp"

PoolAllocator *__Semaphore::allocator = nullptr;

__Semaphore *__Semaphore::create(int val)
{
    if (allocator->full())
    {
        return nullptr;
    }
    else
    {
        return new __Semaphore(val);
    }
}

void __Semaphore::signal()
{
    if (blocked.empty())
    {
        val += 1;
    }
    else
    {
        __Thread *thread = blocked.get();
        thread->suspended = false;
        thread->wait_failed = false;
        if (thread->timed_wait)
        {
            Timer::instance()->remove_timedwait(thread);
        }

        Scheduler::instance()->put(thread);
    }
}

#define SEM_SUCCESS 0
#define SEM_FAIL 1

int __Semaphore::wait()
{
    if (val > 0)
    {
        val -= 1;
    }
    else
    {
        __Thread *thread = Scheduler::instance()->running;
        thread->suspended = true;
        blocked.put(thread);
        yield();

        if (thread->wait_failed)
        {
            return SEM_FAIL;
        }
    }

    return SEM_SUCCESS;
}

int __Semaphore::try_wait()
{
    if (val > 0)
    {
        val -= 1;
        return SEM_SUCCESS;
    }
    else
    {
        return SEM_FAIL;
    }
}

void __Semaphore::close()
{
    while (!blocked.empty())
    {
        __Thread *thread = blocked.get();
        if (thread->timed_wait)
        {
            Timer::instance()->remove_timedwait(thread);
        }

        thread->wait_failed = true;
        Scheduler::instance()->put(thread);
    }
}

int __Semaphore::timedwait(time_t time)
{
    if (val > 0)
    {
        val -= 1;
        return SEM_SUCCESS;
    }
    else
    {
        __Thread *thread = Scheduler::instance()->running;
        thread->suspended = true;
        thread->timed_wait = true;
        thread->blocked_on = this;
        blocked.put(thread);
        Timer::instance()->add_timedwait(thread, time);
        
        yield();

        thread->blocked_on = nullptr;
        thread->timed_wait = false;

        if (thread->wait_failed)
        {
            return SEM_FAIL;
        }
        else
        {
            return SEM_SUCCESS;
        }
    }
}

void __Semaphore::remove_blocked(__Thread *thread)
{
    blocked.remove_thread(thread);
}
