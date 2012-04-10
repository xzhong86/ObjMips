#include <sys/regdef.h>
#include <mips.h>

	.text
	.align	2

	.globl	current_pgd
	.globl	_tlb_miss
	.ent	_tlb_miss
_tlb_miss:
	.set	noreorder
	.set	noat
	mfc0	k0, C0_EBASE
	lui	k1, %hi(current_pgd)
	and	k0, k0, 0x3ff
	addiu	k1, %lo(current_pgd)
	sll	k0, k0, 2
	addu	k1, k0
	lw	k1, 0(k1)	/* struct pgd addr */
	beqz	k1, badtlb
	nop

	mfc0	k0, C0_BadVaddr
	srl	k0, 22
	sll	k0, 2
	addu	k1, k0
	lw	k1, 0(k1)	/* struct pte addr */
	beqz	k1, badtlb
	nop

	mfc0	k0, C0_BadVaddr
	srl	k0, 10
	and	k0, k0, 0xff8
	addu	k1, k0
	lw	k0, 0(k1)
	lw	k1, 4(k1)

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
