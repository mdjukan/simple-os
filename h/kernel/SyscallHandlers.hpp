#pragma once
#include "../syscall_codes.hpp"

typedef void (*syscall_handler_t)();

namespace SyscallHandlers
{
    extern syscall_handler_t handlers[SYS_COUNT];

    void mem_alloc();
    void mem_free();

    void thread_create();
    void thread_exit();
    void thread_dispatch();

    void sem_open();
    void sem_close();
    void sem_wait();
    void sem_signal();
    void sem_timedwait();
    void sem_trywait();

    void time_sleep();

    void getc();
    void putc();

    void init_handlers();
};
