#pragma once
#include "../../lib/hw.h"
#include "ConsoleManager.hpp"

void print(const char *str);

#define DIGIT_COUNT 30
template <typename NUM>
void print_num(NUM x)
{
    if (x == 0)
    {
        ConsoleManager::instance()->putc('0');
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

// vraca slucajan broj is skupa [p,q]
uint64 random(uint64 p, uint64 q);

void exit();
void error(const char *msg);