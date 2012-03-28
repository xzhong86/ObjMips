/*
 * crt0.S -- startup file for MIPS.
 *
 * Copyright (c) 1995, 1996, 1997 Cygnus Support
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */


#include <sys/regdef.h>
#include <pcpu.h>
#include <cachecfg.h>
#include <mips.h>

#define KSEG0 K0BASE

/*
 * Set up some room for a stack. We just grab a chunk of memory.
 */
#define STACK_SIZE  0x4000
#define GLOBAL_SIZE 0x2000

#define STARTUP_STACK_SIZE	0x0100

#define SETUP_CCA	3

/* This is for referencing addresses that are not in the .sdata or
   .sbss section under embedded-pic, or before we've set up gp.  */
#define LA(t,x) la t,x

	.comm	__memsize, 12
	.comm	__lstack, STARTUP_STACK_SIZE
	.comm   __stackbase,4

	.text
	.align	2

/* Without the following nop, GDB thinks _start is a data variable.
 * This is probably a bug in GDB in handling a symbol that is at the
 * start of the .text section.
 */
	nop

	.globl	fw_arg_arr

	.globl	_start
	.ent	_start
_start:
	.set	noreorder

#define STATUS_MASK (SR_CU0)
	li	v0, STATUS_MASK
	mtc0	v0, C0_SR
	mtc0	zero, C0_CAUSE
	nop

	/* flush d-cache */
	li	t0, KSEG0
	addu	t1, t0, CFG_DCACHE_SIZE
1:
	cache	Index_Writeback_Inv_D, 0(t0)
	bne	t0, t1, 1b
	addi	t0, CFG_CACHELINE_SIZE

	sync

	/* flush i-cache */
	li	t0, KSEG0
	addu	t1, t0, CFG_ICACHE_SIZE
1:
	cache	Index_Invalidate_I, 0(t0)
	bne	t0, t1, 1b
	addi	t0, CFG_CACHELINE_SIZE

	mfc0	v0, C0_CONFIG
	li	v1, ~0x7
	and	v0, v1
	ori	v0, v0, SETUP_CCA
	mtc0	v0, C0_CONFIG
	mtc0	zero, C0_CAUSE

	LA	v0, fw_arg_arr
	sw	a0, 0(v0)
	sw	a1, 4(v0)
	sw	a2, 8(v0)
	sw	a3, 12(v0)

2:

/* Fix high bits, if any, of the PC so that exception handling 
   doesn't get confused.  */
	LA (v0, 3f)
	jr	v0
	nop
3:
	LA (gp, _gp)				# set the global data pointer
	.end _start

/*
 * zero out the bss section.
 */
	.globl	__memsize
	.globl	get_mem_info .text
	.globl	__stack
	.globl	__global
	.globl	zerobss
	.ent	zerobss
zerobss:
	LA (v0, _fbss)
	LA (v1, _end)
3:
	sw	zero,0(v0)
	bltu	v0,v1,3b
	addiu	v0,v0,4				# executed in delay slot
	
	la	t0, __lstack			# make a small stack so we
	addiu	sp, t0, STARTUP_STACK_SIZE	# can run some C code
	la	a0, __memsize			# get the usable memory size
	jal	get_mem_info
	nop

	/* setup the stack pointer */
	LA (t0, __stack)			# is __stack set ?
	bne	t0,zero,4f
	nop

	/* NOTE: a0[0] contains the amount of memory available, and
	         not the last memory address. */
	lw	t0,0(a0)			# last address of memory available
	la	t1,K0BASE			# cached kernel memory
	addu	t0,t0,t1			# get the end of memory address
	/* We must subtract 24 bytes for the 3 8 byte arguments to main, in
	   case main wants to write them back to the stack.  The caller is
	   supposed to allocate stack space for parameters in registers in
	   the old MIPS ABIs.  We must do this even though we aren't passing
	   arguments, because main might be declared to have them. 

	   Some ports need a larger alignment for the stack, so we subtract
	   32, which satisifes the stack for the arguments and keeps the
	   stack pointer better aligned.  */
	subu	t0,t0,32			# and generate a starting stack-pointer
