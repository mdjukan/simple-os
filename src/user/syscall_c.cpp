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

int sem_open(sem_t *handle, unsigned init) {
    return (int)RISCV::make_syscall(SYS_sem_open, (uint64)handle, (uint64)init);
}

int sem_close(sem_t handle) {
    return (int)RISCV::make_syscall(SYS_sem_close, (uint64)handle);
}

int sem_wait(sem_t id) {
    return (int)RISCV::make_syscall(SYS_sem_wait, (uint64)id);
}

int sem_signal(sem_t id) {
    return (int)RISCV::make_syscall(SYS_sem_signal, (uint64)id);
}

int sem_timedwait(sem_t id, time_t timeout) {
    return (int)RISCV::make_syscall(SYS_sem_timedwait, (uint64)id, (uint64)timeout);
}

int sem_trywait(sem_t id) {
    return (int)RISCV::make_syscall(SYS_sem_trywait, (uint64)id);
}

int time_sleep(time_t time) {
    return (int)RISCV::make_syscall(SYS_time_sleep, (uint64)time);
}