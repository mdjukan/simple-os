#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/Utils.hpp"
#include "../../h/RISCV.hpp"

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

extern "C" uint64 sched(uint64 old_kernel_sp)
{
    __Thread *old = Scheduler::instance()->running;
    old->kernel_sp = old_kernel_sp;
    old->sepc = RISCV::r_sepc();

    if (!old->done)
    {
        Scheduler::instance()->put(old);
    }
    else
    {
        //delete old;
    }

    __Thread *next = Scheduler::instance()->get();
    Scheduler::instance()->running = next;
    RISCV::w_sepc(next->sepc);
    
    RISCV::w_sscratch((uint64)next->kernel_stack_top);
    return Scheduler::instance()->running->kernel_sp;
}