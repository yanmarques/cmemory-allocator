#include "vmemory.h"

#include <stdio.h>
#include <assert.h>

void test_mmap_anon_and_unmap_succeeds_with_correct_size()
{
    void *addr;
    int retval;

    addr = mmap_anon(1);

    assert(addr != NULL);

    retval = munmap(addr, 1);

    assert(retval == 0);
}

int main(int argc, char **argv)
{
    test_mmap_anon_and_unmap_succeeds_with_correct_size();
}