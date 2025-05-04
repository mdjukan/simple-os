#include "../../h/kernel/syscall_handlers.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/__Thread.hpp"

syscall_handler_t syscall_handlers[SYS_COUNT];

void sys_mem_alloc() {
    uint64 size = Scheduler::instance()->running->read_user_a1();
    void *p = MemoryAllocator::instance()->allocate_bytes(size);

    print_string("sys_mem_alloc:");
    print_num((uint64)p);
    print_string("\n");

    Scheduler::instance()->running->write_user_a0((uint64)p);
}

void sys_mem_free() {
    void *p = (void*)Scheduler::instance()->running->read_user_a1();
    MemoryAllocator::instance()->free(p);
    Scheduler::instance()->running->write_user_a0((uint64)0);
}

void sys_thread_create() {
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

void sys_thread_exit() {
    Scheduler::instance()->running->done = true;
    yield();
}

void sys_thread_dispatch() {
    yield();
}

void sys_hello_world() {
    print_string("Hello world by syscall!\n");
}

void init_syscall_handlers() {
    for (uint64 i=0; i<SYS_COUNT; i++) {
        syscall_handlers[i] = nullptr;
    }

    syscall_handlers[SYS_thread_create] = sys_thread_create;
    syscall_handlers[SYS_thread_exit] = sys_thread_exit;
    syscall_handlers[SYS_thread_dispatch] = sys_thread_dispatch;

    syscall_handlers[SYS_hello_world] = sys_hello_world;
    syscall_handlers[SYS_mem_alloc] = sys_mem_alloc;
    syscall_handlers[SYS_mem_free] = sys_mem_free;
}
