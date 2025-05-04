#pragma once
#include "MemoryAllocator.hpp"
#include "../../lib/hw.h"
#include "Scheduler.hpp"
#include "PoolAllocator.hpp"
#include "../user/thread_wrapper.hpp"

#define THREAD_COUNT 64

#define SP_ALIGN 16
#define REG_COUNT 32

#define RA_IDX 1
#define SP_IDX 2
#define A0_IDX 10
#define A1_IDX 11
#define A2_IDX 12
#define A3_IDX 13

typedef void (*body_t)(void *);

extern "C" void yield();
extern "C" void restore_user_ctxt();

class __Thread
{
public:
    void *operator new(size_t size)
    {
        void *p = allocator->get();

        return p;
    }

    void *operator new[](size_t size) = delete;

    void operator delete(void *p) noexcept
    {
        allocator->release(p);
    }

    void operator delete[](void *p) = delete;

public:
    body_t body;
    void *arg;

    void *kernel_stack;
    void *user_stack;

    void *user_stack_top;
    void *kernel_stack_top;

    bool done = false;
    __Thread *next = nullptr;

    uint64 kernel_sp;
    uint64 sepc;

    uint64 *kernel_ctxt;
    uint64 *user_ctxt;

    __Thread()
        : body(nullptr), arg(nullptr), kernel_stack(nullptr), user_stack(nullptr)
    {
    }

    __Thread(body_t body, void *arg)
        : body(body), arg(arg)
    {
        init();
        Scheduler::instance()->active_threads += 1;
        Scheduler::instance()->put(this);
    }

    void init()
    {
        // TODO allocate error handeling
        user_stack = MemoryAllocator::instance()->allocate_bytes(DEFAULT_STACK_SIZE);
        user_stack_top = (void *)(((uint64)((uint8 *)user_stack + DEFAULT_STACK_SIZE) / SP_ALIGN) * SP_ALIGN);

        kernel_stack = MemoryAllocator::instance()->allocate_bytes(DEFAULT_STACK_SIZE);
        kernel_stack_top = (void *)(((uint64)((uint8 *)kernel_stack + DEFAULT_STACK_SIZE) / SP_ALIGN) * SP_ALIGN);

        user_ctxt = (uint64 *)kernel_stack_top - REG_COUNT;
        user_ctxt[A0_IDX] = (uint64)body;
        user_ctxt[A1_IDX] = (uint64)arg;
        user_ctxt[SP_IDX] = (uint64)user_stack_top;
        sepc = (uint64)thread_wrapper;

        kernel_ctxt = (uint64 *)kernel_stack_top - 2 * REG_COUNT;
        kernel_ctxt[RA_IDX] = (uint64)restore_user_ctxt;

        kernel_sp = (uint64)kernel_ctxt;
    }

    ~__Thread()
    {
        MemoryAllocator::instance()->free(user_stack);
        MemoryAllocator::instance()->free(kernel_stack);
    }

    static PoolAllocator *allocator;
    static __Thread *create(body_t body, void *arg);
    static __Thread *create();
    static __Thread *create_idle();

    // ocitavanju registre a0, a1, ... iz korisnickog konteksta
    // read_user_a0() -> a0 user ctxt
    // write_user_a0()
    uint64 read_user_a0()
    {
        return user_ctxt[A0_IDX];
    }

    void write_user_a0(uint64 val)
    {
        user_ctxt[A0_IDX] = val;
    }

    uint64 read_user_a1()
    {
        return user_ctxt[A1_IDX];
    }

    uint64 read_user_a2()
    {
        return user_ctxt[A2_IDX];
    }

    uint64 read_user_a3()
    {
        return user_ctxt[A3_IDX];
    }
};
