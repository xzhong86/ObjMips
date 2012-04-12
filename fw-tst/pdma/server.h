#ifndef __SERVER_H__
#define __SERVER_H__

#include "instructions.h"

/* Define macros for access memory mapped register */
#define REG32(addr)     (*((unsigned int  volatile *)(addr)))
#define REG16(addr)     (*((unsigned short volatile *)(addr)))
#define REG8(addr)      (*((unsigned char  volatile *)(addr)))

#define printf printk

unsigned int vector_offset[6];
unsigned int vector_exp_handler[32];
unsigned int vector_intc_handler[66];
unsigned int vector_dexp_handler[6];

#define __place_sys_data__  

#define MSC0
#define CGR     (PMC_BASE + 0x20)    // 0xB0000020
#define CGR1    (PMC_BASE + 0x28)    // 0xB0000020
#define MSCR    CGR
#define INTC_MSC   14
#define INTC_DMA   21
#define	ENDIAN		    1
#define IO_BASE	            0xB0000000
#define	GPIO_BASE	   (IO_BASE + 0x00010000)
#define GPIO_PPIN(N)       (GPIO_BASE + 0x00 + (N << 8))

#define GPIO_PPIN(N)       (GPIO_BASE + 0x00 + (N << 8))
#define GPIO_PDAT(N)       (GPIO_BASE + 0x10 + (N << 8))
#define GPIO_PDATS(N)      (GPIO_BASE + 0x14 + (N << 8))
#define GPIO_PDATC(N)      (GPIO_BASE + 0x18 + (N << 8))
#define GPIO_PIM(N)        (GPIO_BASE + 0x20 + (N << 8))
#define GPIO_PIMS(N)       (GPIO_BASE + 0x24 + (N << 8))
#define GPIO_PIMC(N)       (GPIO_BASE + 0x28 + (N << 8))
#define GPIO_PPE(N)        (GPIO_BASE + 0x30 + (N << 8))
#define GPIO_PPES(N)       (GPIO_BASE + 0x34 + (N << 8))
#define GPIO_PPEC(N)       (GPIO_BASE + 0x38 + (N << 8))
#define GPIO_PFUN(N)       (GPIO_BASE + 0x40 + (N << 8))
#define GPIO_PFUNS(N)      (GPIO_BASE + 0x44 + (N << 8))
#define GPIO_PFUNC(N)      (GPIO_BASE + 0x48 + (N << 8))
#define GPIO_PSEL(N)       (GPIO_BASE + 0x50 + (N << 8))
#define GPIO_PSELS(N)      (GPIO_BASE + 0x54 + (N << 8))
#define GPIO_PSELC(N)      (GPIO_BASE + 0x58 + (N << 8))
#define GPIO_PDIR(N)       (GPIO_BASE + 0x60 + (N << 8))
#define GPIO_PDIRS(N)      (GPIO_BASE + 0x64 + (N << 8))
#define GPIO_PDIRC(N)      (GPIO_BASE + 0x68 + (N << 8))
#define GPIO_PTRG(N)       (GPIO_BASE + 0x70 + (N << 8))
#define GPIO_PTRGS(N)      (GPIO_BASE + 0x74 + (N << 8))
#define GPIO_PTRGC(N)      (GPIO_BASE + 0x78 + (N << 8))
#define GPIO_PFLG(N)       (GPIO_BASE + 0x80 + (N << 8))

#define set_pdat(port, pin)   i_s32((0x1 << pin), GPIO_PDATS(port), 0x0)
#define clr_pdat(port, pin)   i_s32((0x1 << pin), GPIO_PDATC(port), 0x0)
#define set_pfun(port, pin)   i_s32((0x1 << pin), GPIO_PFUNS(port), 0x0)
#define clr_pfun(port, pin)   i_s32((0x1 << pin), GPIO_PFUNC(port), 0x0)
#define set_psel(port, pin)   i_s32((0x1 << pin), GPIO_PSELS(port), 0x0)
#define clr_psel(port, pin)   i_s32((0x1 << pin), GPIO_PSELC(port), 0x0)
#define set_pdir(port, pin)   i_s32((0x1 << pin), GPIO_PDIRS(port), 0x0)
#define clr_pdir(port, pin)   i_s32((0x1 << pin), GPIO_PDIRC(port), 0x0)
#define set_ptrg(port, pin)   i_s32((0x1 << pin), GPIO_PTRGS(port), 0x0)
#define clr_ptrg(port, pin)   i_s32((0x1 << pin), GPIO_PTRGC(port), 0x0)
#define set_pim(port, pin)    i_s32((0x1 << pin), GPIO_PIMS(port) , 0x0)
#define clr_pim(port, pin)    i_s32((0x1 << pin), GPIO_PIMC(port) , 0x0)
#define set_ppe(port, pin)    i_s32((0x1 << pin), GPIO_PPES(port) , 0x0)
#define clr_ppe(port, pin)    i_s32((0x1 << pin), GPIO_PPEC(port) , 0x0)

