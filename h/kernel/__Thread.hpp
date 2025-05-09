#pragma once
#include "../../lib/hw.h"
#include "../defs.hpp"
#include "MemoryAllocator.hpp"
#include "Scheduler.hpp"
#include "PoolAllocator.hpp"
#include "__Semaphore.hpp"

extern "C" void yield();
extern "C" void restore_user_ctxt();

class __Thread
{
public:
    __Thread *next = nullptr;

    bool done = false;
    bool suspended = false;

    uint64 kernel_sp;
    uint64 sepc;

    void *kernel_stack_top;

    bool wait_failed = false;

    uint64 sleep_ticks;

    bool timed_wait = false;
    __Semaphore *blocked_on = nullptr;
    
private:
    body_t body;
    void *arg;

    uint8 kernel_stack[DEFAULT_STACK_SIZE];
    void *user_stack;

    void *user_stack_top;

    uint64 *kernel_ctxt;
    uint64 *user_ctxt;

    __Thread() {}
    __Thread(body_t body, void *arg, void *user_stack_top);
    void init_context();

public:
    ~__Thread()
    {
        MemoryAllocator::instance()->free(user_stack);
    }

    void make_ready()
    {
        Scheduler::instance()->add_new_thread(this);
    }

    static __Thread *create(body_t body, void *arg, void *user_stack_top = nullptr);
    static __Thread *create_main_thread();

    uint64 read_user_a0();
    void write_user_a0(uint64 val);
    uint64 read_user_a1();
    uint64 read_user_a2();
    uint64 read_user_a3();
    uint64 read_user_a4();

public:
    static PoolAllocator *allocator;

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
};
