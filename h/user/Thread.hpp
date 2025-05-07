#pragma once
#include "../defs.hpp"

class Thread
{
public:
    Thread(body_t body, void *arg);
    virtual ~Thread();

    int start();

    static void dispatch();
    static int sleep(time_t time);

protected:
    Thread() : body(nullptr), arg(nullptr) {}
    virtual void run() {}

private:
    thread_t handle;
    body_t body;
    void *arg;

public:
    static void run_wrapper(void *arg);
};