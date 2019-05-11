#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/mman.h>

#define ADDR_MIN   0x0000100000000000UL  // Low-ish
#define PAGE_SIZE 4096
static void *map_page(void *addr, int size)
{
    void *ret = mmap(addr, size,
            PROT_READ|PROT_WRITE,
            MAP_ANONYMOUS|MAP_PRIVATE | (addr != NULL ? MAP_FIXED : 0),
            -1, 0);
#ifdef SIMPLIFIED
    if (ret == MAP_FAILED)
        err(8, "Could not mmap() at %p -- maybe I was not lucky with random picking?", addr);
    if (addr != NULL && ret != addr)
        err(99, "Wrong flags to mmap? ret = %p != %p = wanted", ret, addr);
#endif
    return ret;
}


int memcheck (void *x, int size) 
{
    if (mprotect(x, size, PROT_READ) != 0){
		return 0;
	}else{
		return 1;
	}
}

int main (int argc, char *argv[])
{
  void *start = (void *)ADDR_MIN;
  void* p1 = map_page(start, PAGE_SIZE*4096*24); 
  printf("p1: %p, start: %p\n", p1, start);
  return (0);
}
