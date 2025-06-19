#include "../../lib/hw.h"
#include "../../h/syscall_c.h"

void thread_wrapper(UserTask body, void *arg)
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
