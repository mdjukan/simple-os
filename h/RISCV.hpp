#pragma once
#include "../lib/hw.h"

namespace RISCV
{
    inline uint64 r_sepc()
    {
        uint64 sepc;
        __asm__ volatile("csrr %0, sepc" : "=r"(sepc));
        return sepc;
    }

    inline void w_sepc(uint64 sepc)
    {
        __asm__ volatile("csrw sepc, %0" : : "r"(sepc));
    }

    inline void w_sscratch(uint64 sscratch)
    {
        __asm__ volatile("csrw sscratch, %0" : : "r"(sscratch));
    }

    inline uint64 r_scause()
    {
        uint64 scause;
        __asm__ volatile("csrr %0, scause" : "=r"(scause));
        return scause;
    }

    inline uint64 r_sip()
    {
        uint64 sip;
        __asm__ volatile("csrr %0, sip" : "=r"(sip));
        return sip;
    }

    inline void w_sip(uint64 sip)
    {
        __asm__ volatile("csrw sip, %0" : : "r"(sip));
    }

#define SSIP ((uint64)2)
    inline void clear_sip()
    {
        w_sip(r_sip() & (~SSIP));
    }

    inline void w_a0(uint64 a0)
    {
        __asm__ volatile("mv a0, %0" : : "r"(a0));
    }

    inline uint64 r_a0()
    {
        uint64 a0;
        __asm__ volatile("mv %0, a0" : "=r"(a0));
        return a0;
    }

    inline void ecall()
    {
        __asm__ volatile("ecall");
    }

    inline void w_stvec(uint64 stvec)
    {
        __asm__ volatile("csrw stvec, %0" : : "r"(stvec));
    }

    inline uint64 r_sstatus()
    {
        uint64 sstatus;
        __asm__ volatile("csrr %0, sstatus" : "=r"(sstatus));
        return sstatus;
    }

    inline void w_sstatus(uint64 sstatus)
    {
        __asm__ volatile("csrw sstatus, %0" : : "r"(sstatus));
    }

#define SPP (((uint64)1) << 8) // Supervisor Previous Privilige
    inline void clear_SPP()
    {
        w_sstatus(r_sstatus() & ~SPP);
    }

#define SPIE (((uint64)1) << 5) // Supervisor Previous Interrupt Enable
    inline void clear_SPIE()
    {
        w_sstatus(r_sstatus() & ~SPIE);
    }

    inline void w_a1(uint64 a1)
    {
        __asm__ volatile("mv a1, %0" : : "r"(a1));
    }

    inline void w_a2(uint64 a2)
    {
        __asm__ volatile("mv a2, %0" : : "r"(a2));
    }

    inline void w_a3(uint64 a3)
    {
        __asm__ volatile("mv a3, %0" : : "r"(a3));
    }

    inline void w_a4(uint64 a4)
    {
        __asm__ volatile("mv a4, %0" : : "r"(a4));
    }

    uint64 make_syscall(uint64 code, uint64 arg1 = 0, uint64 arg2 = 0, uint64 arg3 = 0, uint64 arg4 = 0, uint64 arg5 = 0);
};