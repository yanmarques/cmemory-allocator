#include "mman.h"

#include <stdio.h>
#include <assert.h>

void assert_allocvm(void *addr, unsigned long size);
void debug_vm_mappings();
void debug_vmm(struct vm_mapping *vmm, int index);

void test_one_allocvm()
{
    void *addr;
    unsigned long size = 1024;

    addr = allocvm(size);

    assert_allocvm(addr, size);
    assert(vmm_head->next->addr == addr);
    assert(vmm_head->next->size == size);

    assert(dealloc());
}

void test_two_allocvms_after_dealloc()
{
    void *addr;
    unsigned long size = 1024;

    // First
    addr = allocvm(size);

    assert_allocvm(addr, size);

    assert(vmm_head->next->addr == addr);
    assert(vmm_head->next->size == size);

    assert(dealloc());

    assert(vmm_head == NULL);
    assert(vmm_end == NULL);

    // Second
    addr = allocvm(size);

    assert_allocvm(addr, size);
    assert(vmm_head->next->addr == addr);
    assert(vmm_head->next->size == size);

    assert(dealloc());

    assert(vmm_head == NULL);
    assert(vmm_end == NULL);
}

void test_many_allocvms()
{
    int nallocvms = 100;
    unsigned long size = 1024;
    void *allocs[nallocvms];

    for (int i = 0; i < nallocvms; i++)
    {
        allocs[i] = allocvm(size);

        assert_allocvm(allocs[i], size);

        if (i == 0)
        {
            assert(vmm_head->next->addr == allocs[i]);
            assert(vmm_head->next->size == size);
        }
    }

    assert(dealloc());

    assert(vmm_head == NULL);
    assert(vmm_end == NULL);
}

void test_huge_allocvm_will_fail()
{
    // 100 GiB
    unsigned long size = 107374200000;
    void *addr;

    addr = allocvm(size);

    assert(((long) addr) == 0);

    assert(dealloc());

    assert(vmm_head == NULL);
    assert(vmm_end == NULL);
}

void test_allocvm_after_a_failed_attempt()
{
    // 100 GiB
    unsigned long huge_size = 107374200000;
    unsigned long small_size = 4096;
    void *addr;

    addr = allocvm(huge_size);

    assert(((long) addr) == 0);

    addr = allocvm(small_size);
    assert_allocvm(addr, small_size);

    assert(dealloc());

    assert(vmm_head == NULL);
    assert(vmm_end == NULL);
}

void assert_allocvm(void *addr, unsigned long size)
{
    assert((long)addr > 0);

    assert(vmm_end->addr == addr);
    assert(vmm_end->size == size);
}

void debug_vm_mappings()
{
    struct vm_mapping *ptr = vmm_head;
    int index = 0;
    while (ptr)
    {
        debug_vmm(ptr, index);
        ptr = ptr->next;
        index++;
    }
}

void debug_vmm(struct vm_mapping *vmm, int index)
{
    void *addr;
    unsigned long size;
    struct vm_mapping *next;
    struct vm_mapping *prev;

    if (vmm == NULL)
    {
        addr = NULL;
        size = 0;
        next = prev = NULL;
    } else
    {
        addr = vmm->addr;
        size = vmm->size;
        next = vmm->next;
        prev = vmm->prev;
    }

    printf(
        "[DEBUG] Index: %d\tAddress: %p\tSize: %lx\tNext: %p\tPrev: %p\n",
        index, addr, size, next, prev
    );
}

int main(int argc, char **argv)
{
    test_one_allocvm();
    test_two_allocvms_after_dealloc();
    test_many_allocvms();
    test_huge_allocvm_will_fail();
    test_allocvm_after_a_failed_attempt();
}