/* $Id: gpio.h,v 1.6 2012/04/03 06:50:01 pjwang Exp $ */
#ifndef __INGENIC_GPIO_H__
#define __INGENIC_GPIO_H__

/* Define all gpio mapped registers */
/* GPIO control reg, 32 bits, initial value  is 0x0 */

/***************************************/
/***** BEGIN for JZ4740 10/13/2006 *****/
/***************************************/
//#define GPIO_PORTA      0x000
//#define GPIO_PORTB      0x100
//#define GPIO_PORTC      0x200
//#define GPIO_PORTD      0x300

#define GPIO_PORTA     0
#define GPIO_PORTB     1
#define GPIO_PORTC     2
#define GPIO_PORTD     3
#define GPIO_PORTE     4
#define GPIO_PORTF     5

#define GPIO_PA     0
#define GPIO_PB     1
#define GPIO_PC     2
#define GPIO_PD     3
#define GPIO_PE     4
#define GPIO_PF     5

#define GPIO_PIN_0     (0x1 << 0)
#define GPIO_PIN_1     (0x1 << 1)
#define GPIO_PIN_2     (0x1 << 2)
#define GPIO_PIN_3     (0x1 << 3)
#define GPIO_PIN_4     (0x1 << 4)
#define GPIO_PIN_5     (0x1 << 5)
#define GPIO_PIN_6     (0x1 << 6)
#define GPIO_PIN_7     (0x1 << 7)
#define GPIO_PIN_8     (0x1 << 8)
#define GPIO_PIN_9     (0x1 << 9)
#define GPIO_PIN_00    (0x1 << 0)
#define GPIO_PIN_01    (0x1 << 1)
#define GPIO_PIN_02    (0x1 << 2)
#define GPIO_PIN_03    (0x1 << 3)
#define GPIO_PIN_04    (0x1 << 4)
#define GPIO_PIN_05    (0x1 << 5)
#define GPIO_PIN_06    (0x1 << 6)
#define GPIO_PIN_07    (0x1 << 7)
#define GPIO_PIN_08    (0x1 << 8)
#define GPIO_PIN_09    (0x1 << 9)
#define GPIO_PIN_10    (0x1 << 10)
#define GPIO_PIN_11    (0x1 << 11)
#define GPIO_PIN_12    (0x1 << 12)
#define GPIO_PIN_13    (0x1 << 13)
#define GPIO_PIN_14    (0x1 << 14)
#define GPIO_PIN_15    (0x1 << 15)
#define GPIO_PIN_16    (0x1 << 16)
#define GPIO_PIN_17    (0x1 << 17)
#define GPIO_PIN_18    (0x1 << 18)
#define GPIO_PIN_19    (0x1 << 19)
#define GPIO_PIN_20    (0x1 << 20)
#define GPIO_PIN_21    (0x1 << 21)
#define GPIO_PIN_22    (0x1 << 22)
#define GPIO_PIN_23    (0x1 << 23)
#define GPIO_PIN_24    (0x1 << 24)
#define GPIO_PIN_25    (0x1 << 25)
#define GPIO_PIN_26    (0x1 << 26)
#define GPIO_PIN_27    (0x1 << 27)
#define GPIO_PIN_28    (0x1 << 28)
#define GPIO_PIN_29    (0x1 << 29)
#define GPIO_PIN_30    (0x1 << 30)
#define GPIO_PIN_31    (0x1 << 31)

// jz4780
#define GPIO_PPIN(N)               (GPIO_BASE + 0x00 + (N << 8))

#define GPIO_PINT(N)               (GPIO_BASE + 0x10 + (N << 8))
#define GPIO_PINTS(N)              (GPIO_BASE + 0x14 + (N << 8))
#define GPIO_PINTC(N)              (GPIO_BASE + 0x18 + (N << 8))

#define GPIO_PMSK(N)               (GPIO_BASE + 0x20 + (N << 8))
#define GPIO_PMSKS(N)              (GPIO_BASE + 0x24 + (N << 8))
#define GPIO_PMSKC(N)              (GPIO_BASE + 0x28 + (N << 8))

#define GPIO_PPAT1(N)              (GPIO_BASE + 0x30 + (N << 8))
#define GPIO_PPAT1S(N)             (GPIO_BASE + 0x34 + (N << 8))
#define GPIO_PPAT1C(N)             (GPIO_BASE + 0x38 + (N << 8))

#define GPIO_PPAT0(N)              (GPIO_BASE + 0x40 + (N << 8))
#define GPIO_PPAT0S(N)             (GPIO_BASE + 0x44 + (N << 8))
#define GPIO_PPAT0C(N)             (GPIO_BASE + 0x48 + (N << 8))

#define GPIO_PFLG(N)               (GPIO_BASE + 0x50 + (N << 8))
#define GPIO_PFLGC(N)              (GPIO_BASE + 0x58 + (N << 8))

#define GPIO_PPEN(N)               (GPIO_BASE + 0x70 + (N << 8))
#define GPIO_PPENS(N)              (GPIO_BASE + 0x74 + (N << 8))
#define GPIO_PPENC(N)              (GPIO_BASE + 0x78 + (N << 8))

#define GPIO_PDRVL(N)              (GPIO_BASE + 0x80 + (N << 8))  
#define GPIO_PDRVLS(N)             (GPIO_BASE + 0x84 + (N << 8)) 
#define GPIO_PDRVLC(N)             (GPIO_BASE + 0x88 + (N << 8))  
		
#define GPIO_PPDIR(N)	           (GPIO_BASE + 0x90 + (N << 8)) 
#define GPIO_PPDIRS(N)	           (GPIO_BASE + 0x94 + (N << 8)) 
#define GPIO_PPDIRC(N)	           (GPIO_BASE + 0x98 + (N << 8)) 
	
#define GPIO_PDRVH(N)              (GPIO_BASE + 0xA0 + (N << 8))
#define GPIO_PDRVHS(N)             (GPIO_BASE + 0xA4 + (N << 8)) 
#define GPIO_PDRVHC(N)             (GPIO_BASE + 0xA8 + (N << 8))  
		
#define GPIO_PDS(N)                (GPIO_BASE + 0x70 + (N << 8))
#define GPIO_PDSS(N)               (GPIO_BASE + 0x74 + (N << 8))
#define GPIO_PDSC(N)               (GPIO_BASE + 0x78 + (N << 8))

#define set_preg(reg, port, pin)   i_s32((0x1 << pin), GPIO_P##reg##S(port), 0x0)
#define clr_preg(reg, port, pin)   i_s32((0x1 << pin), GPIO_P##reg##C(port), 0x0)
#define get_preg(reg, port, pin)   (i_l32(GPIO_P##reg(port), 0x0) & (0x1 << pin)) >> pin

#define set_preg2(reg, port, pins) i_s32(pins, GPIO_P##reg##S(port), 0x0)
#define clr_preg2(reg, port, pins) i_s32(pins, GPIO_P##reg##C(port), 0x0)

#define set_pint(port, pin)        set_preg(INT, port, pin)
#define set_pmsk(port, pin)        set_preg(MSK, port, pin)
#define set_ppat1(port, pin)       set_preg(PAT1, port, pin)
#define set_ppat0(port, pin)       set_preg(PAT0, port, pin)

#define set_pint2(port, pins)      set_preg2(INT, port, pins)
#define set_pmsk2(port, pins)      set_preg2(MSK, port, pins)
#define set_ppat12(port, pins)     set_preg2(PAT1, port, pins)
#define set_ppat02(port, pins)     set_preg2(PAT0, port, pins)

#define set_ppen(port, pin)        set_preg(PEN, port, pin)
#define set_pdrvl(port,pin)        set_preg(DRVL, port, pin)
#define set_ppdir(port,pin)        set_preg(PDIR, port, pin)
#define set_pdrvh(port,pin)        set_preg(DRVH, port,pin)

#define set_pds(port, pin)         set_preg(DS, port, pin)


#define clr_pint(port, pin)        clr_preg(INT, port, pin)
#define clr_pmsk(port, pin)        clr_preg(MSK, port, pin)
#define clr_ppat1(port, pin)       clr_preg(PAT1, port, pin)
#define clr_ppat0(port, pin)       clr_preg(PAT0, port, pin)

#define clr_pint2(port, pin)       clr_preg2(INT, port, pin)
#define clr_pmsk2(port, pin)       clr_preg2(MSK, port, pin)
#define clr_ppat12(port, pin)      clr_preg2(PAT1, port, pin)
#define clr_ppat02(port, pin)      clr_preg2(PAT0, port, pin)

#define clr_pflg_jz4780(port, pin)        clr_preg(FLG, port, pin)
#define clr_ppen(port, pin)        clr_preg(PEN, port, pin)
#define clr_pdrvl(port,pin)        clr_preg(DRVL, port, pin)
#define clr_ppdir(port,pin)        clr_preg(PDIR, port, pin)
#define clr_pdrvh(port,pin)        clr_preg(DRVH, port,pin)

#define clr_pds(port, pin)         clr_preg(DS, port, pin)

#define get_pint(port, pin)        get_preg(INT, port, pin)
#define get_pmsk(port, pin)        get_preg(MSK, port, pin)
#define get_ppat1(port, pin)       get_preg(PAT1, port, pin)
#define get_ppat0(port, pin)       get_preg(PAT0, port, pin)
#define get_pflg_jz4780(port, pin)        get_preg(FLG, port, pin)
#define get_ppen(port, pin)        get_preg(PEN, port, pin)
#define get_pdrvl(port,pin)        get_preg(DRVL, port, pin)
#define get_ppdir(port,pin)        get_preg(PDIR, port, pin)
#define get_pdrvh(port,pin)        get_preg(DRVH, port,pin)

#define get_pds(port, pin)         get_preg(DS, port, pin)

#define set_pin_gpio_i(port, pin)  \
({                                 \
    clr_pint(port, pin);           \
    set_pmsk(port, pin);           \
    set_ppat1(port, pin);          \
})

#define set_pin_gpio_o(port, pin)  \
({                                 \
    clr_pint(port, pin);           \
    set_pmsk(port, pin);           \
    clr_ppat1(port, pin);          \
})

#define set_pin_dev0(port, pin)    \
({                                 \
    clr_pint(port, pin);           \
    clr_pmsk(port, pin);           \
    clr_ppat1(port, pin);          \
    clr_ppat0(port, pin);          \
})

#define set_pin_dev1(port, pin)    \
({                                 \
    clr_pint(port, pin);           \
    clr_pmsk(port, pin);           \
    clr_ppat1(port, pin);          \
    set_ppat0(port, pin);          \
})

#define set_pin_dev2(port, pin)    \
({                                 \
    clr_pmsk(port, pin);           \
    clr_pint(port, pin);           \
    set_ppat1(port, pin);          \
    clr_ppat0(port, pin);          \
})

#define set_pin_dev3(port, pin)    \
({                                 \
    clr_pint(port, pin);           \
    clr_pmsk(port, pin);           \
    set_ppat1(port, pin);          \
    set_ppat0(port, pin);          \
})

