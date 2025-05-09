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

void ThreadList::remove_thread(__Thread *thread)
{
    __Thread *previous = nullptr;
    __Thread *current = head;

    while (current != nullptr && current != thread)
    {
        previous = current;
        current = current->next;
    }

    if (current == nullptr)
    {
        error("remove_thread not found\n");
    }

    if (head == thread && tail == thread)
    {
        head = nullptr;
        tail = nullptr;
    }
    else if (head == thread)
    {
        head = head->next;
    }
    else if (tail == thread)
    {
        previous->next = nullptr;
        tail = previous;
    }
    else
    {
        previous->next = thread->next;
    }

    thread->next = nullptr;
    thread_count -= 1;
}

void SleepingThreadList::put(__Thread *thread)
{
    if (head == nullptr)
    {
        head = thread;
        tail = thread;
    }
    else
    {
        __Thread *previous = nullptr;
        __Thread *current = head;

        while (current != nullptr && current->sleep_ticks < thread->sleep_ticks)
        {
            previous = current;
            current = current->next;
        }

        if (previous == nullptr)
        {
            thread->next = head;
            head = thread;
        }
        else if (current == nullptr)
        {
            tail->next = thread;
            tail = thread;
        }
        else
        {
            previous->next = thread;
            thread->next = current;
        }
    }

    thread_count += 1;
}

void SleepingThreadList::update_ticks()
{
    __Thread *current = head;
    while (current != nullptr)
    {
        current->sleep_ticks -= 1;
        current = current->next;
    }

    while (head != nullptr && head->sleep_ticks == 0)
    {
        __Thread *thread = get();
        thread->suspended = false;
        Scheduler::instance()->put(thread);
    }
}

void TimedwaitThreadList::update_ticks() {
    __Thread *current = head;
    while (current != nullptr)
    {
        current->sleep_ticks -= 1;
        current = current->next;
    }

    while (head != nullptr && head->sleep_ticks == 0)
    {
        __Thread *thread = get();
        thread->suspended = false;
        thread->blocked_on->remove_blocked(thread);
        thread->wait_failed = true;
        Scheduler::instance()->put(thread);
    }
}