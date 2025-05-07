#include "../../h/kernel/ThreadList.hpp"
#include "../../h/kernel/__Thread.hpp"

__Thread *ThreadList::get()
{
    if (head == nullptr)
    {
        return nullptr;
    }

    __Thread *thread = head;
    if (head == tail)
    {
        head = nullptr;
        tail = nullptr;
    }
    else
    {
        head = thread->next;
        thread->next = nullptr;
    }

    thread_count -= 1;
    return thread;
}

void ThreadList::put(__Thread *thread)
{
    if (head == nullptr)
    {
        head = thread;
        tail = thread;
    }
    else
    {
        tail->next = thread;
        tail = thread;
    }

    thread_count += 1;
}

bool ThreadList::empty()
{
    return thread_count == 0;
}