	.file	"out.c"
	.text
	.section	.rodata
.LC2:
	.string	"%d\n"
.LC3:
	.string	"%lf\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -16(%rbp)
	movsd	-16(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
	movsd	-8(%rbp), %xmm0
	cvttsd2sil	%xmm0, %eax
	cvtsi2sdl	%eax, %xmm1
	movsd	-8(%rbp), %xmm0
	subsd	%xmm1, %xmm0
	pxor	%xmm1, %xmm1
	ucomisd	%xmm1, %xmm0
	jp	.L2
	pxor	%xmm1, %xmm1
	ucomisd	%xmm1, %xmm0
	jne	.L2
	movsd	-8(%rbp), %xmm0
	cvttsd2sil	%xmm0, %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L4
.L2:
	movq	-8(%rbp), %rax
	movq	%rax, %xmm0
	leaq	.LC3(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
.L4:
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1076232192
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
