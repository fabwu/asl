	.file	"microbenchmark.cpp"
	.text
	.globl	_Z8init_tscv
	.type	_Z8init_tscv, @function
_Z8init_tscv:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_Z8init_tscv, .-_Z8init_tscv
	.globl	_Z9start_tscv
	.type	_Z9start_tscv, @function
_Z9start_tscv:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	.cfi_offset 3, -24
	movl	$0, %eax
#APP
# 83 "include/tsc_x86.h" 1
	cpuid
# 0 "" 2
# 84 "include/tsc_x86.h" 1
	rdtsc
# 0 "" 2
#NO_APP
	movl	%eax, -16(%rbp)
	movl	%edx, -12(%rbp)
	movq	-16(%rbp), %rax
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	_Z9start_tscv, .-_Z9start_tscv
	.globl	_Z8stop_tscy
	.type	_Z8stop_tscy, @function
_Z8stop_tscy:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	.cfi_offset 3, -24
	movq	%rdi, -32(%rbp)
#APP
# 90 "include/tsc_x86.h" 1
	rdtsc
# 0 "" 2
#NO_APP
	movl	%eax, -16(%rbp)
	movl	%edx, -12(%rbp)
	movl	$0, %eax
#APP
# 91 "include/tsc_x86.h" 1
	cpuid
# 0 "" 2
#NO_APP
	movq	-16(%rbp), %rax
	subq	-32(%rbp), %rax
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	_Z8stop_tscy, .-_Z8stop_tscy
	.type	_ZL6sqrtsdd, @function
_ZL6sqrtsdd:
.LFB249:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movsd	%xmm0, -24(%rbp)
	movsd	-24(%rbp), %xmm0
#APP
# 5 "include/sigmoid.h" 1
	sqrtsd %xmm0, %xmm0
