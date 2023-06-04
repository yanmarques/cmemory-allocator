/**
 * Modest Linux functions that aims to mimic malloc() and memory allocator functionality. Just for fun ;)
*/

#include "mman.h"

struct vm_mapping *vmm_head = NULL;
struct vm_mapping *vmm_end = NULL;

/**
 * Initialize memory mapping.
*/
void init_mm()
{
    vmm_head = (struct vm_mapping *) mmap_anon(vm_mapping_size);
    vmm_head->addr = vmm_head;
    vmm_head->size = vm_mapping_size;
    vmm_head->next = NULL;
    vmm_head->prev = NULL;

    vmm_end = vmm_head;
}

/**
 * Free that virtual memory mapping, both user allocated memory
 * as well as the associated struct allocation
 * 
 * @vm: Virtual memory mapping to deallocate
*/
int dealloc_vm_mapping(struct vm_mapping *vmm)
{
    int retval;

    // We must know whether the ``vmm`` is the head,
    // if so, we should only munmap() the struct
    unsigned short is_vmm_head = vmm == vmm_head;

    // Deallocate the previously allocated address
    retval = munmap(vmm->addr, vmm->size);
    if (retval != 0)
    {
        return 0;
    }

    // If ``vmm`` is head, ``vmm->addr`` points to the ``vmm`` pointer itself,
    // therefore, we must not try to deallocate it again
    if (is_vmm_head) {
        return 1;
    }

    if (vmm->prev)
    {
        vmm->prev->next = vmm->next;
    }

    if (vmm->next)
    {
        vmm->next->prev = vmm->prev;
    }

    return !munmap(vmm, vm_mapping_size);
}

/**
 * Free all allocated virtual memories.
*/
int dealloc()
{
    int retval;
    struct vm_mapping *ptr = vmm_end;
    struct vm_mapping *prev = NULL;

    while (ptr != NULL)
    {
        // Save previous ``vm_mapping`` to be deallocated later
        prev = ptr->prev;

        retval = dealloc_vm_mapping(ptr);
        if (retval == 0) {
            return 0;
        }

        // Put ``vm_mapping`` in the loop to dellocation
        ptr = prev;
    }

    // Clean up it all
    vmm_head = vmm_end = NULL;

    return 1;
}

/**
 * Allocate virtual memory region from kernel using mmap.
 *
 * @size: Amount of bytes to allocate
*/
void *allocvm(unsigned long size)
{
    void *addr;
    struct vm_mapping *next;

    if(vmm_head == NULL)
    {
        init_mm();
    }

    addr = mmap_anon(size);
    if (addr == NULL) {
        return NULL;
    }

    next = (struct vm_mapping *)mmap_anon(vm_mapping_size);

    next->addr = (unsigned long *)addr;
    next->size = size;
    next->next = NULL;

    next->prev = vmm_end;
    vmm_end->next = next;

    vmm_end = next;

    return addr;
}

/**
 * Free a specific virtual memory region. Unless you want specifically to
 * free this region, the recommended method would be to call ``dealloc``
 * at the end of you program.
 * 
 * @addr: Address of a previously allocated region
*/
int freevm(void *addr)
{
    // We should not deallocate the head of the linked-list, it
    // would break the memory allocator
    if (addr == vmm_head)
    {
        return 0;
    }

    struct vm_mapping *ptr = vmm_head;
    while (ptr)
    {
        if (ptr->addr == addr)
        {
            return dealloc_vm_mapping(ptr);
        }

        ptr = ptr->next;
    }

    return 0;
}
