#include "../../lib/hw.h"
#include "../RISCV.hpp"
#include "../syscall_codes.hpp"

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

int thread_exit() {
    RISCV::w_a0(SYS_thread_exit);
    RISCV::ecall();
    return (int)RISCV::r_a0();
}

void hello_world() {
    RISCV::w_a0(SYS_hello_world);
    RISCV::ecall();
}