#define set_pin_dev0_2(port, pin)    \
({                                 \
    clr_pint2(port, pin);           \
    clr_pmsk2(port, pin);           \
    clr_ppat12(port, pin);          \
    clr_ppat02(port, pin);          \
})

#define set_pin_dev1_2(port, pin)    \
({                                 \
    clr_pint2(port, pin);           \
    clr_pmsk2(port, pin);           \
    clr_ppat12(port, pin);          \
    set_ppat02(port, pin);          \
})

#define set_pin_dev2_2(port, pin)    \
({                                 \
    clr_pmsk2(port, pin);           \
    clr_pint2(port, pin);           \
    set_ppat12(port, pin);          \
    clr_ppat02(port, pin);          \
})

#define set_pin_dev3_2(port, pin)    \
({                                 \
    clr_pint2(port, pin);           \
    clr_pmsk2(port, pin);           \
    set_ppat12(port, pin);          \
    set_ppat02(port, pin);          \
})

// begin jz4760

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

// end jz4760

/*************************************/
/***** END for JZ4740 10/13/2006 *****/
/*************************************/

/*for jz5730 02/26/2006*/
#define GPSLR0  (GPIO_BASE + 0x10)
#define GPSUR0  (GPIO_BASE + 0x14)
#define GPFLR0  (GPIO_BASE + 0x18)
#define GPFUR0  (GPIO_BASE + 0x1C)
#define GPSLR1  (GPIO_BASE + 0x40)
#define GPSUR1  (GPIO_BASE + 0x44)
#define GPFLR1  (GPIO_BASE + 0x48)
#define GPFUR1  (GPIO_BASE + 0x4c)
#define GPSLR2  (GPIO_BASE + 0x70)
#define GPFLR2  (GPIO_BASE + 0x78)
/*-----------*/
#define GPDR0   (GPIO_BASE + 0x00)
#define GPDIR0  (GPIO_BASE + 0x04)
#define GPODR0  (GPIO_BASE + 0x08)
#define GPPUR0  (GPIO_BASE + 0x0C)
#define GPALR0  (GPIO_BASE + 0x10)
#define GPAUR0  (GPIO_BASE + 0x14)
#define GPIDLR0 (GPIO_BASE + 0x18)
#define GPIDUR0 (GPIO_BASE + 0x1c)
#define GPIER0  (GPIO_BASE + 0x20)
#define GPIMR0  (GPIO_BASE + 0x24)
#define GPFR0   (GPIO_BASE + 0x28)

#define GPDR1   (GPIO_BASE + 0x30)
#define GPDIR1  (GPIO_BASE + 0x34)
#define GPODR1  (GPIO_BASE + 0x38)
#define GPPUR1  (GPIO_BASE + 0x3C)
#define GPALR1  (GPIO_BASE + 0x40)
#define GPAUR1  (GPIO_BASE + 0x44)
#define GPIDLR1 (GPIO_BASE + 0x48)
#define GPIDUR1 (GPIO_BASE + 0x4c)
#define GPIER1  (GPIO_BASE + 0x50)
#define GPIMR1  (GPIO_BASE + 0x54)
#define GPFR1   (GPIO_BASE + 0x58)

#define GPDR2   (GPIO_BASE + 0x60)
#define GPDIR2  (GPIO_BASE + 0x64)
#define GPODR2  (GPIO_BASE + 0x68)
#define GPPUR2  (GPIO_BASE + 0x6C)
#define GPALR2  (GPIO_BASE + 0x70)
#define GPAUR2  (GPIO_BASE + 0x74)
#define GPIDLR2 (GPIO_BASE + 0x78)
#define GPIDUR2 (GPIO_BASE + 0x7c)
#define GPIER2  (GPIO_BASE + 0x80)
#define GPIMR2  (GPIO_BASE + 0x84)
#define GPFR2   (GPIO_BASE + 0x88)

#define GPDR3   (GPIO_BASE + 0x90)
#define GPDIR3  (GPIO_BASE + 0x94)
#define GPODR3  (GPIO_BASE + 0x98)
#define GPPUR3  (GPIO_BASE + 0x9C)
#define GPALR3  (GPIO_BASE + 0xa0)
#define GPAUR3  (GPIO_BASE + 0xa4)
#define GPIDLR3 (GPIO_BASE + 0xa8)
#define GPIDUR3 (GPIO_BASE + 0xac)
#define GPIER3  (GPIO_BASE + 0xb0)
#define GPIMR3  (GPIO_BASE + 0xb4)
#define GPFR3   (GPIO_BASE + 0xb8)

#define GPBCR   GPDIR3
#define GPBDR   GPDR3

#define GPDR4   (GPIO_BASE + 0xc0)
#define GPDIR4  (GPIO_BASE + 0xc4)
#define GPODR4  (GPIO_BASE + 0xc8)
#define GPPUR4  (GPIO_BASE + 0xcc)
#define GPALR4  (GPIO_BASE + 0xd0)
#define GPAUR4  (GPIO_BASE + 0xd4)
#define GPIDLR4 (GPIO_BASE + 0xd8)
#define GPIDUR4 (GPIO_BASE + 0xdc)
#define GPIER4  (GPIO_BASE + 0xe0)
#define GPIMR4  (GPIO_BASE + 0xe4)
#define GPFR4   (GPIO_BASE + 0xe8)

#define H_LEVEL 0x3
#define L_LEVEL 0x0
#define X_LEVEL 0x1
#define Z_LEVEL 0x2

#define GPIO_IN          0x0
#define GPIO_OUT         0x1

#define GPIO_WU_DIS      0x0
#define GPIO_WU_EN       0x1

#define GPIO_PU_OFF      0x0
#define GPIO_PU_ON       0x1

#define GPIO_OD_DIS      0x0
#define GPIO_OD_EN       0x1

#define GPIO_A_0         0x0
#define GPIO_A_1         0x1
#define GPIO_A_2         0x2
#define GPIO_A_3         0x3

#define IRQ_LOW          0x0
#define IRQ_HIGH         0x1
#define IRQ_FALL         0x2
#define IRQ_RISE         0x3

#define ALT_GPIO         0x0
#define ALT_INT          ALT_GPIO
#define ALT_F0           0x1
#define ALT_F1           0x2
#define ALT_F2           0x3

#define EXT_GPIO_P         0
#define EXT_GPIO_M         1
#define EXT_GPIO_PCI       2

#define EXT_GPCR0      (GPIO_DRV_BASE + 0xc00)
#define EXT_GPMD0      (GPIO_DRV_BASE + 0xc04)
#define EXT_GPDR0      (GPIO_DRV_BASE + 0xc08)
#define EXT_GPPR0_HIGH (GPIO_DRV_BASE + 0xc0c)
#define EXT_GPPR0_LOW  (GPIO_DRV_BASE + 0xc10)

#define EXT_GPCR1      (GPIO_DRV_BASE + 0xc14)
#define EXT_GPMD1      (GPIO_DRV_BASE + 0xc18)
#define EXT_GPDR1      (GPIO_DRV_BASE + 0xc1c)
#define EXT_GPPR1_HIGH (GPIO_DRV_BASE + 0xc20)
#define EXT_GPPR1_LOW  (GPIO_DRV_BASE + 0xc24)

#define EXT_GPCR2      (GPIO_DRV_BASE + 0xc28)
#define EXT_GPMD2      (GPIO_DRV_BASE + 0xc2c)
#define EXT_GPDR2      (GPIO_DRV_BASE + 0xc30)
#define EXT_GPPR2_HIGH (GPIO_DRV_BASE + 0xc34)
#define EXT_GPPR2_LOW  (GPIO_DRV_BASE + 0xc38)

#define EXT_GPCR3      (GPIO_DRV_BASE + 0xc3c)
#define EXT_GPMD3      (GPIO_DRV_BASE + 0xc40)
#define EXT_GPDR3      (GPIO_DRV_BASE + 0xc44)
#define EXT_GPPR3_HIGH (GPIO_DRV_BASE + 0xc48)
#define EXT_GPPR3_LOW  (GPIO_DRV_BASE + 0xc4c)

#define EXT_START_MD   (GPIO_DRV_BASE + 0xc50)
#define EXT_EMI_HIGH   (GPIO_DRV_BASE + 0xc54)
#define EXT_EMI_LOW    (GPIO_DRV_BASE + 0xc58)

#define EXT_GPCR4      (GPIO_DRV_BASE + 0xc5c)
#define EXT_GPMD4      (GPIO_DRV_BASE + 0xc60)
#define EXT_GPDR4      (GPIO_DRV_BASE + 0xc64)
#define EXT_GPPR4_HIGH (GPIO_DRV_BASE + 0xc68)
#define EXT_GPPR4_LOW  (GPIO_DRV_BASE + 0xc6c)

#define EXT_RSTCR    (GPIO_DRV_BASE + 0xca0)
#define EXT_RSTMD    (GPIO_DRV_BASE + 0xca4)
#define EXT_RSTDA    (GPIO_DRV_BASE + 0xca8)
#define EXT_RSTPIN   (GPIO_DRV_BASE + 0xcac)

#define EXT_WAIT_P0(N)     ((GPIO_DRV_BASE + 0x000) + (N << 4))
#define EXT_ASSERT_P0(N)   ((GPIO_DRV_BASE + 0x004) + (N << 4))
#define EXT_TIMES_P0(N)    ((GPIO_DRV_BASE + 0x008) + (N << 4))
#define EXT_LP_STAGE(port, pin) ((GPIO_DRV_BASE + 0x200 * port + 0x00c) + (pin << 4))

#define EXT_WAIT_P1(N)     ((GPIO_DRV_BASE + 0x200) + (N << 4))
#define EXT_ASSERT_P1(N)   ((GPIO_DRV_BASE + 0x204) + (N << 4))
#define EXT_TIMES_P1(N)    ((GPIO_DRV_BASE + 0x208) + (N << 4))

#define EXT_WAIT_P2(N)     ((GPIO_DRV_BASE + 0x400) + (N << 4))
#define EXT_ASSERT_P2(N)   ((GPIO_DRV_BASE + 0x404) + (N << 4))
#define EXT_TIMES_P2(N)    ((GPIO_DRV_BASE + 0x408) + (N << 4))

#define EXT_WAIT_P3(N)     ((GPIO_DRV_BASE + 0x600) + (N << 4))
#define EXT_ASSERT_P3(N)   ((GPIO_DRV_BASE + 0x604) + (N << 4))
#define EXT_TIMES_P3(N)    ((GPIO_DRV_BASE + 0x608) + (N << 4))

#define EXT_WAIT_P4(N)     ((GPIO_DRV_BASE + 0x800) + (N << 4))
#define EXT_ASSERT_P4(N)   ((GPIO_DRV_BASE + 0x804) + (N << 4))
#define EXT_TIMES_P4(N)    ((GPIO_DRV_BASE + 0x808) + (N << 4))

