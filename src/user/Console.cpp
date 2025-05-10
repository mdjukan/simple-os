#include "../../h/syscall_c.h"
#include "../../h/user/Console.hpp"

void __putc(char c)
{
    putc(c);
}

char __getc()
{
    return getc();
}

void Console::putc(char c)
{
    __putc(c);
}

char Console::getc()
{
    return __getc();
}
void Console::print(const char *str)
{
    while (*str)
    {
        putc(*(str++));
    }
}
