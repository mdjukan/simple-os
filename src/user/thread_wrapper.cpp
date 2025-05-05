#include "../../h/syscall_c.hpp"
#include "../../h/kernel/Utils.hpp"

typedef void (*body_t)(void *);

void thread_wrapper(body_t body, void *arg)
{
    body(arg);
    thread_exit();
}

#define ARR_SIZE 100
void rec(int *arr, int idx)
{

    if (idx == ARR_SIZE)
    {
        return;
    }

    if (arr[idx] != 0)
    {
        error("arr corrupted");
    } 

    int ctrl = 99;

    rec(arr, idx + 1);

    if (ctrl != 99)
    {
        error("ctrl corrupted");
    }
}

void worker(void *arg)
{
    uint64 tid = (uint64)arg;
    int *arr = (int *)mem_alloc(sizeof(int) * ARR_SIZE);

    for (int i = 0; i < ARR_SIZE; i++)
    {
        arr[i] = 0;
    }

    rec(arr, 0);

    print_num(tid);
    print_string(" - ok\n");
    mem_free((void *)arr);
}

void idle(void *arg)
{
    while (1) {
        thread_dispatch();
    }
}

#define TCOUNT 20
void user_main(void *arg)
{
    for (uint64 i = 0; i < TCOUNT; i++)
    {
        thread_t t;
        thread_create(&t, worker, (void *)i);
    }
}