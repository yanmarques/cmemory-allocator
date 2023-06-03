// This is a simple Linux function that aims to mimic malloc() and free() calls. Just for fun ;)

#include <stdio.h>

int open(char *filepath, int flags)
{
    
}

static void * ohyeah(unsigned long len)
{
    void *addr;
    unsigned long fd;

    // Open "/dev/zero" as read/write and save file descriptor in RAX
    __asm(
        // ``flags`` as O_RDWR (2)
        "mov $0x2, %rsi\n"
        "xor %rdx, %rdx\n"

        "push %rdx\n"                       // put null byte to terminate string

        // put "o" into the stack
        "mov $0x6f72657a2f766564, %rbx\n"   // "orez/ved" 
        "push %rbx\n"

        "movb $0x2f, %bl\n"                 // "/"
        "sub $0x1, %rsp\n"                  // allocate 1 byte in the stack to hold the "/"
        "movb %bl, (%rsp)\n"                // push the "/" exactly to the byte allocated in the stack

        "mov %rsp, %rdi\n"                  // filename
    );

    __asm(
        "mov $0x2, %rax\n"                       // sys_open
        "syscall\n"
    );

    // Cleanup stack
    __asm(
        "pop %rdx\n"
        "pop %rdx\n"
        "add $0x1, %rsp\n"
    );

    asm volatile ("mov %%rax, %0" : "=r"(fd));
    if (is_negative(fd)) {
        printf("[ERROR] Unable to open '/dev/zero'. Error code: %i", fd);
        exit(1);
    }

    // ``fd`` as opened file descriptor
    asm volatile("mov %0, %%r8\n" :: "r"(fd));

    __asm(
        // ``addr`` as 0 or NULL
        "xor %rdi, %rdi\n"

        // ``prot`` as PROT_READ (0x1) | PROT_WRITE (0x2)
        "mov $0x3, %rdx\n"

        // ``flags`` as MAP_PRIVATE (0x2)
        "mov $0x2, %r10\n"

        // ``off`` as 4096 bytes, the default page size almost everywhere
        "mov $0x1000, %r9\n"
    );

    asm volatile ("mov %0, %%rsi" :: "r"(len));

    __asm(
        // Syscall number 9
        "mov $0x9, %rax\n"
        "syscall\n"
    );

    // Get allocated virtual memory address from RAX
    asm volatile ("mov %%rax, %0" : "=r"(addr));

    if (is_negative(addr)) {
        printf("[ERROR] Unable to allocate memory. Error code: %i\n", addr);
        exit(1);
    }

    return addr;
}

int is_negative(unsigned long i)
{
    return ((i >> 0x3c) ^ 0xf) == 0;
}

void exit(int error_code) {
    asm volatile ("mov %0, %%rax" :: "m"(error_code));
    __asm(
        "mov $0x3c, %rax\n"
        "syscall\n"
    );
}

char *strcpy(char *dest, char *src)
{
    while ((*dest++ = *src++) != '\0');
    return dest;
}

int main(int argc, char* argv[])
{
    char myname[] = "awesome name!";

    char *allocated_mm;

    printf("[INFO] Uninitilized variable address: 0x%lx\n", (unsigned long *)allocated_mm);
    allocated_mm = (char *)ohyeah(4);
    printf("[INFO] New address provided by the memory allocator: 0x%lx\n", (unsigned long *)allocated_mm);

    strcpy(allocated_mm, myname);

    printf("Value: %s\n", allocated_mm);

    return 0;
}