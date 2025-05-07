#include "../../h/kernel/__Thread.hpp"
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

PoolAllocator *__Thread::allocator = nullptr;

__Thread::__Thread(body_t body, void *arg, void *user_stack_top)
    : body(body), arg(arg), user_stack_top(user_stack_top)
{
    user_stack = (void *)((uint8 *)user_stack_top - DEFAULT_STACK_SIZE);
    kernel_stack_top = (void *)(((uint64)(kernel_stack + DEFAULT_STACK_SIZE) / SP_ALIGN) * SP_ALIGN);
    init_context();
}

void __Thread::init_context()
{
    user_ctxt = (uint64 *)kernel_stack_top - REG_COUNT;
    user_ctxt[A0_IDX] = (uint64)body;
    user_ctxt[A1_IDX] = (uint64)arg;
    user_ctxt[SP_IDX] = (uint64)user_stack_top;
    sepc = (uint64)thread_wrapper;

    kernel_ctxt = (uint64 *)kernel_stack_top - 2 * REG_COUNT;
    kernel_ctxt[RA_IDX] = (uint64)restore_user_ctxt;

    kernel_sp = (uint64)kernel_ctxt;
}

__Thread *__Thread::create(body_t body, void *arg, void *user_stack_top)
{
    if (allocator->full())
    {
        return nullptr;
    }
    else
    {
        if (user_stack_top == nullptr)
        {
            void *stack = MemoryAllocator::instance()->allocate_bytes(DEFAULT_STACK_SIZE);
            if (stack == nullptr)
            {
                return nullptr;
            }

            user_stack_top = (uint8 *)stack + DEFAULT_STACK_SIZE;
        }

        return new __Thread(body, arg, user_stack_top);
    }
}

__Thread *__Thread::create_main_thread()
{
    if (allocator->full())
    {
        return nullptr;
    }
    else
    {
        return new __Thread();
    }
}

uint64 __Thread::read_user_a0()
{
    return user_ctxt[A0_IDX];
}

void __Thread::write_user_a0(uint64 val)
{
    user_ctxt[A0_IDX] = val;
}

uint64 __Thread::read_user_a1()
{
    return user_ctxt[A1_IDX];
}

uint64 __Thread::read_user_a2()
{
    return user_ctxt[A2_IDX];
}

uint64 __Thread::read_user_a3()
{
    return user_ctxt[A3_IDX];
}

uint64 __Thread::read_user_a4()
{
    return user_ctxt[A4_IDX];
}
