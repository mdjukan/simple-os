#include "../../h/kernel/TCB.hpp"
#include "../../h/kernel/Utils.hpp"
#include "../../h/RISCV.hpp"

void Scheduler::add_new_thread(TCB *thread)
{
    if (thread->type == ThreadType::USER)
    {
        active_user_threads += 1;
    }

    put(thread);
}

void Scheduler::put(TCB *thread)
{
    if (thread == main_thread || thread == idle_thread)
    {
        return;
    }

    ready.put(thread);
}

TCB *Scheduler::get()
{
    if (!ready.empty())
    {
        return ready.get();
    }

    //get je vratio nullptr -> console worker je suspedovan
    if (active_user_threads > 0)
    {
        return idle_thread;
    }
    else
    {
        return main_thread;
    }
}

extern "C" uint64 sched(uint64 old_sp)
{
    TCB *old = Scheduler::instance()->running;
    old->sp = old_sp;

    if (old->type == ThreadType::USER)
    {
        ((UserThread *)old)->sepc = RISCV::r_sepc();
    }

    if (old->done)
    {
        Scheduler::instance()->active_user_threads -= 1;
        delete old;
    }
    else if (!old->suspended)
    {
        Scheduler::instance()->put(old);
    }

    TCB *next = Scheduler::instance()->get();
    next->running_ticks = 0;
    Scheduler::instance()->running = next;

    if (next->type == ThreadType::USER)
    {
        RISCV::w_sepc(((UserThread *)next)->sepc);
    }

    RISCV::w_sscratch((uint64)next->stack_top);
    return Scheduler::instance()->running->sp;
}