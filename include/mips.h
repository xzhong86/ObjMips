/*
 * regs.S -- standard MIPS register names.
 *
 * Copyright (c) 1995 Cygnus Support
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

/* Useful memory constants: */
#define K0BASE		0x80000000
#ifndef __mips64
#define K1BASE		0xA0000000
#else
#define K1BASE		0xFFFFFFFFA0000000LL
#endif

#define PHYS_TO_K0(a)   ((unsigned)(a) + K0BASE)
#define PHYS_TO_K1(a)   ((unsigned)(a) + K1BASE)
#define K0_TO_PHYS(a)   ((unsigned)(a) - K0BASE)
#define K1_TO_PHYS(a)   ((unsigned)(a) - K1BASE)

#ifndef K0_TO_K1
#define K0_TO_K1(a)   ((unsigned)(a) - K0BASE + K1BASE)
#endif
#define K1_TO_K0(a)   ((unsigned)(a) - K1BASE + K0BASE)

/* Standard Co-Processor 0 register numbers: */
#define C0_COUNT	$9		/* Count Register */
#define C0_SR		$12		/* Status Register */
#define C0_CAUSE	$13		/* last exception description */
#define C0_EPC		$14		/* Exception error address */
#define C0_PRID		$15		/* Processor Revision ID */
#define C0_EBASE	$15, 1		/* CPU exception base and CPU NUM */
#define C0_CONFIG	$16		/* CPU configuration */

/* For MMU mannage */
#define C0_Index	$0		/*  */
#define C0_Random	$1		/*  */
#define C0_EntryLo0	$2		/*  */
#define C0_EntryLo1	$3		/*  */
#define C0_Context	$4		/*  */
#define C0_PageMask	$5		/*  */
#define C0_BadVaddr	$8		/*  */
#define C0_EntryHi	$10		/*  */


/* Standard Processor Revision ID Register field offsets */
#define PR_IMP	8

/* Standard Config Register field offsets */
#define CR_DB	4
#define CR_IB	5
#define CR_DC	6	/* NOTE v4121 semantics != 43,5xxx semantics */
#define CR_IC	9	/* NOTE v4121 semantics != 43,5xxx semantics */
#define CR_SC	17
#define CR_SS	20
#define CR_SB	22


/* Standard Status Register bitmasks: */
#define SR_CU1		0x20000000	/* Mark CP1 as usable */
#define SR_CU0		0x10000000	/* Mark CP0 as usable */
#define SR_FR		0x04000000	/* Enable MIPS III FP registers */
#define SR_BEV		0x00400000	/* Controls location of exception vectors */
#define SR_PE		0x00100000	/* Mark soft reset (clear parity error) */

#define SR_KX		0x00000080	/* Kernel extended addressing enabled */
#define SR_SX		0x00000040	/* Supervisor extended addressing enabled */
#define SR_UX		0x00000020	/* User extended addressing enabled */

/* Standard (R4000) cache operations. Taken from "MIPS R4000
   Microprocessor User's Manual" 2nd edition: */

#define CACHE_I		(0)	/* primary instruction */
#define CACHE_D		(1)	/* primary data */
#define CACHE_SI	(2)	/* secondary instruction */
#define CACHE_SD	(3)	/* secondary data (or combined instruction/data) */

#define INDEX_INVALIDATE		(0)	/* also encodes WRITEBACK if CACHE_D or CACHE_SD */
#define INDEX_LOAD_TAG			(1)
#define INDEX_STORE_TAG			(2)
#define CREATE_DIRTY_EXCLUSIVE		(3)	/* CACHE_D and CACHE_SD only */
#define HIT_INVALIDATE			(4)
#define CACHE_FILL			(5)	/* CACHE_I only */
#define HIT_WRITEBACK_INVALIDATE	(5)	/* CACHE_D and CACHE_SD only */
#define HIT_WRITEBACK			(6)	/* CACHE_I, CACHE_D and CACHE_SD only */
#define HIT_SET_VIRTUAL			(7)	/* CACHE_SI and CACHE_SD only */

