# Simple OS

A small freestanding operating-system kernel for 64-bit RISC-V, written in C++11. It runs on QEMU's `virt` machine and is intended as an educational systems project.

## Features

- Block-based heap allocator with fixed-size pools for kernel objects
- Round-robin, timer-preempted scheduling for user and kernel threads
- Semaphores, including timed and non-blocking waits
- System calls for memory, threads, synchronization, sleeping, and console I/O
- C API plus lightweight C++ `Thread`, `Semaphore`, `Console`, and `PeriodicThread` wrappers

The kernel currently starts a producer/consumer demonstration that exercises threads, semaphores, sleeping, and console output.

## Build and run

Requires GNU Make, a RISC-V 64-bit GCC/binutils cross-toolchain, and `qemu-system-riscv64`.

```bash
make
make qemu
```

The Makefile automatically detects the `riscv64-unknown-elf-`, `riscv64-linux-gnu-`, or `riscv64-unknown-linux-gnu-` toolchain prefixes. For a custom prefix:

```bash
make TOOLPREFIX=riscv64-unknown-elf-
```

To debug with GDB:

```bash
make qemu-gdb
gdb-multiarch kernel
```
