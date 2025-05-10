#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/Utils.hpp"
#include "../../h/RISCV.hpp"

void Scheduler::add_new_thread(__Thread *thread)
{
    active_threads += 1;
    put(thread);
}

void Scheduler::put(__Thread *thread)
{
    if (thread == main_thread || thread == idle_thread)
    {
        return;
    }

    ready.put(thread);
}

__Thread *Scheduler::get()
{
    __Thread *thread = ready.get();

    if (thread != nullptr)
    {
        return thread;
    }

    if (active_threads > 0)
    {
        return idle_thread;
    }
    else
    {
        return main_thread;
    }
}

extern "C" uint64 sched(uint64 old_kernel_sp)
{
    __Thread *old = Scheduler::instance()->running;
    old->kernel_sp = old_kernel_sp;
    old->sepc = RISCV::r_sepc();

    if (old->done)
    {
        Scheduler::instance()->active_threads -= 1;
        delete old;
    }
    else if (!old->suspended)
    {
        Scheduler::instance()->put(old);
    }

    __Thread *next = Scheduler::instance()->get();
    next->running_ticks = 0;
    Scheduler::instance()->running = next;
    RISCV::w_sepc(next->sepc);

    RISCV::w_sscratch((uint64)next->kernel_stack_top);
    return Scheduler::instance()->running->kernel_sp;
}