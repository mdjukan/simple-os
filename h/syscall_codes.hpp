#pragma once
#include "../lib/hw.h"

#define SYS_COUNT (uint64)0x42

#define SYS_mem_alloc (uint64)0x01
#define SYS_mem_free (uint64)0x02

#define SYS_thread_create (uint64)0x11
#define SYS_thread_exit (uint64)0x12
#define SYS_thread_dispatch (uint64)0x13

#define SYS_sem_open (uint64)0x21
#define SYS_sem_close (uint64)0x22
#define SYS_sem_wait (uint64)0x23
#define SYS_sem_signal (uint64)0x24
#define SYS_sem_timedwait (uint64)0x25
#define SYS_sem_trywait (uint64)0x26
