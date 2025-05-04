#pragma once
#include "../syscall_codes.hpp"

typedef void (*syscall_handler_t)();

extern syscall_handler_t syscall_handlers[SYS_COUNT];

void sys_mem_alloc();
void sys_mem_free();

void sys_thread_create();
void sys_thread_exit();
void sys_thread_dispatch();

void sys_hello_world();

void init_syscall_handlers();
