/**
 * Core functions for memory allocation using mmap system call
*/

#include "vmemory.h"

/**
 * Allocate ``size`` bytes of anonymous backed memory from kernel
 * 
 * @size: Amount of bytes to allocate
*/
void *mmap_anon(unsigned long size)
{
    void *addr;

    __asm(
        // ``addr`` as 0 or NULL
        "xor %rdi, %rdi\n"

        // ``prot`` as PROT_READ (0x1) | PROT_WRITE (0x2)
        "mov $0x3, %rdx\n"

        // ``flags`` as MAP_PRIVATE (0x2) and MAP_ANONYMOUS (0x20)
        "mov $0x22, %r10\n"

        // ``fd`` as -1
        "xor %r8, %r8\n"
        "dec %r8\n"

        // ``off`` as 0 or NULL
        "xor %r9, %r9\n"
    );

    // ``len`` as amount of memory to allocate
    asm volatile ("mov %0, %%rsi" :: "r"(size));

    __asm(
        // sys_mmap
        "mov $0x9, %rax\n"
        "syscall\n"
    );

    // Get allocated virtual memory address from RAX
    asm volatile ("mov %%rax, %0" : "=r"(addr));

    if (((long) addr) <= 0) {
        return NULL;
    }

    return addr;
}

/**
 * Free allocated memory
 * 
 * @addr: Virtual memory address previously allocated
 * @size: Amount of bytes to free
*/
long munmap(void *addr, unsigned long size)
{
    void *retval;

    asm volatile ("mov %0, %%rdi" :: "m"(addr));
    asm volatile ("mov %0, %%rsi" :: "r"(size));

    __asm(
        // sys_munmap
        "mov $0xb, %rax\n"
        "syscall\n"
    );

    asm volatile ("mov %%rax, %0" : "=r"(retval));

    return (long) retval;
}
