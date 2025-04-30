#include "../../h/kernel/Utils.hpp"
#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/Scheduler.hpp"

void rec_print(uint64 x) {
    if (x == 0) {
        return;
    }

    uint8 arr[] = {1, 2, 3, 4, 5};
    uint8 arr_size = 5;

    rec_print(x-1);

    for (uint8 i = 0; i < arr_size; i++) {
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
    __Thread t1(worker, (void *)"thread 1\n");
    __Thread t2(worker, (void *)"thread 2\n");
    __Thread t3(worker, (void *)"thread 3\n");

    __Thread main_thread;
    Scheduler::instance()->running = &main_thread;

    while (!t1.done || !t2.done || !t3.done) {
        yield();
        print_string("returned to main\n");
    }

    print_string("exiting main\n");

    return 0;
}