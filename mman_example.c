#include "mman.h"

int main(int argc, char **argv)
{
    // Initialize local variable ``input`` 
    char input[] = "my awesome name!";
    
    // Define unitialized local variable ``allocated_mm`` 
    char *allocated_mm;
    printf(
        "[INFO] Uninitilized variable address: %p\n",
        allocated_mm
    );

    // Now allocate memory to initialize variable
    allocated_mm = (char *)allocvm(sizeof(input));
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
    dealloc();

    return 0;
}
