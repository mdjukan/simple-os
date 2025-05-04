#pragma once

#include "../../lib/hw.h"
#include "syscall_handlers.hpp"
#include "../RISCV.hpp"
#include "../../lib/console.h"

#define TIMER ((((uint64)1) << 63) + (uint64)1)
#define SYSCALL ((uint64)8)
#define CONSOLE ((((uint64)1) << 63) + (uint64)9)
#define INVALID_READ ((uint64)5)
#define INVALID_WRITE ((uint64)7)
#define ILEGAL_INSTRUCTION ((uint64)2)
#define INSTRUCTION_ACCESS_FAULT ((uint64)1)

extern "C" void handle_interrupt()
{
    uint64 scause = RISCV::r_scause();
    if (scause == SYSCALL)
    {
        //print_string("[INT-SYSCALL]\n");
        RISCV::w_sepc(RISCV::r_sepc() + 4);

        uint64 syscall_code = Scheduler::instance()->running->read_user_a0();
        if (syscall_code < SYS_COUNT && syscall_handlers[syscall_code] != nullptr)
        {
            syscall_handlers[syscall_code]();
        }
        else
        {
            //sistemski poziv sa nevalidnom kodom
            //TODO error handling
        }

        RISCV::clear_sip();
    }
    else if (scause == CONSOLE)
    {
        // print_string("[INT-CONSOLE]\n");
        console_handler();
    }
    else if (scause == TIMER)
    {
        //print_string("[INT-TIMER]\n");
        RISCV::clear_sip();
    }

    /*     if (scause == INVALID_READ) {
            print_string("sepc=");
            print_num(RISCV::r_sepc());
            print_string("\n");
            error("invalid read");
        } else if (scause == INVALID_WRITE) {
            print_string("sepc=");
            print_num(RISCV::r_sepc());
            print_string("\n");
            error("invalid read");
        } else if (scause == ILEGAL_INSTRUCTION) {
            print_string("sepc=");
            print_num(RISCV::r_sepc());
            print_string("\n");
            error("ilegal instruction");
        } else if (scause == INSTRUCTION_ACCESS_FAULT) {
            print_string("sepc=");
            print_num(RISCV::r_sepc());
            print_string("\n");
            error("instruction access fault");
        }  */

    RISCV::w_sscratch((uint64)Scheduler::instance()->running->kernel_stack_top);
}