#define EXT_WAIT_PRESETP      (GPIO_DRV_BASE + 0xf00)     //addr[7:0]=0x10
#define EXT_ASSERT_PRESETP    (GPIO_DRV_BASE + 0xf04)     //addr[7:0]=0x14
#define EXT_TIMES_PRESETP     (GPIO_DRV_BASE + 0xf08)     //addr[7:0]=0x18

#define EXT_WAIT_PRESETM      (GPIO_DRV_BASE + 0xf10)     //addr[7:0]=0x20
#define EXT_ASSERT_PRESETM    (GPIO_DRV_BASE + 0xf14)     //addr[7:0]=0x24
#define EXT_TIMES_PRESETM     (GPIO_DRV_BASE + 0xf18)     //addr[7:0]=0x28

#define EXT_WAIT_PRESETPCI    (GPIO_DRV_BASE + 0xf20)     //addr[7:0]=0x30
#define EXT_ASSERT_PRESETPCI  (GPIO_DRV_BASE + 0xf24)     //addr[7:0]=0x34
#define EXT_TIMES_PRESETPCI   (GPIO_DRV_BASE + 0xf28)     //addr[7:0]=0x38


//================================= config gpio port in/out function
// set GPCR : 0 -> input pin; 1 -> output pin;
#define set_gpdr(port, val)    i_s32(val, GPDR##port, 0x0)
#define set_gpodr(port, val)   i_s32(val, GPODR##port, 0x0)
#define set_gppur(port, val)   i_s32(val, GPPUR##port, 0x0)
#define set_gpdir(port, val)   i_s32(val, GPDIR##port, 0x0)
#define set_gpidlr(port, val)  i_s32(val, GPIDLR##port, 0x0)
#define set_gpidur(port, val)  i_s32(val, GPIDUR##port, 0x0)
#define set_gpalr(port, val)   i_s32(val, GPALR##port, 0x0)
#define set_gpaur(port, val)   i_s32(val, GPAUR##port, 0x0)

#ifdef JZ5730
#define set_gpier(port, val) \
({                           \
  unsigned int k, m, n;      \
  unsigned int val_ll=0;     \
  unsigned int val_hh=0;     \
  unsigned int *value;       \
  show_long(val);            \
  for(n=0;n<32;n++) value[n]=((val&(0x1<<n))>>n);\
  if(port<=1){               \
                             \
     for(k=0;k<32;k=k+2) val_ll=(value[k/2]<<k)|(value[k/2]<<(k+1))|val_ll;\
     for(m=0;m<32;m=m+2) val_hh=(value[m/2+16]<<m)|(value[m/2+16]<<(m+1))|val_hh;\
                             \
     set_gpalr(port, val_ll);\
     set_gpaur(port, val_hh);\
    }                        \
  else{                      \
     for(k=0;k<14;k=k+2) val_ll=(value[k/2]<<k)|(value[k/2]<<(k+1))|val_ll;\
     set_gpalr(port, val_ll);\
    }                        \
})
#else
#define set_gpier(port, val)   i_s32(val, GPIER##port, 0x0)
#endif
/*for jz5730 02/26/2006*/
#define set_gpslr(port, val)   i_s32(val, GPSLR##port, 0x0)
#define set_gpsur(port, val)   i_s32(val, GPSUR##port, 0x0)
#define set_gpflr(port, val)   i_s32(val, GPFLR##port, 0x0)
#define set_gpfur(port, val)   i_s32(val, GPFUR##port, 0x0)
#define get_gpslr(port)   i_l32(GPSLR##port, 0x0)
#define get_gpsur(port)   i_l32(GPSUR##port, 0x0)
#define get_gpflr(port)   i_l32(GPFLR##port, 0x0)
#define get_gpfur(port)   i_l32(GPFUR##port, 0x0)
/*-----------*/

#define set_gpimr(port, val)   i_s32(val, GPIMR##port, 0x0)
#define set_gpfr(port, val)    i_s32(val, GPFR##port, 0)

#define get_gpdr(port)         i_l32(GPDR##port, 0x0)
#define get_gpodr(port)        i_l32(GPODR##port, 0x0)
#define get_gppur(port)        i_l32(GPPUR##port, 0x0)
#define get_gpdir(port)        i_l32(GPDIR##port, 0x0)
#define get_gpidlr(port)       i_l32(GPIDLR##port, 0x0)
#define get_gpidur(port)       i_l32(GPIDUR##port, 0x0)
#define get_gpalr(port)        i_l32(GPALR##port, 0x0)
#define get_gpaur(port)        i_l32(GPAUR##port, 0x0)
#define get_gpier(port)        i_l32(GPIER##port, 0x0)
#define get_gpimr(port)        i_l32(GPIMR##port, 0x0)
#define get_gpfr(port)         i_l32(GPFR##port, 0x0)
#define get_gpdr_pin(port,pin)       ( get_gpdr(port)  & ( 0x1 << pin ))
#define get_gpier_pin(port, pin)     ( get_gpier(port) & ( 0x1 << pin ))
#define get_gpimr_pin(port, pin)     ( get_gpimr(port) & ( 0x1 << pin ))
#define get_gpfr_pin(port, pin)      ( get_gpfr(port)  & ( 0x1 << pin ))

#define only_set_gpimr_pin(port, pin, val)   i_s32(val<<pin, GPIMR##port, 0x0)
#define set_gpfr_pin(port, pin, val)    i_s32(val<<pin, GPFR##port, 0)

#define set_gpdr_pin(port, pin, val) \
({										\
     set_gpdr(port, get_gpdr(port) & ~(0x1 << pin) | (val<<pin));	        \
})

#define set_gpdir_pin(port, pin, val)						\
({										\
     set_gpdir(port, get_gpdir(port) & ~(0x1 << pin) | (val<<pin));	        \
})

#define set_gpodr_pin(port, pin, val) \
({										\
     set_gpodr(port, get_gpodr(port) & ~(0x1 << pin) | (val<<pin));	        \
})

#define set_gppur_pin(port, pin, val) \
({										\
     set_gppur(port, get_gppur(port) & ~(0x1 << pin) | (val<<pin));	        \
})

#define set_gpidlr_pin(port, pin, val)					\
({										\
     set_gpidlr(port, get_gpidlr(port) & ~(0x3 << pin*2) | (val<<pin*2));	\
})

#define set_gpidur_pin(port, pin, val)					\
({										\
     set_gpidur(port, get_gpidur(port) & ~(0x3 << pin*2) | (val<<pin*2));	\
})

#define set_gpalr_pin(port, pin, val)					\
({										\
     set_gpalr(port, get_gpalr(port) & ~(0x3 << pin*2) | (val<<pin*2));	\
})

#define set_gpaur_pin(port, pin, val)					\
({										\
     set_gpaur(port, get_gpaur(port) & ~(0x3 << pin*2) | (val<<pin*2));	\
})

#ifdef JZ5730
#define set_gpier_pin(port, pin, val)\
({                                                                      \
    int idr_pin;                                                        \
    if(pin<=15)                                                         \
         set_gpalr(port, get_gpalr(0) & ~(0x3 << pin*2) | (val<<pin*2) | (0x1<<(pin*2+1)));\
    else if(pin>15){                                                    \
         idr_pin = pin-16;                                              \
         set_gpaur(port, get_gpaur(0) & ~(0x3 << idr_pin*2) | (val<<idr_pin*2) | (0x1<<(pin*2+1)));\
    }                                                                   \
})
#else
#define set_gpier_pin(port, pin, val)\
({										\
     set_gpier(port, get_gpier(port) & ~(0x1 << pin) | (val<<pin));	        \
})
#endif

#define set_gpimr_pin(port, pin, val)\
({										\
     set_gpimr(port, get_gpimr(port) & ~(0x1 << pin) | (val<<pin));	        \
})

#define set_gpxr(gpxr, val)    i_s32(val, gpxr, 0x0)
#define get_gpxr(gpxr)         i_l32(gpxr, 0x0)


#define EXT_GPCR(port)   EXT_GPCR##port
#define EXT_GPDR(port)   EXT_GPDR##port
#define EXT_GPIDLR(port) EXT_GPIDLR##port
#define EXT_GPIDUR(port) EXT_GPIDUR##port

#define set_alter_low_pin(port, pin, value)	set_gpalr_pin(port, pin, value)

#define set_alter_up_pin(port, pin, value)	set_gpaur_pin(port, pin, value)

#define set_low_dt_pin(port, pin)   set_gpidr_dt_pin(port, pin, IRQ_LOW)
#define set_high_dt_pin(port, pin)  set_gpidr_dt_pin(port, pin, IRQ_HIGH) 
#define set_rise_dt_pin(port, pin)  set_gpidr_dt_pin(port, pin, IRQ_RISE)  
#define set_fall_dt_pin(port, pin)  set_gpidr_dt_pin(port, pin, IRQ_FALL)

#ifdef JZ5730
#define set_gpidr_dt_pin(port, pin, IRQ_DTCT)                                            \
({                                                                                       \
    int idr_pin;                                                                         \
    if(pin<=15){                                                                         \
         set_gpalr(port, get_gpalr(port) & ~(0x3 << pin*2) | (0x2<<pin*2));                    \
         set_gpidlr(port, get_gpidlr(port) & ~(0x3 << pin*2) | (IRQ_DTCT<<pin*2));       \
    }                                                                                    \
    else if(pin>15){                                                                     \
         idr_pin = pin-16;                                                               \
         set_gpaur(port, get_gpaur(port) & ~(0x3 << idr_pin*2) | (0x2<<idr_pin*2));            \
         set_gpidur(port,get_gpidur(port) & ~(0x3<<idr_pin*2) | (IRQ_DTCT<<idr_pin*2));  \
    }                                                                                    \
})
#else
#define set_gpidr_dt_pin(port, pin, IRQ_DTCT)                                            \
({                                                                                       \
    int idr_pin;                                                                         \
    if(pin<=15)                                                                          \
         set_gpidlr(port, get_gpidlr(port) & ~(0x3 << pin*2) | (IRQ_DTCT<<pin*2));       \
    else if(pin>15){                                                                     \
         idr_pin = pin-16;                                                               \
         set_gpidur(port,get_gpidur(port) & ~(0x3<<idr_pin*2) | (IRQ_DTCT<<idr_pin*2));  \
    }                                                                                    \
})
#endif

#define set_gpidlr_dt_pin(port,pin,IRQ_DTCT)                                        \
({                                                                                  \
    set_gpidlr(port, get_gpidlr(port) & ~(0x3 << pin*2) | (IRQ_DTCT<<pin*2));       \
})

#define set_gpidur_dt_pin(port,pin,IRQ_DTCT)                                        \
({                                                                                  \
    set_gpidur(port, get_gpidur(port) & ~(0x3 << pin*2) | (IRQ_DTCT<<pin*2));       \
})

#define check_dt_pin(port, pin, IRQ_DTCT) (pin<=15) ? (get_gpidlr(port)>>(pin*2) & 0x3  == IRQ_DTCT) : (get_gpidlr(port)>>((pin-16)*2) & 0x3  == IRQ_DTCT) 

