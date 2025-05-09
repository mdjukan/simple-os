#include "../../h/syscall_c.h"
#include "../../h/kernel/Utils.hpp"
#include "../../h/syscall_cpp.hpp"

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

#define BUFFER_SIZE 10
class Buffer
{
private:
    Semaphore items;
    Semaphore slots;
    Semaphore mutex;
    uint64 data[BUFFER_SIZE];
    uint64 head, tail;

    uint64 item_count = 0;

public:
    Buffer() : items(0), slots(BUFFER_SIZE), mutex(1), head(0), tail(0) {}

    uint64 get()
    {
        items.wait();
        mutex.wait();

        if (item_count == 0)
        {
            print_string("invalid get");
        }
        else
        {
            print_string("valid get");
        }
        uint64 val = data[head];
        head = (head + 1) % BUFFER_SIZE;
        item_count -= 1;
        mutex.signal();
        slots.signal();
        return val;
    }

    void put(uint64 val)
    {
        slots.wait();
        mutex.wait();
        if (item_count == BUFFER_SIZE)
        {
            print_string("invalid put");
        }
        else
        {
            print_string("valid put");
        }

        data[tail] = val;
        tail = (tail + 1) % BUFFER_SIZE;
        item_count += 1;
        mutex.signal();
        items.signal();
    }

    void log()
    {
        print_string("Buffer ");
        print_num(item_count);
        print_string("/");
        print_num(BUFFER_SIZE);
        print_string("\n");
    }
};

struct WorkerArg
{
    Buffer *buffer;
    int idx;
    WorkerArg(Buffer *buffer, int idx) : buffer(buffer), idx(idx) {}
};

void consumer(void *arg)
{
    Buffer *buffer = ((WorkerArg *)arg)->buffer;
    int idx = ((WorkerArg *)arg)->idx;

    while (1)
    {
        time_sleep(20);
        uint64 val = buffer->get();
        print_string("Consumer  ");
        print_num(idx);
        print_string(" got from buffer ");
        print_num(val);
        print_string("\n");
        buffer->log();
    }
}

void producer(void *arg)
{
    Buffer *buffer = ((WorkerArg *)arg)->buffer;
    int idx = ((WorkerArg *)arg)->idx;

    while (1)
    {
        time_sleep(10);
        buffer->put(idx);
        print_string("Producer ");
        print_num(idx);
        print_string(" added to buffer\n");
        buffer->log();
    }
}

#define CONSUMER_COUNT 3
#define PRODUCER_COUNT 3
void sem_test(void *arg)
{
    Buffer *buffer = new Buffer();
    thread_t consumers[CONSUMER_COUNT];
    for (int i = 0; i < CONSUMER_COUNT; i++)
    {
        thread_create(&consumers[i], consumer, new WorkerArg(buffer, i));
    }

    thread_t producers[PRODUCER_COUNT];
    for (int i = 0; i < PRODUCER_COUNT; i++)
    {
        thread_create(&producers[i], producer, new WorkerArg(buffer, i));
    }
}

void signaler(void *arg) {
    sem_t sem = (sem_t)arg;
    time_sleep(30);
    sem_close(sem);
}

void timer_test(void *arg)
{
    sem_t sem;
    sem_open(&sem, 0);

    thread_t sig;
    thread_create(&sig, signaler, (void*)sem);

    //int ret = sem_timedwait(sem, 50);
    int ret = sem_wait(sem);

    if (ret == 0) {
        print_string("wait success\n");
    } else {
        print_string("wait fail\n");
    }
}
