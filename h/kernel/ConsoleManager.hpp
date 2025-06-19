#pragma once
#include "IOBuffer.hpp"
#include "ThreadList.hpp"

#define CONSOLE_BUFFER_SIZE 1024

class ConsoleManager
{
public:
    static ConsoleManager *instance()
    {
        static ConsoleManager console;
        return &console;
    }

private:
    volatile char *status = (char *)CONSOLE_STATUS;
    volatile char *write = (char *)CONSOLE_TX_DATA;
    volatile char *read = (char *)CONSOLE_RX_DATA;

    IOBuffer<CONSOLE_BUFFER_SIZE> input;  // sa tastature
    IOBuffer<CONSOLE_BUFFER_SIZE> output; // na ekran

public:
    void putc(char c);
    char getc();
    void handle_interrupt();

    void flush_out_buffer();

    // funkcija nad kojom ce biti pokrenuta kernel nit zaduzena za ispis sadrzaja sa konzole
    void output_worker();
    static void output_worker_wrapper();
};