#define check_low_dt(port, pin)      check_dt_pin(port,pin,IRQ_LOW)
#define check_high_dt(port, pin)     check_dt_pin(port,pin,IRQ_HIGH)
#define check_fall_dt(port, pin)     check_dt_pin(port,pin,IRQ_FALL)
#define check_rise_dt(port, pin)     check_dt_pin(port,pin,IRQ_RISE)

#define check_gpdr_in_level_pin(port, pin, val)   ((get_gpdr(port) >> pin) & 0x1 != val)

#define check_gpdr_pin(port,pin,val)   ((get_gpdr(port) >> pin) & 0x1 != val)     
#define check_gpfr_pin(port,pin)   (REG32(GPFR##port)>>pin) != 0     
#define clear_gpfr_pin(port,pin)   (REG32(GPFR##port) = (0x1 << pin))  // Write 1 to clear gpfr
#define clear_gpier_pin(port,pin)  (REG32(GPIER##port) &= (~(1 << pin)))
     
#define check_gpidr_pin(port, pin, LorU,mode)                   \
   ((~LorU) && ((REG32(GPIDLR##port) >> (pin*2)) == mode) ||\
      LorU  && ((REG32(GPIDUR##port) >> (pin*2)) == mode))

#define set_alter(N,value)\
({                        \
  unsigned port,pin;      \
  port = N / 32;          \
  pin  = N % 32;          \
  if(value == 3) {        \
    set_pin_dev3(port, pin);  \
    }                     \
  else if(value == 2) {   \
    set_pin_dev2(port, pin);  \
    }                     \
  else if(value == 1) {   \
    set_pin_dev1(port, pin);  \
    }                     \
  else {                  \
    set_pin_dev0(port, pin);  \
    }                     \
})

#ifdef FALCON
#define set_alter_dis(N,value)\
({                        \
  unsigned port,pin;      \
  port = N / 32;          \
  pin  = N % 32;          \
  set_pfun(port, pin);    \
  if(value == 3) {        \
    clr_pint(port,pin);   \
    set_pmsk(port, pin);  \
    }                     \
  else if(value == 2) {        \
    clr_pint(port,pin);   \
    set_pmsk(port, pin);  \
    }                     \
  else if(value == 1) {   \
    clr_pint(port,pin);   \
    set_pmsk(port, pin);  \
    }                     \
  else {                  \
    clr_pint(port,pin);   \
    set_pmsk(port, pin);  \
    }                     \
})
#else
#define set_alter_dis(N,value)\
({                        \
  unsigned port,pin;      \
  port = N / 32;          \
  pin  = N % 32;          \
  set_pfun(port, pin);    \
  if(value == 3) {        \
    clr_ptrg(port,pin);   \
    clr_psel(port, pin);  \
    }                     \
  else if(value == 2) {        \
    clr_ptrg(port,pin);   \
    set_psel(port, pin);  \
    }                     \
  else if(value == 1) {   \
    set_ptrg(port,pin);   \
    clr_psel(port, pin);  \
    }                     \
  else {                  \
    set_ptrg(port,pin);   \
    set_psel(port, pin);  \
    }                     \
})
#endif

#define set_pindat(N,value)\
({                        \
  unsigned port,pin;      \
  port = N / 32;          \
  pin  = N % 32;          \
  if(value == 1) {        \
    set_pdat(port,pin);   \
    }                     \
  else if(value == 0) {   \
    clr_pdat(port,pin);   \
    }                     \
})
#define set_pinpull(N,value)\
({                        \
  unsigned port,pin;      \
  port = N / 32;          \
  pin  = N % 32;          \
  if(value == 1) {        \
    set_ppe(port,pin);   \
    }                     \
  else if(value == 0) {   \
    clr_ppe(port,pin);   \
    }                     \
})
#define set_pindir(N,value)			\
({						\
  unsigned port,pin;				\
  port = N / 32;				\
  pin  = N % 32;				\
  if(value == 1) {				\
    set_pdir(port, pin);			\
  }						\
  else if(value == 0) {				\
    clr_pdir(port, pin);			\
  }						\
})
#define set_pinout(N,value)			\
({						\
  unsigned port,pin;				\
  port = N / 32;				\
  pin  = N % 32;				\
  if(value == 1) {				\
    set_pdat(port, pin);			\
    set_pdir(port, pin);			\
    set_ppe(port, pin);				\
  }						\
  else if(value == 0) {				\
    clr_pdat(port, pin);			\
    set_pdir(port, pin);			\
    set_ppe(port, pin);				\
  }						\
})

//uart
#define UART0_nf {4,0,0,0}
#define UART0_ALT_F0 {160,161,162,163}
#define UART0_ALT_F1 {}
#define UART0_ALT_F2 {}
#define UART0_ALT_F3 {}

#define UART1_nf {4,0,0,0}
#define UART1_ALT_F0 {122,123,124,125}
#define UART1_ALT_F1 {}
#define UART1_ALT_F2 {}
#define UART1_ALT_F3 {}

#define UART2_nf {0,4,0,0}
#define UART2_ALT_F0 {}
#define UART2_ALT_F1 {100,101,102,103}
#define UART2_ALT_F2 {}
#define UART2_ALT_F3 {}

#define UART3_nf {3,1,0,0}
#define UART3_ALT_F0 {108, 136, 137}
#define UART3_ALT_F1 {133}
#define UART3_ALT_F2 {}
#define UART3_ALT_F3 {}

#define UART4_nf {0,0,3,0}
#define UART4_ALT_F0 {}
#define UART4_ALT_F1 {}
#define UART4_ALT_F2 {74, 84, 84}
#define UART4_ALT_F3 {}

#define alter4uart0() alter4mod(UART0)
#define alter4uart1() alter4mod(UART1)
#define alter4uart2() alter4mod(UART2)
#define alter4uart3() alter4mod(UART3)
#define alter4uart4() alter4mod(UART4)

#define alter4uart(uart_id)            \
({ if(uart_id==0) {alter4uart0();}     \
   else if(uart_id==1) {alter4uart1();}\
   else if(uart_id==2) {alter4uart2();}\
   else if(uart_id==3) {alter4uart3();}\
   else if(uart_id==4) {alter4uart4();}\
})

//mic
#define MIC_nf {0,2,0,0}
#define MIC_ALT_F0 {}
#define MIC_ALT_F1 {50,51}
#define MIC_ALT_F2 {}
#define MIC_ALT_F3 {}

#define alter4mic() alter4mod(MIC)


//owi
#define OWI_nf {0,1,0,0}
#define OWI_ALT_F0 {}
#define OWI_ALT_F1 {29}
#define OWI_ALT_F2 {}
#define OWI_ALT_F3 {}

#define alter4owi() alter4mod(OWI)

#ifdef TSSI0
#define TSSI_PB_F0  (GPIO_PIN_22 | GPIO_PIN_23 | GPIO_PIN_24 | GPIO_PIN_25 | GPIO_PIN_26 | GPIO_PIN_27)
#define TSSI_PB_F3  (GPIO_PIN_20 | GPIO_PIN_21 | GPIO_PIN_28 | GPIO_PIN_29 | GPIO_PIN_30 | GPIO_PIN_31)

#define alter4tssi()                    \
({					\
  set_pin_dev0_2(GPIO_PB, TSSI_PB_F0);	\
  set_pin_dev3_2(GPIO_PB, TSSI_PB_F3);  \
})
#endif

#ifdef TSSI1
#define TSSI_PB_F1  (GPIO_PIN_06 | GPIO_PIN_07 | GPIO_PIN_08 | GPIO_PIN_09 | GPIO_PIN_10 | GPIO_PIN_11 | \
		     GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_16 | GPIO_PIN_17)

#define alter4tssi() set_pin_dev1_2(GPIO_PB, TSSI_PB_F1)
#endif

#ifdef CIM0
#define CIM_PORTB_F0_PIN  (GPIO_PIN_06 | GPIO_PIN_07 | GPIO_PIN_08 | GPIO_PIN_09 | \
                           GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
			   GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_16 | GPIO_PIN_17)

#define CIM_PORTB_F0_PIN_HI  (GPIO_PIN_18 | GPIO_PIN_19)

#define CIM_PORTF_F0_PIN  (GPIO_PIN_16 | GPIO_PIN_17)

#define alter4cim()  {                                                     \
                        REG32(GPIO_PINTC(GPIO_PORTB)) = CIM_PORTB_F0_PIN;  \
                        REG32(GPIO_PMSKC(GPIO_PORTB)) = CIM_PORTB_F0_PIN;  \
                        REG32(GPIO_PPAT0C(GPIO_PORTB)) = CIM_PORTB_F0_PIN; \
                        REG32(GPIO_PPAT1C(GPIO_PORTB)) = CIM_PORTB_F0_PIN; \
                        REG32(GPIO_PINTC(GPIO_PORTB)) = CIM_PORTB_F0_PIN_HI;  \
                        REG32(GPIO_PMSKC(GPIO_PORTB)) = CIM_PORTB_F0_PIN_HI;  \
                        REG32(GPIO_PPAT0C(GPIO_PORTB)) = CIM_PORTB_F0_PIN_HI; \
                        REG32(GPIO_PPAT1C(GPIO_PORTB)) = CIM_PORTB_F0_PIN_HI; \
                        REG32(GPIO_PINTC(GPIO_PORTF)) = CIM_PORTF_F0_PIN;  \
                        REG32(GPIO_PMSKC(GPIO_PORTF)) = CIM_PORTF_F0_PIN;  \
                        REG32(GPIO_PPAT0C(GPIO_PORTF)) = CIM_PORTF_F0_PIN; \
                        REG32(GPIO_PPAT1C(GPIO_PORTF)) = CIM_PORTF_F0_PIN; \
                       }
#endif

#ifdef CIM00
#define CIM_PB_F0 (GPIO_PIN_18 | GPIO_PIN_19)

#define CIM_PF_F1 (GPIO_PIN_04 | GPIO_PIN_05 | GPIO_PIN_06 | GPIO_PIN_07 | \
		   GPIO_PIN_08 | GPIO_PIN_09 | GPIO_PIN_10 | GPIO_PIN_11 | \
		   GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)

#define CIM_PF_F0 (GPIO_PIN_16 | GPIO_PIN_17)

#define alter4cim()                   \
({				      \
  set_pin_dev0_2(GPIO_PB, CIM_PB_F0); \
  set_pin_dev1_2(GPIO_PF, CIM_PF_F1); \
  set_pin_dev0_2(GPIO_PF, CIM_PF_F0); \
})
#endif

//msc0
#define MSC0_nf {0,10,0,0}
#define MSC0_ALT_F0 {}
#define MSC0_ALT_F1 {4, 5, 6, 7, 18,19,20,21,22,23}
#define MSC0_ALT_F2 {}
#define MSC0_ALT_F3 {}

#define MSC00_nf {6,0,0,0}
#define MSC00_ALT_F0 {148,149,150,151,156,157}
#define MSC00_ALT_F1 {}
#define MSC00_ALT_F2 {}
#define MSC00_ALT_F3 {}

//msc1
#define MSC1_nf {6,0,0,0}
#define MSC1_ALT_F0 {116,117,118,119,120,121}
#define MSC1_ALT_F1 {}
#define MSC1_ALT_F2 {}
#define MSC1_ALT_F3 {}

#define MSC11_nf {0,6,0,0}
#define MSC11_ALT_F0 {}
#define MSC11_ALT_F1 {148,149,150,151,156,157}
#define MSC11_ALT_F2 {}
#define MSC11_ALT_F3 {}

//msc2
#define MSC2_nf {6,0,0,0}
#define MSC2_ALT_F0 {52,53,60,61,62,63}
#define MSC2_ALT_F1 {}
#define MSC2_ALT_F2 {}
#define MSC2_ALT_F3 {}

#define MSC22_nf {0,0,6,0}
#define MSC22_ALT_F0 {}
#define MSC22_ALT_F1 {}
#define MSC22_ALT_F2 {148,149,150,151,156,157}
#define MSC22_ALT_F3 {}

#define alter4msc0() alter4mod(MSC0)
#define alter4msc1() alter4mod(MSC1)
#define alter4msc2() alter4mod(MSC2)
#define alter4msc00() alter4mod(MSC00)
#define alter4msc11() alter4mod(MSC11)
#define alter4msc22() alter4mod(MSC22)

#ifdef MSC0
#define alter4msc() alter4msc0()
#endif
#ifdef MSC1
#define alter4msc() alter4msc1()
#endif
#ifdef MSC2
#define alter4msc() alter4msc2()
#endif
#ifdef MSC00
#define alter4msc() alter4msc00()
#endif
#ifdef MSC11
#define alter4msc() alter4msc11()
#endif
#ifdef MSC22
#define alter4msc() alter4msc22()
#endif

#ifdef SSI0
#define SSI_nf {0,0,4,0}
#define SSI_ALT_F0 {}
#define SSI_ALT_F1 {}
#define SSI_ALT_F2 {20,23,21,18}
#define SSI_ALT_F3 {}
#endif

#define p2b(N) (1 << (N))

#ifdef SSI0
#define alter4ssi()     alter4mod(SSI)
#define alter4ssi_dis() alter4mod_dis(SSI)
#endif

#ifdef SSI00
#define alter4ssi()                                            \
({                                                             \
   set_pin_dev1_2(GPIO_PB, p2b(5));                                  \
   set_pin_dev2_2(GPIO_PA, (p2b(25) | p2b(27) | p2b(28)));           \
})
#endif

#ifdef SSI000
#define SSI_PB_F1 (p2b(20) | p2b(21) | p2b(28) | p2b(29) | p2b(30) | p2b(31))
#define alter4ssi() set_pin_dev1_2(GPIO_PB, SSI_PB_F1)
#endif

#ifdef SSI0000
#define SSI_PD_F1 (p2b(20) | p2b(21) | p2b(22) | p2b(23) | p2b(24) | p2b(25))
#define alter4ssi() set_pin_dev1_2(GPIO_PD, SSI_PD_F1)
#endif

#ifdef SSI00000
#define SSI_PE_F0 (p2b(14) | p2b(15) | p2b(16) | p2b(17) | p2b(18) | p2b(19))
#define alter4ssi() set_pin_dev0_2(GPIO_PE, SSI_PE_F0)
#endif

#ifdef SSI1
#define SSI_PB_F2 (p2b(20) | p2b(21) | p2b(28) | p2b(29) | p2b(30) | p2b(31))
#define alter4ssi() set_pin_dev2_2(GPIO_PB, SSI_PB_F2)
#endif

#ifdef SSI11
#define SSI_PD_F2 (p2b(20) | p2b(21) | p2b(22) | p2b(23) | p2b(24) | p2b(25))
#define alter4ssi() set_pin_dev2_2(GPIO_PD, SSI_PD_F2)
#endif

#ifdef SSI111
#define SSI_PE_F1 (p2b(14) | p2b(15) | p2b(16) | p2b(17) | p2b(18) | p2b(19))
#define alter4ssi()  set_pin_dev1_2(GPIO_PE, SSI_PE_F1)
#endif

//cim
#define VCI_nf {16,0,0,0}
#define VCI_ALT_F0 {38,39,40,41,42,43,44,45,46,47,48,49,50,51,176,177}
#define VCI_ALT_F1 {}
#define VCI_ALT_F2 {}
#define VCI_ALT_F3 {}

#define alter4vci() alter4mod(VCI)

//pwm
#define PWM_nf {8,0,0,0}
#define PWM_ALT_F0 {106,107,128,129,130,131,132,133}
#define PWM_ALT_F1 {}
#define PWM_ALT_F2 {}
#define PWM_ALT_F3 {}

#define alter4pwm() alter4mod(PWM)

//pcm
#define PCM_nf {4,0,0,0}
#define PCM_ALT_F0 {96, 97, 98, 99}
#define PCM_ALT_F1 {}
#define PCM_ALT_F2 {}
#define PCM_ALT_F3 {}

//pcm1
#define PCM1_nf {0,4,0,0}
#define PCM1_ALT_F0 {}
#define PCM1_ALT_F1 {172, 173, 174, 175}
#define PCM1_ALT_F2 {}
#define PCM1_ALT_F3 {}

#define PCM0_PORTD_F0_PIN  (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)

#define PCM1_PORTF_F1_PIN  (GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)

#define alter4pcm0() {                                                      \
                        REG32(GPIO_PINTC(GPIO_PORTD)) = PCM0_PORTD_F0_PIN;  \
                        REG32(GPIO_PMSKC(GPIO_PORTD)) = PCM0_PORTD_F0_PIN;  \
                        REG32(GPIO_PPAT0C(GPIO_PORTD)) = PCM0_PORTD_F0_PIN; \
                        REG32(GPIO_PPAT1C(GPIO_PORTD)) = PCM0_PORTD_F0_PIN; \
                       }

#define alter4pcm1() {                                                      \
                        REG32(GPIO_PINTC(GPIO_PORTF)) = PCM1_PORTF_F1_PIN;  \
                        REG32(GPIO_PMSKC(GPIO_PORTF)) = PCM1_PORTF_F1_PIN;  \
                        REG32(GPIO_PPAT0S(GPIO_PORTF)) = PCM1_PORTF_F1_PIN; \
                        REG32(GPIO_PPAT1C(GPIO_PORTF)) = PCM1_PORTF_F1_PIN; \
                       }

#ifdef TEST_PCM0
#define alter4pcm()  alter4pcm0()
#else
#define alter4pcm()  alter4pcm1()
#endif

#ifdef I2C0
#define I2C_PD_F0 (GPIO_PIN_30 | GPIO_PIN_31)
#define alter4i2c() set_pin_dev0_2(GPIO_PD, I2C_PD_F0)
#endif

#ifdef I2C1
#define I2C_PE_F0 (GPIO_PIN_30 | GPIO_PIN_31)
#define alter4i2c() set_pin_dev0_2(GPIO_PE, I2C_PE_F0)
#endif

#ifdef I2C2
#define I2C_PF_F2 (GPIO_PIN_16 | GPIO_PIN_17)
#define alter4i2c() set_pin_dev2_2(GPIO_PF, I2C_PF_F2)
#endif

#ifdef I2C3
#define I2C_PD_F1 (GPIO_PIN_10 | GPIO_PIN_11)
#define alter4i2c() set_pin_dev1_2(GPIO_PD, I2C_PD_F1)
#endif

#ifdef I2C4
#define I2C_PE_F1 (GPIO_PIN_03 | GPIO_PIN_04)
#define alter4i2c() set_pin_dev1_2(GPIO_PE, I2C_PE_F1)
#endif

#ifdef I2C44
#define I2C_PE_F1 (GPIO_PIN_12 | GPIO_PIN_13)
#define alter4i2c() set_pin_dev1_2(GPIO_PE, I2C_PE_F1)
#endif

#ifdef I2C444
#define I2C_PF_F1 (GPIO_PIN_24 | GPIO_PIN_25)
#define alter4i2c() set_pin_dev1_2(GPIO_PF, I2C_PF_F1)
#endif

//lcd
#define LCD0_nf {28,0,0,22}
#define LCD0_ALT_F0 {64,65,66,67,68,69,70,\
                     71,72,73,74,75,76,77,\
                     78,79,80,81,82,83,84,\
                     85,86,87,88,89,90,91}
