#pragma once
#include "IOBuffer.hpp"
#include "ThreadList.hpp"


class __Console
{
public:
    static __Console *instance()
    {
        static __Console console;
        return &console;
    }

private:
    IOBuffer input;  // sa tastature
    IOBuffer output; // na ekran

    ThreadList blocked_on_input;
    ThreadList blocked_on_output;

public:
    void putc(char c);
    char getc();
    void handle_interrupt();
};