#include "../../h/user/Semaphore.hpp"
#include "../../h/syscall_c.h"

Semaphore::Semaphore(unsigned init)
{
    sem_open(&handle, init);
}

int Semaphore::wait()
{
    return sem_wait(handle);
}

int Semaphore::signal() {
    return sem_signal(handle);
}

int Semaphore::timedWait(time_t time)
{
    return sem_timedwait(handle, time);
}

int Semaphore::tryWait()
{
    return sem_trywait(handle);
}