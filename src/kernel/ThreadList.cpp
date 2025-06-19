#include "../../h/kernel/ThreadList.hpp"
#include "../../h/kernel/TCB.hpp"
#include "../../h/kernel/Utils.hpp"

PoolAllocator *ThreadListElement::allocator = nullptr;

ThreadListElement *ThreadListElement::create(TCB *thread)
{
    if (allocator->full())
    {
        // ovo ne treba da se desava
        error("ThreadListElement::create allocation failed");
        return nullptr; // unreachable, ali se kompajer zali
    }
    else
    {
        return new ThreadListElement(thread);
    }
}

bool ThreadList::empty()
{
    return sentinel.next == &sentinel;
}

TCB *ThreadList::get()
{
    if (empty())
    {
        error("ThreadList::get on empty list");
    }

    ThreadListElement *target = sentinel.next;
    TCB *thread = target->thread;
    remove_element(target);

    return thread;
}

void ThreadList::put(TCB *thread)
{
    ThreadListElement *element = ThreadListElement::create(thread);

    if (element == nullptr)
    {
        error("ThreadListElement::create failed");
    }

    // dodaje nit na kraj liste

    element->next = &sentinel;
    element->prev = sentinel.prev;

    sentinel.prev->next = element;
    sentinel.prev = element;
}

void ThreadList::remove_thread(TCB *thread)
{
    ThreadListElement *current = sentinel.next;

    while (current != &sentinel && current->thread != thread)
    {
        current = current->next;
    }

    if (current == &sentinel)
    {
        error("ThreadList::remove_thread thread not in list");
    }

    remove_element(current);
}

void ThreadList::remove_element(ThreadListElement *target)
{
    ThreadListElement *before = target->prev;
    ThreadListElement *after = target->next;

    before->next = after;
    after->prev = before;

    delete target;
}