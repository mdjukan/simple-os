#include "../../lib/console.h"
#include "../../lib/hw.h"

void print_string(const char *str)
{
    while (*str != '\0')
    {
        __putc(*str);
        str += 1;
    }
}

void exit() {
    uint32 *exit_addr = (uint32*)0x100000;
    uint32 exit_code = 0x5555;
    *exit_addr = exit_code;
}

void panic(const char *msg) {
    print_string("[PANIC]");
    print_string(msg);
    print_string("\n");
    exit();
}

uint64 div_roundup(uint64 x, uint64 y) {
    if (x % y) {
        return x/y + 1;
    } else {
        return x/y;
    }
}