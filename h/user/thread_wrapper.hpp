#pragma once
typedef void (*body_t)(void *);

void thread_wrapper(body_t body, void *arg);
void idle_worker(void *arg);