##FIXED INIT
.section	.rodata
printintstr: .asciz	"%d" 
printstringstr: .asciz "%s" 
printcharstr: .asciz "%c" 
printfloatstr: .asciz "%f" 
.text
##TAC_COPY
	movl	_'x'(%rip), %eax
	movl	%eax, _c(%rip)
##TAC_COPY
	movl	_100(%rip), %eax
	movl	%eax, _d(%rip)
##TAC_COPY
	movl	_2(%rip), %eax
	movl	%eax, _a(%rip)
##TAC_COPY
	movl	_1(%rip), %eax
	movl	%eax, _i(%rip)
##TAC_COPY
	movl	_1(%rip), %eax
	movl	%eax, _x(%rip)
##TAC_COPY
	movl	_1.2(%rip), %eax
	movl	%eax, _xx(%rip)
##TAC_BEGINFUNC
	.globl	foo
foo:
	pushq	%rbp
	movq	%rsp, %rbp
##TAC_OUTPUT
	leaq	_str558f4167d460(%rip), %rax
	movq	%rax, %rsi
	leaq	printstringstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
##TAC_INPUT
	leaq	_mYWeeidT_emp0(%rip), %rax
	movq	%rax, %rsi
	leaq	printintstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
##TAC_COPY
	movl	_mYWeeidT_emp0(%rip), %eax
	movl	%eax, _xa(%rip)
##TAC_RETURN
	movl	_xa(%rip), %eax
	popq	%rbp
	ret
##TAC_ENDFUN
	popq %rbp
	ret
##TAC_BEGINFUNC
	.globl	main
main:
	pushq	%rbp
	movq	%rsp, %rbp
##TAC_SUB
	movl	_d(%rip), %eax
	movl	_i(%rip), %edx
	subl	%edx, %eax
	movl	%eax, _mYWeeidT_emp1(%rip)
##TAC_COPY
	movl	_mYWeeidT_emp1(%rip), %eax
	movl	%eax, _d(%rip)
##TAC_DIV
	movl	_3(%rip), %eax
	movl	_2(%rip), %ecx
	cltd
	idivl	%ecx
	movl	%eax, _mYWeeidT_emp2(%rip)
##TAC_COPY
	movl	_mYWeeidT_emp2(%rip), %eax
	movl	%eax, _a(%rip)
##TAC_OUTPUT
	movl	_a(%rip), %esi
	leaq	printintstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
##TAC_OUTPUT
	leaq	_str558f4167dbf0(%rip), %rax
	movq	%rax, %rsi
	leaq	printstringstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
##TAC_COPY
	movl	_a(%rip), %eax
	movl	%eax, _xa(%rip)
##TAC_FUNC_CALL
	call	foo
	movl	%eax, _mYWeeidT_emp3(%rip) ##FUNC RETURN
##TAC_COPY
	movl	_mYWeeidT_emp3(%rip), %eax
	movl	%eax, _a(%rip)
##TAC_DIV - REAL
	movss	_3.0(%rip), %xmm0
	movss	_2.0(%rip), %xmm1
	divss   %xmm1, %xmm0
	movss	%xmm0, _mYWeeidT_emp4(%rip)
##TAC_COPY
	movl	_mYWeeidT_emp4(%rip), %eax
	movl	%eax, _xx(%rip)
##TAC_OUTPUT
	movss	_xx(%rip), %xmm0
	pxor	%xmm1, %xmm1
	cvtss2sd	%xmm0, %xmm1
	movq	%xmm1, %rax
	movq	%rax, %xmm0
	leaq	printfloatstr(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
##TAC_OUTPUT
	leaq	_str558f4167dbf0(%rip), %rax
	movq	%rax, %rsi
	leaq	printstringstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
##READ_VEC
	movl	_0(%rip), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	_vv(%rip), %rax
	movl	(%rdx,%rax), %eax
	movl	%eax, _mYWeeidT_emp5(%rip)
##TAC_OUTPUT
	movss	_mYWeeidT_emp5(%rip), %xmm0
	pxor	%xmm1, %xmm1
	cvtss2sd	%xmm0, %xmm1
	movq	%xmm1, %rax
	movq	%rax, %xmm0
	leaq	printfloatstr(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
##TAC_OUTPUT
	leaq	_str558f4167dbf0(%rip), %rax
	movq	%rax, %rsi
	leaq	printstringstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
_mYWeeidL_abule0:
##COMPARE
	movl	_a(%rip), %edx
	movl	_30(%rip), %eax
	cmpl	%eax, %edx
	jge	_mYWeeidL_abule1
##TAC_OUTPUT
	movl	_a(%rip), %esi
	leaq	printintstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
##TAC_OUTPUT
	leaq	_str558f4167e4c0(%rip), %rax
	movq	%rax, %rsi
	leaq	printstringstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
##TAC_ADD
	movl	_a(%rip), %edx
	movl	_1(%rip), %eax
	addl	%edx, %eax
	movl	%eax, _mYWeeidT_emp7(%rip)
##TAC_COPY
	movl	_mYWeeidT_emp7(%rip), %eax
	movl	%eax, _a(%rip)
	jmp	_mYWeeidL_abule0
_mYWeeidL_abule1:
##COMPARE
	movl	_a(%rip), %edx
	movl	_100(%rip), %eax
	cmpl	%eax, %edx
	jge	_mYWeeidL_abule2
##TAC_OUTPUT
	leaq	_str558f4167e950(%rip), %rax
	movq	%rax, %rsi
	leaq	printstringstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	_mYWeeidL_abule3
_mYWeeidL_abule2:
##TAC_OUTPUT
	leaq	_str558f4167eab0(%rip), %rax
	movq	%rax, %rsi
	leaq	printstringstr(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
_mYWeeidL_abule3:
##TAC_ENDFUN
	popq %rbp
	ret
#DATA SECTION
.data

_mYWeeidT_emp0:	.long	0
_mYWeeidT_emp5:	.float	0.0
_vv:
.float 2.2
.float 3.2
_a:	.long	2
_c:	.long	120
_d:	.long	100
_i:	.long	1
_x:	.long	1
_mYWeeidT_emp2:	.long	0
_mYWeeidT_emp7:	.long	0
_mYWeeidT_emp4:	.float	0.0
_xx:	.float	1.2
_mYWeeidT_emp1:	.long	0
_mYWeeidT_emp6:	.long	0
_xa:	.long	0
_mYWeeidT_emp3:	.long	0
_mYWeeidT_emp8:	.long	0
_3.2:	.float	3.2
_0:	.long	0
_1:	.long	1
_2:	.long	2
_3:	.long	3
_3.0:	.float	3.0
_str558f4167e4c0:	.string	" "
_str558f4167d460:	.string	"escolha um numero: "
_str558f4167eab0:	.string	"OK!\n"
_1.2:	.float	1.2
_30:	.long	30
_100:	.long	100
_2.2:	.float	2.2
_'x':	.long	120
_str558f4167e950:	.string	"Nao tem como isso...\n"
_str558f4167dbf0:	.string	"\n"
_2.0:	.float	2.0
