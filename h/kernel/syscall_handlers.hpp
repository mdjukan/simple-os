#pragma once
#include "../syscall_codes.hpp"

typedef void (*syscall_handler_t)();

namespace Syscalls
{
    extern syscall_handler_t handlers[SYS_COUNT];

    void sys_mem_alloc();
    void sys_mem_free();

    void sys_thread_create();
    void sys_thread_exit();
    void sys_thread_dispatch();

    void sys_sem_open();
    void sys_sem_close();
    void sys_sem_wait();
    void sys_sem_signal();
    void sys_sem_timedwait();
    void sys_sem_trywait();

    void sys_time_sleep();

    void init_handlers();
};
