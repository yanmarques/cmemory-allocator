#ifndef NULL
#define NULL ((void *) 0)
#endif

void *mmap_anon(unsigned long size);
long munmap(void *addr, unsigned long size);
