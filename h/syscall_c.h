#pragma once
#include "defs.hpp"

void *mem_alloc(size_t size);
int mem_free(void *p);

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg);
int thread_exit();
void thread_dispatch();
