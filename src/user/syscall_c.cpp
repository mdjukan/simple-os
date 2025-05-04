#include "../../lib/hw.h"
#include "../../h/RISCV.hpp"
#include "../../h/syscall_codes.hpp"
#include "../../h/syscall_c.hpp"

void *mem_alloc(size_t size) {
    RISCV::w_a1(size);
    RISCV::w_a0(SYS_mem_alloc);
    RISCV::ecall();
    return (void*)RISCV::r_a0();
}

int mem_free(void *p) {
    RISCV::w_a1((uint64)p);
    RISCV::w_a0(SYS_mem_free);
    RISCV::ecall();
    return (int)RISCV::r_a0();
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg)
{
    RISCV::w_a3((uint64)arg);
    RISCV::w_a2((uint64)start_routine);
    RISCV::w_a1((uint64)handle);
    RISCV::w_a0(SYS_thread_create);
    RISCV::ecall();
    return (int)RISCV::r_a0();
}

int thread_exit()
{
    RISCV::w_a0(SYS_thread_exit);
    RISCV::ecall();
    return (int)RISCV::r_a0();
}

void thread_dispatch()
{
    RISCV::w_a0(SYS_thread_dispatch);
    RISCV::ecall();
}