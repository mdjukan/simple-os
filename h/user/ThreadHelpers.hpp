#pragma once
#include "../defs.hpp"

void thread_wrapper(UserTask body, void *arg);
void idle_worker(void *);