#define LCD0_ALT_F1 {}
#define LCD0_ALT_F2 {}
#define LCD0_ALT_F3 {41,42,43,44,45,46,47,\
                     48,49,50,51,108,109,134,\
                     135,136,137,139,140,141,\
                     176,177}

//lcd1
#define LCD1_nf {20,4,0,0}
#define LCD1_ALT_F0 {         67,68,69,70,\
                     71,72,73,      76,77,\
                     78,79,80,81,82,83,   \
                           87,88,89,90,91}
#define LCD1_ALT_F1 {64,65,74,84}
#define LCD1_ALT_F2 {}
#define LCD1_ALT_F3 {}

#define alter4lcd0() alter4mod(LCD0)
#define alter4lcd1() alter4mod(LCD1)
#define alter4lcd()  alter4lcd0()

//aic
#define AIC_nf {6,3,1,0}
#define AIC_ALT_F0 {109,134,135,139,140,141}
#define AIC_ALT_F1 {108,136,137}
#define AIC_ALT_F2 {133}
#define AIC_ALT_F3 {}

#define alter4sac(bitclkd,syncd) alter4mod(AIC)

#define AIC1_nf {5,2,0,0}
#define AIC1_ALT_F0 {178,179,180,181,182}
#define AIC1_ALT_F1 {140,141}
#define AIC1_ALT_F2 {}
#define AIC1_ALT_F3 {}

#define alter4sac_aic1(bitclkd,syncd) alter4mod(AIC1)

//spdif
#define SPDIF_nf {1,0,1,0}
#define SPDIF_ALT_F0 {135}
#define SPDIF_ALT_F1 {}
#define SPDIF_ALT_F2 {133}
#define SPDIF_ALT_F3 {}

#define alter4spdif() alter4mod(SPDIF)

//dma
#define DMA_nf {2,2,0,0}
#define DMA_ALT_F0 {28,29}
#define DMA_ALT_F1 {36,37}  
#define DMA_ALT_F2 {}
#define DMA_ALT_F3 {}

#define EXT0_nf {2,0,0,0}
#define EXT0_ALT_F0 {28,29}
#define EXT0_ALT_F1 {}  
#define EXT0_ALT_F2 {}
#define EXT0_ALT_F3 {}

#define EXT00_nf {1,0,0,0}
#define EXT00_ALT_F0 {28}
#define EXT00_ALT_F1 {}  
#define EXT00_ALT_F2 {}
#define EXT00_ALT_F3 {}

