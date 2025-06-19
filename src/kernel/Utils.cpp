#include "../../lib/hw.h"
#include "../../h/kernel/ConsoleManager.hpp"
#include "../../h/kernel/TCB.hpp"

void print(const char *str)
{
    while (*str != '\0')
    {
        ConsoleManager::instance()->putc(*str);
        str += 1;
    }
}

void exit()
{
    uint32 *exit_addr = (uint32 *)0x100000;
    uint32 exit_code = 0x5555;
    *exit_addr = exit_code;
}

void error(const char *msg)
{
    print("[ERROR]");
    print(msg);
    print("\n");
    ConsoleManager::instance()->flush_out_buffer();
    exit();
}

uint64 random(uint64 p, uint64 q)
{
    static long seed = 9551615UL;

    seed = seed * 1103515245 + 12345;

    return p + seed % (q - p + 1);
}