#include "../../h/kernel/printing.hpp"
#include "../../h/kernel/MemoryAllocator.hpp"

#define UNUSED(var) (void)var

int main() {
    void *p1 = MemoryAllocator::instance()->allocate(1);
    void *p2 = MemoryAllocator::instance()->allocate(1);
    void *p3 = MemoryAllocator::instance()->allocate(1);
    void *p4 = MemoryAllocator::instance()->allocate(4);

    MemoryAllocator::instance()->print_map();

    MemoryAllocator::instance()->free(p2);
    MemoryAllocator::instance()->print_map();

    MemoryAllocator::instance()->free(p3);
    MemoryAllocator::instance()->print_map();

    MemoryAllocator::instance()->free(p1);
    MemoryAllocator::instance()->print_map();

    MemoryAllocator::instance()->free(p4);
    MemoryAllocator::instance()->print_map();

    UNUSED(p1);
    UNUSED(p2);
    UNUSED(p3);
    UNUSED(p4);

    return 0;
}