#define EXT01_nf {1,0,0,0}
#define EXT01_ALT_F0 {29}
#define EXT01_ALT_F1 {}  
#define EXT01_ALT_F2 {}
#define EXT01_ALT_F3 {}

#define EXT1_nf {0,2,0,0}
#define EXT1_ALT_F0 {}
#define EXT1_ALT_F1 {36,37}  
#define EXT1_ALT_F2 {}
#define EXT1_ALT_F3 {}

#define EXT10_nf     {0,1,0,0}
#define EXT10_ALT_F0 {}
#define EXT10_ALT_F1 {37}  
#define EXT10_ALT_F2 {}
#define EXT10_ALT_F3 {}

#define EXT11_nf     {0,1, 0,0}
#define EXT11_ALT_F0 {}
#define EXT11_ALT_F1 {36}  
#define EXT11_ALT_F2 {}
#define EXT11_ALT_F3 {}

#define alter4dma()  alter4mod(DMA)

#define alter4ext0() alter4mod(EXT0)
#define alter4ext1() alter4mod(EXT1)

#define alter4ext00() alter4mod(EXT00)
#define alter4ext01() alter4mod(EXT01)

#define alter4ext10() alter4mod(EXT10)
#define alter4ext11() alter4mod(EXT11)

// SCC
/* #define SCC_nf {2,0,0,0} */
/* #define SCC_ALT_F0 {104,105} */
/* #define SCC_ALT_F1 {}   */
/* #define SCC_ALT_F2 {} */
/* #define SCC_ALT_F3 {} */

/* #define alter4scc0() alter4mod(SCC) */
/* #define alter4scc1() alter4mod(SCC) */
/* #define alter4scc(scc_id)            \ */
/* ({                                   \ */
/*    if(scc_id==0) {alter4scc0();}     \ */
/*    else if(scc_id==1) {alter4scc1();}\ */
/* }) */

#define SCC_PORTD_F0_PIN  (GPIO_PIN_8 | GPIO_PIN_9)

#define alter4scc(scc_id)  {                                                     \
                        REG32(GPIO_PINTC(GPIO_PORTD)) = SCC_PORTD_F0_PIN;  \
                        REG32(GPIO_PMSKC(GPIO_PORTD)) = SCC_PORTD_F0_PIN;  \
                        REG32(GPIO_PPAT0C(GPIO_PORTD)) = SCC_PORTD_F0_PIN; \
                        REG32(GPIO_PPAT1C(GPIO_PORTD)) = SCC_PORTD_F0_PIN; \
                       }


//PS2
#define PS2_nf {4,0,0,0}
#define PS2_ALT_F0 {100,101,102,103}
#define PS2_ALT_F1 {}  
#define PS2_ALT_F2 {}
#define PS2_ALT_F3 {}

#define alter4ps2() alter4mod(PS2)

//gps
#define GPS_nf {0,3,0,0}
#define GPS_ALT_F0 {}
#define GPS_ALT_F1 {160,161,162}  
#define GPS_ALT_F2 {}
#define GPS_ALT_F3 {}

#define alter4gps() alter4mod(GPS)

#define gps_tb_start_gpio(N) \
({ \
  if (N == 1) { \
    set_pdat(2, 0); \
  } \
  else { \
    clr_pdat(2, 0); \
  } \
  set_pdir(2, 0); \
  clr_pfun(2, 0); \
})

#define gps_tb_finish_gpio(N) \
({ \
  if (N == 1) { \
    set_pdat(2, 1); \
  } \
  else { \
    clr_pdat(2, 1); \
  } \
  set_pdir(2, 1); \
  clr_pfun(2, 1); \
})

#define gps_tb_start()  ({gps_tb_start_gpio(0); gps_tb_finish_gpio(0); gps_tb_start_gpio(1);})

#define gps_tb_finish() ({gps_tb_start_gpio(0); gps_tb_finish_gpio(0); gps_tb_finish_gpio(1);})

//emi
#define EMI_nf {32,0,0,0}
#define EMI_ALT_F0 { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,\
                    11,12,13,14,15,16,17,18,19,21,22,\
                    23,24,25,26,32,33,34,35,36,37}
#define EMI_ALT_F1 {}
#define EMI_ALT_F2 {}
#define EMI_ALT_F3 {}

#define alter4emi()     ({alter4mod(EMI); set_pin_gpio_i(0, 20);})

#define RDWR__nf {0,1,0,0}
#define RDWR__ALT_F0 {}
#define RDWR__ALT_F1 {26}
#define RDWR__ALT_F2 {}
#define RDWR__ALT_F3 {}

#define alter4rdwr_()     alter4mod(RDWR_)

//clk48m

#define CLK48M_nf {0,1,0,0}
#define CLK48M_ALT_F0 {}
#define CLK48M_ALT_F1 {105}
#define CLK48M_ALT_F2 {}
#define CLK48M_ALT_F3 {}

#define alter4clk48m()     alter4mod(CLK48M)

//exclk

#define EXCLK_nf {1,0,0,0}
#define EXCLK_ALT_F0 {}
#define EXCLK_ALT_F1 {111}
#define EXCLK_ALT_F2 {}
#define EXCLK_ALT_F3 {}

#define alter4exclk()     alter4mod(EXCLK)

//drvvbus
#define DRVVBUS_nf {1,0,0,0}
#define DRVVBUS_ALT_F0 {}
#define DRVVBUS_ALT_F1 {138}
#define DRVVBUS_ALT_F2 {}
#define DRVVBUS_ALT_F3 {}

#define alter4drvvbus()     alter4mod(DRVVBUS)
//mii
#define MII_nf {12,4,1,0}
#define MII_ALT_F0 {164,165,166,167,168,169,170,\
                    171,172,173,174,175}
#define MII_ALT_F1 {125,124,123,122}  
#define MII_ALT_F2 {36}
#define MII_ALT_F3 {}

#define alter4mii() alter4mod(MII)

#define alter4jtag2uart0() ({;})
#define alter4jtag2uart1() ({;})
#define alter4jtag2uart2() ({;})
#define alter4jtag2uart3()                                     \
({                                                             \
  clr_pmsk(0, 31);                                             \
  clr_ppat1(0, 31);                                             \
  set_ppat0(0, 31);                                             \
  set_ppat1(0,30);                                               \
})
#define alter4jtag2uart4() ({;})

#define alter4jtag2uart(N)  alter4jtag2uart##N()

#define alter4jtag2ps2()                                       \
({                                                             \
  clr_pmsk(0, 30);                                             \
  clr_ppat1(0, 30);                                             \
  set_ppat0(0, 30);                                             \
  set_ppat1(0,31);                                               \
})

#define alter4jtag()                                           \
({                                                             \
  clr_pmsk(0, 30);                                             \
  set_ppat1(0, 30);                                             \
  set_ppat0(0, 30);                                             \
  set_ppat1(0,31);                                               \
})

#define SSI_nf {0,1,3,0}
#define SSI_ALT_F0 {}
#define SSI_ALT_F1 {37}
#define SSI_ALT_F2 {25,27,28}
#define SSI_ALT_F3 {}

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

#define alter4mod_dis(MODULE)                                  \
({                                                             \
   unsigned int nf[]=MODULE##_nf;                              \
   unsigned int F0[]=MODULE##_ALT_F0;                          \
   unsigned int F1[]=MODULE##_ALT_F1;                          \
   unsigned int F2[]=MODULE##_ALT_F2;                          \
   unsigned int F3[]=MODULE##_ALT_F3;                          \
   int j;                                                      \
   for(j=0;j<nf[0];j++)					       \
     set_alter_dis(F0[j],0);				       \
   for(j=0;j<nf[1];j++)					       \
     set_alter_dis(F1[j],1);				       \
   for(j=0;j<nf[2];j++)					       \
     set_alter_dis(F2[j],2);				       \
   for(j=0;j<nf[3];j++)					       \
     set_alter_dis(F3[j],3);				       \
})

// set EMI_ADD[25:20] as gpio function
#define set_add_mults_as_gpio()    \
({unsigned int i;                  \
  for(i=76;i<=81;i++)              \
     set_alter(i,0x0);             \
})

#define start_power_analyse()                   \
({                                              \
  REG8(GPADR) =  0x0;                           \
  REG32(EXT_GPAMR) |= 0x1  ;                    \
  REG32(GPACR) |= 0x1;                          \
})

#define get_ext_gpdr(port)            i_l32(EXT_GPDR##port, 0x0)
#define get_ext_gppr_high(port)       i_l32(EXT_GPPR##port##_HIGH, 0x0)
#define get_ext_gppr_low(port)        i_l32(EXT_GPPR##port##_LOW, 0x0)


#define check_ext_gppr_high(port,val)                 \
({                                                    \
   if(get_ext_gppr_high(port)!=val)                   \
     return 1;                                        \
})

#define check_ext_gppr_low(port,val)                  \
({                                                    \
   if(get_ext_gppr_low(port)!=val)                    \
     return 1;                                        \
})

