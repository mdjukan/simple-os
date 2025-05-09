#pragma once
#include "../defs.hpp"

class Semaphore
{
public:
    Semaphore(unsigned init = 1);
    virtual ~Semaphore() {}

    int wait();
    int signal();
    int timedWait(time_t time);
    int tryWait();

private:
    sem_t handle;
};