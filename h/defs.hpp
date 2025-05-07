#pragma once

#define THREAD_COUNT 64
#define SEMAPHORE_COUNT 256

typedef void (*body_t)(void *);

class _thread;
typedef _thread *thread_t;

using size_t = decltype(sizeof(0));

typedef unsigned long time_t;