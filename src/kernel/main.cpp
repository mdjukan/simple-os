#include "../../h/kernel/Utils.hpp"
#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/PoolAllocator.hpp"
#include "../../h/RISCV.hpp"
#include "../../h/kernel/handle_interrupt.hpp"
#include "../../h/kernel/syscall_handlers.hpp"
#include "../../h/user/thread_wrapper.hpp"

extern "C" void interrupt_routine();

int main()
{
    init_syscall_handlers();
    __Thread::allocator = new PoolAllocator(sizeof(__Thread), THREAD_COUNT);
    RISCV::w_stvec((uint64)interrupt_routine);
    RISCV::set_previous_privilege_to_user();
    RISCV::set_previous_interrupt_enable();

    __Thread *main_thread = __Thread::create();
    if (main_thread == nullptr)
    {
        error("main thread alloc failed");
    }
    Scheduler::instance()->running = main_thread;

    __Thread *t1 = __Thread::create(user_main, (void *)0);
    if (t1 == nullptr)
    {
        error("user main alloc failed");
    }

    while (!t1->done)
    {
        print_string("about to yield\n");
        yield();
        print_string("returned to main\n");
    }

    delete t1;
    delete main_thread;

    print_string("exiting main\n");

    delete __Thread::allocator;
    return 0;
}