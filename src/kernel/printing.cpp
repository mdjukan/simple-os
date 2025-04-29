#include "../../lib/console.h"

void print_string(const char *str) {
    while (*str!='\0') {
        __putc(*str);
        str += 1;
    }
}