#define get_ppin(port, pin)   (i_l32(GPIO_PPIN(port), 0x0) & (0x1 << pin)) >> pin
#define get_pflg(port, pin)   (i_l32(GPIO_PFLG(port), 0x0) & (0x1 << pin)) >> pin
#define get_pdat(port, pin)   (i_l32(GPIO_PDAT(port), 0x0) & (0x1 << pin)) >> pin
#define get_pfun(port, pin)   (i_l32(GPIO_PFUN(port), 0x0) & (0x1 << pin)) >> pin
#define get_psel(port, pin)   (i_l32(GPIO_PSEL(port), 0x0) & (0x1 << pin)) >> pin
#define get_pdir(port, pin)   (i_l32(GPIO_PDIR(port), 0x0) & (0x1 << pin)) >> pin
#define get_ptrg(port, pin)   (i_l32(GPIO_PTRG(port), 0x0) & (0x1 << pin)) >> pin
#define get_pim(port, pin)    (i_l32(GPIO_PIM(port) , 0x0) & (0x1 << pin)) >> pin
#define get_ppe(port, pin)    (i_l32(GPIO_PPE(port) , 0x0) & (0x1 << pin)) >> pin

#define NEMC_BASE	(IO_BASE + 0x03410000)
#define	BCH_BASE	(IO_BASE + 0x034D0000)

//#define BCH_BASE    0x134d0000
#define BHCR      (BCH_BASE + 0x00)  
#define BHCSR     (BCH_BASE + 0x04) 
#define BHCCR     (BCH_BASE + 0x08)
#define BHCNT     (BCH_BASE + 0x0c)
#define BHDR      (BCH_BASE + 0x10)
#define BHPAR0    (BCH_BASE + 0x14)
#define BHPAR1    (BCH_BASE + 0x18)
#define BHPAR2    (BCH_BASE + 0x1c)
#define BHPAR3    (BCH_BASE + 0x20)
#define BHPAR4    (BCH_BASE + 0x24)
#define BHPAR5    (BCH_BASE + 0x28)
#define BHPAR6    (BCH_BASE + 0x2c)
#define BHPAR7    (BCH_BASE + 0x30)
#define BHPAR8    (BCH_BASE + 0x34)
#define BHPAR9    (BCH_BASE + 0x38)
#define BHERR0    (BCH_BASE + 0x3c) 
#define BHERR1    (BCH_BASE + 0x40)
#define BHERR2    (BCH_BASE + 0x44)  
#define BHERR3    (BCH_BASE + 0x48) 
#define BHERR4    (BCH_BASE + 0x4c) 
#define BHERR5    (BCH_BASE + 0x50) 
#define BHERR6    (BCH_BASE + 0x54) 
#define BHERR7    (BCH_BASE + 0x58) 
#define BHERR8    (BCH_BASE + 0x5c) 
#define BHERR9    (BCH_BASE + 0x60) 
#define BHERR10   (BCH_BASE + 0x64) 
#define BHERR11   (BCH_BASE + 0x68) 
#define BHINT     (BCH_BASE + 0x6c)
#define BHINTE    (BCH_BASE + 0x70)
#define BHINTES   (BCH_BASE + 0x74)
#define BHINTEC   (BCH_BASE + 0x78)  
#define BCHISR    (BCH_BASE + 0x114)  

//MASK of reserve bits
#define BHCR_MASK           0xffffffe0
#define BHCSR_MASK          0xffffffe0
#define BHCCR_MASK          0xffffffe0
#define BHCNT_MASK          0xfc00fc00
#define BHDR_MASK           0xffffff00
#define BHPAR3_MASK         0xff000000
#define BHPAR6_MASK         0xffff0000
#define BHINT_MASK          0x0fffff80
#define BHINTE_MASK         0xffffff80
#define BHINTES_MASK        0xffffff80
#define BHINTEC_MASK        0xffffff80
#define BHERR0_MASK         0xe000e000

