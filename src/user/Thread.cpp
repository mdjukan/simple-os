#include "../../h/syscall_cpp.hpp"
#include "../../h/syscall_c.h"

Thread::Thread(body_t body, void *arg)
    : body(body), arg(arg) {}

Thread::~Thread() {}

void Thread::run_wrapper(void *arg)
{
    Thread *thread = (Thread *)arg;
    thread->run();
}

int Thread::start()
{
    if (body==nullptr) {
        return thread_create(&handle, Thread::run_wrapper, (void *)this);
    } else {
        return thread_create(&handle, body, arg);
    }
}

void Thread::dispatch()
{
    thread_dispatch();
}

int Thread::sleep(time_t time)
{
    return time_sleep(time);
}