4:
	move	sp,t0				# set stack pointer
	li	sp,PCPU_SP_TOP(0)
	sw	sp,__stackbase			# keep this for future ref
	.end	zerobss

/*
 * initialize target specific stuff. Only execute these
 * functions it they exist.
 */
	.globl	_call_main

	.globl	hardware_init_fun .text
	.globl	software_init_fun .text
	.globl	atexit .text
	.globl	exit .text
	.globl	init
	.ent	init
init:
	jal	hardware_init_fun
	nop
	jal	software_init_fun
	nop

	move	a0,zero				# set argc to 0
	jal	_call_main			# call the program start function
	nop

	# fall through to the "exit" routine
	jal	exit				# call libc exit to run the G++
						# destructors
	move	a0,v0				# pass through the exit code
	.end	init
	
/*
 * _exit -- Exit from the application. Normally we cause a user trap
 *          to return to the ROM monitor for another run. NOTE: This is
 *	    the only other routine we provide in the crt0.o object, since
 *          it may be tied to the "_start" routine. It also allows
 *          executables that contain a complete world to be linked with
 *          just the crt0.o object.
 */
	.globl	sys_exit
	.globl	_exit
	.ent _exit
_exit:
	jal	sys_exit
	nop
7:
	wait
	nop
	b	7b				# but loop back just in-case
	nop
	.end _exit


	.globl	start_secondary
	.globl	_start_secondary
	.ent	_start_secondary
_start_secondary:
	.set	noreorder

#define STATUS_MASK (SR_CU0)
	mtc0	zero, C0_CAUSE
	li	v0, STATUS_MASK
	mtc0	v0, C0_SR
	nop

	li	v0, 3
	sw	v0, 0($26)

#if 1
	/* Init cache */
	li	t0, KSEG0
	addu	t1, t0, CFG_DCACHE_SIZE
	mtc0	zero, $28, 0		// CP0_TAGLO
	mtc0	zero, $28, 1		// CP0_DATALO
1:
	/* cache_clear_a_line */
	cache	Index_Store_Tag_I, 0(t0)	// Index_Store_Tag_I
	cache	Index_Store_Tag_D, 0(t0)	// Index_Store_Tag_D
	bne	t0, t1, 1b
	addu	t0, t0, 32		// increment CACHE_LINE_SIZE
#else

	/* flush d-cache */
	li	t0, KSEG0
	addu	t1, t0, CFG_DCACHE_SIZE
2:
	cache	Index_Writeback_Inv_D, 0(t0)
	bne	t0, t1, 2b
	addi	t0, CFG_CACHELINE_SIZE

	sync

	/* flush i-cache */
	li	t0, KSEG0
	addu	t1, t0, CFG_ICACHE_SIZE
3:
	cache	Index_Invalidate_I, 0(t0)
	bne	t0, t1, 3b
	addi	t0, CFG_CACHELINE_SIZE
#endif

	mfc0	v0, C0_CONFIG
	li	v1, ~0x7
	and	v0, v1
	ori	v0, v0, SETUP_CCA
	mtc0	v0, C0_CONFIG
	mtc0	zero, C0_CAUSE
	
	li	v0, 4
	sw	v0, 0($26)
#if 0
1:	li	v0, 7
	sw	v0, 0($26)
	b	1b
	nop
#endif

	/* get cpu mask bit */
	mfc0	v0, C0_EBASE
	and	v0, v0, 0x3ff
	addu	v1, zero, 1
	sllv	v1, v1, v0

	/* set uncached cpu_ready bitmap */
	li	v0, 0x20000000
	addu	t8, t8, v0
1:	
	ll	v0, 0(t8)
	or	v0, v0, v1
	sc	v0, 0(t8)
	beqz	v0, 1b
	nop

	/* wait for set cpu_start. busy loop! */
2:
	lw	v0, 0(t7)
	and	v0, v0, v1
	beqz	v0, 2b
	nop

	/* read out sp */
	lw	sp, 0(sp)

	/* set cpu_running bitmap */
3:
	ll	v0, 0(t9)
	or	v0, v0, v1
	sc	v0, 0(t9)
	beqz	v0, 3b
	nop

	LA (gp, _gp)				# set the global data pointer

	jal	start_secondary
	nop
	
	.end _start_secondary