#define BHCR_SET_F          0x000000bd
#define BHCNT_SET_F         0x07ff07ff
#define BHINTE_SET_F        0x0000005f

// bcr control bits
#define BDMA_ENB            0x00000080
#define BDMA_DIS            0x00000080

#define ENC_ENB             0x00000004
#define ENC_DIS             0x00000004

#define DEC_ENB             0x00000000
#define DEC_DIS             0x00000000

#define BCH_24BIT           0x00000028
#define BCH_20BIT           0x00000020
#define BCH_16BIT           0x00000018
#define BCH_12BIT           0x00000010
#define BCH_8BIT            0x00000008
#define BCH_4BIT            0x00000000

#define BRST_ENB            0x00000002

#define BCH_ENB             0x00000001
#define BCH_DIS             0x00000001

// bhcnt control bits
#define ENC_CNT             0x000
#define DEC_CNT             0x000
#define ENC_CNT_INIT        0x200
#define DEC_CNT_INIT        0x020d0000

//INT status bits
#define DMA_ERR              0x00000040 
#define ALL_ZERO             0x00000020
#define ALL_F                0x00000010
#define DEC_FINISH           0x00000008 
#define ENC_FINISH           0x00000004 
#define UNCOR_ERR            0x00000002
#define BCH_ERR              0x00000001

#define ENCFES               0x00000004




#define show_string(x) report_msg((x))
#define report_msg(src)                           \
({                                                \
  register unsigned int r0 asm ("$0");            \
  register unsigned int t_tmp = *(unsigned int *)&src; \
  r0 = t_tmp;                                     \
  r0 = i_ori(r0, 0);                              \
  r0 = i_ori(r0, 0);                              \
  r0 = i_ori(r0, 0);                              \
  r0 = i_ori(r0, 0);                              \
  r0 = i_ori(r0, 0);                              \
  r0 = i_ori(r0, 0);                              \
  r0 = i_ori(r0, PLI_PROBE_MSG);                  \
  r0 = i_andi(t_tmp, __LINE__);                   \
})


#define ITB_BASE	0xB00F0000
#define ITB_EMC_BASE	(ITB_BASE + 0x0600)
#define ITB_EMC_SMCFG	(ITB_EMC_BASE + 0x1c)
#define ITB_EMC_SMCR1	(ITB_EMC_BASE + 0x04)

#define WAIT_BANK2      4
#define NAND            5
#define SMCR_BW8          0


#define NEMI_BASE	(IO_BASE + 0x03410000)
#define SMCR1	(NEMI_BASE + 0x14)

struct REG_SRAM_CFG
{
#if ENDIAN == 0
  unsigned int wait_en    : 1;  // 31
  unsigned int            : 3;  // 30-28
  unsigned int wait_bank  : 4;  // 27-24   one hot:b3/b2/b1/b0 
  unsigned int wait_cnt   : 8;  // 23-16
  unsigned int bank4      : 4;  // 15-12
  unsigned int bank3      : 4;  // 11-8
  unsigned int bank2      : 4;  // 7-4
  unsigned int bank1      : 4;  // 3-0
#else
  unsigned int bank1      : 4;  // 3-0
  unsigned int bank2      : 4;  // 7-4
  unsigned int bank3      : 4;  // 11-8
  unsigned int bank4      : 4;  // 15-12
  unsigned int wait_cnt   : 8;  // 23-16
  unsigned int wait_bank  : 4;  // 27-24   one hot:b3/b2/b1/b0 
  unsigned int            : 3;  // 30-28
  unsigned int wait_en    : 1;  // 31
#endif
};


struct REG_SMCR
{
#if ENDIAN == 0
  unsigned int        : 4;   // 31-28
  unsigned int strv   : 4;   // 27-24
  unsigned int taw    : 4;   // 23-20
  unsigned int tbp    : 4;   // 19-16
  unsigned int        : 1;   // 15
  unsigned int tah    : 3;   // 14-12
  unsigned int        : 1;   // 11
  unsigned int tas    : 3;   // 10-8
  unsigned int bw     : 2;   // 7-6
  unsigned int        : 2;   // 5-4
  unsigned int bcm    : 1;   // 3
  unsigned int bl     : 2;   // 2-1
  unsigned int smt    : 1;   // 0
#else
  unsigned int smt    : 1;   // 0
  unsigned int bl     : 2;   // 2-1
  unsigned int bcm    : 1;   // 3
  unsigned int        : 2;   // 5-4
  unsigned int bw     : 2;   // 7-6
  unsigned int tas    : 3;   // 10-8
  unsigned int        : 1;   // 11
  unsigned int tah    : 3;   // 14-12
  unsigned int        : 1;   // 15
  unsigned int tbp    : 4;   // 19-16
  unsigned int taw    : 4;   // 23-20
  unsigned int strv   : 4;   // 27-24
  unsigned int        : 4;   // 31-28
#endif
};

