#include "../../h/syscall_c.hpp"
#include "../../h/kernel/Utils.hpp"

typedef void (*body_t)(void*);

void thread_wrapper(body_t body, void *arg)
{
    body(arg);
    thread_exit();
}

void user_main(void *arg) {
    void *p1 = mem_alloc(1024);
    print_string("ptr 1:");
    print_num((uint64)p1);
    print_string("\n");

    //mem_free(p);

    void *p2 = mem_alloc(1024);
    print_string("ptr 2:");
    print_num((uint64)p2);
    print_string("\n");

    mem_free(p1);

    p1 = mem_alloc(1024);
    print_string("ptr 1 realloc:");
    print_num((uint64)p1);
    print_string("\n");


    hello_world();
}