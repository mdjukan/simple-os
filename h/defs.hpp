#pragma once

#define USER_THREAD_COUNT 64
#define KERNEL_THREAD_COUNT 16
#define THREAD_LIST_ELEMENT_COUNT (4 * (USER_THREAD_COUNT + KERNEL_THREAD_COUNT))
#define SEMAPHORE_COUNT 256

typedef void (*UserTask)(void *);
typedef void (*KernelTask)();

class _thread;
typedef _thread *thread_t;

class _sem;
typedef _sem *sem_t;

using size_t = decltype(sizeof(0));

typedef unsigned long time_t;