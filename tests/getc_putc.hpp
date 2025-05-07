#pragma once
#include "../lib/console.h"

//ODR - one definition rule

inline int getc() {
    return __getc();
}

inline void putc(char c) {
    __putc(c);
}