#define EMI_nf {33,0,0,0}
#define EMI_ALT_F0 { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,\
                    11,12,13,14,15,16,17,18,19,21,22,\
                    23,24,25,26,27,32,33,34,35,36,37}
#define EMI_ALT_F1 {}
#define EMI_ALT_F2 {}
#define EMI_ALT_F3 {}

#define alter4emi()     alter4mod(EMI)

#define alter4mod(MODULE)                                      \
({                                                             \
   unsigned int nf[]=MODULE##_nf;                              \
   unsigned int F0[]=MODULE##_ALT_F0;                          \
   unsigned int F1[]=MODULE##_ALT_F1;                          \
   unsigned int F2[]=MODULE##_ALT_F2;                          \
   unsigned int F3[]=MODULE##_ALT_F3;                          \
   int j;                                                      \
   for(j=0;j<nf[0];j++)                                        \
	   set_alter(F0[j],0);                                 \
   for(j=0;j<nf[1];j++)                                        \
	   set_alter(F1[j],1);                                 \
   for(j=0;j<nf[2];j++)                                        \
           set_alter(F2[j],2);                                 \
   for(j=0;j<nf[3];j++)                                        \
           set_alter(F3[j],3);                                 \
})


#define itb_set_smcfg(id)			                                      \
({					                                              \
  struct REG_SRAM_CFG t;		                                              \
  *((unsigned long *)((unsigned long)&t)) = REG32(ITB_EMC_SMCFG);		      \
  t.wait_en                               = 1;			                      \
  t.wait_bank                             = WAIT_BANK2;		                      \
  t.wait_cnt                              = 0x30;			              \
  t.bank##id                              = NAND;			              \
  REG32(ITB_EMC_SMCFG)                    = *((unsigned long *)((unsigned long)&t));  \
})


#define init_nf(id)                                          \
({	 						           \
  struct REG_SMCR t;					           \
  *((unsigned long *)((unsigned long)&t)) = REG32(ITB_EMC_SMCR##id);  \
  t.bw    = SMCR_BW8; /*8-bit*/  				   \
  t.strv  = 0xf;					           \
  t.taw   = 0xf;					           \
  t.tbp   = 0xf;					           \
  t.tah   = 0x7;					           \
  t.tas   = 0x7;					           \
  REG32(ITB_EMC_SMCR##id) = *((unsigned long *)((unsigned long)&t));  \
  REG32(SMCR##id) = *((unsigned long *)((unsigned long)&t));          \
  i_nop; i_nop;			                                   \
  alter4emi();						           \
})

#define set_alter(N,value)\
({                        \
  unsigned port,pin;      \
  port = N / 32;          \
  pin  = N % 32;          \
  set_pfun(port, pin);    \
  if(value == 3) {        \
    set_ptrg(port,pin);   \
    set_psel(port, pin);  \
    }                     \
  else if(value == 2) {   \
    set_ptrg(port,pin);   \
    clr_psel(port, pin);  \
    }                     \
  else if(value == 1) {   \
    clr_ptrg(port,pin);   \
    set_psel(port, pin);  \
    }                     \
  else {                  \
    clr_ptrg(port,pin);   \
    clr_psel(port, pin);  \
    }                     \
})



#define show_long(val)                                    \
({                                                        \
    printk("%08x\n",(unsigned int)val);                   \
})	

#define printd(val)                                       \
({                                                        \
    printk("%d\n",(unsigned int)val);                     \
})			

#define printx(val)                                       \
({                                                        \
    printk("%x\n",(unsigned int)val);                     \
})						

		

#define va_2_pa(va)                                       \
({                                                        \
    unsigned long pa;                                     \
    pa = ((va) & 0x0fffffff);                             \
    pa;                                                   \
})

#endif
