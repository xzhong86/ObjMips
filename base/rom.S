#include <sys/regdef.h>
	.section .text_rom
	.align	2

	.globl	_start
	.globl	rom_reset
rom_reset:
	la	v0, _start
	jr	v0