#define check_ext_gppr_pin(port,pin,val)                            \
({unsigned int value;                                               \
  if(pin<=15)                                                       \
   value = (i_l32(EXT_GPPR##port##_LOW, 0x0) & (0x3 << pin*2)) ;    \
  else                                                              \
   value = (i_l32(EXT_GPPR##port##_HIGH, 0x0) & (0x3 << pin*2));    \
  if(value!=val) return 1;                                          \
})

#define ext_start_pin(port, pin)   REG32(EXT_GPCR##port) |=  (1 << pin)

#define ext_start_port(port,value)  REG32(EXT_GPCR##port) = value
#define ext_stop_port(port)  REG32(EXT_GPCR##port) = 0x0

#define set_ext_gpdr(port,val)   REG32(EXT_GPDR##port) = (val)

#define set_ext_gpdr_pin(port,pin,level)        \
({                                              \
    REG32(EXT_GPDR##port) = i_l32(EXT_GPDR##port, 0x0) & ~(1 << pin) |(level << pin);\
})

#define set_ext_gpdr_port(port, val)  REG32(EXT_GPDR##port) =  val

#define set_ext_gpmd_pin(port,pin,level)        \
({                                              \
    REG32(EXT_GPMD##port) = i_l32(EXT_GPMD##port, 0x0) & ~(1 << pin) |(level << pin);\
})

#define set_ext_gpmd_port(port, value)  REG32(EXT_GPMD##port) =  value

#define check_ext_gppr_in_all_pin(port,val)   get_ext_gppr(port) != val

#define prepare_p_resetp_driver(pin_level, wait, assert)       \
({                                                             \
  REG32(EXT_RSTDA) = pin_level << EXT_GPIO_P;                  \
  REG32(EXT_WAIT_PRESETP) = wait;                              \
  REG32(EXT_ASSERT_PRESETP) = assert;                          \
  REG32(EXT_TIMES_PRESETP) = 1;                                \
})

#define prepare_p_resetm_driver(pin_level, wait, assert)       \
({                                                             \
  REG32(EXT_RSTDA) = pin_level << EXT_GPIO_M;                  \
  REG32(EXT_WAIT_PRESETM) = wait;                              \
  REG32(EXT_ASSERT_PRESETM) = assert;                          \
  REG32(EXT_TIMES_PRESETM) = 1;                                \
})

#define prepare_p_resetpci_driver(pin_level, wait, assert)     \
({                                                             \
  REG32(EXT_RSTDA) = pin_level << EXT_GPIO_PCI;                \
  REG32(EXT_WAIT_PRESETPCI) = wait;                            \
  REG32(EXT_ASSERT_PRESETPCI) = assert;                        \
  REG32(EXT_TIMES_PRESETPCI) = 1;                              \
})

#define PCI_RESET_ENB()                                      \
({                                                           \
  REG32(EXT_RSTCR) = (1 << EXT_GPIO_PCI);    \
})

#define M_RESET_ENB()                                        \
({                                                           \
  REG32(EXT_RSTCR) = (1 << EXT_GPIO_M);      \
})

#define P_RESET_ENB()                                        \
({                                                           \
  REG32(EXT_RSTCR) = (1 << EXT_GPIO_P);      \
})

#define prepare_irq_drv_pin(port, pin, wait, assert, times)    \
({                                                             \
     REG32(EXT_WAIT_P##port(pin)) = wait;                      \
     REG32(EXT_ASSERT_P##port(pin)) = assert;                  \
     REG32(EXT_TIMES_P##port(pin)) = times;                    \
})

#define prepare_irq_drv_port(port, val, wait, assert, times)    \
({                                                              \
  int pin, b1=0;                                                \
  for(pin=0;pin<=31;pin++){                                     \
     b1 = (val>>pin) & 0x1;                                     \
     if(b1){                                                    \
        i_s32(wait, EXT_WAIT_P##port(pin), 0x0);                \
        i_s32(assert, EXT_ASSERT_P##port(pin), 0x0);            \
        i_s32(times, EXT_TIMES_P##port(pin), 0x0);              \
     }                                                          \
  }                                                             \
})

#define prepare_irq_level_pin(port, pin, value)                 \
({								\
  set_ext_gpdr_pin(port, pin, value);	                        \
  prepare_irq_drv_pin(port, pin, 0xa, 0xf, 0);	                \
  ext_start_pin(port,pin);                                      \
})

#define prepare_irq_level_pin2(port, pin, value, wait_time, assert_time, times)   \
({								\
  set_ext_gpdr_pin(port, pin, value);	                        \
  prepare_irq_drv_pin(port, pin, wait_time, assert_time, times);\
  ext_start_pin(port,pin);                                      \
})

#define prepare_irq_level_port(port, value)                     \
({								\
  int pin;                                                      \
  unsigned long st_p=0;		                                \
  set_ext_gpdr_port(port, value);	                        \
  prepare_irq_drv_port(port, value, 0xa, 0xf, 0);	        \
  ext_start_port(port,value);                                   \
})

#define prepare_pulse_irq_level_pin(port, pin, value)           \
({								\
  set_ext_gpdr_pin(port, pin, value);	                        \
  prepare_irq_drv_pin(port, pin, 0x0, 0x1, 1);	                \
  ext_start_pin(port,pin);                                      \
})

#define set_ext_gpdrN(N, level)                                \
({                                                             \
  if(N<=31)                                                    \
     REG32(EXT_GPDR0) = level << N;                            \
  else if(N<=63)                                               \
     REG32(EXT_GPDR1) = level << (N-32);                       \
  else                                                         \
     REG32(EXT_GPDR2) = level << (N-64);                       \
})

#define ext_startN(N)                           \
({                                              \
  if(N<=31)                                     \
     REG32(EXT_GPCR0) |= 1 << N;                \
  else if(N<=63)                                \
     REG32(EXT_GPCR1) |= 1 << (N-32);           \
  else                                          \
     REG32(EXT_GPCR2) |= 1 << (N-64);           \
})

#define set_ext_gpio_drN(level, N)              \
({                                              \
  if(N<=31)                                     \
    REG32(EXT_GPDR0) = i_l32(EXT_GPDR0, 0x0) & ~(1 << N) |(level << N);\
  else if(N<=63)                                \
    REG32(EXT_GPDR1) = i_l32(EXT_GPDR1, 0x0) & ~(1 << (N-32)) |(level << (N-32));\
  else                                          \
    REG32(EXT_GPDR2) = i_l32(EXT_GPDR2, 0x0) & ~(1 << (N-64)) |(level << (N-64));\
})

#define set_ext_gpio_mdN(level, N)              \
({                                              \
  if(N<=31)                                     \
    REG32(EXT_GPMD0) = i_l32(EXT_GPMD0, 0x0) & ~(1 << N) |(level << N);\
  else if(N<=63)                                \
    REG32(EXT_GPMD1) = i_l32(EXT_GPMD1, 0x0) & ~(1 << (N-32)) |(level << (N-32));\
  else                                          \
    REG32(EXT_GPMD2) = i_l32(EXT_GPMD2, 0x0) & ~(1 << (N-64)) |(level << (N-64));\
})

#define prepare_irq_driverN(N, wait, assert, times)            \
({                                                             \
  int pin;                                                     \
  if(N<=31)                                                    \
     pin =N;                                                   \
     REG32(EXT_WAIT_P0(pin)) = wait;                           \
     REG32(EXT_ASSERT_P0(pin)) = assert;                       \
     REG32(EXT_TIMES_P0(pin)) = times;                         \
  }                                                            \
  else if(N<=63){                                              \
     pin =N-32;                                                \
     REG32(EXT_WAIT_P1(pin)) = wait;                           \
     REG32(EXT_ASSERT_P1(pin)) = assert;                       \
     REG32(EXT_TIMES_P1(pin)) = times;                         \
  }                                                            \
  else{                                                        \
     pin =N-64;                                                \
     REG32(EXT_WAIT_P2(pin)) = wait;                           \
     REG32(EXT_ASSERT_P2(pin)) = assert;                       \
     REG32(EXT_TIMES_P2(pin)) = times;                         \
  }                                                            \
})

#define set_mult_dtN(N0,N1,IRQ_DTCT)                           \
({                                                             \
  int N;                                                       \
  for(N=N0;N<N1;N++){                                          \
     set_dt(N,IRQ_DTCT);                                       \
  }                                                            \
})

/*--------------------------------------------------*/
/*                    jz4760                        */
/*--------------------------------------------------*/  
//#include "itb_def.h"  
//#include "itb_memap.h"
// N = 0 1 2 3 4 5 6 7 8 (only 0 1 2 are used in jz4760)

#define get_itb_gpio_gpcfg(port) REG32(ITB_GPIO_GPCFG(port))
#define get_itb_gpio_gpdat(port) REG32(ITB_GPIO_GPDAT(port))
#define get_itb_gpio_gpssl(port) REG32(ITB_GPIO_GPSSL(port))
#define get_itb_gpio_gpssh(port) REG32(ITB_GPIO_GPSSH(port))

//---------------------------------------------------

#define get_itb_gpio_gpmsk(port)  REG32(ITB_GPIO_GPMSK(port))
#define get_itb_gpio_gpused(port) REG32(ITB_GPIO_GPUSED(port))
#define get_itb_gpio_gppuls(port) REG32(ITB_GPIO_GPPULS(port))
#define get_itb_gpio_gpsdrv(port) REG32(ITB_GPIO_GPSDRV(port))
#define get_itb_gpio_gppdrv(port) REG32(ITB_GPIO_GPPDRV(port))
#define get_itb_gpio_gpsdr(port)  REG32(ITB_GPIO_GPSDR(port))
#define get_itb_gpio_gppdr(port)  REG32(ITB_GPIO_GPPDR(port))

#define set_itb_gpio_gpmsk(port, value)        \
({                                             \
    REG32(ITB_GPIO_GPMSK(port)) = value;       \
})
#define set_itb_gpio_gpused(port, value)       \
({                                             \
    REG32(ITB_GPIO_GPUSED(port)) = value;      \
})
#define set_itb_gpio_gppuls(port, value)       \
({                                             \
    REG32(ITB_GPIO_GPPULS(port)) = value;      \
})
#define set_itb_gpio_gpsdrv(port, value)       \
({                                             \
    REG32(ITB_GPIO_GPSDRV(port)) = value;      \
})
#define set_itb_gpio_gppdrv(port, value)       \
({                                             \
    REG32(ITB_GPIO_GPPDRV(port)) = value;      \
})
#define set_itb_gpio_gpsdr(port, value)        \
({                                             \
    REG32(ITB_GPIO_GPSDR(port)) = value;       \
})
#define set_itb_gpio_gppdr(port, value)        \
({                                             \
    REG32(ITB_GPIO_GPPDR(port)) = value;       \
})

//ITB_GPIO_GPCFG(N)

#define set_itb_gpio_gpused_pin(port, pin, val)                \
  REG32(ITB_GPIO_GPUSED(port)) = (0x1 << pin);

#define set_itb_gpio_gpmsk_pin(port, pin, val)                 \
  REG32(ITB_GPIO_GPMSK(port)) =                                \
  REG32(ITB_GPIO_GPMSK(port)) & ~(0x1 << pin) | (val << pin)   \

#define set_itb_gpio_gppuls_pin(port, pin, val)                \
  REG32(ITB_GPIO_GPPULS(port)) = (0x1 << pin)                  \

#define set_itb_gpio_gpsdrv_pin(port, pin, val)                \
  REG32(ITB_GPIO_GPSDRV(port)) =                               \
  REG32(ITB_GPIO_GPSDRV(port)) & ~(0x1 << pin) | (val << pin)  \

#define set_itb_gpio_gppdrv_pin(port, pin, val)                \
  REG32(ITB_GPIO_GPPDRV(port)) =                               \
  REG32(ITB_GPIO_GPPDRV(port)) & ~(0x1 << pin) | (val << pin)  \

#define set_itb_gpio_gpsdr_pin(port, pin, val)                 \
  REG32(ITB_GPIO_GPSDR(port)) =                                \
  REG32(ITB_GPIO_GPSDR(port)) & ~(0x1 << pin) | (val << pin)   \

#define set_itb_gpio_gppdr_pin(port, pin, val)                 \
  REG32(ITB_GPIO_GPPDR(port)) =                                \
  REG32(ITB_GPIO_GPPDR(port)) & ~(0x1 << pin) | (val << pin)   \

#define get_itb_gpio_gpcfg_pin(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPCFG(port)) >> pin) & 0x00000001)

#define get_itb_gpio_gpdat_pin(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPDAT(port)) >> pin) & 0x00000001)

#define get_itb_gpio_gpssl_pin(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPSSL(port)) >> pin) & 0x00000001)

#define get_itb_gpio_gpssh_pin(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPSSH(port)) >> pin) & 0x00000001)

#define get_itb_gpio_gpssl_pin2l(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPSSL(port)) >> (pin * 2)) & 0x00000003)

#define get_itb_gpio_gpssh_pin2h(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPSSH(port)) >> (pin - 16) * 2) & 0x00000003)

//---------------------------------------------------

#define get_itb_gpio_gpmsk_pin(port,  pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPMSK(port))  >> pin) && 0x00000001)

#define get_itb_gpio_gpused_pin(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPUSED(port)) >> pin) && 0x00000001)

#define get_itb_gpio_gppuls_pin(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPPULS(port)) >> pin) && 0x00000001)

#define get_itb_gpio_gpsdrv_pin(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPSDRV(port)) >> pin) && 0x00000001)

#define get_itb_gpio_gppdrv_pin(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPPDRV(port)) >> pin) && 0x00000001)

#define get_itb_gpio_gpsdr_pin(port,  pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPSDR(port))  >> pin) && 0x00000001)

#define get_itb_gpio_gppdr_pin(port,  pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPPDR(port))  >> pin) && 0x00000001)

//---------------------------------------------------------------

#define set_gpio_reg_pin(gpreg, port, pin, val)       \
  i_s32((i_l32(gpreg##0 + 0x30 * port, 0x0) &         \
       ~(0x1 << pin) | (val << pin)), gpreg##0 + 0x30 * port, 0x0)

#define get_gpio_reg_pin(gpreg, port, pin)            \
  (unsigned int) ((i_l32(gpreg##0 + 0x30 * port, 0x0) >> pin) & 0x00000001)

  // only for GPFLR GPSLR
#define set_gpio_reg_pin2l(gpreg, port, pin, val)                         \
  i_s32((i_l32(gpreg##0 + 0x30 * port, 0x0) &                             \
       ~(0x3 << pin * 2) | (val << pin * 2)), gpreg##0 + 0x30 * port, 0x0)
  // only for GPFUR GPSUR
#define set_gpio_reg_pin2h(gpreg, port, pin, val)            \
  i_s32((i_l32(gpreg##0 + 0x30 * port, 0x0) &                \
       ~(0x3 << (pin - 16) * 2) |                            \
        (val << (pin - 16) * 2)), gpreg##0 + 0x30 * port, 0x0)

#define get_gpio_reg_pin2l(gpreg, port, pin)            \
  (unsigned int) ((i_l32(gpreg##0 + 0x30 * port, 0x0) >> pin * 2) & 0x00000003)

#define get_gpio_reg_pin2h(gpreg, port, pin)            \
  (unsigned int) ((i_l32(gpreg##0 + 0x30 * port, 0x0) >> (pin - 16) * 2) & 0x00000003)

#define check_gpio_reg_rw_bit(gpreg, port, pin, val)\
({                                                  \
  set_gpio_reg_pin(gpreg, port, pin, val);          \
  if(get_gpio_reg_pin(gpreg, port, pin)!=val)       \
      return 1;                                     \
})

#define check_gpio_reg_rw_bit2l(gpreg, port, pin, val)\
({                                                    \
  set_gpio_reg_pin2l(gpreg, port, pin, val);          \
  if(get_gpio_reg_pin2l(gpreg, port, pin)!=val)       \
      return 1;                                       \
})

#define check_gpio_reg_rw_bit2h(gpreg, port, pin, val)\
({                                                    \
  set_gpio_reg_pin2h(gpreg, port, pin, val);          \
  if(get_gpio_reg_pin2h(gpreg, port, pin)!=val)       \
      return 1;                                       \
})

//-------------------------------------------------------

#define get_itb_gpio_gppdly(port) REG32(ITB_GPIO_GPPDLY(port))

#define set_itb_gpio_gppdly(port, value)       \
({                                             \
    REG32(ITB_GPIO_GPPDLY(port)) = value;      \
})

#define set_itb_gpio_gppdly_pin(port, pin, val)                \
  REG32(ITB_GPIO_GPPDLY(port)) =                               \
  REG32(ITB_GPIO_GPPDLY(port)) & ~(0x1 << pin) | (val << pin)  \

#define get_itb_gpio_gppdly_pin(port, pin) \
  (unsigned int) ((REG32(ITB_GPIO_GPPDLY(port)) >> pin) && 0x00000001)

//--------------------------------------------------------

#define prepare_gpio_int_rise(port, pin)         \
({                                               \
   set_itb_gpio_gpused_pin(port, pin, 0x1);      \
   set_pim(port, pin);                           \
   clr_pfun(port, pin);                          \
   set_psel(port, pin);                          \
                                                 \
   set_ptrg(port, pin);                          \
   set_pdir(port, pin);                          \
                                                 \
   set_itb_gpio_gpsdr_pin(port, pin, 0x1);       \
   set_itb_gpio_gpsdrv_pin(port, pin, 0x0);      \
                                                 \
   set_pdat(port, pin);                          \
   clr_pim(port, pin);                           \
})

#define prepare_gpio_int_fall(port, pin)         \
({                                               \
   set_itb_gpio_gpused_pin(port, pin, 0x1);      \
   set_pim(port, pin);                           \
   clr_pfun(port, pin);                          \
   set_psel(port, pin);                          \
                                                 \
   set_ptrg(port, pin);                          \
   clr_pdir(port, pin);                          \
                                                 \
   set_itb_gpio_gpsdr_pin(port, pin, 0x1);       \
   set_itb_gpio_gpsdrv_pin(port, pin, 0x1);      \
                                                 \
   set_pdat(port, pin);                          \
   clr_pim(port, pin);                           \
})

#define gpio_int_start(port, pin, delay)         \
({                                               \
   if(delay == 0x1)                              \
       set_itb_gpio_gppdly_pin(port, pin, 0x1);  \
                                                 \
       set_itb_gpio_gppuls_pin(port, pin, 0x1);  \
})

#define TP_DATA1    (ADC_DRV_BASE + 0x20)
#define TP_DATA0    (ADC_DRV_BASE + 0x24)
#define TP_DATA2    (ADC_DRV_BASE + 0x28)

#define get_sadc_cmd0()  (REG32(TP_DATA1) & 0x00000001)
#define get_sadc_cmd1()  (REG32(TP_DATA1) & 0x00000002) >> 1
#define get_sadc_cmd2()  (REG32(TP_DATA1) & 0x00000004) >> 2
#define get_sadc_cmd3()  (REG32(TP_DATA1) & 0x00000008) >> 3
#define get_sadc_cclk()  (REG32(TP_DATA1) & 0x00000010) >> 4
#define get_sadc_rstn()  (REG32(TP_DATA1) & 0x00000020) >> 5
#define get_sadc_pwdn()  (REG32(TP_DATA1) & 0x00000040) >> 6
#define get_sadc_sten()  (REG32(TP_DATA1) & 0x00000080) >> 7
#define get_codc_mclk()  (REG32(TP_DATA1) & 0x00000100) >> 8
#define get_codc_bclk()  (REG32(TP_DATA1) & 0x00000200) >> 9
#define get_codc_lrck()  (REG32(TP_DATA1) & 0x00000400) >> 10
#define get_codc_ddat()  (REG32(TP_DATA1) & 0x00000800) >> 11
   
#define set_sadc_clko()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA2) = 0x00000001 << 12;\
})
   
#define clr_sadc_clko()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000001 << 12;\
})   

#define set_sadc_ipen()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA2) = 0x00000001 << 13;\
})
   
#define clr_sadc_ipen()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000001 << 13;\
})   

#define set_codc_adat()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA2) = 0x00000001 << 14;\
})
   
#define clr_codc_adat()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000001 << 14;\
})   

#define set_tuhc_trcv()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA2) = 0x00000001 << 15;\
})
   
#define clr_tuhc_trcv()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000001 << 15;\
})   

#define get_tuhc_trcv() (REG32(TP_UHC_ADDR) & TP_RCV_TEST) >> 8

#define set_tuhc_tpvp()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA2) = 0x00000001 << 16;\
})
   
#define clr_tuhc_tpvp()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000001 << 16;\
})   

#define get_tuhc_tpvp() (REG32(TP_UHC_ADDR) & TP_DVP_TEST) >> 6
   
#define set_tuhc_tpvm()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA2) = 0x00000001 << 17;\
})
   
#define clr_tuhc_tpvm()              \
({                                   \
  REG32(TP_DATA2) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000000;      \
  REG32(TP_DATA0) = 0x00000001 << 17;\
})   

#define get_tuhc_tpvm() (REG32(TP_UHC_ADDR) & TP_DVM_TEST) >> 7

#define get_tuhc_susp()  (REG32(TP_DATA1) & 0x00040000) >> 18
#define get_tuhc_spee()  (REG32(TP_DATA1) & 0x00080000) >> 19
#define get_tuhc_tvm0()  (REG32(TP_DATA1) & 0x00100000) >> 20
#define get_tuhc_tvp0()  (REG32(TP_DATA1) & 0x00200000) >> 21
#define get_tuhc_txen()  (REG32(TP_DATA1) & 0x00400000) >> 22
#define get_tuhc_txse()  (REG32(TP_DATA1) & 0x00800000) >> 23
   
#define TP_UHC_ADDR (CGU_BASE + 0x70)
#define TP_ALL_ADDR (CGU_BASE + 0x24)

#define TP_TEST_CLR 0x3fffffff
#define TP_ADC_TEST 0x80000000
#define TP_CDC_TEST 0x40000000
#define TP_SUS_TEST 0x00000080

#define TP_UHC_TEST 0x00000020
#define TP_RCV_TEST 0x00000100
#define TP_DVM_TEST 0x00000080
#define TP_DVP_TEST 0x00000040
#define TP_ENL_TEST 0x00000010
#define TP_SPE_TEST 0x00000008
#define TP_SE0_TEST 0x00000004
#define TP_VM0_TEST 0x00000002
#define TP_VP0_TEST 0x00000001

#define set_sadc_test_en() \
REG32(TP_ALL_ADDR) = REG32(TP_ALL_ADDR) & TP_TEST_CLR | TP_ADC_TEST

#define set_codc_test_en() \
REG32(TP_ALL_ADDR) = REG32(TP_ALL_ADDR) & TP_TEST_CLR | TP_CDC_TEST

#define clr_test_en() \
REG32(TP_ALL_ADDR) = REG32(TP_ALL_ADDR) & TP_TEST_CLR

#define set_uhc_test_en() \
REG32(TP_UHC_ADDR) = TP_UHC_TEST

#define clr_uhc_test_en() \
REG32(TP_UHC_ADDR) = 0x00000000

#define enb_gps_tb_start()  set_ppat0(2, 0)
#define dis_gps_tb_start()  clr_ppat0(2, 0)

#define enb_gps_tb_finish()  set_ppat0(2, 1)
#define dis_gps_tb_finish()  clr_ppat0(2, 1)

#define alter4gpstb()       ({set_pin_gpio_o(2, 0);  set_pin_gpio_o(2, 1); dis_gps_tb_start(); dis_gps_tb_finish();})// pc00

#endif /* __INGENIC_GPIO_H_ */

