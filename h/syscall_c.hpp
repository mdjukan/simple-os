#pragma once

using size_t = decltype(sizeof(0));

void *mem_alloc(size_t size);
int mem_free(void *p);

int thread_exit();
void hello_world();