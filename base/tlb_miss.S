#include <sys/regdef.h>
#include <mips.h>

	.text
	.align	2

	.globl	current_range
	.globl	_tlb_miss
	.ent	_tlb_miss
_tlb_miss:
	.set	noreorder
	.set	noat
	mfc0	k0, C0_EBASE
	lui	k1, %hi(current_range)
	and	k0, k0, 0x3ff
	addiu	k1, %lo(current_range)
	sll	k0, k0, 2
	addu	k1, k0
	lw	k1, 0(k1)
	beqz	k1, badtlb
	nop

	subu	sp, sp, (4)*4
	sw	AT, 0(sp)
	sw	v0, 4(sp)
	sw	v1, 8(sp)
	sw	a0, 12(sp)
	.set	at

	addu	k0, zero
	mfc0	a0, C0_BadVaddr
	lw	v0, 0(k1)	/* range.start */
	blt	a0, v0, out_mem
	nop
	lw	v1, 4(k1)	/* range.end */
	bge	a0, v1, out_mem
	nop

	subu	v0, a0, v0
	srl	v0, v0, 13	/* double page group */
	sll	v0, v0, 3	/* 8-word each pair */
	addu	v1, k1, 8	/* EntryLo array addr */
	addu	v1, v1, v0	/* EntryLo pair addr */
	lw	k0, 0(v1)	/* load EntryLo0 */
	lw	k1, 4(v1)	/* load EntryLo1 */
out_mem:
	.set	noat
	lw	AT, 0(sp)
	lw	v0, 4(sp)
	lw	v1, 8(sp)
	lw	a0, 12(sp)
	addu	sp, sp, (4)*4

	beqz	k0, badtlb
	nop
	mtc0	zero, C0_PageMask
	mtc0	k0, C0_EntryLo0
	mtc0	k1, C0_EntryLo1
	nop
	nop
	nop
	sll	$0, $0, 3	/* ehb */
	tlbwr
	nop
	eret
	nop

badtlb:
	mtc0	zero, C0_PageMask
	li	k0, 0x400
	mtc0	k0, C0_EntryLo0
	addu	k0, k0, 0x400
	mtc0	k0, C0_EntryLo1
	nop
	nop
	nop
	sll	$0, $0, 3	/* ehb */
	tlbwr
	nop
	eret
	.end	_tlb_miss
