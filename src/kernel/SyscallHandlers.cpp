#include "../../h/kernel/SyscallHandlers.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/TCB.hpp"
#include "../../h/kernel/SCB.hpp"
#include "../../h/kernel/TimerManager.hpp"
#include "../../h/kernel/ConsoleManager.hpp"

syscall_handler_t SyscallHandlers::handlers[SYS_COUNT];

void SyscallHandlers::mem_alloc()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    uint64 size_in_blocks = running->read_user_a1();
    void *p = MemoryAllocator::instance()->allocate(size_in_blocks);
    running->write_user_a0((uint64)p);
}

void SyscallHandlers::mem_free()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    void *p = (void *)running->read_user_a1();
    int ret = MemoryAllocator::instance()->free(p);
    running->write_user_a0(ret);
}

#define SUCCESS 0
#define ERROR_BAD_ALLOC -1

void SyscallHandlers::thread_create()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    UserThread **handle = (UserThread **)running->read_user_a1();

    UserThreadConfig config;

    config.user_task = (UserTask)running->read_user_a2();
    config.user_task_arg = (void *)running->read_user_a3();
    config.user_stack = (void *)running->read_user_a4();
    config.user_stack_size = running->read_user_a5();

    *handle = UserThread::create(config);
    if (handle == nullptr)
    {
        running->write_user_a0((uint64)ERROR_BAD_ALLOC);
    }
    else
    {
        (*handle)->activate();
        running->write_user_a0((uint64)SUCCESS);
    }
}

// no return
void SyscallHandlers::thread_exit()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    running->done = true;
    yield();
}

void SyscallHandlers::thread_dispatch()
{
    yield();
}

#define SUCCESS 0
#define ERROR_BAD_ALLOC -1
void SyscallHandlers::sem_open()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    SCB **handle = (SCB **)running->read_user_a1();
    unsigned val = (unsigned)running->read_user_a2();

    *handle = SCB::create(val);
    if (*handle == nullptr)
    {
        running->write_user_a0((uint64)ERROR_BAD_ALLOC);
    }
    else
    {
        running->write_user_a0((uint64)SUCCESS);
    }
}

#define SUCCESS 0
#define ERROR_BAD_HANDLE -1
void SyscallHandlers::sem_close()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    SCB *handle = (SCB *)running->read_user_a1();
    if (SCB::allocator->valid_ptr(handle))
    {
        handle->close();
        delete handle;
        running->write_user_a0((uint64)SUCCESS);
    }
    else
    {
        running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

#define ERROR_BAD_HANDLE -1
#define SEM_WAIT_SUCCEEDED 0
#define SEM_WAIT_FAILED 1

void SyscallHandlers::sem_wait()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    SCB *handle = (SCB *)running->read_user_a1();
    if (SCB::allocator->valid_ptr(handle))
    {
        int ret = handle->wait();
        running->write_user_a0((uint64)ret);
    }
    else
    {
        running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

#define SUCCESS 0
#define ERROR_BAD_HANDLE -1
void SyscallHandlers::sem_signal()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    SCB *handle = (SCB *)running->read_user_a1();
    if (SCB::allocator->valid_ptr(handle))
    {
        handle->signal();
        running->write_user_a0((uint64)SUCCESS);
    }
    else
    {
        running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

void SyscallHandlers::sem_timedwait()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    SCB *handle = (SCB *)running->read_user_a1();
    time_t time = (time_t)running->read_user_a2();
    if (SCB::allocator->valid_ptr(handle))
    {
        int ret = handle->timedwait(time);
        running->write_user_a0((uint64)ret);
    }
    else
    {
        running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

void SyscallHandlers::sem_trywait()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    SCB *handle = (SCB *)running->read_user_a1();
    if (SCB::allocator->valid_ptr(handle))
    {
        int ret = handle->try_wait();
        running->write_user_a0((uint64)ret);
    }
    else
    {
        running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

void SyscallHandlers::time_sleep()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    time_t time = (time_t)running->read_user_a1();
    TimerManager::instance()->put_to_sleep(time);
    running->write_user_a0((uint64)SUCCESS);
}

void SyscallHandlers::getc()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    char c = ConsoleManager::instance()->getc();
    running->write_user_a0((uint64)c);
}

void SyscallHandlers::putc()
{
    UserThread *running = (UserThread *)Scheduler::instance()->running;

    char c = (char)running->read_user_a1();
    ConsoleManager::instance()->putc(c);
}

void SyscallHandlers::init_handlers()
{
    for (uint64 i = 0; i < SYS_COUNT; i++)
    {
        handlers[i] = nullptr;
    }

    handlers[SYS_mem_alloc] = mem_alloc;
    handlers[SYS_mem_free] = mem_free;

    handlers[SYS_thread_create] = thread_create;
    handlers[SYS_thread_exit] = thread_exit;
    handlers[SYS_thread_dispatch] = thread_dispatch;

    handlers[SYS_sem_open] = sem_open;
    handlers[SYS_sem_close] = sem_close;
    handlers[SYS_sem_wait] = sem_wait;
    handlers[SYS_sem_signal] = sem_signal;
    handlers[SYS_sem_timedwait] = sem_timedwait;
    handlers[SYS_sem_trywait] = sem_trywait;

    handlers[SYS_time_sleep] = time_sleep;

    handlers[SYS_getc] = getc;
    handlers[SYS_putc] = putc;
}
