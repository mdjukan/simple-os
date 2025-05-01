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

void error(const char *msg) {
    print_string("[ERROR]");
    print_string(msg);
    print_string("\n");
    exit();
}
