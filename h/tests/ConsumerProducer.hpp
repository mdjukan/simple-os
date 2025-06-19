#include "../../lib/hw.h"
#include "../../h/syscall_c.h"
#include "../../h/syscall_cpp.hpp"
#include "../../h/kernel/Utils.hpp"

#define CP_BUFFER_SIZE 10
class CPBuffer
{
private:
    uint64 data[CP_BUFFER_SIZE];
    uint64 r = 0;
    uint64 w = 0;
    uint64 count = 0;
    Semaphore items;
    Semaphore slots;
    Semaphore mutex;

public:
    CPBuffer() : items(0), slots(CP_BUFFER_SIZE), mutex(1)
    {
    }

    uint64 get()
    {
        items.wait();
        mutex.wait();
        uint64 val = data[r];
        r = (r + 1) % CP_BUFFER_SIZE;
        count -= 1;
        mutex.signal();
        slots.signal();

        return val;
    }

    void put(uint64 val)
    {
        slots.wait();
        mutex.wait();
        data[w] = val;
        w = (w + 1) % CP_BUFFER_SIZE;
        count += 1;
        mutex.signal();
        items.signal();
    }

    void log()
    {
        Console::print("\t\t\t\t\t\t\t\t\t\t{BUFFER ");
        Console::print_num(count);
        Console::print("/");
        Console::print_num(CP_BUFFER_SIZE);
        Console::print("}\n");
    }
};

struct WorkerArg
{
    uint64 idx;
    CPBuffer *buffer;
};

void consumer(void *arg)
{
    WorkerArg *wa = (WorkerArg *)arg;

    while (true)
    {
        int val = wa->buffer->get();
        Console::print("\t\t\t\t\t[CONSUMER ");
        Console::print_num(wa->idx);
        Console::print("] ");
        Console::print_num(val);
        Console::print("\n");
        time_sleep(random());
    }
}

void producer(void *arg)
{
    WorkerArg *wa = (WorkerArg *)arg;

    while (true)
    {
        wa->buffer->put(wa->idx);
        Console::print("[PRODUCER ");
        Console::print_num(wa->idx);
        Console::print("]\n");
        time_sleep(random());
    }
}

#define PRODUCER_COUNT 5
#define CONSUMER_COUNT 5
void consumer_producer_test(void *arg)
{
    CPBuffer buffer;

    thread_t consumers[CONSUMER_COUNT];
    WorkerArg cargs[CONSUMER_COUNT];
    for (int i = 0; i < CONSUMER_COUNT; i++)
    {
        cargs[i].idx = i;
        cargs[i].buffer = &buffer;
        thread_create(&consumers[i], consumer, &cargs[i]);
    }

    thread_t producers[PRODUCER_COUNT];
    WorkerArg pargs[CONSUMER_COUNT];
    for (int i = 0; i < PRODUCER_COUNT; i++)
    {
        pargs[i].idx = i;
        pargs[i].buffer = &buffer;
        thread_create(&producers[i], producer, &pargs[i]);
    }

    while (true)
    {
        thread_dispatch();
    }
}