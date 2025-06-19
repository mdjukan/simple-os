#pragma once
#include "../../lib/hw.h"
#include "PoolAllocator.hpp"
#include "SCB.hpp"
#include "Scheduler.hpp"

extern "C" void yield();
extern "C" void restore_user_ctxt();

enum ThreadType
{
    KERNEL,
    USER
};

class TCB
{
public:
    ThreadType type;

    bool done = false;
    bool suspended = false;

    uint64 time_slice;

    uint64 sp;
    uint8 *stack_top;

    uint64 sleep_ticks;
    uint64 running_ticks;

    // semaphore resources
    bool wait_failed = false;
    bool timed_wait = false;
    SCB *blocked_on = nullptr;

    void activate();

    virtual ~TCB() {}

protected:
    uint8 stack[DEFAULT_STACK_SIZE];
    TCB(ThreadType type, uint64 time_slice);
    virtual void init_context() {}
};

class KernelThread : public TCB
{
    POOL_ALLOCATOR

private:
    uint64 *context;
    KernelTask task;

public:
    KernelThread() : TCB(ThreadType::KERNEL, DEFAULT_TIME_SLICE), task(nullptr) {}
    KernelThread(KernelTask task);
    ~KernelThread() {}
    void init_context() override;
    static KernelThread *create(KernelTask kernel_task = nullptr);
};

struct UserThreadConfig
{
    UserTask user_task;
    void *user_task_arg;
    uint64 time_slice = DEFAULT_TIME_SLICE;
    void *user_stack = nullptr;
    uint64 user_stack_size = DEFAULT_STACK_SIZE;
};

class UserThread : public TCB
{
    POOL_ALLOCATOR

public:
    uint64 sepc;

private:
    UserTask user_task;
    void *user_task_arg;

    void *user_stack;
    uint64 user_stack_size;
    void *user_stack_top;

    uint64 *kernel_context;
    uint64 *user_context;

public:
    UserThread(const UserThreadConfig &config);
    ~UserThread();

    static UserThread *create(UserThreadConfig &config);
    void init_context();

    uint64 read_user_a0();
    void write_user_a0(uint64 val);
    uint64 read_user_a1();
    uint64 read_user_a2();
    uint64 read_user_a3();
    uint64 read_user_a4();
    uint64 read_user_a5();
};
