#pragma once

using size_t = decltype(sizeof(0));

void *mem_alloc(size_t size);
int mem_free(void *p);

class _thread;
typedef _thread* thread_t;

int thread_create(thread_t *handle, void (*start_routine)(void*), void *arg);
int thread_exit();
void thread_dispatch();
