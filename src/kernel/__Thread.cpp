#include "../../h/kernel/__Thread.hpp"

void thread_wrapper(__Thread *thread) {
    thread->body(thread->arg);
    thread->done = true;
    yield();
}