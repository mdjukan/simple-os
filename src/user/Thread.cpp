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
    return thread_create(&handle, Thread::run_wrapper, (void *)this);
}

void Thread::dispatch()
{
    thread_dispatch();
}

int Thread::sleep(time_t time)
{
    // TODO
    return 0;
}