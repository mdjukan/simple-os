#pragma once
#include "../defs.hpp"

void *operator new(size_t size);
void operator delete(void *p);
void *operator new[](size_t size);
void operator delete[](void *p);