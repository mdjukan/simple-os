#include "../../h/kernel/Utils.hpp"
#include "../../h/RISCV.hpp"
#include "../../h/user/memory_operators.hpp"

#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/PoolAllocator.hpp"

#include "../../h/kernel/__Semaphore.hpp"

#include "../../h/kernel/syscall_handlers.hpp"
#include "../../h/kernel/handle_interrupt.hpp"
#include "../../h/user/thread_wrapper.hpp"
#include "../../tests/userMain.hpp"

extern "C" void interrupt_routine();

int main()
{
    Syscalls::init_handlers();
    __Thread::allocator = new PoolAllocator(sizeof(__Thread), THREAD_COUNT);
    __Semaphore::allocator = new PoolAllocator(sizeof(__Semaphore), SEMAPHORE_COUNT);

    RISCV::w_stvec((uint64)interrupt_routine);
    RISCV::clear_SPP();

    __Thread *main_thread = __Thread::create_main_thread();
    if (main_thread == nullptr)
    {
        error("main thread alloc failed");
    }

    Scheduler::instance()->running = main_thread;
    Scheduler::instance()->main_thread = main_thread;

    __Thread *idle_thread = __Thread::create(idle_worker, nullptr);
    if (idle_thread == nullptr)
    {
        error("idle thread alloc failed");
    }

    Scheduler::instance()->idle_thread = idle_thread;

    /*     __Thread *user_main = __Thread::create((body_t)userMain, nullptr); */
    __Thread *user_main = __Thread::create(timer_test, nullptr);
    if (user_main == nullptr)
    {
        error("user main alloc failed");
    }

    user_main->make_ready();

    yield();

    delete main_thread;
    delete idle_thread;
    delete __Thread::allocator;
    delete __Semaphore::allocator;
    return 0;
}