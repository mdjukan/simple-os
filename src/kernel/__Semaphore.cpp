#include "../../h/kernel/__Semaphore.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/__Thread.hpp"

void __Semaphore::signal()
{
    if (blocked.empty())
    {
        val += 1;
    }
    else
    {
        __Thread *unblocked = blocked.get();
        Scheduler::instance()->put(unblocked);
    }
}

void __Semaphore::wait()
{
    if (val > 0)
    {
        val -= 1;
    }
    else
    {
        __Thread *running = Scheduler::instance()->running;
        running->suspended = true; // wait failed
        blocked.put(running);
        yield();
        running->suspended = false;
    }
}

#define SEM_SUCCESS 0
#define SEM_FAIL 1

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