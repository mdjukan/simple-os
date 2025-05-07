#include "../h/RISCV.hpp"

uint64 RISCV::make_syscall(uint64 code, uint64 arg1, uint64 arg2, uint64 arg3, uint64 arg4)
{
    RISCV::ecall();
    return RISCV::r_a0();
}