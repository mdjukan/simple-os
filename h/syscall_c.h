#pragma once
#include "defs.hpp"

// nullptr on fail
void *mem_alloc(size_t size);

// return codes
#define SUCCESS 0
#define ERROR_ADDR_NOT_ALIGNED -1
#define ERROR_ADDR_NOT_ON_HEAP -2
#define ERROR_NOT_ALLOCATED -3
int mem_free(void *p);

// return codes
#define SUCCESS 0
#define ERROR_BAD_ALLOC -1
int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg);

int thread_exit();      // no return, can not fail
void thread_dispatch(); // can not fail

#define SUCCESS 0
#define ERROR_BAD_ALLOC -1
int sem_open(sem_t *handle, unsigned init);

// return codes
#define SUCCESS 0
#define ERROR_BAD_HANDLE -1
int sem_close(sem_t handle);

// return codes
#define SUCCESS 0
#define ERROR_BAD_HANDLE -1
#define SEM_WAIT_SUCCEEDED 0
#define SEM_WAIT_FAILED 1
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_timedwait(sem_t id, time_t timeout);
int sem_trywait(sem_t id);

// return codes
#define SUCCESS 0
int time_sleep(time_t time); // can not fail

const int EOF = -1;
char getc();       // can not fail
void putc(char c); // can not fail