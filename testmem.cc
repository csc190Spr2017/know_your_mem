#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/mman.h>

#define ADDR_MIN   0x0000100000000000UL  // Low-ish
#define PAGE_SIZE 4096
static void *map_page(void *addr)
{
    void *ret = mmap(addr, 4096,
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
  void* p1 = map_page(start); 
  void* p2 = start;
	int res = memcheck(p2, PAGE_SIZE*2);
	printf("p2 valid: %d\n", res);
  for(int i=0; i<1024*1024*256; i++){
  void* p3 = start+PAGE_SIZE;
	res = memcheck(p3, PAGE_SIZE);
	//printf("p3 valid: %d\n", res);
  }
  printf("DONE!");
  return (0);
}