#define BUILD_CACHE_OP(o,c)		(((o) << 2) | (c))

/* Individual cache operations: */
#define INDEX_INVALIDATE_I		BUILD_CACHE_OP(INDEX_INVALIDATE,CACHE_I)
#define INDEX_WRITEBACK_INVALIDATE_D	BUILD_CACHE_OP(INDEX_INVALIDATE,CACHE_D)
#define INDEX_INVALIDATE_SI             BUILD_CACHE_OP(INDEX_INVALIDATE,CACHE_SI)
#define INDEX_WRITEBACK_INVALIDATE_SD	BUILD_CACHE_OP(INDEX_INVALIDATE,CACHE_SD)

#define INDEX_LOAD_TAG_I		BUILD_CACHE_OP(INDEX_LOAD_TAG,CACHE_I)
#define INDEX_LOAD_TAG_D                BUILD_CACHE_OP(INDEX_LOAD_TAG,CACHE_D)
#define INDEX_LOAD_TAG_SI               BUILD_CACHE_OP(INDEX_LOAD_TAG,CACHE_SI)
#define INDEX_LOAD_TAG_SD               BUILD_CACHE_OP(INDEX_LOAD_TAG,CACHE_SD)

#define INDEX_STORE_TAG_I              	BUILD_CACHE_OP(INDEX_STORE_TAG,CACHE_I)
#define INDEX_STORE_TAG_D               BUILD_CACHE_OP(INDEX_STORE_TAG,CACHE_D)
#define INDEX_STORE_TAG_SI              BUILD_CACHE_OP(INDEX_STORE_TAG,CACHE_SI)
#define INDEX_STORE_TAG_SD              BUILD_CACHE_OP(INDEX_STORE_TAG,CACHE_SD)

#define CREATE_DIRTY_EXCLUSIVE_D        BUILD_CACHE_OP(CREATE_DIRTY_EXCLUSIVE,CACHE_D)
#define CREATE_DIRTY_EXCLUSIVE_SD       BUILD_CACHE_OP(CREATE_DIRTY_EXCLUSIVE,CACHE_SD)

#define HIT_INVALIDATE_I                BUILD_CACHE_OP(HIT_INVALIDATE,CACHE_I)
#define HIT_INVALIDATE_D                BUILD_CACHE_OP(HIT_INVALIDATE,CACHE_D)
#define HIT_INVALIDATE_SI               BUILD_CACHE_OP(HIT_INVALIDATE,CACHE_SI)
#define HIT_INVALIDATE_SD               BUILD_CACHE_OP(HIT_INVALIDATE,CACHE_SD)

#define CACHE_FILL_I                    BUILD_CACHE_OP(CACHE_FILL,CACHE_I)
#define HIT_WRITEBACK_INVALIDATE_D      BUILD_CACHE_OP(HIT_WRITEBACK_INVALIDATE,CACHE_D)
#define HIT_WRITEBACK_INVALIDATE_SD     BUILD_CACHE_OP(HIT_WRITEBACK_INVALIDATE,CACHE_SD)

#define HIT_WRITEBACK_I                 BUILD_CACHE_OP(HIT_WRITEBACK,CACHE_I)
#define HIT_WRITEBACK_D                 BUILD_CACHE_OP(HIT_WRITEBACK,CACHE_D)
#define HIT_WRITEBACK_SD                BUILD_CACHE_OP(HIT_WRITEBACK,CACHE_SD)

#define HIT_SET_VIRTUAL_SI		BUILD_CACHE_OP(HIT_SET_VIRTUAL,CACHE_SI)
#define HIT_SET_VIRTUAL_SD              BUILD_CACHE_OP(HIT_SET_VIRTUAL,CACHE_SD)

/*> EOF regs.S <*/
