#include <sys/regdef.h>
#include <mips.h>

#define  SAVE(n) sw	$##n,n*4(k1)
#define  LOAD(n) lw	$##n,n*4(k1)

	.macro	setup_c0_status set clr
	.set	push
	mfc0	t0, C0_SR
	or	t0, \set|0x1f|\clr
	xor	t0, 0x1f|\clr
	mtc0	t0, C0_SR
	.set	noreorder
	sll	zero,3				# ehb
	.set	pop
	.endm

	.macro	clear_exl_ie
	setup_c0_status 0 3
	.endm

	.macro	set_exl_ie
	setup_c0_status 3 0
	.endm
	
	.text
	.align	2

	.globl	trap_entry
	.globl	_exp_int
	.ent	_exp_int
_exp_int:
	.set	noreorder
	.set	noat
	subu	k1,sp,32*4 + 16
	SAVE(1)
	SAVE(2)
	SAVE(3)
	SAVE(4)
	SAVE(5)
	SAVE(6)
	SAVE(7)
	SAVE(8)
	SAVE(9)
	SAVE(10)
	SAVE(11)
	SAVE(12)
	SAVE(13)
	SAVE(14)
	SAVE(15)
	SAVE(16)
	SAVE(17)
	SAVE(18)
	SAVE(19)
	SAVE(20)
	SAVE(21)
	SAVE(22)
	SAVE(23)
	SAVE(24)
	SAVE(25)
	SAVE(28)
	SAVE(29) /* save SP */
	SAVE(30)
	SAVE(31)

	mfc0	a1, C0_SR
	move	a0, k1
	subu	sp,k1,4*4
	clear_exl_ie
	jal trap_entry
	nop
/*	set_exl_ie */
	addu	k1,sp,4*4
	
	LOAD(1)
	LOAD(2)
	LOAD(3)
	LOAD(4)
	LOAD(5)
	LOAD(6)
	LOAD(7)
	LOAD(8)
	LOAD(9)
	LOAD(10)
	LOAD(11)
	LOAD(12)
	LOAD(13)
	LOAD(14)
	LOAD(15)
	LOAD(16)
	LOAD(17)
	LOAD(18)
	LOAD(19)
	LOAD(20)
	LOAD(21)
	LOAD(22)
	LOAD(23)
	LOAD(24)
	LOAD(25)
	LOAD(28)
	LOAD(29) /* restore SP */
	LOAD(30)
	LOAD(31)
	eret
	.end	_exp_int