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
    Syscalls::init_handlers();
    __Thread::allocator = new PoolAllocator(sizeof(__Thread), THREAD_COUNT);
    
    RISCV::w_stvec((uint64)interrupt_routine);

    RISCV::clear_SPP();

    __Thread *main_thread = __Thread::create();
    if (main_thread == nullptr)
    {
        error("main thread alloc failed");
    }

    Scheduler::instance()->running = main_thread;
    Scheduler::instance()->main_thread = main_thread;

    __Thread *idle_thread = __Thread::create_idle();
    if (idle_thread == nullptr) {
        error("idle thread alloc failed");
    }

    __Thread *umain = __Thread::create(user_main, (void *)0);
    if (umain == nullptr)
    {
        error("user main alloc failed");
    }

    print_string("yielding from main\n");
    yield();
    print_string("exiting main\n");

    delete main_thread;
    delete idle_thread;
    delete __Thread::allocator;
    return 0;
}