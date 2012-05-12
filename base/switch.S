#include <sys/regdef.h>
#include <mips.h>
#include <thread.h>

	.text
	.align	2

	.globl	new_thread_init
	.globl	thread_switch
	.ent	thread_switch
thread_switch:
	.set	noreorder
	subu	sp, 11*4
	sw	s0, 0 * 4(sp)
	sw	s1, 1 * 4(sp)
	sw	s2, 2 * 4(sp)
	sw	s3, 3 * 4(sp)
	sw	s4, 4 * 4(sp)
	sw	s5, 5 * 4(sp)
	sw	s6, 6 * 4(sp)
	sw	s7, 7 * 4(sp)
	sw	gp, 8 * 4(sp)
	sw	ra, 9 * 4(sp)
	sw	s8, 10* 4(sp)
	sw	sp, THREAD_SP(a0)

	lw	a2, THREAD_FLAGS(a1)
	lw	sp, THREAD_SP(a1)
	move	HEAD_REG, a1
	and	a2, _TIF_CREATE

	bnez	a2, 1f
	nop

	lw	s0, 0 * 4(sp)
	lw	s1, 1 * 4(sp)
	lw	s2, 2 * 4(sp)
	lw	s3, 3 * 4(sp)
	lw	s4, 4 * 4(sp)
	lw	s5, 5 * 4(sp)
	lw	s6, 6 * 4(sp)
	lw	s7, 7 * 4(sp)
	lw	gp, 8 * 4(sp)
	lw	ra, 9 * 4(sp)
	lw	s8, 10* 4(sp)
	addu	sp, 11*4
	jr	ra
	nop

1:
	subu	sp, 16
	la	ra, new_thread_init
	jr	ra
	nop
	.end	thread_switch
