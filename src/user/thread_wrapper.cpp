#include "../../h/syscall_c.h"
#include "../../h/kernel/Utils.hpp"

void thread_wrapper(body_t body, void *arg)
{
    body(arg);
    thread_exit();
}

void idle_worker(void *arg)
{
    while (1)
    {
        thread_dispatch();
    }
}
