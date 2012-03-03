
#ifndef JZ4770_GPIO_H
#define JZ4770_GPIO_H

#define PORTA_BASE 0xB0010000
#define PORTB_BASE 0xB0010100
#define PORTC_BASE 0xB0010200
#define PORTD_BASE 0xB0010300
#define PORTE_BASE 0xB0010400
#define PORTF_BASE 0xB0010500

#define _OFF_PIN	0x0
#define _OFF_INT	0x10
#define _OFF_INTS	0x14
#define _OFF_INTC	0x18
#define _OFF_MSK	0x20
#define _OFF_MSKS	0x24
#define _OFF_MSKC	0x28
#define _OFF_PAT1	0x30
#define _OFF_PAT1S	0x34
#define _OFF_PAT1C	0x38
#define _OFF_PAT0	0x40
#define _OFF_PAT0S	0x44
#define _OFF_PAT0C	0x48

#define PGPIOREG(class,reg) ((volatile unsigned int*)(PORT##class##_BASE + _OFF_##reg))
#define GPIOREG(class,reg) (*(volatile unsigned int*)(PORT##class##_BASE + _OFF_##reg))

#define GPIO_FUN_DEVICE(class,bit,n)				\
	do {							\
		GPIOREG(class,INTC) = 1<<(bit);			\
		GPIOREG(class,MSKC) = 1<<(bit);			\
		if(n&1)	GPIOREG(class,PAT0S) = 1<<(bit);	\
		else GPIOREG(class,PAT0C) = 1<<(bit);		\
		if(n&2)	GPIOREG(class,PAT1S) = 1<<(bit);	\
		else GPIOREG(class,PAT1C) = 1<<(bit);		\
	} while(0)

#define GPIO_FUN_DEF(class,bit,n)				\
	do {							\
		GPIOREG(class,INTC) = 1<<(bit);			\
		GPIOREG(class,MSKS) = 1<<(bit);			\
		GPIOREG(class,PAT1S) = 1<<(bit);		\
		GPIOREG(class,PAT0C) = 1<<(bit);		\
	} while(0)


#endif
