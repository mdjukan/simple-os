
#include "../../h/user/memory_operators.hpp"
#include "../../h/syscall_c.h"

void *operator new(size_t size)
{
    return mem_alloc(size);
}

void operator delete(void *p)
{
    mem_free(p);
}

void *operator new[](size_t size)
{
    return mem_alloc(size);
}

void operator delete[](void *p)
{
    mem_free(p);
}