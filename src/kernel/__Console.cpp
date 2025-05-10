#include "../../h/kernel/__Console.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/__Thread.hpp"

void __Console::putc(char c)
{
    while (output.full())
    {
        __Thread *thread = Scheduler::instance()->running;
        blocked_on_output.put(thread);
        thread->suspended = true;
        yield();
    }

    output.put(c);
}

char __Console::getc()
{
    while (input.empty())
    {
        __Thread *thread = Scheduler::instance()->running;
        blocked_on_input.put(thread);
        thread->suspended = true;
        yield();
    }

    return input.get();
}

void __Console::handle_interrupt()
{
    volatile char *status = (char *)CONSOLE_STATUS;
    volatile char *write = (char *)CONSOLE_TX_DATA;
    volatile char *read = (char *)CONSOLE_RX_DATA;

    while (((*status) & CONSOLE_RX_STATUS_BIT) && !input.full())
    {
        char c = *read;
        output.put(c);
        input.put(c);
    }

    while (((*status) & CONSOLE_TX_STATUS_BIT) && !output.empty())
    {
        *write = output.get();
    }

    while (!blocked_on_input.empty()) {
        __Thread *thread = blocked_on_input.get();
        thread->suspended = false;
        Scheduler::instance()->put(thread);
    }

    
    while (!blocked_on_output.empty()) {
        __Thread *thread = blocked_on_output.get();
        thread->suspended = false;
        Scheduler::instance()->put(thread);
    }
}
