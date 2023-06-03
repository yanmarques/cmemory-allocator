// This is how a normal C application would function to transfer data
// in memory in runtime:
//
//  1. Define the uninitilized destination variable, ``allocated_mm`` ::
//      The pointer of that variable is at a low memory region in Linux
//      usually the stack, starting with 0x7fff...
//  2. Allocate a given amount of memory with the ``malloc()`` call provided by GNU C Library ::
//      The result will be either memory in the heap, allocated in userspace or mapped by the kernel
//      and managed in userspace
//  4. Perform the data transfer however necessary
//  3. Unlock that physical memory to be used elsewhere with the ``free()`` call

#include <stdlib.h> // provides malloc() and free()
#include <stdio.h> // provides printf()
#include <string.h> // provides strncpy() 


int main(int argc, char* argv[])
{
    // Initialize local variable ``input`` 
    char input[] = "my awesome name!";
    
    // Define unitialized local variable ``allocated_mm`` 
    char *allocated_mm;
    printf(
        "[INFO] Uninitilized variable address: %p\n",
        allocated_mm
    );

    // Now initialize with memory likely from the heap, since the requested
    // memory is small
    allocated_mm = (char *)malloc(sizeof(input));
    if (allocated_mm == NULL)
    {
        printf("[ERROR] Failed to allocate memory with malloc()");
        return 1;
    }

    printf(
        "[INFO] New address provided by the memory allocator: %p\n",
        allocated_mm
    );

    // The memory is properly allocated, hence we can perform copy operations
    strncpy(allocated_mm, input, sizeof(input));

    printf(
        "[INFO] Allocate memory string: %s\n",
        allocated_mm
    );

    // Finally unlock that memory region
    free(allocated_mm);

    return 0;
}