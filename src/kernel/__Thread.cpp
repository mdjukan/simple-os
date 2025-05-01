#include "../../h/kernel/__Thread.hpp"

void thread_wrapper(__Thread *thread) {
    thread->body(thread->arg);
    thread->done = true;
    yield();
}

PoolAllocator* __Thread::allocator =  nullptr;

__Thread *__Thread::create(body_t body, void *arg) {
    if (__Thread::allocator->has_free())
    {
        return new __Thread(body, arg);
    } else {
        return nullptr;
    }
}

__Thread *__Thread::create() {
    if (__Thread::allocator->has_free())
    {
        return new __Thread();
    } else {
        return nullptr;
    }
}