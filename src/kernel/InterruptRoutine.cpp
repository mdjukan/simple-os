#include "../../lib/hw.h"
#include "../../h/kernel/SyscallHandlers.hpp"
#include "../../h/RISCV.hpp"
#include "../../h/kernel/Scheduler.hpp"
#include "../../h/kernel/TCB.hpp"
#include "../../h/kernel/TimerManager.hpp"
#include "../../h/kernel/ConsoleManager.hpp"
#include "../../h/kernel/Utils.hpp"

namespace ScauseValues
{
    const uint64 TIMER = (1UL << 63) + 1UL;
    const uint64 SYSCALL = 8UL;
    const uint64 HARDWARE_INTERRUPT = (1UL << 63) + 9UL;
    const uint64 INVALID_READ = 5UL;
    const uint64 INVALID_WRITE = 7UL;
    const uint64 ILEGAL_INSTRUCTION = 2UL;
    const uint64 INSTRUCTION_ACCESS_FAULT = 1UL;
};

const char *error_message(uint64 scause)
{
    switch (scause)
    {
    case ScauseValues::INVALID_READ:
        return "invalid read";
    case ScauseValues::INVALID_WRITE:
        return "invalid write";
    case ScauseValues::ILEGAL_INSTRUCTION:
        return "ilegal instruction";
    case ScauseValues::INSTRUCTION_ACCESS_FAULT:
        return "instruction access fault";
    default:
        return "unknown";
    }
}

extern "C" void handle_interrupt()
{
    uint64 scause = RISCV::r_scause();

    if (scause == ScauseValues::SYSCALL)
    {
        if (Scheduler::instance()->running->type != ThreadType::USER)
        {
            error("kernel thread made a syscall");
        }

        UserThread *running = (UserThread *)Scheduler::instance()->running;

        RISCV::w_sepc(RISCV::r_sepc() + 4);

        uint64 syscall_code = running->read_user_a0();
        if (syscall_code < SYS_COUNT && SyscallHandlers::handlers[syscall_code] != nullptr)
        {
            SyscallHandlers::handlers[syscall_code]();
        }
        else
        {
            error("invalid syscall code");
        }
    }
    else if (scause == ScauseValues::HARDWARE_INTERRUPT)
    {
        int irq = plic_claim();

        if (irq == CONSOLE_IRQ)
        {
            ConsoleManager::instance()->handle_interrupt();
        }

        plic_complete(irq);
    }
    else if (scause == ScauseValues::TIMER)
    {
        TimerManager::instance()->handle_interrupt();
        RISCV::clear_sip();
    }
    else
    {
        print("scause=");
        print_num(scause);
        print("\n");
        print("sepc=");
        print_num(RISCV::r_sepc());
        print("\n");
        error(error_message(scause));
    }
}