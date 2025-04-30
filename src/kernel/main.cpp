#include "../../h/kernel/Utils.hpp"
#include "../../h/kernel/MemoryAllocator.hpp"
#include "../../h/kernel/PoolAllocator.hpp"

#define UNUSED(var) (void)var

int main()
{
    MemoryAllocator::instance()->print_map();
    PoolAllocator pool(MEM_BLOCK_SIZE, 65);
    MemoryAllocator::instance()->print_map();
    pool.~PoolAllocator();
    MemoryAllocator::instance()->print_map();

/*  void *p1 = MemoryAllocator::instance()->allocate(1);
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
 */
    return 0;
}