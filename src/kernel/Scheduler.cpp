#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/Utils.hpp"

void Scheduler::put(__Thread *thread)
{
    if (ready_head == nullptr)
    {
        ready_head = thread;
        ready_tail = thread;
    }
    else
    {
        ready_tail->next = thread;
        ready_tail = thread;
    }
}

__Thread *Scheduler::get()
{
    if (ready_head == nullptr)
    {
        return nullptr;
    }

    __Thread *thread = ready_head;
    ready_head = thread->next;
    thread->next = nullptr;

    return thread;
}

extern "C" uint64 sched(uint64 old_sp) {
    __Thread *old = Scheduler::instance()->running;
    old->sp = old_sp;

    if (!old->done) {
        Scheduler::instance()->put(old);
    }

    Scheduler::instance()->running = Scheduler::instance()->get();
    return Scheduler::instance()->running->sp;
}