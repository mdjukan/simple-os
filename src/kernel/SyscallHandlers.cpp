#include "../../h/kernel/SyscallHandlers.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/__Semaphore.hpp"
#include "../../h/kernel/Timer.hpp"
#include "../../h/kernel/__Console.hpp"

syscall_handler_t SyscallHandlers::handlers[SYS_COUNT];

void SyscallHandlers::mem_alloc()
{
    uint64 size_in_blocks = Scheduler::instance()->running->read_user_a1();
    void *p = MemoryAllocator::instance()->allocate(size_in_blocks);
    Scheduler::instance()->running->write_user_a0((uint64)p);
}

void SyscallHandlers::mem_free()
{
    void *p = (void *)Scheduler::instance()->running->read_user_a1();
    int ret = MemoryAllocator::instance()->free(p);
    Scheduler::instance()->running->write_user_a0(ret);
}

#define SUCCESS 0
#define ERROR_BAD_ALLOC -1

void SyscallHandlers::thread_create()
{
    __Thread **handle = (__Thread **)Scheduler::instance()->running->read_user_a1();
    body_t body = (body_t)Scheduler::instance()->running->read_user_a2();
    void *arg = (void *)Scheduler::instance()->running->read_user_a3();
    void *user_stack_top = (void *)Scheduler::instance()->running->read_user_a4();

    *handle = __Thread::create(body, arg, user_stack_top);
    if (handle == nullptr)
    {
        Scheduler::instance()->running->write_user_a0((uint64)-1);
    }
    else
    {
        (*handle)->make_ready();
        Scheduler::instance()->running->write_user_a0((uint64)0);
    }
}

//no return
void SyscallHandlers::thread_exit()
{
    Scheduler::instance()->running->done = true;
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
    __Semaphore **handle = (__Semaphore **)Scheduler::instance()->running->read_user_a1();
    unsigned val = (unsigned)Scheduler::instance()->running->read_user_a2();

    *handle = __Semaphore::create(val);
    if (*handle == nullptr)
    {
        Scheduler::instance()->running->write_user_a0((uint64)ERROR_BAD_ALLOC);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)SUCCESS);
    }
}

#define SUCCESS 0
#define ERROR_BAD_HANDLE -1
void SyscallHandlers::sem_close()
{
    __Semaphore *handle = (__Semaphore *)Scheduler::instance()->running->read_user_a1();
    if (__Semaphore::allocator->valid_ptr(handle))
    {
        handle->close();
        delete handle;
        Scheduler::instance()->running->write_user_a0((uint64)SUCCESS);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

#define ERROR_BAD_HANDLE -1
#define SEM_WAIT_SUCCEEDED 0
#define SEM_WAIT_FAILED 1

void SyscallHandlers::sem_wait()
{
    __Semaphore *handle = (__Semaphore *)Scheduler::instance()->running->read_user_a1();
    if (__Semaphore::allocator->valid_ptr(handle))
    {
        int ret = handle->wait();
        Scheduler::instance()->running->write_user_a0((uint64)ret);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

#define SUCCESS 0
#define ERROR_BAD_HANDLE -1
void SyscallHandlers::sem_signal()
{
    __Semaphore *handle = (__Semaphore *)Scheduler::instance()->running->read_user_a1();
    if (__Semaphore::allocator->valid_ptr(handle))
    {
        handle->signal();
        Scheduler::instance()->running->write_user_a0((uint64)SUCCESS);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

void SyscallHandlers::sem_timedwait()
{
    __Semaphore *handle = (__Semaphore *)Scheduler::instance()->running->read_user_a1();
    time_t time = (time_t)Scheduler::instance()->running->read_user_a2();
    if (__Semaphore::allocator->valid_ptr(handle))
    {
        int ret = handle->timedwait(time);
        Scheduler::instance()->running->write_user_a0((uint64)ret);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

void SyscallHandlers::sem_trywait()
{
    __Semaphore *handle = (__Semaphore *)Scheduler::instance()->running->read_user_a1();
    if (__Semaphore::allocator->valid_ptr(handle))
    {
        int ret = handle->try_wait();
        Scheduler::instance()->running->write_user_a0((uint64)ret);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)ERROR_BAD_HANDLE);
    }
}

void SyscallHandlers::time_sleep()
{
    time_t time = (time_t)Scheduler::instance()->running->read_user_a1();
    Timer::instance()->put_to_sleep(time);
    Scheduler::instance()->running->write_user_a0((uint64)SUCCESS);
}

void SyscallHandlers::getc()
{
    char c = __Console::instance()->getc();
    Scheduler::instance()->running->write_user_a0((uint64)c);
}

void SyscallHandlers::putc()
{
    char c = (char)Scheduler::instance()->running->read_user_a1();
    __Console::instance()->putc(c);
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
