#include "../../lib/hw.h"
#include "../../h/RISCV.hpp"
#include "../../h/syscall_codes.hpp"
#include "../../h/syscall_c.h"

void *mem_alloc(size_t size)
{
    uint64 size_in_blocks = size / MEM_BLOCK_SIZE;
    if (size % MEM_BLOCK_SIZE != 0)
    {
        size_in_blocks += 1;
    }

    return (void *)RISCV::make_syscall(SYS_mem_alloc, size_in_blocks);
}

int mem_free(void *p)
{
    return (int)RISCV::make_syscall(SYS_mem_free, (uint64)p);
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg)
{
    void *user_stack = mem_alloc(DEFAULT_STACK_SIZE);
    if (user_stack == nullptr)
    {
        return -1;
    }

    void *user_stack_top = (void *)((uint8 *)user_stack + DEFAULT_STACK_SIZE);

    return (int)RISCV::make_syscall(SYS_thread_create, (uint64)handle, (uint64)start_routine, (uint64)arg, (uint64)user_stack_top);
}

int thread_exit()
{
    return (int)RISCV::make_syscall(SYS_thread_exit);
}

void thread_dispatch()
{
    RISCV::make_syscall(SYS_thread_dispatch);
}