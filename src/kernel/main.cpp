#include "../../h/user/memory_operators.hpp"

#include "../../h/kernel/Utils.hpp"
#include "../../h/RISCV.hpp"

#include "../../h/kernel/PoolAllocator.hpp"

#include "../../h/kernel/TCB.hpp"
#include "../../h/kernel/Scheduler.hpp"

#include "../../h/kernel/SCB.hpp"

#include "../../h/kernel/SyscallHandlers.hpp"

#include "../../h/kernel/InterruptRoutine.hpp"
#include "../../h/user/thread_wrapper.hpp"

int main()
{
    SyscallHandlers::init_handlers();
    PoolAllocator::init_allocators();

    RISCV::w_stvec((uint64)interrupt_routine);
    RISCV::clear_SPP();

    TCB *main_thread = KernelThread::create();
    if (main_thread == nullptr)
    {
        error("main_thread construction failed");
    }

    Scheduler::instance()->running = main_thread;
    Scheduler::instance()->main_thread = main_thread;

    KernelThread *console_worker = KernelThread::create(ConsoleManager::output_worker_wrapper);
    if (console_worker == nullptr)
    {
        error("console_worker construction failed");
    }
    console_worker->activate();

    UserThreadConfig idle_thread_config;
    idle_thread_config.user_task = idle_worker;
    TCB *idle_thread = UserThread::create(idle_thread_config);
    if (idle_thread == nullptr)
    {
        error("idle_thread construction failed");
    }

    Scheduler::instance()->idle_thread = idle_thread;

    UserThreadConfig user_main_config;
    user_main_config.user_task = consumer_producer_test;
    user_main_config.user_task_arg = nullptr;
    UserThread *user_main = UserThread::create(user_main_config);

    if (user_main == nullptr)
    {
        error("user_main construction failed");
    }

    user_main->activate();

    yield();

    delete main_thread;
    delete idle_thread;
    PoolAllocator::realse_allocators();
    return 0;
}