#include "../../h/kernel/ConsoleManager.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/TCB.hpp"

void ConsoleManager::putc(char c)
{
    output.put(c);
}

char ConsoleManager::getc()
{
    return input.get();
}

void ConsoleManager::handle_interrupt()
{
    while (((*status) & CONSOLE_RX_STATUS_BIT) && !input.full())
    {
        input.put(*read); // ne zelim nit da suspendujem u interrupt_handler-u
    }
}

void ConsoleManager::flush_out_buffer()
{
    while (!output.empty())
    {
        if ((*status) & CONSOLE_TX_STATUS_BIT)
        {
            *write = output.get();
        }
    }
}

void ConsoleManager::output_worker()
{
    while (true)
    {
        char c = output.get();

        while (!((*status) & CONSOLE_TX_STATUS_BIT))
            ;

        *write = c;
    }
}

void ConsoleManager::output_worker_wrapper()
{
    ConsoleManager::instance()->output_worker();
}