# 0 "" 2
#NO_APP
	movsd	%xmm0, -8(%rbp)
	movsd	-8(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE249:
	.size	_ZL6sqrtsdd, .-_ZL6sqrtsdd
	.type	_ZL8sigmoid1d, @function
_ZL8sigmoid1d:
.LFB250:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movsd	%xmm0, -8(%rbp)
	movsd	-8(%rbp), %xmm0
	movapd	%xmm0, %xmm1
	mulsd	%xmm0, %xmm1
	movsd	.LC0(%rip), %xmm0
	addsd	%xmm1, %xmm0
	call	_ZL6sqrtsdd
	movsd	-8(%rbp), %xmm1
	divsd	%xmm0, %xmm1
	movapd	%xmm1, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE250:
	.size	_ZL8sigmoid1d, .-_ZL8sigmoid1d
	.type	_ZL8sigmoid2d, @function
_ZL8sigmoid2d:
.LFB251:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movsd	%xmm0, -8(%rbp)
	movsd	-8(%rbp), %xmm1
	movsd	.LC1(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	call	atan@PLT
	movsd	.LC2(%rip), %xmm1
	mulsd	%xmm1, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE251:
	.size	_ZL8sigmoid2d, .-_ZL8sigmoid2d
	.globl	_Z30initialize_microbenchmark_data21microbenchmark_mode_t
	.type	_Z30initialize_microbenchmark_data21microbenchmark_mode_t, @function
_Z30initialize_microbenchmark_data21microbenchmark_mode_t:
.LFB252:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE252:
	.size	_Z30initialize_microbenchmark_data21microbenchmark_mode_t, .-_Z30initialize_microbenchmark_data21microbenchmark_mode_t
	.globl	_Z31microbenchmark_get_sqrt_latencyv
	.type	_Z31microbenchmark_get_sqrt_latencyv, @function
_Z31microbenchmark_get_sqrt_latencyv:
.LFB253:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$100, -28(%rbp)
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -24(%rbp)
	call	_Z9start_tscv
	movq	%rax, -16(%rbp)
	movl	$1, -32(%rbp)
.L15:
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jge	.L14
	movq	-24(%rbp), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -24(%rbp)
	addl	$1, -32(%rbp)
	jmp	.L15
.L14:
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	_Z8stop_tscy
	movl	-28(%rbp), %edx
	movslq	%edx, %rcx
	movl	$0, %edx
	divq	%rcx
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	testq	%rax, %rax
	js	.L16
	cvtsi2sdq	%rax, %xmm0
	jmp	.L18
.L16:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L18:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE253:
	.size	_Z31microbenchmark_get_sqrt_latencyv, .-_Z31microbenchmark_get_sqrt_latencyv
	.globl	_Z27microbenchmark_get_sqrt_gapv
	.type	_Z27microbenchmark_get_sqrt_gapv, @function
_Z27microbenchmark_get_sqrt_gapv:
.LFB254:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movl	$1000, -108(%rbp)
	call	_Z9start_tscv
	movq	%rax, -16(%rbp)
	movl	$1, -112(%rbp)
.L21:
	movl	-112(%rbp), %eax
	cmpl	-108(%rbp), %eax
	jge	.L20
	movq	.LC0(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -104(%rbp)
	movq	.LC3(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -96(%rbp)
	movq	.LC4(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -88(%rbp)
	movq	.LC5(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -80(%rbp)
	movq	.LC6(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -72(%rbp)
	movq	.LC7(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -64(%rbp)
	movq	.LC7(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -56(%rbp)
	movq	.LC7(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -48(%rbp)
	movq	.LC7(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -40(%rbp)
	movq	.LC7(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -32(%rbp)
	movq	.LC7(%rip), %rax
	movq	%rax, %xmm0
	call	_ZL6sqrtsdd
	movq	%xmm0, %rax
	movq	%rax, -24(%rbp)
	addl	$1, -112(%rbp)
	jmp	.L21
.L20:
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	_Z8stop_tscy
	movl	-108(%rbp), %edx
	movslq	%edx, %rcx
	movl	$0, %edx
	divq	%rcx
	movabsq	$3353953467947191203, %rdx
	mulq	%rdx
	movq	%rdx, %rax
	shrq	%rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	testq	%rax, %rax
	js	.L22
	cvtsi2sdq	%rax, %xmm0
	jmp	.L24
.L22:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L24:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE254:
	.size	_Z27microbenchmark_get_sqrt_gapv, .-_Z27microbenchmark_get_sqrt_gapv
	.globl	_Z35microbenchmark_get_sigmoid1_latencyv
	.type	_Z35microbenchmark_get_sigmoid1_latencyv, @function
_Z35microbenchmark_get_sigmoid1_latencyv:
.LFB255:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pxor	%xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE255:
	.size	_Z35microbenchmark_get_sigmoid1_latencyv, .-_Z35microbenchmark_get_sigmoid1_latencyv
	.globl	_Z31microbenchmark_get_sigmoid1_gapv
	.type	_Z31microbenchmark_get_sigmoid1_gapv, @function
_Z31microbenchmark_get_sigmoid1_gapv:
.LFB256:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pxor	%xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE256:
	.size	_Z31microbenchmark_get_sigmoid1_gapv, .-_Z31microbenchmark_get_sigmoid1_gapv
	.globl	_Z35microbenchmark_get_sigmoid2_latencyv
	.type	_Z35microbenchmark_get_sigmoid2_latencyv, @function
_Z35microbenchmark_get_sigmoid2_latencyv:
.LFB257:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pxor	%xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE257:
	.size	_Z35microbenchmark_get_sigmoid2_latencyv, .-_Z35microbenchmark_get_sigmoid2_latencyv
	.globl	_Z31microbenchmark_get_sigmoid2_gapv
	.type	_Z31microbenchmark_get_sigmoid2_gapv, @function
_Z31microbenchmark_get_sigmoid2_gapv:
.LFB258:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pxor	%xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE258:
	.size	_Z31microbenchmark_get_sigmoid2_gapv, .-_Z31microbenchmark_get_sigmoid2_gapv
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC1:
	.long	1413754136
	.long	1073291771
	.align 8
.LC2:
	.long	1841940611
	.long	1071931184
	.align 8
.LC3:
	.long	0
	.long	1073741824
	.align 8
.LC4:
	.long	0
	.long	1074266112
	.align 8
.LC5:
	.long	0
	.long	1074790400
	.align 8
.LC6:
	.long	0
	.long	1075052544
	.align 8
.LC7:
	.long	0
	.long	1075314688
	.ident	"GCC: (Arch Linux 9.2.1+20200130-2) 9.2.1 20200130"
	.section	.note.GNU-stack,"",@progbits
