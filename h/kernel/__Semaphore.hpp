#pragma once
#include "../../lib/hw.h"
#include "ThreadList.hpp"

class __Semaphore
{
private:
    uint64 val;
    ThreadList blocked;

public:
    __Semaphore(int val = 1) : val(val) {}
    void signal();
    void wait();
    int try_wait();
};