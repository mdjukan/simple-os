#include "../../lib/hw.h"
#include "../../h/kernel/SyscallHandlers.hpp"
#include "../../h/RISCV.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/__Thread.hpp"
#include "../../h/kernel/Timer.hpp"
#include "../../h/kernel/__Console.hpp"

#define TIMER ((((uint64)1) << 63) + (uint64)1)
#define SYSCALL ((uint64)8)
#define HARDWARE_INTERRUPT ((((uint64)1) << 63) + (uint64)9)

#define INVALID_READ ((uint64)5)
#define INVALID_WRITE ((uint64)7)
#define ILEGAL_INSTRUCTION ((uint64)2)
#define INSTRUCTION_ACCESS_FAULT ((uint64)1)

const char *error_message(uint64 scause)
{
    switch (scause)
    {
    case INVALID_READ:
        return "invalid read";
    case INVALID_WRITE:
        return "invalid write";
    case ILEGAL_INSTRUCTION:
        return "ilegal instruction";
    case INSTRUCTION_ACCESS_FAULT:
        return "instruction access fauld";
    default:
        return "unknown";
    }
}

extern "C" void handle_interrupt()
{
    __Console::instance()->handle_interrupt();
    uint64 scause = RISCV::r_scause();

    if (scause == SYSCALL)
    {
        RISCV::w_sepc(RISCV::r_sepc() + 4);

        uint64 syscall_code = Scheduler::instance()->running->read_user_a0();
        if (syscall_code < SYS_COUNT && SyscallHandlers::handlers[syscall_code] != nullptr)
        {
            SyscallHandlers::handlers[syscall_code]();
        }
        else
        {
            error("invalid syscall code");
        }
    }
    else if (scause == HARDWARE_INTERRUPT)
    {
        int irq = plic_claim();

        if (irq == CONSOLE_IRQ)
        {
            //__Console::instance()->handle_interrupt();
        }

        plic_complete(irq);
    }
    else if (scause == TIMER)
    {
        Timer::instance()->handle_interrupt();
        RISCV::clear_sip();
    }
    else
    {
        print_string("scause=");
        print_num(scause);
        print_string("\n");
        print_string("sepc=");
        print_num(RISCV::r_sepc());
        print_string("\n");
        error(error_message(scause));
    }
}