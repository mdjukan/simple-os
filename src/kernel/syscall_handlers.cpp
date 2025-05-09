#include "../../h/kernel/syscall_handlers.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/__Semaphore.hpp"
#include "../../h/kernel/Timer.hpp"

syscall_handler_t Syscalls::handlers[SYS_COUNT];

void Syscalls::sys_mem_alloc()
{
    uint64 size_in_blocks = Scheduler::instance()->running->read_user_a1();
    void *p = MemoryAllocator::instance()->allocate(size_in_blocks);
    Scheduler::instance()->running->write_user_a0((uint64)p);
}

void Syscalls::sys_mem_free()
{
    void *p = (void *)Scheduler::instance()->running->read_user_a1();
    int ret = MemoryAllocator::instance()->free(p);
    Scheduler::instance()->running->write_user_a0(ret);
}

void Syscalls::sys_thread_create()
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

void Syscalls::sys_thread_exit()
{
    Scheduler::instance()->running->done = true;
    yield();
}

void Syscalls::sys_thread_dispatch()
{
    yield();
}

void Syscalls::sys_sem_open()
{
    __Semaphore **handle = (__Semaphore **)Scheduler::instance()->running->read_user_a1();
    unsigned val = (unsigned)Scheduler::instance()->running->read_user_a2();

    *handle = __Semaphore::create(val);
    if (*handle == nullptr)
    {
        Scheduler::instance()->running->write_user_a0((uint64)-1);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)0);
    }
}
void Syscalls::sys_sem_close()
{
    __Semaphore *handle = (__Semaphore *)Scheduler::instance()->running->read_user_a1();
    if (__Semaphore::allocator->valid_ptr(handle))
    {
        handle->close();
        delete handle;
        Scheduler::instance()->running->write_user_a0((uint64)0);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)-1);
    }
}

void Syscalls::sys_sem_wait()
{
    __Semaphore *handle = (__Semaphore *)Scheduler::instance()->running->read_user_a1();
    if (__Semaphore::allocator->valid_ptr(handle))
    {
        int ret = handle->wait();
        Scheduler::instance()->running->write_user_a0((uint64)ret);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)-1);
    }
}

void Syscalls::sys_sem_signal()
{
    __Semaphore *handle = (__Semaphore *)Scheduler::instance()->running->read_user_a1();
    if (__Semaphore::allocator->valid_ptr(handle))
    {
        handle->signal();
        Scheduler::instance()->running->write_user_a0((uint64)0);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)-1);
    }
}

void Syscalls::sys_sem_timedwait()
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
        Scheduler::instance()->running->write_user_a0((uint64)-1);
    }
}

void Syscalls::sys_sem_trywait()
{
    __Semaphore *handle = (__Semaphore *)Scheduler::instance()->running->read_user_a1();
    if (__Semaphore::allocator->valid_ptr(handle))
    {
        int ret = handle->try_wait();
        Scheduler::instance()->running->write_user_a0((uint64)ret);
    }
    else
    {
        Scheduler::instance()->running->write_user_a0((uint64)-1);
    }
}

void Syscalls::sys_time_sleep() {
    time_t time = (time_t)Scheduler::instance()->running->read_user_a1();
    Timer::instance()->put_to_sleep(time);
}

void Syscalls::init_handlers()
{
    for (uint64 i = 0; i < SYS_COUNT; i++)
    {
        handlers[i] = nullptr;
    }

    handlers[SYS_mem_alloc] = sys_mem_alloc;
    handlers[SYS_mem_free] = sys_mem_free;

    handlers[SYS_thread_create] = sys_thread_create;
    handlers[SYS_thread_exit] = sys_thread_exit;
    handlers[SYS_thread_dispatch] = sys_thread_dispatch;

    handlers[SYS_sem_open] = sys_sem_open;
    handlers[SYS_sem_close] = sys_sem_close;
    handlers[SYS_sem_wait] = sys_sem_wait;
    handlers[SYS_sem_signal] = sys_sem_signal;
    handlers[SYS_sem_timedwait] = sys_sem_timedwait;
    handlers[SYS_sem_trywait] = sys_sem_trywait;

    handlers[SYS_time_sleep] = sys_time_sleep;
}
