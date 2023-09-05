##FIXED INIT
.section	.rodata
printintstr: .asciz	"%d" 
printstringstr: .asciz "%s" 
printcharstr: .asciz "%c" 
.text
##TAC_BEGINFUNC
	.globl	_func
_func:
	pushq	%rbp
	movq	%rsp, %rbp
##TAC_ENDFUN
	popq %rbp
	ret
##TAC_BEGINFUNC
	.globl	_main
_main:
	pushq	%rbp
	movq	%rsp, %rbp
##TAC_OUTPUT
leaq	_str55d53b4eaba0(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4eaba0(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4ead70(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4ead70(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4eb6d0(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4eb6d0(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4eb4a0(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4eb4a0(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4ebaf0(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4ebaf0(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4ec000(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4ec000(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4ec160(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_OUTPUT
leaq	_str55d53b4ec160(%rip), %rax
movq	%rax, %rsi
leaq	printstringstr(%rip), %rax
movq	%rax, %rdi
movl	$0, %eax
call	printf@PLT
##TAC_ENDFUN
	popq %rbp
	ret
##TAC_BEGINFUNC
	.globl	_incn
_incn:
	pushq	%rbp
	movq	%rsp, %rbp
##TAC_ENDFUN
	popq %rbp
	ret
#DATA SECTION
.data

_mYWeeidT_emp17:	.long	0
_mYWeeidT_emp0:	.long	0
_mYWeeidT_emp5:	.long	0
_a:	.long	9
_b:	.long	0
_c:	.long	120
_d:	.long	100
_e:	.long	0
_f:	.long	2.5
_g:	.long	0
_h:	.long	0
_i:	.long	1
_n:	.long	0
_mYWeeidT_emp14:	.long	0
_x:	.long	0
_mYWeeidT_emp11:	.long	0
_mYWeeidT_emp2:	.long	0
_mYWeeidT_emp7:	.long	0
_mYWeeidT_emp16:	.long	0
_mYWeeidT_emp4:	.long	0
_mYWeeidT_emp9:	.long	0
_mYWeeidT_emp13:	.long	0
_boo:	.long	1
_mYWeeidT_emp10:	.long	0
_mYWeeidT_emp1:	.long	0
_mYWeeidT_emp6:	.long	0
_a.a:	.long	65
_mYWeeidT_emp15:	.long	0
_abc:	.long	0
_xa:	.long	0
_na:	.long	0
_mYWeeidT_emp12:	.long	0
_mYWeeidT_emp3:	.long	0
_mYWeeidT_emp8:	.long	0
_0:	.long	0
_1:	.long	1
_2:	.long	2
_5:	.long	5
_9:	.long	9
_str55d53b4ebaf0:	.string	"A era=15\n"
_str55d53b4ead70:	.string	"Digite mais um numero: \n"
_str55d53b4ec160:	.string	"OK!\n"
_10:	.long	10
_100:	.long	100
_15:	.long	15
_str55d53b4eaba0:	.string	"Digite um numero: \n"
_'A':	.long	65
_2.5:	.long	2.5
_'x':	.long	120
_'a':	.long	97
_str55d53b4ec000:	.string	"Nao tem como isso...\n"
_str55d53b4eb6d0:	.string	"\n"
_55:	.long	55
_str55d53b4eb4a0:	.string	"Incrementado algumas vezes a fica "
