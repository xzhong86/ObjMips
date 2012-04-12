
/* $Id: cpm.h,v 1.7 2012/03/27 07:05:11 yzhai Exp $ */
#ifndef __INGENIC_CPM_H__
#define __INGENIC_CPM_H__

#define CPCCR    (CGU_BASE + 0x00)
#define LCR      (CGU_BASE + 0x04)
#define CPPCR    (CGU_BASE + 0x0C)
#define CPAPCR   (CGU_BASE + 0x10)
#define CPMPCR   (CGU_BASE + 0x14)
#define OPCR     (CGU_BASE + 0x24)
#define DDRCDR   (CGU_BASE + 0x2C)
#define VPUCDR   (CGU_BASE + 0x30)
#define MSC0CDR  (CGU_BASE + 0x68)
#define MSC1CDR  (CGU_BASE + 0xA4)
#define MSC2CDR  (CGU_BASE + 0xA8)
#define CPCSR    (CGU_BASE + 0xD4)
#define HDMICDR  (CGU_BASE + 0x8c)

// CPCCR

#define CPCCR_SEL_SRC    30
#define CPCCR_SEL_CPLL   28
#define CPCCR_SEL_H0PLL  26
#define CPCCR_SEL_H2PLL  24
#define CPCCR_CE_AHB2    22
#define CPCCR_CE_AHB0    21
#define CPCCR_CE_CPU     20
#define CPCCR_PDIV       16
#define CPCCR_H2DIV      12
#define CPCCR_H0DIV      8
#define CPCCR_L2CDIV     4
#define CPCCR_CDIV       0

// LCR

#define LCR_PD_SCPU      31
#define LCR_SCPUS        27
#define LCR_LPM          0

#define LCR_PD_SCPU_M    (1 << LCR_PD_SCPU)
#define LCR_SCPUS_M      (1 << LCR_SCPUS)
#define LCR_LPM_M        (0x3 << LCR_LPM)

#define LCR_LPM_ML       (~LCR_LPM_M)

// CPPCR

#define CPPCR_PLLST      0

#define CPPCR_PLLST_ML   (0xFFFFFF00)

// CPAPCR

#define CPAPCR_APLLM     19
#define CPAPCR_APLLN     13
#define CPAPCR_APLLOD    9
#define CPAPCR_APLLON    4
#define CPAPCR_AP_MODE   3
#define CPAPCR_APLLEN    0

#define CPAPCR_APLLM_M   (0x1FFF << CPAPCR_APLLM)
#define CPAPCR_APLLN_M   (0x3F << CPAPCR_APLLN)
#define CPAPCR_APLLOD_M  (0xF << CPAPCR_APLLOD)
#define CPAPCR_APLLON_M  (1 << CPAPCR_APLLON)
#define CPAPCR_AP_MODE_M (1 << CPAPCR_AP_MODE)

#define CPAPCR_APLLM_ML  (~CPAPCR_APLLM_M)
#define CPAPCR_APLLN_ML  (~CPAPCR_APLLN_M)
#define CPAPCR_APLLOD_ML (~CPAPCR_APLLOD_M)

// CPMPCR

#define CPMPCR_MPLLM     19
#define CPMPCR_MPLLN     13
#define CPMPCR_MPLLON    4
#define CPMPCR_MPLLEN    0

#define CPMPCR_MPLLON_M (1 << CPMPCR_MPLLON)

// OPCR

#define OPCR_L2CM_PD     26
#define OPCR_P0_PD       3
#define OPCR_ERCS        2

#define OPCR_L2CM_PD_M   (3 << OPCR_L2CM_PD)
#define OPCR_P0_PD_M     (1 << OPCR_P0_PD)
#define OPCR_ERCS_M      (1 << OPCR_ERCS)

#define OPCR_L2CM_PD_ML  (~OPCR_L2CM_PD_M)

// DDRCDR

#define DDRCDR_DCS       30
#define DDRCDR_CE_DDR    29
#define DDRCDR_DDRCDR    0

// MSC0CDR

#define MSC0CDR_MPCS     30
#define MSC0CDR_CE_MSC0  29
#define MSC0CDR_BUSY     28
#define MSC0CDR_CDR      0

#define MSC0CDR_BUSY_MH     (1 << MSC0CDR_BUSY)

#define MSC0CDR_MPCS_ML     0x3FFFFFFF
#define MSC0CDR_CE_MSC0_ML  0xDFFFFFFF
#define MSC0CDR_CDR_ML      0xFFFFFF00

// MSC1CDR

#define MSC1CDR_CE_MSC1  29
#define MSC1CDR_BUSY     28
#define MSC1CDR_CDR      0

#define MSC1CDR_BUSY_MH    (1 << MSC1CDR_BUSY)

#define MSC1CDR_CE_MSC1_ML  0xDFFFFFFF
#define MSC1CDR_CDR_ML      0xFFFFFF00

// MSC2CDR

#define MSC2CDR_CE_MSC2  29
#define MSC2CDR_BUSY     28
#define MSC2CDR_CDR      0

#define MSC2CDR_BUSY_MH    (1 << MSC2CDR_BUSY)

#define MSC2CDR_CE_MSC2_ML  0xDFFFFFFF
#define MSC2CDR_CDR_ML      0xFFFFFF00

/////////////////////////////////////////////////////////////////

#define DIV_TYPE 6

#define CPPSR    (CGU_BASE + 0x14)
#define I2SCDR   (CGU_BASE + 0x60)
#define LPCDR    (CGU_BASE + 0x64)

#define MSC0CDR  (CGU_BASE + 0x68)
#define MSC1CDR  (CGU_BASE + 0xA4)
#define MSC2CDR  (CGU_BASE + 0xA8)

#if ((defined MSC0) || (defined MSC00))
#define MSCCDR MSC0CDR
#endif

#if ((defined MSC1) || (defined MSC11))
#define MSCCDR MSC1CDR
#endif

#if ((defined MSC2) || (defined MSC22))
#define MSCCDR MSC2CDR
#endif

#define MSCCDR_MPCS_A  30
#define MSCCDR_CE_A    29
#define MSCCDR_BUSY_A  28
#define MSCCDR_STOP_A  27
#define MSCCDR_SSEL_A  15
#define MSCCDR_DIV_A   0

#define MSCCDR_MPCS  (3 << 30)
#define MSCCDR_CE    (1 << 29)
#define MSCCDR_BUSY  (1 << 28)
#define MSCCDR_STOP  (1 << 27)
#define MSCCDR_SSEL  (1 << 15)
#define MSCCDR_DIV   (0xFF)

#define UHCCDR   (CGU_BASE + 0x6C)
#define SSICDR   (CGU_BASE + 0x74)

#define CIMCDR   (CGU_BASE + 0x7c)
#define GPSCDR   (CGU_BASE + 0x80)
#define PCMCDR   (CGU_BASE + 0x84)
#define GPUCDR   (CGU_BASE + 0x88)
#define BCHCDR   (CGU_BASE + 0xac)

#define CFCR     CPCCR
#define CPPCR1   CPPCR
#define PLCR     CPPCR
#define PLCR1    CPPCR
#define PLSR     CPPSR
#define CFCR2    I2SCDR

#define CPPSR_MASK  0x3c000007
#define I2SCDR_MASK 0x000001ff
#define LPCDR_MASK  0x000007ff
#define MSCCDR_MASK 0x0000001f
#define SSICDR_MASK 0x0000000f

#define CFCR_POWER_ON_VALUE        0x00000000
#define PLCR1_POWER_ON_VALUE       0x00000011
#define OCR_POWER_ON_VALUE         0x00150015

// ------------------------------ CPCCR
#define CPCCR_I2CS             0x80000000
#define CPCCR_ECS              0x40000000
#define CPCCR_UCS              0x20000000
#define CPCCR_UDIV             0x1F800000
#define CPCCR_CE               0x00400000
#define CPCCR_PCS              0x00200000
#define CPCCR_H1DIV            0x000F0000
#define CPCCR_MDIV             0x0000F000
//#define CPCCR_PDIV             0x00000F00
//#define CPCCR_H0DIV            0x000000F0
//#define CPCCR_CDIV             0x0000000F

