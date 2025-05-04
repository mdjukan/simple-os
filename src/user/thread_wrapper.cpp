#include "../../h/syscall_c.hpp"
#include "../../h/kernel/Utils.hpp"

typedef void (*body_t)(void*);

void thread_wrapper(body_t body, void *arg)
{
    body(arg);
    print_string("exiting\n");
    thread_exit();
}

void worker(void *arg) {
    const char *msg = (const char *)arg;
    for (int i=0; i<5; i++) {
        print_string(msg);
        thread_dispatch();
    }
}

void user_main(void *arg) {
    thread_t t1, t2, t3;

    thread_create(&t1, worker, (void*)"t1\n");
    thread_create(&t2, worker, (void*)"t2\n");
    thread_create(&t3, worker, (void*)"t3\n");

}