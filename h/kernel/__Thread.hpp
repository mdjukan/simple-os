#pragma once
#include "MemoryAllocator.hpp"
#include "../../lib/hw.h"
#include "Scheduler.hpp"

#define SP_ALIGN 16
#define REG_COUNT 32

#define RA_IDX 1
#define A0_IDX 10

typedef void (*body_t)(void*);

extern "C" void yield();

void thread_wrapper(__Thread *thread);

class __Thread {
    public:
    body_t body;
    void *arg;
    void *stack;
    void *stack_top;

    bool done = false;
    __Thread *next = nullptr;

    uint64 sp;

    //konstruktor samo za main nit
    __Thread()
    : body(nullptr), arg(nullptr), stack(nullptr)
    {}

    __Thread(body_t body, void *arg)
    : body(body), arg(arg) {
        stack = MemoryAllocator::instance()->allocate_bytes(DEFAULT_STACK_SIZE);
        stack_top = (void*)(((uint64)((uint8*)stack + DEFAULT_STACK_SIZE) / SP_ALIGN) * SP_ALIGN);

        uint64 *regs = (uint64*)stack_top - REG_COUNT;
        regs[RA_IDX] = (uint64)thread_wrapper;
        regs[A0_IDX] = (uint64)this;

        sp = (uint64)regs;
        Scheduler::instance()->put(this);
    }

    ~__Thread() {
        MemoryAllocator::instance()->free(stack);
    }
};
