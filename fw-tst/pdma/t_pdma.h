#ifndef __PDMA_H__
#define __PDMA_H__

#define PDMA

#include <cache.h>
#include "server.h"
#include "dmac.h"


/*------------------------------------------------------
  config_ddr_nemc_trans 
  ------------------------------------------------------*/


#define SID(val) (val<<26)
#define DID(val) (val<<24)

//__place_sys_data__ unsigned int ddr_nemc_trans[4] __attribute__ ((aligned (32)));
//__place_k0_data__ unsigned int ddr_nemc_trans[4];//cached
 unsigned int ddr_nemc_trans[7];//uncached
//__place_k1_data__ unsigned int ddr_data[256];//uncached

#define TCSM_BANK0_PA     0x13422000
#define TCSM_BANK1_PA     0x13422800
#define TCSM_BANK2_PA     0x13423000
#define TCSM_BANK3_PA     0x13423800
#define TCSM_BANK4_PA     0x13424000
#define TCSM_BANK5_PA     0x13424800


#define DDR_TO_NEMC       0x1
#define NEMC_TO_DDR       0x2
#define COM               0x3
#define SPECIAL           0x4
//type is the upper 4 kinds
//com is just like one non-descriptor transfer or descriptor transfer
//the  non-descriptor transfer format is : dsa,dta,dtc,COM,drt,ch_num (ch_num is 0 ... 31)
#define config_ddr_nemc_trans(dsa,dta,dtc,type,nemc_addr0,nemc_addr1,nemc_addr2,nemc_addr3,nemc_addr4,nemc_bank,use_toggle_nand,page_size,block_size)   \
({ \
  unsigned int temp;\
  ddr_nemc_trans[0]  = dsa;\
  ddr_nemc_trans[1]  = dta;\
  ddr_nemc_trans[2]  = dtc;\
  ddr_nemc_trans[3]  = type;\
  ddr_nemc_trans[4]  = (unsigned int)(nemc_addr0) + (((unsigned int)(nemc_addr1))<<8) + (((unsigned int)(nemc_addr2))<<16) +\
                       (((unsigned int)(nemc_addr3))<<24);\
  ddr_nemc_trans[5]  = (unsigned int)(nemc_addr4) + (((unsigned int)(nemc_bank))<<8) + (((unsigned int)(use_toggle_nand))<<16);\
  ddr_nemc_trans[6]  = (unsigned int)(page_size) + (((unsigned int)(block_size))<<16);\
  printk("%x\n",0xfffffff);\
  printk("%x\n",0x1111111);\
  printk("%p\n",ddr_nemc_trans);\
  printk("%x\n",ddr_nemc_trans[0]);\
  printk("%x\n",ddr_nemc_trans[1]);\
  printk("%x\n",ddr_nemc_trans[2]);\
  printk("%x\n",ddr_nemc_trans[3]);\
  printk("%x\n",0xfffffff);\
  printk("%x\n",0x1111111);\
  blast_dcache();\
  jz_sync();\
  open_pdmac();					\
  temp = i_l32(DMAC,0);\
  REG32(DMAC)       = (temp | DMAC_CH01 | DMAC_ENB); \
  REG32(DSA1)       = va_2_pa((unsigned int)ddr_nemc_trans); \
  REG32(DTA1)       = TCSM_BANK5_PA + 0x800 - 0x40; \
  REG32(DTC1)        = 0x1c; \
  REG32(DRT1)        = RS_ADDR_ADDR; \
  REG32(DCM1)       = (SID(0x2) | DID(0x0) | SADDR_INC | DADDR_INC | SRC_WIDTH_4 | DST_WIDTH_4 | DATA_SIZE_AUTO | TEND_INT_ENB); \
  REG32(DMACP)       = 0x2;\
  REG32(DCS1)        = NDES_ENB | CTE;\
})

#define send_secall(func) (REG32(DMCS) = REG32(DMCS) & 0xff0000ff | 0x8 | ((func)<<0x8) )

//////////// end of adding pdma /////////////////////////////

#endif
