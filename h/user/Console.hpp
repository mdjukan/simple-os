#pragma once
#include "../syscall_c.h"

class Console
{
public:
    static void putc(char c);
    static char getc();
    static void print(const char *str);

#define DIGIT_COUNT 30
    template <typename NUM>
    static void print_num(NUM x)
    {
        if (x == 0)
        {
            putc('0');
            return;
        }

        int sgn = x > 0 ? 1 : -1;
        if (x < 0)
        {
            x *= -1;
        }

        char digits[DIGIT_COUNT];
        digits[DIGIT_COUNT - 1] = '\0';

        int idx = DIGIT_COUNT - 2;
        while (x > 0)
        {
            digits[idx] = '0' + (x % 10);
            x /= 10;
            idx -= 1;
        }

        if (sgn == -1)
        {
            digits[idx] = '-';
            idx -= 1;
        }

        print(&digits[idx + 1]);
    }
};