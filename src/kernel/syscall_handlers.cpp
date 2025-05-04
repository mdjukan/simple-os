#include "../../h/kernel/syscall_handlers.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/__Thread.hpp"

syscall_handler_t Syscalls::handlers[SYS_COUNT];

void Syscalls::sys_mem_alloc() {
    uint64 size = Scheduler::instance()->running->read_user_a1();
    void *p = MemoryAllocator::instance()->allocate_bytes(size);
    Scheduler::instance()->running->write_user_a0((uint64)p);
}

void Syscalls::sys_mem_free() {
    void *p = (void*)Scheduler::instance()->running->read_user_a1();
    MemoryAllocator::instance()->free(p);
    Scheduler::instance()->running->write_user_a0((uint64)0);
}

void Syscalls::sys_thread_create() {
    __Thread **handle = (__Thread**)Scheduler::instance()->running->read_user_a1();
    body_t body = (body_t)Scheduler::instance()->running->read_user_a2();
    void *arg = (void*)Scheduler::instance()->running->read_user_a3();

    *handle = __Thread::create(body, arg);
    if (handle == nullptr) {
        Scheduler::instance()->running->write_user_a0((uint64)-1);
    } else {
        Scheduler::instance()->running->write_user_a0((uint64)0);
    }
}

void Syscalls::sys_thread_exit() {
    Scheduler::instance()->running->done = true;
    yield();
}

void Syscalls::sys_thread_dispatch() {
    yield();
}

void Syscalls::init_handlers() {
    for (uint64 i=0; i<SYS_COUNT; i++) {
        handlers[i] = nullptr;
    }

    handlers[SYS_mem_alloc] = sys_mem_alloc;
    handlers[SYS_mem_free] = sys_mem_free;
    
    handlers[SYS_thread_create] = sys_thread_create;
    handlers[SYS_thread_exit] = sys_thread_exit;
    handlers[SYS_thread_dispatch] = sys_thread_dispatch;
}
