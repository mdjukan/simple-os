#include "../../h/kernel/TCB.hpp"
#include "../../h/user/thread_wrapper.hpp"

#define SP_ALIGN 16
#define REG_COUNT 32

#define RA_IDX 1
#define SP_IDX 2
#define A0_IDX 10
#define A1_IDX 11
#define A2_IDX 12
#define A3_IDX 13
#define A4_IDX 14
#define A5_IDX 15

TCB::TCB(ThreadType type, uint64 time_slice)
    : type(type), time_slice(time_slice)
{
    stack_top = (uint8 *)((((uint64)(stack + DEFAULT_STACK_SIZE)) / SP_ALIGN) * SP_ALIGN);
}

void TCB::activate()
{
    Scheduler::instance()->add_new_thread(this);
}

PoolAllocator *UserThread::allocator = nullptr;
PoolAllocator *KernelThread::allocator = nullptr;

KernelThread::KernelThread(KernelTask task)
    : TCB(ThreadType::KERNEL, DEFAULT_TIME_SLICE), task(task)
{
    init_context();
}

void KernelThread::init_context()
{
    context = (uint64 *)stack_top - REG_COUNT;
    context[RA_IDX] = (uint64)task;
    sp = (uint64)context;
}

KernelThread *KernelThread::create(KernelTask kernel_task)
{
    if (allocator->full())
    {
        return nullptr;
    }
    else if (kernel_task == nullptr)
    {
        return new KernelThread();
    }
    else
    {
        return new KernelThread(kernel_task);
    }
}

UserThread::UserThread(const UserThreadConfig &config)
    : TCB(ThreadType::USER, DEFAULT_TIME_SLICE)
{
    user_task = config.user_task;
    user_task_arg = config.user_task_arg;
    user_stack = config.user_stack;
    user_stack_size = config.user_stack_size;
    time_slice = config.time_slice;

    user_stack_top = (void *)(((uint64)((uint8 *)user_stack + user_stack_size) / SP_ALIGN) * SP_ALIGN);
    init_context();
}

UserThread::~UserThread()
{
    MemoryAllocator::instance()->free(user_stack);
}

UserThread *UserThread::create(UserThreadConfig &config)
{
    if (allocator->full())
    {
        return nullptr;
    }
    else
    {
        if (config.user_stack == nullptr)
        {
            void *user_stack = MemoryAllocator::instance()->allocate_bytes(config.user_stack_size);
            if (user_stack == nullptr)
            {
                return nullptr;
            }

            config.user_stack = user_stack;
        }

        return new UserThread(config);
    }
}

void UserThread::init_context()
{
    kernel_context = (uint64 *)stack_top - 2 * REG_COUNT;
    kernel_context[RA_IDX] = (uint64)restore_user_ctxt;

    user_context = (uint64 *)stack_top - REG_COUNT;
    user_context[A0_IDX] = (uint64)user_task;
    user_context[A1_IDX] = (uint64)user_task_arg;
    user_context[SP_IDX] = (uint64)user_stack_top;

    sepc = (uint64)thread_wrapper;
    sp = (uint64)kernel_context;
}

uint64 UserThread::read_user_a0()
{
    return user_context[A0_IDX];
}

void UserThread::write_user_a0(uint64 val)
{
    user_context[A0_IDX] = val;
}

uint64 UserThread::read_user_a1()
{
    return user_context[A1_IDX];
}

uint64 UserThread::read_user_a2()
{
    return user_context[A2_IDX];
}

uint64 UserThread::read_user_a3()
{
    return user_context[A3_IDX];
}

uint64 UserThread::read_user_a4()
{
    return user_context[A4_IDX];
}

uint64 UserThread::read_user_a5()
{
    return user_context[A5_IDX];
}
