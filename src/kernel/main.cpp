#include "../../h/kernel/Utils.hpp"
#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/PoolAllocator.hpp"

void rec_print(uint64 x)
{
    if (x == 0)
    {
        return;
    }

    uint8 arr[] = {1, 2, 3, 4, 5};
    uint8 arr_size = 5;

    rec_print(x - 1);

    for (uint8 i = 0; i < arr_size; i++)
    {
        print_num(arr[i]);
        print_string(" ");
    }
    print_string("\n");
}

void worker(void *arg)
{
    char *msg = (char *)arg;
    for (int i = 0; i < 10; i++)
    {
        print_string(msg);
        rec_print(3);
        yield();
        print_string("returned to ");
        print_string(msg);
    }
}

int main()
{
    __Thread::allocator = new PoolAllocator(sizeof(__Thread), THREAD_COUNT);

    __Thread *t1 = __Thread::create(worker, (void *)"thread 1\n");
    if (t1 == nullptr) {
        error("thread1 alloc failed\n");
    }

    __Thread *t2 = __Thread::create(worker, (void *)"thread 2\n");
    if (t2 == nullptr) {
        error("thread2 alloc failed\n");
    }

    __Thread *t3 = __Thread::create(worker, (void *)"thread 3\n");
    if (t3 == nullptr) {
        error("thread3 alloc failed\n");
    }

    __Thread *main_thread = __Thread::create();
    if (main_thread == nullptr) {
        error("main thread alloc failed\n");
    }

    Scheduler::instance()->running = main_thread;

    while (!t1->done || !t2->done || !t3->done)
    {
        yield();
        print_string("returned to main\n");
    }

    delete t1;
    delete t2;
    delete t3;
    delete main_thread;

    print_string("exiting main\n");

    delete __Thread::allocator;
    return 0;
}