##FIXED_INIT
	.section	.rodata
	printintstr: .asciz	"%d\n"
	printstringstr: .asciz "%s\n"
	.text	

##TAC_BEGINFUNC
	.globl	main
main:
	pushq	%rbp
	movq	%rsp, %rbp

##TAC_OUTPUT
	movl	a(%rip), %esi
	leaq	printintstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
##TAC_ENDFUNC
	popq	%rbp
	ret
#DATA SECTION
	.data
a:
	.long	1
_888: .long 888