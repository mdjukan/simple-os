#pragma once
typedef void (*body_t)(void*);

void thread_wrapper(body_t body, void *arg);
void user_main(void *arg);