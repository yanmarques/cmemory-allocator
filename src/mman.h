#include "vmemory.h"

struct vm_mapping {
    // Represents the start virtual address of the allocated memory
    void *addr;

    // Amount of allocated bytes
    unsigned long size;

    // Double-linked list
    struct vm_mapping *next;
    struct vm_mapping *prev;
};

#define vm_mapping_size sizeof(struct vm_mapping)

void init_mm();
int dealloc_vm_mapping(struct vm_mapping *vmm);

int dealloc();
void *allocvm(unsigned long size);
int freevm(void *addr);

extern struct vm_mapping *vmm_head;
extern struct vm_mapping *vmm_end;
