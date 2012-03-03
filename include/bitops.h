/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1994 - 1997, 99, 2000, 06, 07  Ralf Baechle (ralf@linux-mips.org)
 * Copyright (c) 1999, 2000  Silicon Graphics, Inc.
 */
#ifndef _ASM_BITOPS_H
#define _ASM_BITOPS_H

#define _MIPS_SZLONG 32

#if _MIPS_SZLONG == 32
#define SZLONG_LOG 5
#define SZLONG_MASK 31UL
#define __LL		"ll	"
#define __SC		"sc	"
#define __INS		"ins    "
#define __EXT		"ext    "
#elif _MIPS_SZLONG == 64
#define SZLONG_LOG 6
#define SZLONG_MASK 63UL
#define __LL		"lld	"
#define __SC		"scd	"
#define __INS		"dins    "
#define __EXT		"dext    "
#endif

#ifndef unlikely
#define unlikely(EXP) (EXP)
#endif

/*
 * clear_bit() doesn't provide any barrier for the compiler.
 */
#define smp_mb__before_clear_bit()	smp_mb__before_llsc()
#define smp_mb__after_clear_bit()	smp_llsc_mb()

/*
 * set_bit - Atomically set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * This function is atomic and may not be reordered.  See __set_bit()
 * if you do not require the atomic guarantees.
 * Note that @nr may be almost arbitrarily large; this function is not
 * restricted to acting on a single-word quantity.
 */
static inline void set_bit(unsigned long nr, volatile unsigned long *addr)
{
	unsigned long *m = ((unsigned long *) addr) + (nr >> SZLONG_LOG);
	unsigned short bit = nr & SZLONG_MASK;
	unsigned long temp;

	do {
		__asm__ __volatile__(
			"	.set	mips3				\n"
			"	" __LL "%0, %1		# set_bit	\n"
			"	or	%0, %2				\n"
			"	" __SC	"%0, %1				\n"
			"	.set	mips0				\n"
			: "=&r" (temp), "+m" (*m)
			: "ir" (1UL << bit));
	} while (unlikely(!temp));
}

/*
 * clear_bit - Clears a bit in memory
 * @nr: Bit to clear
 * @addr: Address to start counting from
 *
 * clear_bit() is atomic and may not be reordered.  However, it does
 * not contain a memory barrier, so if it is used for locking purposes,
 * you should call smp_mb__before_clear_bit() and/or smp_mb__after_clear_bit()
 * in order to ensure changes are visible on other processors.
 */
static inline void clear_bit(unsigned long nr, volatile unsigned long *addr)
{
	unsigned long *m = ((unsigned long *) addr) + (nr >> SZLONG_LOG);
	unsigned short bit = nr & SZLONG_MASK;
	unsigned long temp;

	do {
		__asm__ __volatile__(
			"	.set	mips3				\n"
			"	" __LL "%0, %1		# clear_bit	\n"
			"	and	%0, %2				\n"
			"	" __SC "%0, %1				\n"
			"	.set	mips0				\n"
			: "=&r" (temp), "+m" (*m)
			: "ir" (~(1UL << bit)));
	} while (unlikely(!temp));
}

/*
 * change_bit - Toggle a bit in memory
 * @nr: Bit to change
 * @addr: Address to start counting from
 *
 * change_bit() is atomic and may not be reordered.
 * Note that @nr may be almost arbitrarily large; this function is not
 * restricted to acting on a single-word quantity.
 */
static inline void change_bit(unsigned long nr, volatile unsigned long *addr)
{
	unsigned short bit = nr & SZLONG_MASK;
	unsigned long *m = ((unsigned long *) addr) + (nr >> SZLONG_LOG);
	unsigned long temp;

	do {
		__asm__ __volatile__(
			"	.set	mips3				\n"
			"	" __LL "%0, %1		# change_bit	\n"
			"	xor	%0, %2				\n"
			"	" __SC	"%0, %1				\n"
			"	.set	mips0				\n"
			: "=&r" (temp), "+m" (*m)
			: "ir" (1UL << bit));
	} while (unlikely(!temp));
}

static inline int test_bit(int nr, const volatile unsigned long *addr)
{
	return 1UL & (addr[(nr) >> SZLONG_LOG] >> (nr & SZLONG_MASK));
}


/*
 * fls - find last bit set.
 * @word: The word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */
static inline int fls(int x)
{
	int r;

	r = 32;
	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

/*
 * ffs - find first bit set.
 * @word: The word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */
static inline int ffs(int word)
{
	if (!word)
		return 0;

	return fls(word & -word);
}

#endif