#define CPCCR_I2CS_S           31
#define CPCCR_ECS_S            30
#define CPCCR_UCS_S            29
#define CPCCR_UDIV_S           23
#define CPCCR_PCS_S            21

#define CPCCR_DIV_MASK         (CPCCR_H1DIV | CPCCR_MDIV | CPCCR_PDIV | CPCCR_H0DIV | CPCCR_CDIV)
#define CPCCR_DIV2_MASK        (CPCCR_DIV_MASK | CPCCR_UDIV)

#define CFCR_SSI               CPCCR_I2CS
#define CFCR_UCS               CPCCR_UCS
#define CFCR_UFR               CPCCR_UDIV
#define CFCR_UPE               CPCCR_CE
#define CFCR_S1FR              CPCCR_H1DIV
#define CFCR_MFR               CPCCR_MDIV
#define CFCR_PFR               CPCCR_PDIV
#define CFCR_SFR               CPCCR_H0DIV
#define CFCR_IFR               CPCCR_CDIV

#define I2SCDR_DIV_MASK        0x000001ff
#define CFCR2_I2SFR            0x000001ff
#define CFCR2_I2SFR_INV        0xfffffe00
#define CFCR2_I2SFR_SHIFT      0

#define CFCR_MD_S              21
#define CFCR_UFR_S             23
#define CFCR_LFR_S             16
#define CFCR_MFR_S             12
#define CFCR_PFR_S             8
#define CFCR_SFR_S             4
#define CPCCR_CDIV_S           0

#define CPCCR_TEST             (CPCCR_I2CS | CPCCR_UCS | CPCCR_UDIV | CPCCR_CE | CPCCR_PCS)

// -------------------------------CPPCR

#define CPPCR_PLLFD            0xff800000
#define CPPCR_PLLRD            0x007c0000
#define CPPCR_PLLOD            0x00030000
#define CPPCR_PLLS             0x00000400
#define CPPCR_PLLBP            0x00000200
#define CPPCR_PLLEN            0x00000100

#define PLCR_PLLFD CPPCR_PLLFD
#define PLCR_PLLRD CPPCR_PLLRD
#define PLCR_PLLOD CPPCR_PLLOD
#define PLCR_PLLS  CPPCR_PLLS
#define PLCR_PLLBP CPPCR_PLLBP
#define PLCR_PLLEN CPPCR_PLLEN
#define PLCR_PLLST CPPCR_PLLST

#define CPPCR_PLL_MASK         (CPPCR_PLLFD | CPPCR_PLLRD | CPPCR_PLLOD)

#define CPPCR_PLLFD_S      23
#define CPPCR_PLLRD_S      18
#define CPPCR_PLLOD_S      16
#define CPPCR_PLLST_S      0

#define PLCR_PLLFD_SHIFT   CPPCR_PLLFD_S   
#define PLCR_PLLRD_SHIFT   CPPCR_PLLRD_S   
#define PLCR_PLLOD_SHIFT   CPPCR_PLLOD_S   
#define PLCR_PLLST_SHIFT   CPPCR_PLLST_S   

// ---- CPPSR

#define CPPSR_PLLOFF           0x80000000
#define CPPSR_PLLBP            0x40000000
#define CPPSR_PLLON            0x20000000
#define CPPSR_PLLPS            0x10000000
#define CPPSR_PLLFS            0x08000000
#define CPPSR_PLLCS            0x04000000

#define CPPSR_PLLOFF_S        31
#define CPPSR_PLLBP_S         30
#define CPPSR_PLLON_S         29
#define CPPSR_PLLPS_S         28
#define CPPSR_PLLFS_S         27
#define CPPSR_PLLCS_S         26

#define LPCDR_PXFR             0x000001ff
#define LPCDR_PXFR_S       0

#define CGU_XFR_1              0
#define CGU_XFR_2              1
#define CGU_XFR_3              2
#define CGU_XFR_4              3
#define CGU_XFR_6              4
#define CGU_XFR_8              5
#define CGU_XFR_12             6
#define CGU_XFR_16             7
#define CGU_XFR_24             8
#define CGU_XFR_32             9

#define OCR_O1ST               0x00ff0000
#define OCR_O2E                0x00000100
#define OCR_SUSPEND            0x000000c0

#define OCR_O1ST_S         16
#define OCR_O2E_S          8

#ifndef SMIC_PLL
#define PLLFD_MASK             0x1ff
#define PLLOD_MASK             0x3
#else // SMIC_PLL
#define PLLFD_MASK             0x0ff
#define PLLOD_MASK             0x1
#endif

/* 
   h1clk, mclk, pclk, h0clk, cclk
   cclk:h1clk != 2:3
   h1clk:h0clk == 2:3 or 1:1
*/

#define div1                     0
#define div2                     1
#define div3                     2
#define div4                     3
#define div6                     4
#define div8                     5

