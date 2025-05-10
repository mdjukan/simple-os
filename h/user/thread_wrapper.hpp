#pragma once
#include "../defs.hpp"

void thread_wrapper(body_t body, void *arg);
void idle_worker(void *arg);

void sem_test(void *arg);
void timer_test(void *arg);
void console_test(void *arg);
void periodic_test(void *arg);

void error_code_test(void *arg);
