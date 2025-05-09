#pragma once
#include "defs.hpp"

void *mem_alloc(size_t size);
int mem_free(void *p);

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg);
int thread_exit();
void thread_dispatch();

int sem_open(sem_t *handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_timedwait(sem_t id, time_t timeout);
int sem_trywait(sem_t id);

int time_sleep(time_t time);