#define CPCCR_DIV_CFG(h1, m, p, h0, c)                                   \
  ((div##h1<<16) | (div##m<<12) | (div##p<<8) | (div##h0<<4) | (div##c))

#define CPCCR_DIV_11111    CPCCR_DIV_CFG(1, 1, 1, 1, 1) 
#define CPCCR_DIV_22221    CPCCR_DIV_CFG(2, 2, 2, 2, 1)
#define CPCCR_DIV_22421    CPCCR_DIV_CFG(2, 2, 4, 2, 1)
#define CPCCR_DIV_22621    CPCCR_DIV_CFG(2, 2, 6, 2, 1)
#define CPCCR_DIV_22821    CPCCR_DIV_CFG(2, 2, 8, 2, 1)
#define CPCCR_DIV_24421    CPCCR_DIV_CFG(2, 4, 4, 2, 1)
#define CPCCR_DIV_24821    CPCCR_DIV_CFG(2, 4, 8, 2, 1)
#define CPCCR_DIV_26621    CPCCR_DIV_CFG(2, 6, 6, 2, 1)
#define CPCCR_DIV_28821    CPCCR_DIV_CFG(2, 8, 8, 2, 1)
#define CPCCR_DIV_23331    CPCCR_DIV_CFG(2, 3, 3, 3, 1)
#define CPCCR_DIV_23631    CPCCR_DIV_CFG(2, 3, 6, 3, 1)
#define CPCCR_DIV_26631    CPCCR_DIV_CFG(2, 6, 6, 3, 1)
#define CPCCR_DIV_33331    CPCCR_DIV_CFG(3, 3, 3, 3, 1)
#define CPCCR_DIV_33631    CPCCR_DIV_CFG(3, 3, 6, 3, 1)
#define CPCCR_DIV_36631    CPCCR_DIV_CFG(3, 6, 6, 3, 1)
#define CPCCR_DIV_44441    CPCCR_DIV_CFG(4, 4, 4, 4, 1)
#define CPCCR_DIV_44841    CPCCR_DIV_CFG(4, 4, 8, 4, 1)
#define CPCCR_DIV_48841    CPCCR_DIV_CFG(4, 8, 8, 4, 1)
#define CPCCR_DIV_66661    CPCCR_DIV_CFG(6, 6, 6, 6, 1)
#define CPCCR_DIV_88881    CPCCR_DIV_CFG(8, 8, 8, 8, 1)
                                                
#define CPCCR_DIV_22222    CPCCR_DIV_CFG(2, 2, 2, 2, 2)
#define CPCCR_DIV_22422    CPCCR_DIV_CFG(2, 2, 4, 2, 2)
#define CPCCR_DIV_22622    CPCCR_DIV_CFG(2, 2, 6, 2, 2)
#define CPCCR_DIV_22822    CPCCR_DIV_CFG(2, 2, 8, 2, 2)
#define CPCCR_DIV_24422    CPCCR_DIV_CFG(2, 4, 4, 2, 2)
#define CPCCR_DIV_24822    CPCCR_DIV_CFG(2, 4, 8, 2, 2)
#define CPCCR_DIV_26622    CPCCR_DIV_CFG(2, 6, 6, 2, 2)
#define CPCCR_DIV_28822    CPCCR_DIV_CFG(2, 8, 8, 2, 2)
#define CPCCR_DIV_44442    CPCCR_DIV_CFG(4, 4, 4, 4, 2)
#define CPCCR_DIV_44842    CPCCR_DIV_CFG(4, 4, 8, 4, 2)
#define CPCCR_DIV_48842    CPCCR_DIV_CFG(4, 8, 8, 4, 2)
#define CPCCR_DIV_66662    CPCCR_DIV_CFG(6, 6, 6, 6, 2)
#define CPCCR_DIV_88882    CPCCR_DIV_CFG(8, 8, 8, 8, 2)
                                                
#define CPCCR_DIV_33333    CPCCR_DIV_CFG(3, 3, 3, 3, 3)
#define CPCCR_DIV_33633    CPCCR_DIV_CFG(3, 3, 6, 3, 3)
#define CPCCR_DIV_36633    CPCCR_DIV_CFG(3, 6, 6, 3, 3)
#define CPCCR_DIV_66663    CPCCR_DIV_CFG(6, 6, 6, 6, 3)
                                                
#define CPCCR_DIV_44444    CPCCR_DIV_CFG(4, 4, 4, 4, 4)
#define CPCCR_DIV_44844    CPCCR_DIV_CFG(4, 4, 8, 4, 4)
#define CPCCR_DIV_48844    CPCCR_DIV_CFG(4, 8, 8, 4, 4)
#define CPCCR_DIV_88884    CPCCR_DIV_CFG(8, 8, 8, 8, 4)
                                                
#define CPCCR_DIV_66666    CPCCR_DIV_CFG(6, 6, 6, 6, 6)

#define CPCCR_DIV_88888    CPCCR_DIV_CFG(8, 8, 8, 8, 8)

#ifndef CFCR_VAL
#define CFCR_VAL 0
#endif

// define common macros

#define FREQ_I2C_RTC       3   // rtcclk=1.2288MHz, i2c_dev_clk=3.6864MHz
#define FREQ_I2C_RTC2_1    9   // rtcclk=1.6384MHz, i2c_dev_clk=3.6864MHz
#define FREQ_I2C_RTC2_2    4   
#define FREQ_UDC_TIMES     4
#define FREQ_UDC_TIMES2    13
#define FREQ_FIR_TIMES     4
#define FREQ_MMC_TIMES     12
#define FREQ_SD_TIMES      8
#define FREQ_UART_TIMES    1
#define FREQ_I2S_TIMES     1
#define FREQ_PCM_TIMES     1

#ifdef SIM_FPGA
#define CGU_CNT  0x2f0
#else
#define CGU_CNT  0x2f
#endif

#ifdef PLL_SMIC_T500
#define FD_OFFSET 2
#define RD_OFFSET 2
#endif

#define set_i2scdr_value(value)					 \
  ({								 \
    REG32(I2SCDR) = value;					 \
  })
#define get_i2scdr_value(value)	(REG32(I2SCDR))

#define get_pll_time()         PLL_TIME(1)
#define PLL_TIME(N)					\
({							\
   int _plcr_, fd, rd, od1, od2, od; \
   _plcr_ = REG32(CPPCR##N);			\
   fd  = ((_plcr_ & CPPCR_PLLFD) >> CPPCR_PLLFD_S) + FD_OFFSET;\
   rd  = ((_plcr_ & CPPCR_PLLRD) >> CPPCR_PLLRD_S) + RD_OFFSET;\
   od1 = ((_plcr_ & 0x10000)     >> 16);	          \
   od2 = ((_plcr_ & 0x20000)     >> 17);	          \
   od  = 1 << (od1 + od2);\
   (fd / rd / od);\
})

#define EXTAL_FREQ   3.6864
#define RTCCLK_FREQ  0.0288
#define ICLK_FREQ    (EXTAL_FREQ * PLL_TIME(1))
#define SCLK_FREQ    (EXTAL_FREQ * PLL_TIME(1) / (_XFR_[get_sfr()]))
#define PCLK_FREQ    (EXTAL_FREQ * PLL_TIME(1) / (_XFR_[get_pfr()]))

#define get_xfr_index(val)			\
({						\
  int __i;					\
  for (__i = 0; __i < 10; __i++)		\
    if (_XFR_[__i] == val)			\
      break;					\
  (__i);					\
})

#define set_ifr(val)     (get_xfr_index(val) << CPCCR_CDIV_S)
#define set_sfr(val)     (get_xfr_index(val) << CFCR_SFR_S)
#define set_pfr(val)     (get_xfr_index(val) << CFCR_PFR_S)
#define set_mfr(val)     (get_xfr_index(val) << CFCR_MFR_S)
#define set_lfr(val)     ((val - 1) << CFCR_LFR_S)
#define set_ufr(val)     ((val - 1) << CFCR_UFR_S)
#define get_ifr()        ((REG32(CPCCR) & CPCCR_CDIV) >> CPCCR_CDIV_S)
#define get_sfr()        ((REG32(CPCCR) & CFCR_SFR) >> CFCR_SFR_S)
#define get_pfr()        ((REG32(CPCCR) & CFCR_PFR) >> CFCR_PFR_S)
#define get_mfr()        ((REG32(CPCCR) & CFCR_MFR) >> CFCR_MFR_S)
#define get_lfr()        ((REG32(CPCCR) & CFCR_LFR) >> CFCR_LFR_S)
#define get_ufr()        ((REG32(CPCCR) & CFCR_UFR) >> CFCR_UFR_S)

#define CPCCR_ECS_HALF 1
#define CPCCR_PCS_HALF 0
#define CPCCR_I2CS_EXT 0
#define CPCCR_UCS_EXT  0

#define set_cpccr_ucs_pll()  {REG32(CPCCR) = REG32(CPCCR) |  CPCCR_UCS | CPCCR_CE;}
#define set_cpccr_ucs_ext()  {REG32(CPCCR) = REG32(CPCCR) & ~CPCCR_UCS | CPCCR_CE;}

#define set_cpccr_pcs_half() {REG32(CPCCR) = REG32(CPCCR) & ~CPCCR_PCS | CPCCR_CE;}
#define set_cpccr_pcs_pll()  {REG32(CPCCR) = REG32(CPCCR) |  CPCCR_PCS | CPCCR_CE;}

#define get_cpccr_i2cs()     ((get_cpm_cpccr() & CPCCR_I2CS) >> CPCCR_I2CS_S)
#define get_cpccr_ucs()      ((get_cpm_cpccr() & CPCCR_UCS ) >> CPCCR_UCS_S )
#define get_cpccr_ecs()      ((get_cpm_cpccr() & CPCCR_ECS ) >> CPCCR_ECS_S )
#define get_cpccr_pcs()      ((get_cpm_cpccr() & CPCCR_PCS ) >> CPCCR_PCS_S )

#define set_pllfd(val)   (val << CPPCR_PLLFD_S)
#define set_pllrd(val)   (val << CPPCR_PLLRD_S)
#define set_pllod(val)   (val << CPPCR_PLLOD_S)
#define set_pllst(val)   (val << CPPCR_PLLST_S)
#define get_pllfd()	 ((REG32(PLCR1) & CPPCR_PLLFD) >> CPPCR_PLLFD_S)
#define get_pllrd()	 ((REG32(PLCR1) & CPPCR_PLLRD) >> CPPCR_PLLRD_S)
#define get_pllod()	 ((REG32(PLCR1) & CPPCR_PLLOD) >> CPPCR_PLLOD_S)
#define get_plls(N)	 (REG32(PLCR##N) & CPPCR_PLLS)

#define wait_pll_stable(N)  while (! get_plls(N))

#define set_bypass(N)    {REG32(CPPCR##N) |=  CPPCR_PLLBP;}
#define clr_bypass(N)    {REG32(CPPCR##N) &= ~CPPCR_PLLBP;}

#define dis_div_ce()     {REG32(CPCCR) &= ~CPCCR_CE;}
#define enb_div_ce()     {REG32(CPCCR) |=  CPCCR_CE;}

#define clear_cfcr_upe(old_cfcr)                        \
              (old_cfcr & ~CFCR_UPE)

#define clear_plcr(old_plcr)                            \
              (old_plcr & ~(CPPCR_PLLFD | CPPCR_PLLRD | CPPCR_PLLOD) & ~CPPCR_PLLEN)

#define clear_plcr_fd(old_plcr)                            \
              (old_plcr & ~CPPCR_PLLFD)

#define clear_plcr_rd(old_plcr)                            \
              (old_plcr & ~CPPCR_PLLRD)

#define clear_plcr_od(old_plcr)                            \
              (old_plcr & ~CPPCR_PLLOD)

#define start_all_clk_monitor()								\
              (START_CLK_MONITOR(CGU_ICLK_MON   | CGU_SCLK_MON | CGU_PCLK_MON		\
                                 | CGU_MCLK_MON | CGU_LCLK_MON))

#define stop_all_clk_monitor()								\
              (STOP_CLK_MONITOR(CGU_ICLK_MON   | CGU_SCLK_MON | CGU_PCLK_MON		\
                                | CGU_MCLK_MON | CGU_LCLK_MON))

#define set_ifr_XFR(N)    (CGU_XFR_##N << CPCCR_CDIV_S)
#define set_sfr_XFR(N)    (CGU_XFR_##N << CFCR_SFR_S)
#define set_pfr_XFR(N)    (CGU_XFR_##N << CFCR_PFR_S)
#define set_mfr_XFR(N)    (CGU_XFR_##N << CFCR_MFR_S)
#define set_lfr_XFR(N)    (CGU_XFR_##N << CFCR_LFR_S)

#define set_cfcr_value_XFR(new_cfcr, NI, NS, NP, NM, NL)    \
              (new_cfcr                                 \
               | set_ifr_XFR(NI) | set_sfr_XFR(NS)      \
	       | set_pfr_XFR(NP) | set_mfr_XFR(NM) \
               | set_lfr_XFR(NL))

#define set_cfcr_value(new_cfcr, ifr, sfr, pfr, mfr, lfr)    \
              (new_cfcr | set_ifr(ifr) | set_sfr(sfr) | set_pfr(pfr) | set_mfr(mfr) | set_lfr(lfr))

#define set_cfcr_value2(new_cfcr, ifr, sfr, pfr, mfr, lfr, ufr)    \
              (new_cfcr | set_ifr(ifr) | set_sfr(sfr) | set_pfr(pfr) | set_mfr(mfr) | set_lfr(lfr) | set_ufr(ufr))

#define set_cfcr_upe(cfcr)       (cfcr | CFCR_UPE)

#define check_scnt(i, s, p, m, l)     \
           (((i > 0 && GET_ICLK_SCNT >= i) || (i == 0 && GET_ICLK_SCNT == i))\
            && ((s > 0 && GET_SCLK_SCNT >= s) || (s == 0 && GET_SCLK_SCNT == s))\
            && ((p > 0 && GET_PCLK_SCNT >= p) || (p == 0 && GET_PCLK_SCNT == p))\
            && ((m > 0 && GET_MCLK_SCNT >= m) || (m == 0 && GET_MCLK_SCNT == m))\
            && ((l > 0 && GET_LCLK_SCNT >= l) || (l == 0 && GET_LCLK_SCNT == l)))

#define set_modules_stop()  {REG32(CGR) = 0xFFFFFFFF;}
#define clr_modules_stop()  {REG32(CGR) = 0x00000000;}

#define enable_pll_all()    enable_pll(1)
#define disable_pll_all()   disable_pll(1)

#define set_pllcr_bp()  {REG32(CPPCR) = REG32(CPPCR) |  CPPCR_PLLBP;}
#define clr_pllcr_bp()  {REG32(CPPCR) = REG32(CPPCR) & ~CPPCR_PLLBP;}

#define set_pll_enb()   {REG32(CPPCR) = REG32(CPPCR) |  CPPCR_PLLEN;}
#define set_pll_dis()   {REG32(CPPCR) = REG32(CPPCR) & ~CPPCR_PLLEN;}

#define get_pll_off()   ((REG32(CPPSR) & CPPSR_PLLOFF) >> CPPSR_PLLOFF_S)
#define get_pll_bp()    ((REG32(CPPSR) & CPPSR_PLLBP)  >> CPPSR_PLLBP_S)
#define get_pll_on()    ((REG32(CPPSR) & CPPSR_PLLON)  >> CPPSR_PLLON_S)
#define get_pll_ps()    ((REG32(CPPSR) & CPPSR_PLLPS)  >> CPPSR_PLLPS_S)
#define get_pll_fs()    ((REG32(CPPSR) & CPPSR_PLLFS)  >> CPPSR_PLLFS_S)
#define get_pll_cs()    ((REG32(CPPSR) & CPPSR_PLLCS)  >> CPPSR_PLLCS_S)

#define clr_pll_off()   {REG32(CPPSR) = REG32(CPPSR) & ~CPPSR_PLLOFF;}
#define clr_pll_bp()    {REG32(CPPSR) = REG32(CPPSR) & ~CPPSR_PLLBP;}
#define clr_pll_on()    {REG32(CPPSR) = REG32(CPPSR) & ~CPPSR_PLLO;}
#define clr_pll_ps()    {REG32(CPPSR) = REG32(CPPSR) & ~CPPSR_PLLPS;}
#define clr_pll_fs()    {REG32(CPPSR) = REG32(CPPSR) & ~CPPSR_PLLFS;}
#define clr_pll_cs()    {REG32(CPPSR) = REG32(CPPSR) & ~CPPSR_PLLCS;}

#define cpm_pll_enb()                           \
({                                              \
  REG32(CPPCR) |= CPPCR_PLLEN;		        \
  wait_pll_stable(1);				\
})

#define enable_pll(N)				\
({						\
  REG32(CPPCR##N) |= CPPCR_PLLEN;		\
  wait_pll_stable(N);				\
})

#define disable_pll(N)				\
({						\
  REG32(CPPCR##N) &= ~CPPCR_PLLEN;		\
})

#define cgu_set_PLCR1(val)			\
({						\
  unsigned int plcr = REG32(PLCR1);		\
  REG32(PLCR1) = val;				\
  wait_pll_stable(1);				\
})

#define cgu_set_PLCR_N(N, val)			\
({						\
  unsigned int plcr = REG32(PLCR##N);		\
  REG32(PLCR##N) = val;				\
  wait_pll_stable(N);				\
})

#define get_cpm_cpccr()    (REG32(CPCCR))
#define set_cpm_cpccr(val) {REG32(CPCCR) = val;}
#define set_cpm_div(val)   {REG32(CPCCR) = get_cpm_cpccr() & ~CPCCR_DIV_MASK | (val);}

#define get_cpm_i2scdr()       (REG32(I2SCDR))
#define set_cpm_i2scdr(val)    {REG32(I2SCDR) = val;}
#define set_cpm_i2s_div(val)   {REG32(I2SCDR) = get_cpm_i2scdr() & ~I2SCDR_DIV_MASK | (val);}

#define get_cpm_cppcr()         (REG32(CPPCR))
#define set_cpm_cppcr(val)      {REG32(CPPCR) = val;}
#define set_cpm_pll(fd, rd, od)                                  \
({                                                               \
  unsigned int new_fd, new_rd, new_od, new_pll;                  \
  new_fd = fd << CPPCR_PLLFD_S;                              \
  new_rd = rd << CPPCR_PLLRD_S;                              \
  new_od = od << CPPCR_PLLOD_S;                              \
  new_pll = new_fd | new_rd | new_od;                            \
  REG32(CPPCR) = get_cpm_cppcr() & ~CPPCR_PLL_MASK | new_pll;    \
})
#define set_pll_value(fd, rd, od) set_cpm_pll(fd, rd, od)

#define clear_cfcr_xfr(cfcr)  (cfcr & ~CPCCR_DIV_MASK)              
#define clear_cfcr_xfr2(cfcr) (cfcr & ~CPCCR_DIV2_MASK)
#define set_cpccr_div(val)    set_cpm_div(val)    
#define cgu_set_cpccr(val)    set_cpm_cpccr(val)
#define cgu_set_CFCR(val)     set_cpm_cpccr(val)
#define cgu_set_CPCCR_PCS()   {REG32(CPCCR) |= CPCCR_PCS;}

/* #define cgu_set_LPCDR(val)			\ */
/* ({						\ */
/*   unsigned int lpcdr;				\ */
/*   lpcdr = REG32(LPCDR);				\ */
/*   printx(0x3333);                               \ */
/*   printx(lpcdr & LPCDR_PXFR);                   \ */
/*   printx(val & LPCDR_PXFR);                     \ */
/*   if (((val) & LPCDR_PXFR) != (lpcdr & LPCDR_PXFR)){\ */
/*     printx(0x4444);                             \ */
/*     set_module_stop(LCD);			\ */
/*   }                                             \ */
/*   REG32(LPCDR) = val;				\ */
/*   printx(REG32(LPCDR));                         \ */
/*   printx(0x3333);                               \ */
/*   if (((val) & LPCDR_PXFR) != (lpcdr & LPCDR_PXFR)){\ */
/*     printx(0x5555);                             \ */
/*     clr_module_stop(LCD);			\ */
/*   }                                             \ */
/* }) */
#define cgu_set_LPCDR(val)			\
({						\
  unsigned int lpcdr;				\
  int volatile busy;                            \
  lpcdr = REG32(LPCDR);				\
  printx(0x3333);                               \
  REG32(LPCDR) = val & 0xff | 1 << 28 | lpcdr & 0xefffff00;\
  lpcdr = REG32(LPCDR);				\
  busy = (lpcdr & 0x08000000) >> 27;            \
  while(busy) {                                 \
     lpcdr = REG32(LPCDR);       		\
     busy = (lpcdr & 0x08000000) >> 27;         \
  }                                             \
  REG32(LPCDR) = lpcdr & 0x3fffffff | 0x40000000;\
})


#define cgu_set_HDMICDR(val)			\
({						\
  unsigned int hdmicdr;				\
  int volatile busy;                            \
  hdmicdr = REG32(HDMICDR);			\
  printx(0x4444);                               \
  REG32(HDMICDR) = val & 0xff | 1 << 29 | hdmicdr & 0xefffff00;\
  hdmicdr = REG32(HDMICDR);			\
  busy = (hdmicdr & 0x08000000) >> 27;          \
  while(busy) {                                 \
     hdmicdr = REG32(HDMICDR);       		\
     busy = (hdmicdr & 0x08000000) >> 27;       \
  }                                             \
  REG32(HDMICDR) = hdmicdr & 0x3fffffff | 0x40000000;\
})

#define cgu_set_clocks(val)			\
({						\
  unsigned int __cfcr;				\
  __cfcr = REG32(CPCCR);				\
  __cfcr = clear_cfcr_xfr(__cfcr);		\
  cgu_set_CFCR(__cfcr | val);			\
})

#define set_cpm_pix_div1(val)                       \
({                                              \
  set_module_stop(LCD);				\
  REG32(LPCDR) = val;			        \
  REG32(CPCCR) = REG32(CPCCR) & ~CPCCR_PCS;   	\
  clr_module_stop(LCD);				\
})

#define set_cpm_pix_div2(val)                       \
({                                              \
  set_module_stop(LCD);				\
  REG32(LPCDR) = val;			        \
  REG32(CPCCR) = REG32(CPCCR) | 0x00600000;   	\
  clr_module_stop(LCD);				\
})

#define set_cfcr_tve_pix(val)                   \
({                                              \
  set_module_stop(TVE);				\
  REG32(LPCDR) = val | 0x80000000;	        \
  REG32(CPCCR) = REG32(CPCCR) & 0xbfffffff;   	\
  clr_module_stop(TVE);				\
})
#define set_cfcr_tve_pix_2(val)                 \
({                                              \
  set_module_stop(TVE);				\
  REG32(LPCDR) = val | 0x20000000;	        \
  REG32(CPCCR) = REG32(CPCCR) | 0x00600000;   	\
  clr_module_stop(TVE);				\
})

#define set_cfcr_uhc(val)                       \
({                                              \
  set_module_stop(UHC);				\
  REG32(UHCCDR) = val;			        \
  clr_module_stop(UHC);				\
})
#define set_cfcr_uhc_2(val)                     \
({                                              \
  set_module_stop(UHC);				\
  REG32(UHCCDR) = val;			        \
  REG32(CPCCR) = REG32(CPCCR) | 0x00600000;   	\
  clr_module_stop(UHC);				\
})

#define set_cfcr_ucs_pll()			\
({						\
  set_module_stop(UDC);				\
  REG32(CPCCR) |= CPCCR_UCS;			\
  clr_module_stop(UDC);				\
})

#define set_cfcr_ucs_drv()			\
({						\
  set_module_stop(UDC);				\
  REG32(CPCCR) &= ~CPCCR_UCS;			\
  clr_module_stop(UDC);				\
})

#define set_cfcr_pcm(val)			\
({						\
  set_module_stop(PCM);				\
  REG32(PCMCDR) = val;                          \
  REG32(PCMCDR) = REG32(PCMCDR) | 0x80000000;   \
  clr_module_stop(PCM);				\
})

#define set_cfcr_pcm_2(val)			\
({						\
  set_module_stop(PCM);				\
  REG32(PCMCDR) = val;                          \
  REG32(PCMCDR) = REG32(PCMCDR) | 0x80000000;   \
  REG32(CPCCR) = REG32(CPCCR) | 0x00600000;     \
  clr_module_stop(PCM);				\
})

#define set_cfcr_pcm_3(val)			\
({						\
  set_module_stop(PCM);				\
  REG32(PCMCDR) = val;                          \
  REG32(PCMCDR) = REG32(PCMCDR) & 0x7fffffff;   \
  REG32(CPCCR) = REG32(CPCCR) | 0x00600000;     \
  clr_module_stop(PCM);				\
})

// pll output 
#define set_cfcr_i2s(val) set_cpm_i2s_div1(val)
#define set_cpm_i2s_div1(val)			\
({						\
  set_module_stop(I2S);				\
  set_cpm_i2s_div(val);                         \
  REG32(CPCCR) |= 0x80000000;                   \
  clr_module_stop(I2S);				\
})

// pll not div half
#define set_cpm_i2s_div2(val)			\
({						\
  set_module_stop(I2S);				\
  set_cpm_i2s_div(val);                         \
  REG32(CPCCR) |= 0x80600000;                   \
  clr_module_stop(I2S);				\
})

// exlk
#define set_cpm_i2s_div3(val)			\
({						\
  set_module_stop(I2S);				\
  set_cpm_i2s_div(val);                         \
  REG32(CPCCR) = REG32(CPCCR) & 0x7fffffff | 0x00600000;   \
  clr_module_stop(I2S);				\
})

#define set_cpm_udc_div1(val)                                            \
({                                                                       \
  set_module_stop(UDC);                                                  \
  set_cpccr_ucs_pll();                                                   \
  set_cpccr_pcs_pll();                                                   \
  REG32(CPCCR) = REG32(CPCCR) & ~CPCCR_UDIV | (val << CPCCR_UDIV_S);     \
  clr_module_stop(UDC);                                                  \
})

#define set_cpm_udc_div2(val)                                            \
({                                                                       \
  set_module_stop(UDC);                                                  \
  set_cpccr_ucs_pll();                                                   \
  set_cpccr_pcs_half();                                                  \
  REG32(CPCCR) = REG32(CPCCR) & ~CPCCR_UDIV | (val << CPCCR_UDIV_S);     \
  clr_module_stop(UDC);                                                  \
})

#define set_cpm_udc_div3(val)                                            \
({                                                                       \
  set_module_stop(UDC);                                                  \
  set_cpccr_ucs_ext();                                                   \
  clr_module_stop(UDC);                                                  \
})

#define set_cfcr_ssi1(val)			\
({						\
  set_module_stop(SSI1);			\
  REG32(SSICDR) = 0x80000000 | val;		\
  clr_module_stop(SSI1);			\
})
#define set_cfcr_ssi1_2(val)			\
({						\
  set_module_stop(SSI1);			\
  REG32(SSICDR) = 0x80000000 | val;		\
  REG32(CPCCR) = REG32(CPCCR) | 0x00600000;   	\
  clr_module_stop(SSI1);			\
})

#define set_cpm_ssi0_div1(val)			\
({						\
  REG32(SSICDR) = 0xa0000000 | val;		\
  while (REG32(SSICDR) & (1 << 28)) ;           \
})

#define set_cpm_ssi0_div2(val)			\
({						\
  set_module_stop(SSI0);			\
  REG32(SSICDR) = 0x80000000 | val;		\
  REG32(CPCCR) = REG32(CPCCR) | 0x00600000;   	\
  clr_module_stop(SSI0);			\
})

#define set_cpm_msc_div1(val)			\
({						\
  REG32(MSCCDR) = val;             		\
})

#define cpm_wait_msccdr_no_busy()                          \
({							   \
  unsigned int volatile flag;				   \
							   \
  flag = (REG32(MSCCDR) & MSCCDR_BUSY) >> MSCCDR_BUSY_A;   \
							   \
  while (flag == 1) {					   \
    flag = (REG32(MSCCDR) & MSCCDR_BUSY) >> MSCCDR_BUSY_A; \
  }                                                        \
})

#define cpm_set_stop_msccdr()                     \
({						  \
  REG32(MSCCDR) = REG32(MSCCDR) | MSCCDR_STOP;    \
})

#define cpm_clr_stop_msccdr()                     \
({						  \
  REG32(MSCCDR) = REG32(MSCCDR) & ~MSCCDR_STOP;   \
})

#define cpm_ssel_180_msccdr()                     \
({						  \
  REG32(MSCCDR) = REG32(MSCCDR) | MSCCDR_SSEL;    \
})

#define cpm_ssel_90_msccdr()                      \
({						  \
  REG32(MSCCDR) = REG32(MSCCDR) & ~MSCCDR_SSEL;   \
})

#define set_cpm_msc_div2(val)			\
({						\
  REG32(MSCCDR) = (REG32(MSCCDR) & ~MSCCDR_DIV) | val | MSCCDR_CE;  \
  cpm_wait_msccdr_no_busy();                                        \
})

#define set_cpm_msc1_div1(val)			\
({						\
  set_module_stop(MSC1);			\
  REG32(MSC1CDR) = val;             		\
  clr_module_stop(MSC1);			\
})

#define set_cpm_msc1_div2(val)			\
({						\
  set_module_stop(MSC1);			\
  REG32(MSC1CDR) = val;            		\
  REG32(CPCCR) = REG32(CPCCR) | 0x00600000;   	\
  clr_module_stop(MSC1);			\
})

#define clr_cfcr_i2s()				\
({						\
  set_module_stop(I2S);				\
  REG32(CPCCR) &= ~CFCR_I2S;			\
  clr_module_stop(I2S);				\
})

#define clr_cfcr_ssi()				\
({						\
  set_module_stop(SSI);				\
  REG32(CPCCR) &= ~CFCR_SSI;			\
  clr_module_stop(SSI);				\
})

#define set_rtcclk_extal2()       (REG32(OCR) |= OCR_O2E)
#define set_rtcclk_extal_128()    (REG32(OCR) &= ~OCR_O2E)
/*#define get_o2s()                 (REG32(OCR) & OCR_O2S)
*/
/********************************************************************************
//////////  set clock monitor ///////////////////////
********************************************************************************/

// CGU_DRV_BASE = 0xbbc90000

#define CGU_MON_ENB            (CGU_DRV_BASE + 0x0)    
#define CGU_MON_DIS            (CGU_DRV_BASE + 0x4)
#define CGU_ICLK_STOP_CNT      (CGU_DRV_BASE + 0x8)    
#define CGU_SCLK_STOP_CNT      (CGU_DRV_BASE + 0xC)    
#define CGU_PCLK_STOP_CNT      (CGU_DRV_BASE + 0x10)   
#define CGU_MCLK_STOP_CNT      (CGU_DRV_BASE + 0x14)   
#define CGU_CKO1_STOP_CNT      (CGU_DRV_BASE + 0x18)   
#define CGU_CKO2_STOP_CNT      (CGU_DRV_BASE + 0x1C)   
#define MON_NRER1              (CGU_DRV_BASE + 0x20)   // 0xa0c90020
#define MON_XRER1              (CGU_DRV_BASE + 0x24)   // 0xa0c90024
#define MON_NRER2              (CGU_DRV_BASE + 0x28)   // 0xa0c90028
#define MON_XRER2              (CGU_DRV_BASE + 0x2C)   // 0xa0c9002c
#define MON_CMP1_CLKSEL        (CGU_DRV_BASE + 0x30)   // 0xa0c90030
#define MON_CMP2_CLKSEL        (CGU_DRV_BASE + 0x34)   // 0xa0c90034
#define MON_CNT_END1           (CGU_DRV_BASE + 0x38)   // 0xa0c90038
#define MON_CNT_END2           (CGU_DRV_BASE + 0x3C)   // 0xa0c9003c
#define CGU_LCLK_STOP_CNT      (CGU_DRV_BASE + 0x40)   
#define CGU_UDC_CLK_STOP_CNT   (CGU_DRV_BASE + 0x44)   
#define CGU_FIR_CLK_STOP_CNT   (CGU_DRV_BASE + 0x48)   
#define CGU_MSC_CLK_STOP_CNT   (CGU_DRV_BASE + 0x4C)   
#define CGU_UART_CLK_STOP_CNT  (CGU_DRV_BASE + 0x50)   
#define CGU_I2S_CLK_STOP_CNT   (CGU_DRV_BASE + 0x54)   
#define CGU_I2C_CLK_STOP_CNT   (CGU_DRV_BASE + 0x58)   
#define CGU_SSI_CLK_STOP_CNT   (CGU_DRV_BASE + 0x5C)   
#define CGU_SCC_CLK_STOP_CNT   (CGU_DRV_BASE + 0x60)   
#define CGU_PWM_CLK_STOP_CNT   (CGU_DRV_BASE + 0x64)   
#define CGU_PLL1OUT_STOP_CNT   (CGU_DRV_BASE + 0x68)   
#define CGU_PLL2OUT_STOP_CNT   (CGU_DRV_BASE + 0x6C)   
#define CGU_KBC_CLK_STOP_CNT   (CGU_DRV_BASE + 0x70)   
#define CGU_BITCLK_STOP_CNT    (CGU_DRV_BASE + 0x74)   
#define CGU_SYSCLK_STOP_CNT    (CGU_DRV_BASE + 0x78)   
#define CGU_DES_CLK_STOP_CNT   (CGU_DRV_BASE + 0x7C)   
#define CGU_ETHC_CLK_STOP_CNT  (CGU_DRV_BASE + 0x80)   
#define CGU_OSC1_CLK_STOP_CNT  (CGU_DRV_BASE + 0x84)   
#define CGU_UHC_CLK_STOP_CNT   (CGU_DRV_BASE + 0x88)   
#define CGU_LPC_CLK_STOP_CNT   (CGU_DRV_BASE + 0x8C)   
#define CGU_PIX_CLK_STOP_CNT   (CGU_DRV_BASE + 0x90)   
#define CGU_ETH_RXCLK_STOP_CNT (CGU_DRV_BASE + 0x94)   
#define CGU_PIX_CLK_VALID      (CGU_DRV_BASE + 0x98)   
#define CGU_SSI1_CLK_STOP_CNT  (CGU_DRV_BASE + 0xa0)
#define CGU_MSC1_CLK_STOP_CNT  (CGU_DRV_BASE + 0xa4)
#define CGU_SADC_CLK_STOP_CNT  (CGU_DRV_BASE + 0xa8)
#define CGU_PCM_CLK_STOP_CNT   (CGU_DRV_BASE + 0xac)
#define CGU_OWI_CLK_STOP_CNT   (CGU_DRV_BASE + 0xb0)
#define CGU_TVE_CLK_STOP_CNT   (CGU_DRV_BASE + 0xb4)
#define CGU_BCH_CLK_STOP_CNT   (CGU_DRV_BASE + 0xb8)
#define CGU_AHB1_CLK_STOP_CNT  (CGU_DRV_BASE + 0xbc)
#define CGU_ACPU_CLK_STOP_CNT  (CGU_DRV_BASE + 0xc0)

#define MON_MAX_CYCLE          (CGU_DRV_BASE + 0x100)   // 0xa0c90100
#define MON_RESET_CNT          (CGU_DRV_BASE + 0x110)   // 0xa0c90110
#define MON_RTCCLK_RISE        (CGU_DRV_BASE + 0x114)   // 0xa0c90114

#define MON_GPCLK_LEVEL        (CGU_DRV_BASE + 0x200)   // 0xa0c90200
   
#define CGU_MON2_ENB           (CGU_DRV_BASE + 0xe00)    
#define CGU_MON2_DIS           (CGU_DRV_BASE + 0xe04)    

#define CGU_CNT_ENB            (CGU_DRV_BASE + 0xf00)
#define CGU_CNT_DIS            (CGU_DRV_BASE + 0xf04) 
#define CGU_CNT_RD_CCLK        (CGU_DRV_BASE + 0xf08)
#define CGU_CNT_RD_HCLK        (CGU_DRV_BASE + 0xf0c)
#define CGU_CNT_RD_PCLK        (CGU_DRV_BASE + 0xf10)
#define CGU_CNT_RD_MCLK        (CGU_DRV_BASE + 0xf14)
#define CGU_CNT_RD_H1CLK       (CGU_DRV_BASE + 0xf18)
#define CGU_CNT_RD_PLL         (CGU_DRV_BASE + 0xf1c)

#define CGU_CNT_PLL            0x80000000
#define CGU_CNT_H1CLK          0x00000010 
#define CGU_CNT_MCLK           0x00000008
#define CGU_CNT_PCLK           0x00000004
#define CGU_CNT_HCLK           0x00000002
#define CGU_CNT_CCLK           0x00000001

#define CGU_ICLK_MON           0x00000001
#define CGU_SCLK_MON           0x00000002
#define CGU_PCLK_MON           0x00000004
#define CGU_MCLK_MON           0x00000008
#define CGU_CKO1_MON           0x00000010
#define CGU_CKO2_MON           0x00000020
#define CGU_LCLK_MON           0x00000040
#define CGU_UDC_CLK_MON        0x00000080
#define CGU_TVE_CLK_MON        0x00000100
#define CGU_MSC_CLK_MON        0x00000200
#define CGU_UART_CLK_MON       0x00000400
#define CGU_I2S_CLK_MON        0x00000800
#define CGU_I2C_CLK_MON        0x00001000
#define CGU_SSI_CLK_MON        0x00002000
#define CGU_SCC_CLK_MON        0x00004000
#define CGU_PWM_CLK_MON        0x00008000
#define CGU_PLL1OUT_MON        0x00010000
#define CGU_PLL2OUT_MON        0x00020000
#define CGU_MSC1_CLK_MON       0x00040000
#define CGU_BITCLK_MON         0x00080000
#define CGU_SYSCLK_MON         0x00100000
#define CGU_SSI1_CLK_MON       0x00200000
#define CGU_ETHC_CLK_MON       0x00400000
#define CGU_OSC1_CLK_MON       0x00800000
#define CGU_UHC_CLK_MON        0x01000000
#define CGU_PCM_CLK_MON        0x02000000
#define CGU_PIX_CLK_MON        0x04000000
#define CGU_FIR_CLK_MON        0x08000000
#define CGU_KBC_CLK_MON        0x08000000
#define CGU_DES_CLK_MON        0x08000000
#define CGU_LPC_CLK_MON        0x08000000
#define CGU_ETH_RXCLK_MON      0x08000000
#define CGU_SADC_CLK_MON       0x10000000
#define CGU_OWI_CLK_MON        0x20000000
#define CGU_CMP1_MON           0x40000000
#define CGU_CMP2_MON           0x80000000

#define CGU_MON2_ACPU_CLK      0x00000001
#define CGU_MON2_AHB1_CLK      0x00000002
#define CGU_MON2_BCH_CLK       0x00000004

#define MON_SEL_ICLK     	1
#define MON_SEL_SCLK     	2
#define MON_SEL_PCLK     	3
#define MON_SEL_MCLK  	 	4
#define MON_SEL_CKO1 	 	5 
#define MON_SEL_CKO2 	 	6 
#define MON_SEL_LCLK  	 	7 
#define MON_SEL_UDC_CLK  	8 
#define MON_SEL_TVE_CLK	        9
#define MON_SEL_MSC_CLK  	10
#define MON_SEL_UART_CLK  	11
#define MON_SEL_I2S_CLK  	12
#define MON_SEL_I2C_CLK  	13
#define MON_SEL_SSI_CLK  	14
#define MON_SEL_SCC_CLK  	15
#define MON_SEL_PWM_CLK  	16
#define MON_SEL_PLL1OUT  	17
#define MON_SEL_PLL2OUT  	18
#define MON_SEL_PLL3OUT  	19
#define MON_SEL_RTCCLK  	20
#define MON_SEL_BITCLK  	21
#define MON_SEL_SYSCLK  	22
#define MON_SEL_ETHC_CLK  	23
#define MON_SEL_FIR_CLK  	23 
#define MON_SEL_UHC_CLK  	24
#define MON_SEL_PWM1_CLK  	25
#define MON_SEL_PIX_CLK  	26
#define MON_SEL_OWI_CLK	        27
#define MON_SEL_SADC_CLK        28
#define MON_SEL_PCM_CLK         29
#define MON_SEL_SSI1_CLK        30
#define MON_SEL_MSC1_CLK        31

#define MON_SEL_CIM_MCLK        32
#define MON_SEL_H1CLK           33

#define get_clk_scnt(CLK)      REG32(CGU_##CLK##_STOP_CNT)

#define GET_ICLK_SCNT          get_clk_scnt(ICLK)
#define GET_SCLK_SCNT          get_clk_scnt(SCLK)
#define GET_PCLK_SCNT          get_clk_scnt(PCLK)
#define GET_MCLK_SCNT          get_clk_scnt(MCLK)
#define GET_CKO1_SCNT          get_clk_scnt(CKO1)
#define GET_CKO2_SCNT          get_clk_scnt(CKO2)
#define GET_LCLK_SCNT          get_clk_scnt(LCLK)
#define GET_UDC_CLK_SCNT       get_clk_scnt(UDC_CLK)
#define GET_FIR_CLK_SCNT       get_clk_scnt(FIR_CLK)
#define GET_MSC_CLK_SCNT       get_clk_scnt(MSC_CLK)
#define GET_UART_CLK_SCNT      get_clk_scnt(UART_CLK)
#define GET_I2S_CLK_SCNT       get_clk_scnt(I2S_CLK)
#define GET_I2C_CLK_SCNT       get_clk_scnt(I2C_CLK)
#define GET_SSI_CLK_SCNT       get_clk_scnt(SSI_CLK)
#define GET_SCC_CLK_SCNT       get_clk_scnt(SCC_CLK)
#define GET_PWM_CLK_SCNT       get_clk_scnt(PWM_CLK)
#define GET_PLL1OUT_SCNT       get_clk_scnt(PLL1OUT)
#define GET_PLL2OUT_SCNT       get_clk_scnt(PLL2OUT)
#define GET_KBC_CLK_SCNT       get_clk_scnt(KBC_CLK)
#define GET_BITCLK_SCNT        get_clk_scnt(BITCLK)
#define GET_SYSCLK_SCNT        get_clk_scnt(SYSCLK)
#define GET_DES_CLK_SCNT       get_clk_scnt(DES_CLK)
#define GET_ETHC_CLK_SCNT      get_clk_scnt(ETHC_CLK)
#define GET_OSC1_CLK_SCNT      get_clk_scnt(OSC1_CLK)
#define GET_UHC_CLK_SCNT       get_clk_scnt(UHC_CLK)
#define GET_LPC_CLK_SCNT       get_clk_scnt(LPC_CLK)
#define GET_PIX_CLK_SCNT       get_clk_scnt(PIX_CLK)
#define GET_ETH_RXCLK_SCNT     get_clk_scnt(ETH_RXCLK)
#define GET_SSI1_CLK_SCNT      get_clk_scnt(SSI1_CLK)
#define GET_MSC1_CLK_SCNT      get_clk_scnt(MSC1_CLK)
#define GET_SADC_CLK_SCNT      get_clk_scnt(SADC_CLK)
#define GET_PCM_CLK_SCNT       get_clk_scnt(PCM_CLK)
#define GET_OWI_CLK_SCNT       get_clk_scnt(OWI_CLK)
#define GET_TVE_CLK_SCNT       get_clk_scnt(TVE_CLK)
#define GET_BCH_CLK_SCNT       get_clk_scnt(BCH_CLK)
#define GET_ACPU_CLK_SCNT      get_clk_scnt(ACPU_CLK)
#define GET_AHB1_CLK_SCNT      get_clk_scnt(AHB1_CLK)

#define START_CLK_MONITOR(value)  {REG32(CGU_MON_ENB) = (value);}
#define STOP_CLK_MONITOR(value)   {REG32(CGU_MON_DIS) = (value);}

#define START_CLK_MONITOR2(value) {REG32(CGU_MON2_ENB) = (value);}
#define STOP_CLK_MONITOR2(value)  {REG32(CGU_MON2_DIS) = (value);}	       

#define GET_CGU_CLK2_CMP1    i_l32(MON_XRER1,0)
#define GET_CGU_CLK2_CMP2    i_l32(MON_XRER2,0)
#define GET_CGU_CNT1_END     i_l32(MON_CNT_END1, 0)
#define GET_CGU_CNT2_END     i_l32(MON_CNT_END2, 0)

#define GET_CGU_GPCLK_LEVEL  i_l32(MON_GPCLK_LEVEL, 0)

#define get_clk2_from_clk1_cnt(clk1, clk2, cnt)         \
({                                                      \
  i_s32((clk2<<16)+clk1,MON_CMP1_CLKSEL,0);             \
  i_s32(cnt,MON_NRER1,0);                               \
  START_CLK_MONITOR(CGU_CMP1_MON);                      \
  while (GET_CGU_CNT1_END == 0);                        \
  STOP_CLK_MONITOR(CGU_CMP1_MON);                       \
  (GET_CGU_CLK2_CMP1);                                  \
})

#define get_clk2_from_clk1_cnt2(clk1, clk2, cnt)         \
({                                                      \
  i_s32((clk2<<16)+clk1,MON_CMP2_CLKSEL,0);             \
  i_s32(cnt,MON_NRER20);                               \
  START_CLK_MONITOR(CGU_CMP2_MON);                      \
  while (GET_CGU_CNT2_END == 0);                        \
  STOP_CLK_MONITOR(CGU_CMP2_MON);                       \
  (GET_CGU_CLK2_CMP2);                                  \
})

#define get_cim_mclk_cnt(cnt)    get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_CIM_MCLK, cnt)
#define get_iclk_cnt(cnt)        get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_ICLK, cnt)
#define get_sclk_cnt(cnt)        get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_SCLK, cnt)
#define get_pclk_cnt(cnt)        get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_PCLK, cnt)
#define get_mclk_cnt(cnt)        get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_MCLK, cnt)
#define get_h1clk_cnt(cnt)       get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_H1CLK, cnt)
#define get_cko_cnt(cnt)         get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_CKO1, cnt)
#define get_cko1_cnt(cnt)        get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_CKO1, cnt)
#define get_cko2_cnt(cnt)        get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_CKO2, cnt)
#define get_lclk_cnt(cnt)        get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_LCLK, cnt)
#define get_udc_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_UDC_CLK, cnt)
#define get_fir_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_FIR_CLK, cnt)
#define get_msc_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_MSC_CLK, cnt)
#define get_msc1_clk_cnt(cnt)    get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_MSC1_CLK, cnt)
#define get_uart_clk_cnt(cnt)    get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_UART_CLK, cnt)
#define get_i2s_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_I2S_CLK, cnt)
#define get_pcm_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_PCM_CLK, cnt)
#define get_ssi1_clk_cnt(cnt)    get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_SSI1_CLK, cnt)
#define get_ssi_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_SSI_CLK, cnt)
#define get_scc_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_SCC_CLK, cnt)
#define get_pwm_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_PWM_CLK, cnt)
#define get_pllout_cnt(cnt)      get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_PLL1OUT, cnt)
#define get_pll1out_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_PLL1OUT, cnt)
#define get_pll2out_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_PLL2OUT, cnt)
#define get_pll3out_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_PLL3OUT, cnt)
#define get_i2c_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_RTCCLK,  MON_SEL_I2C_CLK, cnt)
#define get_owi_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_RTCCLK,  MON_SEL_OWI_CLK, cnt)
#define get_sadc_clk_cnt(cnt)    get_clk2_from_clk1_cnt(MON_SEL_RTCCLK,  MON_SEL_SADC_CLK, cnt)
#define get_uhc_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_UHC_CLK, cnt)
#define get_pwm1_clk_cnt(cnt)    get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_PWM1_CLK, cnt)
#define get_ethc_clk_cnt(cnt)    get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_ETHC_CLK, cnt)
#define get_pix_clk_cnt(cnt)     get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_PIX_CLK, cnt)
#define get_tve_pix_clk_cnt(cnt) get_clk2_from_clk1_cnt(MON_SEL_I2C_CLK, MON_SEL_TVE_CLK, cnt)

#define check_f_rate(D0,R0,D1,R1,N) \
     (((D0 - 1) * (R1 - 1) > N * (D1 + 1) * (R0 + 1)) ||\
     ((N * (D1 - 1) * (R0 - 1)) > (D0 + 1) * (R1 + 1)))

#define check_f_rate1(D0,R0,D1,R1,N0, N1)               \
     ((N1 * (D0 - 1) * (R1 - 1) > N0 * (D1 + 1) * (R0 + 1)) ||\
     ((N0 * (D1 - 1) * (R0 - 1)) > N1 * (D0 + 1) * (R1 + 1)))

#define check_f_rate2(R0, R1, N0, N1)                   \
     (((N0) * (R1 - 1) > N1 * (R0 + 1)) || ((N0) * (R1 + 1) < N1 * (R0 - 1)))

#define check_f_rate2_2(R0, R1, N0, N1)                   \
     (((N0) * (R1 - 2) > N1 * (R0 + 2)) || ((N0) * (R1 + 2) < N1 * (R0 - 2)))

#define check_f_rate2_m(R0, R1, N0, N1)                   \
     (((N0) * (R1 - 5) > N1 * (R0 + 5)) || ((N0) * (R1 + 5) < N1 * (R0 - 5)))

#define check_f_rate_m(R0, R1, N0, N1)                \
({                                                    \
  int num1, num2, numd;                               \
                                                      \
  num1 = R0 * N1;                                     \
  num2 = R1 * N0;                                     \
                                                      \
  if (num1 >= num2) {                                 \
    numd = num1 - num2;                               \
  }                                                   \
  else {                                              \
    numd = num2 - num1;                               \
  }                                                   \
                                                      \
  if (((num1 + num2) / 100 * 5) < (numd * 2)) {       \
    return 1;\
  }\
})

//                        .                                          .
// 24MHz clock cycle   41.6ns       PLL out 192MHz clock cycle  5.2083ns
// This value chenge to interger will lost accuracy, so  
// Add the max error(ERR) for the calculation.
// ERR is depend on R1 cycles
#define check_f_rate2_herr(R0, R1, N0, N1, ERR)		\
     (((N0) * (R1 - 1 - ERR) > N1 * (R0 + 1)) || ((N0) * (R1 + 1 + ERR) < N1 * (R0 - 1)))

#endif /* __INGENIC_CPM_H__ */
