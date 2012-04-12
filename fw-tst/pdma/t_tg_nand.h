#ifndef __DMAC_H__
#define __DMAC_H__

#include "t_nemc_pdma.h"
#include "server.h"

#undef AddrAddr
#undef CmdAddr
#undef DataAddr

#define REG_ONE      0XFFFFFFFF
#define REG_ZERO     0X00000000

#define VADDR1_KU   0x07305040
#define VADDR1_K0   0x9b840040
#define VADDR1_K1   0xbb047040
#define VADDR1_K2   0xc7054040
#define VADDR_SET   0x02000040
#define MIX_WORD    0x12345678
#define dcache_prefetch(va,offset) i_dcache_fetch_lock(va, offset)
#define BUFFERABLE  1

#define NAND_CH     0

// nand : static bank 1
// static bank 1 start address : 0xb800_0000

#ifdef CS2_NAND

//jz4770
//#define AddrAddr        0xb8000008
//#define CmdAddr         0xb8000004
//#define DataAddr        0xb8000000

//jz4780
#define AddrAddr        0xBA800000
#define CmdAddr         0xBA400000
#define DataAddr        0xBA000000

#else

#ifdef CS3_NAND

//4770
//#define AddrAddr        0xB7000008
//#define CmdAddr         0xB7000004
//#define DataAddr        0xB7000000

//jz4780
#define AddrAddr        0xB9800000
#define CmdAddr         0xB9400000
#define DataAddr        0xB9000000

#else

#ifdef CS4_NAND
//jz4780
#define AddrAddr        0xB8800000
#define CmdAddr         0xB8400000
#define DataAddr        0xB8000000

#else

#ifdef CS5_NAND
//jz4780
#define AddrAddr        0xB7800000
#define CmdAddr         0xB7400000
#define DataAddr        0xB7000000

#else

#ifdef CS6_NAND
//jz4780
#define AddrAddr        0xB6800000
#define CmdAddr         0xB6400000
#define DataAddr        0xB6000000

#else

//jz4770
//#define AddrAddr        0xBA000008
//#define CmdAddr         0xBA000004
//#define DataAddr        0xBA000000

//jz4780
#define AddrAddr        0xBB800000
#define CmdAddr         0xBB400000
#define DataAddr        0xBB000000

#endif

#endif

#endif
#endif

#endif


#define nf_rb_mask      0x00100000
#define DAEC 0x80000000
//#define wait_nf_busy()  while((REG32(GPIO_PPIN(0)) & nf_rb_mask) == nf_rb_mask)
//#define wait_nf_ready() while((REG32(GPIO_PPIN(0)) & nf_rb_mask) == 0)

int wait_nf_busy()  
{
	unsigned int cnt =0;
	printd(1986);
	while((REG32(GPIO_PPIN(0)) & nf_rb_mask) == nf_rb_mask)
	{
		cnt ++;
		if(cnt > 128) return 0 ;
	}
}

void wait_nf_ready() 
{
	printd(1987);
	while((REG32(GPIO_PPIN(0)) & nf_rb_mask) == 0);
	printd(1988);
}
/*
#ifdef TG_NAND
volatile  unsigned int tg_en = (0x00010000 << (NEMC_BANK-1))|(0x00000003 << ((NEMC_BANK-1)*2));
volatile  unsigned int tg_dis_en = (0x00010000 << (NEMC_BANK-1))|(0x00000001 << ((NEMC_BANK-1)*2)) ;
#else
volatile  unsigned int tg_en = (0x00000003 << ((NEMC_BANK-1)*2));
volatile  unsigned int tg_dis_en = (0x00000001 << ((NEMC_BANK-1)*2)) ;
#endif
*/
#ifdef TG_NAND
#define tg_en     ((0x00010000 << (NEMC_BANK-1))|(0x00000003 << ((NEMC_BANK-1)*2))) 
#define tg_dis_en ((0x00010000 << (NEMC_BANK-1))|(0x00000001 << ((NEMC_BANK-1)*2)) | 0x10000000)
#else
#define tg_en     (0x00000003 << ((NEMC_BANK-1)*2))
#define tg_dis_en (0x00000001 << ((NEMC_BANK-1)*2))
#endif

#define GPIO_PXINTC(n)	(GPIO_BASE + (0x18 + (n)*0x100)) /* Port Interrupt Clear Register */
#define GPIO_PXMASKC(n)	(GPIO_BASE + (0x28 + (n)*0x100)) /* Port Interrupt Mask Clear Reg */
#define GPIO_PXPAT1C(n)	(GPIO_BASE + (0x38 + (n)*0x100)) /* Port Pattern 1 Clear Reg. */
#define GPIO_PXPAT0C(n)	(GPIO_BASE + (0x48 + (n)*0x100)) /* Port Pattern 0 Clear Register */
#define GPIO_PXPENS(n)	(GPIO_BASE + (0x74 + (n)*0x100)) /* Port Pull Disable Set Register */
#define GPIO_PXMASKS(n)	(GPIO_BASE + (0x24 + (n)*0x100)) /* Port Interrupt Mask Set Reg */
#define GPIO_PXPAT1S(n)	(GPIO_BASE + (0x34 + (n)*0x100)) /* Port Pattern 1 Set Reg. */

#define REG_GPIO_PXINTC(n)	REG32(GPIO_PXINTC((n)))
#define REG_GPIO_PXMASKC(n)	REG32(GPIO_PXMASKC((n)))
#define REG_GPIO_PXPAT1C(n)	REG32(GPIO_PXPAT1C((n)))
#define REG_GPIO_PXPAT0C(n)	REG32(GPIO_PXPAT0C((n)))
#define REG_GPIO_PXPENS(n)	REG32(GPIO_PXPENS((n))) /* 0:Level-trigger/Fun0, 1:Edge-trigger/Fun1 */
#define REG_GPIO_PXMASKS(n)	REG32(GPIO_PXMASKS((n)))
#define REG_GPIO_PXPAT1S(n)	REG32(GPIO_PXPAT1S((n)))

/*
 * SD0 ~ SD7, CS1#, CLE, ALE, FRE#, FWE#, FRB#
 * @n: chip select number(1 ~ 6)
 */

static inline gpio_as_nand_8bit(void)	
{
	unsigned int n = 1;
	
	REG_GPIO_PXINTC(0) = 0x000c00ff; /* SD0 ~ SD7, FRE#, FWE# */   
	REG_GPIO_PXMASKC(0) = 0x000c00ff;			       
	REG_GPIO_PXPAT1C(0) = 0x000c00ff;			       
	REG_GPIO_PXPAT0C(0) = 0x000c00ff;			       
	REG_GPIO_PXPENS(0) = 0x000c00ff;			       
								       
	REG_GPIO_PXINTC(1) = 0x00000003; /* CLE(SA2), ALE(SA3) */      
	REG_GPIO_PXMASKC(1) = 0x00000003;			       
	REG_GPIO_PXPAT1C(1) = 0x00000003;			       
	REG_GPIO_PXPAT0C(1) = 0x00000003;			       
	REG_GPIO_PXPENS(1) = 0x00000003;			       
								       
	REG_GPIO_PXINTC(0) = 0x00200000 << ((n)-1); /* CSn */	       
	REG_GPIO_PXMASKC(0) = 0x00200000 << ((n)-1);		       
	REG_GPIO_PXPAT1C(0) = 0x00200000 << ((n)-1);		       
	REG_GPIO_PXPAT0C(0) = 0x00200000 << ((n)-1);		       
	REG_GPIO_PXPENS(0) = 0x00200000 << ((n)-1);		       
								       
	REG_GPIO_PXINTC(0) = 0x00100000; /* FRB#(input) */	       
	REG_GPIO_PXMASKS(0) = 0x00100000;			       
	REG_GPIO_PXPAT1S(0) = 0x00100000;			       
	REG_GPIO_PXPENS(0) = 0x00100000;			       

	//printk("\nset ce 1st\n");
	//REG32(0xb3410050) = 0x0 ;
	//REG32(0xb3410050) = 0x3 ;
	printk("CE_N = %x\n",REG32(0xb3410050));
	printk("GPIO:\n");
	printk("%x ",REG32(0x10010010|0xa0000000))  ;//set PAINT
	printk("%x ",REG32(0x10010020|0xa0000000))  ;//set PAINT
	printk("%x ",REG32(0x10010030|0xa0000000))  ;//set PAINT
	printk("%x ",REG32(0x10010040|0xa0000000))  ;//set PAINT
	//printk("\nset ce 2ed\n");
	//REG32(0xb3410050) = 0x3 ;
}



unsigned int nand_buf[8192/4+1024/4] ;

void set_ahb2_24mhz()
{
	REG32(0xb0000018) = REG32(0xb0000018) | (31<<19) | (15<<9);
	REG32(0xb0000018) = REG32(0xb0000018) | 0x00000001 ;
	while((REG32(0xb0000018) & 0x00000010) != 0x00000010);
	REG32(0xb0000000) = REG32(0xb0000000) & 0xffefffff ;
	REG32(0xb0000000) = (REG32(0xb0000000) & 0xfff00fff)|0x00010000 ;
	REG32(0xb0000000) = REG32(0xb0000000) & 0x00100000 ;
    while((REG32(0xb00000d4) & 0x00000004) == 0x00000004);
	REG32(0xb0000000) = REG32(0xb0000000) | 0x03000000 ;
}

void tg_cdqsh_done_check()
{
	printd(13577);
	while((REG32(0x13410150|0xa0000000)&0x01000000) != 0x01000000);
	printd(13579);
}

//set dqs pin function0
void tg_set_dqs_pin()
{
	REG32(0x10010010|0xa0000000) = REG32(0x10010010|0xa0000000) & 0xdfffffff ;//set PAINT
	REG32(0x10010020|0xa0000000) = REG32(0x10010020|0xa0000000) & 0xdfffffff ;//
	REG32(0x10010030|0xa0000000) = REG32(0x10010030|0xa0000000) & 0xdfffffff ;
	REG32(0x10010040|0xa0000000) = REG32(0x10010040|0xa0000000) & 0xdfffffff ;
}

void tg_auto_delay_line()
{
	int unsigned tmp = 0x07c00100;
	REG32(0xb341014c) = tmp ;
	tmp = REG32(0xb341014c) ;
	while( (tmp&0x10000000) != 0x10000000)
		tmp = REG32(0xb341014c) ;
	printd(1145);
	printx(tmp);
}

void tg_set_delay_line(unsigned int rdqs)
{
	printd(rdqs);
	REG32(0xb341014c) = (REG32(0xb341014c)&0xffffffe0)| (rdqs&0x1f);
	printx(REG32(0xb341014c));
}

void tg_delay_line(unsigned int rdqs)
{
    int unsigned tmp = 0x07000100|(rdqs&0x1f) ; //0x07c00100;
    REG32(0xb341014c) = tmp ;
}

int tg_ce_done_check()
{
    unsigned int tmp ;
    #ifdef TG_NAND
    tmp = REG32(0xb341013C) ;
    while ( (tmp & 0x3F) != 0x3F )
        tmp = REG32(0xb341013C) ;
    #endif
    return 0 ;
}

void tg_wait_cwaw() 
{
    unsigned int tmp ;
    #ifdef TG_NAND
    REG32(0xb3410148) = 330/3;
    tmp = REG32(0xb3410148) ;
    while ( (tmp & 0x00010000) != 0x00010000 )
        tmp = REG32(0xb3410148) ;
	#endif
}

void  fidle_check() 
{
    #ifdef TG_NAND
	//while((REG32(0xb341010c)&0x40000000) != 0x40000000) ;
	#endif
}

void  set_drs()
{
    #ifdef TG_NAND
	//fidle_check();
	unsigned int tmp ;
	REG32(0xb341010c) = 0x80000000;
	tmp = REG32(0xb341010c) ;
	//while((REG32(0xb341010c)&0x80000000) != 0x80000000) ;
    #endif
}

void  clr_drs_dws()
{
    #ifdef TG_NAND
	//fidle_check();
    unsigned int tmp ;
    REG32(0xb341010c) = 0x00000000;
	tmp = REG32(0xb341010c) ;
	//while((REG32(0xb341010c)&0x80000000) != 0x80000000) ;
    #endif
}


int tg_read_stat(unsigned char cmd , unsigned char aim , unsigned int bank)
{
  unsigned char tmp ;

  REG8(CmdAddr) = cmd & 0xff ;//0x70;
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif
  tmp     = REG8(DataAddr);
  #ifdef TG_NAND
  tmp     = REG8(DataAddr);
  clr_drs_dws();
//#ifndef TG_CDQSH
  REG32(0xb3410050) = tg_dis_en;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en;
//#endif
  #endif
  if (tmp != aim /*0xe0*/) {
    return 1;
  }
  else {
    return 0;
  }
}

int tg_read_stat_interl(unsigned char cmd , unsigned char aim , unsigned int bank)
{
  unsigned char tmp ;

  do{
    REG8(CmdAddr) = cmd & 0xff ;//0x70;
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

    tmp     = REG8(DataAddr);
    #ifdef TG_NAND
    tmp     = REG8(DataAddr);
clr_drs_dws();
#ifndef TG_CDQSH
    REG32(0xb3410050) = tg_dis_en;
    tg_ce_done_check() ;
    REG32(0xb3410050) = tg_en;
#endif
    #endif
  }while((tmp&aim)!=aim);
}

unsigned int tg_rst()
{
  REG8(CmdAddr)  = 0xff ;//reset toggle nand
  printd(33) ;
  wait_nf_busy();
  printd(44) ;
  wait_nf_ready();
  printd(3344) ;
  return 0 ;
}

unsigned int tg_erase(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int bank)
{
    #ifdef TG_NAND
  REG8(CmdAddr)  = 0x60 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(CmdAddr)  = 0xD0 ;
  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  #else
  REG8(CmdAddr)  = 0x60 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(CmdAddr)  = 0xD0 ;
  wait_nf_busy();
  wait_nf_ready();

  //if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
	#endif
  return 0 ;
}

void tg_set_pn()
{
    REG32(0xb3410100) = 0x3;
}

void counter_set()
{   //enabel counter of "1"
    REG32(0xb3410100) =0x28 ;
}
unsigned int counter_clr()
{   //enabel counter of "1"
    unsigned int cnt ;
    cnt = REG32(0xb3410108) ;
    printd(332211) ;
    printx(cnt);
    REG32(0xb3410100) = (REG32(0xb3410100) & 0x03)|0x20 ;//reset coutner, then BITCNT must be 0
	if(REG32(0xb3410108) != 0x0) {printx(0xdeaddead); printd(REG32(0xb3410108)) ; return 0 ;}
}


void tg_clr_pn()
{
  //unsigned int tmp ;
    REG32(0xb3410100) = 0x2;
	REG32(0xb3410104) = 0x5aa5 ;
	
}

/*
void tg_timing_set( unsigned int bank , unsigned int wr )
{
	//----------------------------------
	//set toggle nand flash timing
	//----------------------------------
	//only for cmd and address write, gave more hold time
	//REG32(0xb3410014+(bank-1)*4) = REG32(0xb3410014+(bank-1)*4)|0xf000    ;
	
	//for data access
    #ifdef TG_NAND
	REG32(0xb3410014 +(bank-1)*4) = 0x001221200 ;
	if(wr == 0){
		REG32(0xb3410110+(bank-1)*4) = 0x3f000109    ;//0x3f000101    ;
		REG32(0xb3410128) = 0x0000000c<<((bank-1)*5) ;//set spacail reg
		if(bank<=3)
		    REG32(0xb3410140) = 0x00000008<<((bank-1)*8) ;
		else
		    REG32(0xb3410144) = 0x00000008<<((bank-1)*8) ;
	}
	else{
		REG32(0xb3410110+(bank-1)*4) = 0x00000109    ;//0x3f000302    ;//0x3f000101    ;
		REG32(0xb3410128) = 0x0000000c<<((bank-1)*5) ;//set spacail reg
		if(bank<=3)
		    REG32(0xb3410140) = 0x00000008<<((bank-1)*8) ;
		else
		    REG32(0xb3410144) = 0x00000008<<((bank-1)*8) ;
	}
    #else
	
	REG32(0xb3410014 +(bank-1)*4) = 0x001221200 ;
	#endif
}
*/
void tg_timing_set( unsigned int bank , unsigned int wr )
{
    //----------------------------------
    //set toggle nand flash timing
    //----------------------------------
//----------------------------------------------

//----------------------------------------------

#ifndef AHB24M
    //for data access
    #ifdef TG_NAND
	//for cmd and addr timing
    REG32(0xb3410014 +(bank-1)*4) = 0x001221200 ;
	//for data write and read
    REG32(0xb3410110+(bank-1)*4) = 0x00000109    ;//0x3f000101    ;

    REG32(0xb3410128) = 0x0000000c<<((bank-1)*5) ;//set spacail reg
    if(bank<=3)
        REG32(0xb3410140) = 0x00000008<<((bank-1)*8) ;
    else
        REG32(0xb3410144) = 0x00000008<<((bank-1)*8) ;
    #else
    REG32(0xb3410014 +(bank-1)*4) = 0x001221200 ;
    #endif
#else
    //for cmd and addr timing
    REG32(0xb3410014 +(bank-1)*4) = 0x001221200 ;
    //for data write and read
    REG32(0xb3410110+(bank-1)*4)  = 0x00000102  ;//0x3f000101    ;
    REG32(0xb3410128) = 0x00000001<<((bank-1)*5) ;//set spacail reg
    if(bank<=3){
		REG32(0xb341012c) = 0x00000004<<((bank-1)*8) ;
		REG32(0xb3410134) = 0x00000001<<((bank-1)*8) ;
        REG32(0xb3410140) = 0x00000004<<((bank-1)*8) ;
	}
    else{
        REG32(0xb3410130) = 0x00000004<<((bank-1)*8) ;
        REG32(0xb3410138) = 0x00000001<<((bank-1)*8) ;
        REG32(0xb3410144) = 0x00000004<<((bank-1)*8) ;
	}
#endif
}

void tg_wcd_check( unsigned int bank )
{
    unsigned int tmp ;
    #ifdef TG_NAND
	fidle_check();
    REG32(0xb341010c)=0x1<<(bank-1) | 0x80000000 ; //set write data after next ce
    tmp = REG32(0xb341010c) ;
    while((tmp & 0x00010000) != 0x00010000) tmp = REG32(0xb341010c) ;
    #endif
}

unsigned int tg_status_read()
{
  unsigned char ID0,ID1,ID2,ID3,ID4,ID5;
  unsigned char tmp ;
//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

  REG8(CmdAddr)  = 0x90 ;
  REG8(AddrAddr) = 0x40 ;
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  ID0 = REG8(DataAddr) ;
  tmp = REG8(DataAddr) ;

  ID1 = REG8(DataAddr) ;
  tmp = REG8(DataAddr) ;

  ID2 = REG8(DataAddr) ;
  tmp = REG8(DataAddr) ;

  ID3 = REG8(DataAddr) ;
  tmp = REG8(DataAddr) ;

  ID4 = REG8(DataAddr) ;
  tmp = REG8(DataAddr) ;

  ID5 = REG8(DataAddr) ;
  //tmp = REG8(DataAddr) ;
clr_drs_dws();
#ifdef TG_NAND
  if(ID0 == 0x4A) printx(ID0);else return 1 ;
  if(ID1 == 0x45) printx(ID1);else return 1 ;
  if(ID2 == 0x44) printx(ID2);else return 1 ;
  if(ID3 == 0x45) printx(ID3);else return 1 ;
  if(ID4 == 0x43) printx(ID4);else return 1 ;
  if(ID5 == 0x02) printx(ID5);else return 1 ;
#endif
//8'h4A;
//8'h45;
//8'h44;
//8'h45;
//8'h43;
//8'h02;
  return 0 ;
}



unsigned int nemc_cfg_tg(
	unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4,
	unsigned int bank
)
{
	// printd(551);
	//dis_interrupt();
	//printd(552);
	//itb_set_smcfg(1);
	//printd(553);
	//init_nf(1);
	printk("initial NEMC GPIO:\n");
	gpio_as_nand_8bit();
#ifdef TG_NAND
tg_set_dqs_pin() ;
//tg_auto_delay_line() ;//auto detect delay line
#endif

//tg_timing_set( bank , 1 ) ;
//----------------------------------
//set toggle nand CE_n
//----------------------------------
printd(343456);
REG32(0xb3410050) = tg_en ;//bank1 is toggle nand
//REG32(0xb3410050) = 0x00010003 ;
printk("\nNEMC : %x",REG32(0xb3410050)) ;
//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

//----------------------------------
//erase block 0
//----------------------------------
#ifdef TG_NAND
if(tg_erase(addr2,addr3,addr4,bank)!=0) return 1 ;
#else
printk("Common NAND erase begin!\n");
REG32(0xb3410050) = tg_en ;
tg_erase(addr2,addr3,addr4,bank);
printk("Common NAND erase done!\n");
#endif
}



void nemc_write_ini(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4,
                           unsigned int bank )
{
//----------------------------------
//set toggle nand CE_n
//----------------------------------
  REG32(0xb3410050) = tg_en ;//bank1 is toggle nand
  printk("nand tg_en is %x\n",REG32(0xb3410050));
  REG8(CmdAddr)  = 0x80 ;
  printk("nand CmdAddr is %x\n",CmdAddr);
  REG8(AddrAddr) = addr0 ;
  printk("nand AddrAddr is %x %x\n",AddrAddr,addr0);
  REG8(AddrAddr) = addr1 ;
  //printk("nand AddrAddr is %x\n",REG8(AddrAddr));
  REG8(AddrAddr) = addr2 ;
  //printk("nand AddrAddr is %x\n",REG8(AddrAddr));
  REG8(AddrAddr) = addr3 ;
  //printk("nand AddrAddr is %x\n",REG8(AddrAddr));
  REG8(AddrAddr) = addr4 ;
  //printk("nand AddrAddr is %x\n",REG8(AddrAddr));
  
  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif
  printk("nemc_write_ini is done\n");
}

#define nemc_plane1_write_ini(addr0, addr1, addr2, addr3, addr4, bank) nemc_write_ini(addr0, addr1, addr2, addr3, addr4, bank)

void nemc_plane2_write_ini(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4,
                           unsigned int bank )
{
//----------------------------------
//set toggle nand CE_n
//----------------------------------
  REG8(CmdAddr)  = 0x81 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif
}


void nemc_write_ck(unsigned int bank)
{
  clr_drs_dws();
  REG8(CmdAddr) = 0x10;

  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif

  //-------------------------------------------------------------------
  // wait toggle nand flash program done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  return 0 ;
}

void nemc_plane1_write_ck(unsigned int bank)
{
  clr_drs_dws();
  REG8(CmdAddr) = 0x11;

  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif

  //-------------------------------------------------------------------
  // wait toggle nand flash program done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  return 0 ;
}

#define nemc_plane2_write_ck(bank) nemc_write_ck(bank)
//-----------------------------------------------
// Toggle Nand Flash
//-----------------------------------------------

unsigned int tg_page_write(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4, unsigned int *page,
    unsigned int bank,
    unsigned int page_size ,
    unsigned int sa ,//spare area
    unsigned int data_seed,
    unsigned int pn_enable
)
{
unsigned int i ,j =0;
/*
srand(data_seed) ;
for (i= 0; i<(page_size+sa); i=i+4,j++) {
  page[j]= rand() % (0xffffffff) ;
}
*/

if((addr2 & 0x80) == 0x00){
	for(j=0 ,i=0; j < (page_size+sa)/4 ; j++,i=i+4){
    	page[j] = (i&0xff)|(((i+1)&0xff)<<8)|(((i+2)&0xff)<<16)|(((i+3)&0xff)<<24) ;
	}
}else{
    for(j=0 ; j < (page_size+sa)/4 ; j++){
        page[j] = j ;
    }
}


//----------------------------------
//set toggle nand flash timing
//----------------------------------
tg_timing_set( bank , 1 ) ;
//----------------------------------
//set toggle nand CE_n
//----------------------------------
REG32(0xb3410050) = tg_en ;//bank1 is toggle nand

//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

//----------------------------------
//erase block 0
//----------------------------------
#ifdef TG_NAND
if(tg_erase(addr2,addr3,addr4,bank)!=0) return 1 ;
#endif
//----------------------------------
//write 8k Byte page
//----------------------------------
  REG8(CmdAddr)  = 0x80 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  if(pn_enable == 1) tg_set_pn() ;

  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif

  for (i= 0; i<(page_size)/4; i++) {
    REG32(DataAddr) = page[i] ;
  }
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  tg_timing_set( bank , 1 ) ;
  j= REG32(0xb341013c) ;
  //REG32(0xb341010c)=0x1<<(bank-1) ; //set write data after next ce
  tg_wcd_check( bank );
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif
  set_drs() ;
  for (; i<(page_size+sa)/4; i++) {
    REG32(DataAddr) = page[i] ;
    tg_timing_set( bank , 1 ) ;
    //j= REG32(0xb341013c) ;
  }
clr_drs_dws();
  if(pn_enable == 1) tg_clr_pn() ;
  REG8(CmdAddr) = 0x10;

#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif

  //-------------------------------------------------------------------
  // wait toggle nand flash program done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  return 0 ;
}


unsigned int tg_two_plane_page_write(
                unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3,  unsigned int addr4,
                unsigned int *page0, unsigned int *page1,
                unsigned int bank,unsigned int page_size, unsigned int sa)
{
unsigned int i ,j =0;

    for(j=0 ,i=0; j < (page_size+sa)/4 ; j++,i=i+4){
        page0[j] = (i&0xff)|(((i+1)&0xff)<<8)|(((i+2)&0xff)<<16)|(((i+3)&0xff)<<24) ;
    }
    for(j=0 ; j < (page_size+sa)/4 ; j++){
        page1[j] = j ;
    }
//----------------------------------
//set toggle nand flash timing
//----------------------------------
tg_timing_set( bank , 1 ) ;
//----------------------------------
//set toggle nand CE_n
//----------------------------------
REG32(0xb3410050) = tg_en ;//bank1 is toggle nand

//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

//----------------------------------
//erase block 0
//----------------------------------
#ifdef TG_NAND
if(tg_erase(addr2,addr3,addr4,bank)!=0) return 1 ;
if(tg_erase(addr2^0x80,addr3,addr4,bank)!=0) return 1 ;
#endif

//----------------------------------
//write 8k Byte page
//----------------------------------
  REG8(CmdAddr)  = 0x80 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  //if(pn_enable == 1) tg_set_pn() ;

  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif

  for (i= 0; i<(page_size+sa)/4; i++) {
    REG32(DataAddr) = page0[i] ;
  }
clr_drs_dws();
  REG8(CmdAddr)  = 0x11 ;

  wait_nf_busy();
  wait_nf_ready();

//----------------------------------
//write 8k Byte page
//----------------------------------
  REG8(CmdAddr)  = 0x81 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2^0x80 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  //if(pn_enable == 1) tg_set_pn() ;

  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif

  for (i= 0; i<(page_size+sa)/4; i++) {
    REG32(DataAddr) = page1[i] ;
  }
clr_drs_dws();
  REG8(CmdAddr)  = 0x10 ;

  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  return 0 ;

}

unsigned int tg_page_write_bch(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4, 
    unsigned int *page,
    unsigned int bank,unsigned int data_cnt ,//unsigned int sa ,//spare area
    unsigned int data_seed
)
{
  unsigned int i;

//----------------------------------
//set toggle nand flash timing
//----------------------------------
tg_timing_set( bank , 1 ) ;
//----------------------------------
//set toggle nand CE_n
//----------------------------------
REG32(0xb3410050) = tg_en ;//bank1 is toggle nand

//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

//----------------------------------
//erase block 0
//----------------------------------
#ifdef TG_NAND
if(tg_erase(addr2,addr3,addr4,bank)!=0) return 1 ;
#endif
//----------------------------------
//write 8k Byte page
//----------------------------------
  REG8(CmdAddr)  = 0x80 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  tg_set_pn() ;

  tg_wcd_check( bank );
  for (i= 0; i<(data_cnt)/4+((((data_cnt)%4)==0)?0:1); i++) {
    REG32(DataAddr) = page[i] ;
  }
clr_drs_dws();
  tg_clr_pn() ;
  REG8(CmdAddr) = 0x10;
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash program done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  return 0 ;
}

unsigned int tg_page_write_port8(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4, 
	unsigned int *page,
    unsigned int bank,unsigned int page_size , unsigned int sa ,//spare area
    unsigned int data_seed,
    unsigned int default_timing
)
{
unsigned int i ,j =0;
unsigned int tmp ;

/*
  srand(data_seed) ;
  for (i= 0; i<(8192+sa); i=i+4,j++) {
    page[j]= rand() % (0xffffffff) ;
  }
*/
for(j=0 ,i=0; j < (page_size+sa)/4 ; j++,i=i+4){
    page[j] = (i&0xff)|(((i+1)&0xff)<<8)|(((i+2)&0xff)<<16)|(((i+3)&0xff)<<24) ;
    //printd(j);
	//printx(page[j]);
}

//----------------------------------
//set toggle nand flash timing
//----------------------------------
if(default_timing ==0 ) tg_timing_set( bank , 1 ) ;
//----------------------------------
//set toggle nand CE_n
//----------------------------------
REG32(0xb3410050) = tg_en ;//bank1 is toggle nand

//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

//----------------------------------
//erase block 0
//----------------------------------
#ifdef TG_NAND
if(tg_erase(addr2,addr3,addr4,bank)!=0) return 1 ;
#endif
//----------------------------------
//write 8k Byte page
//----------------------------------
  REG8(CmdAddr)  = 0x80 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
//  tg_set_pn() ;

  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif
  for (i= 0; i<(page_size)/4; i++) {
    tmp = page[i] ;
    REG8(DataAddr) = tmp ;
	REG8(DataAddr) = tmp>>8 ;
    REG8(DataAddr) = tmp>>16 ;
    REG8(DataAddr) = tmp>>24 ;
  }
  for (; i<(page_size+sa)/4; i++) {
    tmp = page[i] ;
    REG8(DataAddr) = tmp ;
    REG8(DataAddr) = tmp>>8 ;
    REG8(DataAddr) = tmp>>16 ;
    REG8(DataAddr) = tmp>>24 ;
  }
clr_drs_dws();
  REG8(CmdAddr) = 0x10;
#ifdef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash program done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  return 0 ;
}

unsigned int tg_page_write_port16(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4, unsigned int *page,
    unsigned int bank,unsigned int page_size, unsigned int sa ,//spare area
    unsigned int data_seed
)
{
unsigned int i ,j =0;
unsigned int tmp ;

  srand(data_seed) ;
  for (i= 0; i<(page_size+sa)/4; i=i+4,j++) {
    page[j]= rand() % (0xffffffff) ;
  }

//----------------------------------
//set toggle nand flash timing
//----------------------------------
tg_timing_set( bank , 1 ) ;
//----------------------------------
//set toggle nand CE_n
//----------------------------------
REG32(0xb3410050) = tg_en ;//bank1 is toggle nand

//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

//----------------------------------
//erase block 0
//----------------------------------
#ifdef TG_NAND
if(tg_erase(addr2,addr3,addr4,bank)!=0) return 1 ;
#endif
//----------------------------------
//write 8k Byte page
//----------------------------------
  REG8(CmdAddr)  = 0x80 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  //tg_set_pn() ;

  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif
  for (i= 0; i<(page_size)/4; i++) {
    tmp = page[i] ;
    REG16(DataAddr) = tmp ;
    REG16(DataAddr) = tmp>>16 ;
  }
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  tg_timing_set( bank , 1 ) ;
  j= REG32(0xb341013c) ;
  //REG32(0xb341010c)=0x1<<(bank-1) ; //set write data after next ce
  tg_wcd_check( bank );
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif
  set_drs() ;
  for (; i<(page_size+sa)/4; i++) {
    tmp = page[i] ;
    REG16(DataAddr) = tmp ;
    REG16(DataAddr) = tmp>>16 ;
    tg_timing_set( bank , 1 ) ;
    j= REG32(0xb341013c) ;
  }
clr_drs_dws();
  //tg_clr_pn() ;
  REG8(CmdAddr) = 0x10;
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash program done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  return 0 ;
}


unsigned int tg_dma_page_write(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4, unsigned int *page,
    unsigned int bank,unsigned int sa ,//spare area
    unsigned int data_seed,
    unsigned int tc ,
    unsigned int sur_port_width , //source port width. 00:32bit ; 01:8bit ; 10:16bit
    unsigned int des_port_width   //destination port width. 00:32bit ; 01:8bit ; 10:16bit

)
{
  //unsigned int i ,j =0;

unsigned int sur_addr       = 0x13423000 ;//(int)page     ^ 0xa0000000; //source address
unsigned int des_addr       = DataAddr ^ 0xa0000000; //target address
//unsigned int tc             = 8192 ; //transfer count, Byte
unsigned int req_type       = 0x8  ; //request types

unsigned int sur_id         = 0; //source identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
unsigned int des_id         = 1; //destination identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
unsigned int sur_addr_inc   = 1; //source address increment. 0:no increment ; 1:increment
unsigned int des_addr_inc   = 0; //destination address increment. 0:no increment ; 1:increment

  printd(1134) ;
//----------------------------------
//set toggle nand flash timing
//----------------------------------
tg_timing_set( bank ,1) ;
//----------------------------------
//set toggle nand CE_n
//----------------------------------
REG32(0xb3410050) = tg_en ;//bank1 is toggle nand

//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

//----------------------------------
//erase block 0
//----------------------------------
#ifdef TG_NAND
if(tg_erase(addr2,addr3,addr4,bank)!=0) return 1 ;
#endif
//----------------------------------
//write 8k Byte page
//----------------------------------
  REG8(CmdAddr)  = 0x80 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  //tg_set_pn() ;
  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif

#ifdef EN_PNDR
  tg_set_pn() ;
#endif

#ifdef EN_BITCNT
  counter_set() ;
#endif

  pdma_cfg(
       sur_addr       , //source address
       des_addr       , //target address
       tc             , //transfer count
       req_type       , //request types
  
       sur_id         , //source identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       des_id         , //destination identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       sur_addr_inc   , //source address increment. 0:no increment ; 1:increment
       des_addr_inc   , //destination address increment. 0:no increment ; 1:increment
       sur_port_width , //source port width. 00:32bit ; 01:8bit ; 10:16bit
       des_port_width   //destination port width. 00:32bit ; 01:8bit ; 10:16bit
  );
  pdma_check_tt() ;
clr_drs_dws();
#ifdef EN_PNDR
tg_clr_pn() ;
#endif

  REG8(CmdAddr) = 0x10;
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash program done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  return 0 ;
}

//NOTE. in Toggle NAND flash , hstrb is 4'b1111 or 4'b0011, Toggle NAND can NOT write 3byte or 1byte
unsigned int tg_dma_page_hstrb_write(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4, unsigned int *page,
    unsigned int bank,unsigned int sa ,//spare area
    unsigned int data_seed,
    unsigned int tc ,
    unsigned int sur_port_width , //source port width. 00:32bit ; 01:8bit ; 10:16bit
    unsigned int des_port_width , //destination port width. 00:32bit ; 01:8bit ; 10:16bit
    unsigned int hstrb
)
{
  //unsigned int i ,j =0;

unsigned int sur_addr       = 0x13423000 ;//(int)page     ^ 0xa0000000; //source address
unsigned int des_addr       = DataAddr ^ 0xa0000000; //target address
unsigned int tmp_tc         = 128-(4-hstrb) ;
unsigned int req_type       = 0x8  ; //request types

unsigned int sur_id         = 0; //source identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
unsigned int des_id         = 1; //destination identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
unsigned int sur_addr_inc   = 1; //source address increment. 0:no increment ; 1:increment
unsigned int des_addr_inc   = 0; //destination address increment. 0:no increment ; 1:increment
unsigned int *tmp_p = (unsigned int *)(0x13423000|0xa0000000) ;
  printd(1134) ;
//----------------------------------
//set toggle nand flash timing
//----------------------------------
tg_timing_set( bank ,1) ;
//----------------------------------
//set toggle nand CE_n
//----------------------------------
REG32(0xb3410050) = tg_en ;//bank1 is toggle nand
//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

//----------------------------------
//erase block 0
//----------------------------------
#ifdef TG_NAND
if(tg_erase(addr2,addr3,addr4,bank)!=0) return 1 ;
#endif
//----------------------------------
//write 8k Byte page
//----------------------------------
  REG8(CmdAddr)  = 0x80 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  //tg_set_pn() ;

  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif
  //-------------------------------------------------------
  //use PDMA data input 128-2=126 byte
  //-------------------------------------------------------
  pdma_cfg(
       sur_addr       , //source address
       des_addr       , //target address
       tmp_tc         , //transfer count
       req_type       , //request types

       sur_id         , //source identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       des_id         , //destination identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       sur_addr_inc   , //source address increment. 0:no increment ; 1:increment
       des_addr_inc   , //destination address increment. 0:no increment ; 1:increment
       sur_port_width , //source port width. 00:32bit ; 01:8bit ; 10:16bit
       des_port_width   //destination port width. 00:32bit ; 01:8bit ; 10:16bit
  );
  pdma_check_tt() ;
  //input the 127th BYTE & 128th BYTE
  //input the 127th BYTE & 128th BYTE
  if((4-hstrb) == 1){
    REG8(DataAddr) = (tmp_p[128/4-1] & 0xff000000)>>(16+8);
  }
  if((4-hstrb) == 2){
    REG8(DataAddr) = (tmp_p[128/4-1] & 0x00ff0000)>>16;
    REG8(DataAddr) = (tmp_p[128/4-1] & 0xff000000)>>(16+8);
  }
  if((4-hstrb) == 3){
    REG8(DataAddr) = (tmp_p[128/4-1] & 0x0000ff00)>>8;
    REG8(DataAddr) = (tmp_p[128/4-1] & 0x00ff0000)>>16;
    REG8(DataAddr) = (tmp_p[128/4-1] & 0xff000000)>>(16+8);
  }

  //-------------------------------------------------------
  //use PDMA data input 128-2=126 byte
  //-------------------------------------------------------
  sur_addr       = 0x13423000+(128) ;//(int)page     ^ 0xa0000000; //source address
  pdma_cfg(
       sur_addr       , //source address
       des_addr       , //target address
       tmp_tc         , //transfer count
       req_type       , //request types

       sur_id         , //source identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       des_id         , //destination identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       sur_addr_inc   , //source address increment. 0:no increment ; 1:increment
       des_addr_inc   , //destination address increment. 0:no increment ; 1:increment
       sur_port_width , //source port width. 00:32bit ; 01:8bit ; 10:16bit
       des_port_width   //destination port width. 00:32bit ; 01:8bit ; 10:16bit
  );
  pdma_check_tt() ;
  //REG8(DataAddr) = (tmp_p[256/4-1] & 0x00ff0000)>>16;
  //REG8(DataAddr) = (tmp_p[256/4-1] & 0xff000000)>>(16+8);

  if((4-hstrb) == 1){
    REG8(DataAddr) = (tmp_p[256/4-1] & 0xff000000)>>(16+8);
  }
  if((4-hstrb) == 2){
    REG8(DataAddr) = (tmp_p[256/4-1] & 0x00ff0000)>>16;
    REG8(DataAddr) = (tmp_p[256/4-1] & 0xff000000)>>(16+8);
  }
  if((4-hstrb) == 3){
    REG8(DataAddr) = (tmp_p[256/4-1] & 0x0000ff00)>>8;
    REG8(DataAddr) = (tmp_p[256/4-1] & 0x00ff0000)>>16;
    REG8(DataAddr) = (tmp_p[256/4-1] & 0xff000000)>>(16+8);
  }

  //-------------------------------------------------------
  //use PDMA data input tc(8192)-128-128 byte
  //-------------------------------------------------------
  sur_addr       = 0x13423000+(128)+(128) ;//(int)page     ^ 0xa0000000; //source address
  tmp_tc         = tc-128-128 ;
  pdma_cfg(
       sur_addr       , //source address
       des_addr       , //target address
       tmp_tc         , //transfer count
       req_type       , //request types

       sur_id         , //source identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       des_id         , //destination identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       sur_addr_inc   , //source address increment. 0:no increment ; 1:increment
       des_addr_inc   , //destination address increment. 0:no increment ; 1:increment
       sur_port_width , //source port width. 00:32bit ; 01:8bit ; 10:16bit
       des_port_width   //destination port width. 00:32bit ; 01:8bit ; 10:16bit
  );
  pdma_check_tt() ;
clr_drs_dws();
  REG8(CmdAddr) = 0x10;
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash program done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  return 0 ;
}


unsigned int tg_random_page_write(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4, unsigned int *page,
    unsigned int bank,unsigned int sa ,//spare area
    unsigned int data_seed
)
{
unsigned int i ,j =0;

  srand(data_seed) ;
  for (i= 0; i<(8192+sa); i=i+4,j++) {
    page[j]= rand() % (0xffffffff) ;
  }

//----------------------------------
//set toggle nand flash timing
//----------------------------------
//REG32(0xb3410110+(bank-1)*4) = 0x3f0001f1    ;
//REG32(0xb3410128) = 0x00000002<<((bank-1)*4) ;//set spacail reg
tg_timing_set( bank ,1) ;
//----------------------------------
//set toggle nand CE_n
//----------------------------------
REG32(0xb3410050) = tg_en ;//bank1 is toggle nand

//----------------------------------
//reset toggle nand
//----------------------------------
tg_rst() ;

//----------------------------------
//erase block 0
//----------------------------------
#ifdef TG_NAND
if(tg_erase(addr2,addr3,addr4,bank)!=0) return 1 ;
#endif
//----------------------------------
//write 8k Byte page
//----------------------------------
  REG8(CmdAddr)  = 0x80 ;
//add by mark, FIX ME
  //REG32(0xb3410144) = 0x00aaaaaa ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;

//----------------------------------
//random write spare area
//----------------------------------
  REG8(CmdAddr)  = 0x85 ;
  #ifdef TG_NAND
  tg_wait_cwaw() ;
  #endif
  REG8(AddrAddr) = 0x00 ;
  REG8(AddrAddr) = 0x20 ;
  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif
  for (i= 8192/4; i<(8192+sa)/4; i++) {
    REG32(DataAddr) = page[i] ;
  }
clr_drs_dws();
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif

//----------------------------------
//random write 8k page
//----------------------------------
  REG8(CmdAddr)  = 0x85 ;
#ifdef TG_CDQSH
tg_cdqsh_done_check();
#endif

  #ifdef TG_NAND
  tg_wait_cwaw() ;
  #endif
  REG8(AddrAddr) = 0x00 ;
  REG8(AddrAddr) = 0x00 ;
  #ifdef TG_NAND
  tg_wcd_check( bank );
  #endif
  for (i= 0; i<(8192)/4; i++) {
    REG32(DataAddr) = page[i] ;
  }
clr_drs_dws();
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//0x00010001;
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ; //0x00010003;
  #endif
#endif
  REG8(CmdAddr) = 0x10;
#ifdef TG_CDQSH
tg_cdqsh_done_check();
#endif


  //-------------------------------------------------------------------
  // wait toggle nand flash program done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  if(tg_read_stat(0x70,0xe0,bank)!=0) return 1;
  //REG32(0xb3410050) = tg_dis_en ;
  //tg_ce_done_check() ;
  //REG32(0xb3410050) = tg_en ;
  return 0 ;
}


void nemc_read_ini(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4,
                           unsigned int bank )
{
  REG32(0xb3410050) = tg_en ; 
  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;

  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

  #ifdef TG_NAND
  set_drs();
  #endif
}

void nemc_plane1_read_ini(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4,
                          unsigned int bank )
{
  unsigned int addr2_plane2 = addr2 | 0x80 ;
  //tg_rst() ;

  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 0 )
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x32 ;
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 1 )
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2_plane2 ;
//  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 0 )
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x05 ;

//  REG8(AddrAddr) = addr0 ;
//  REG8(AddrAddr) = addr1 ;
//  REG8(CmdAddr)  = 0xe0 ;

}

void nemc_plane2_read_ini(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3, unsigned int addr4,
                          unsigned int bank )
{
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 1 )
  //-------------------------------------------------------------------
  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x05 ;

  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(CmdAddr)  = 0xe0 ;
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif
}

void nemc_read_ck()
{
  REG32(0xb3410050) = tg_dis_en|DAEC ;//disable toggle nand
  tg_ce_done_check() ;
}

void nemc_plane1_read_ck()
{
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en|DAEC ;//disable toggle nand
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ;
  #endif
}

#define nemc_plane2_read_ck() nemc_read_ck()

unsigned int tg_page_read(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3,  unsigned int addr4, unsigned int *page,
                unsigned int bank,unsigned int page_size , unsigned int sa ,
				unsigned int pn_enable ,//set 1 then enable PN 
				unsigned int bit_enable //set 1 then enable bit counter
)
{
  unsigned int i , j;
  unsigned int tmp [(8192+1024)/4] ;
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;

#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

#ifndef TG_CDQSH
  #ifdef TG_NAND
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ;
  #endif
#endif
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
  if(pn_enable == 1) tg_set_pn() ;
  if(bit_enable == 1) counter_set() ;

  tg_timing_set( bank ,0) ;
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (i= 0; i<(page_size)/4; i++) {
    tmp[i] = REG32(DataAddr) ;
  }
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  tg_timing_set( bank ,0) ;
  j= REG32(0xb341013c) ;

  REG32(0xb3410050) = tg_en ;
  #endif
#endif
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (; i<(page_size+sa)/4; i++) {
    tmp[i] = REG32(DataAddr) ;//try to distrub data read
    j= REG32(0xb341013c) ;
  }
clr_drs_dws();
  if(bit_enable == 1){
	if(REG32(0xb3410108) != (page_size+sa)*8){
		printx(0xdeaddead);
		return 1;
	}
	counter_clr() ;
  }
  if(pn_enable == 1) tg_clr_pn() ;

  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  #endif

  for(i=0; i<(page_size+sa)/4; i++ ){
    if(page[i]!=tmp[i]) {printd(i) ;printx(tmp[i]);printx(page[i]);printx(0xdead0001); return 1 ;}
  }
  printd(1234);
  return 0 ;
}



unsigned int tg_page_read_bch(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3,  unsigned int addr4, 
                unsigned int *page,unsigned int *rd_page,
                unsigned int bank,unsigned int data_cnt)
{
unsigned int i;
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

#ifndef TG_CDQSH
  #ifdef TG_NAND
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ;
  #endif
#endif
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
  tg_set_pn() ;
  counter_set() ;
  tg_timing_set( bank ,0) ;
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (i= 0; i<(data_cnt)/4+((((data_cnt)%4)==0)?0:1); i++) {
    rd_page[i] = REG32(DataAddr) ;
  }
clr_drs_dws();
  counter_clr() ;
  tg_clr_pn() ;

  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  #endif

  for(i=0; i<(data_cnt)/4; i++ ){
    //printd(i) ;
    //printx(rd_page[i]);
    //printx(page[i]);
    if(page[i]!=rd_page[i]) {printd(i) ;printx(rd_page[i]);printx(page[i]);printx(0xdead0001); return 1 ;}
  }

  if((data_cnt)%4==3){
    printd(i) ;
    printx(rd_page[i]);
    printx(page[i]);
    if((page[i]&0x00ffffff)!=(rd_page[i]&0x00ffffff)) {printx(0xdead0001); return 1 ;}
  }
  if((data_cnt)%4==2){
    printd(i) ;
    printx(rd_page[i]);
    printx(page[i]);
    if((page[i]&0x0000ffff)!=(rd_page[i]&0x0000ffff)) {printx(0xdead0001); return 1 ;}
  }
  if((data_cnt)%4==1){
    printd(i) ;
    printx(rd_page[i]);
    printx(page[i]);
    if((page[i]&0x000000ff)!=(rd_page[i]&0x000000ff)) {printx(0xdead0001); return 1 ;}
  }
  printd(1234);
  return 0 ;
}

unsigned int tg_page_read_port8(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3,  unsigned int addr4, 
				unsigned int *page,
                unsigned int bank,unsigned int page_size,unsigned int sa,
                unsigned int default_timing
)
{
unsigned int i;
unsigned int tmp [(8192+1024)/4] ;
unsigned int buf ;
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();
#ifndef TG_CDQSH
  #ifdef TG_NAND
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ;
  #endif
#endif
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
  if(default_timing==0)  tg_timing_set( bank ,0) ;
  printd(552) ;
  for(i=0;i<16;i++)
  {
      printd(i);
      printx(page[i]);
  }

  printd(90);
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (i= 0; i<(page_size)/4; i++) {
	buf = REG8(DataAddr) ;
    buf = ((0xff & REG8(DataAddr))<<8) | buf ;
    buf = ((0xff & REG8(DataAddr))<<16) | buf ;
    buf = ((0xff & REG8(DataAddr))<<24) | buf ;
    tmp[i] = buf ;
  }

  for (; i<(page_size+sa)/4; i++) {
    buf = REG8(DataAddr) ;
    buf = ((0xff & REG8(DataAddr))<<8) | buf ;
    buf = ((0xff & REG8(DataAddr))<<16) | buf ;
    buf = ((0xff & REG8(DataAddr))<<24) | buf ;
    tmp[i] = buf ;
  }
clr_drs_dws();
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  #endif
  for(i=0; i<(page_size+sa)/4; i++ ){
    if(page[i]!=tmp[i]) {
		printd(i) ;
		printx(tmp[i]);
		printx(page[i]);
		printx(0xdead0001); 
		return 1 ;
	}
  }
  printd(1234);
  return 0 ;
}

unsigned int tg_page_read_port16(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3,  unsigned int addr4, unsigned int *page,
                unsigned int bank,unsigned int page_size,unsigned int sa)
{
unsigned int i , j;
unsigned int tmp [(8192+1024)/4] ;
unsigned int buf ;
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();

#ifndef TG_CDQSH
  #ifdef TG_NAND
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ;
  #endif
#endif
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
  //tg_set_pn() ;
  //counter_set() ;
  tg_timing_set( bank ,0) ;
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (i= 0; i<(page_size)/4; i++) {
    buf = REG16(DataAddr) ;
    buf = ((0xffff & REG16(DataAddr))<<16) | buf ;
    tmp[i] = buf ;
  }

#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  tg_timing_set( bank ,0) ;
  j= REG32(0xb341013c) ;

  REG32(0xb3410050) = tg_en ;
  #endif
#endif
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (; i<(page_size+sa)/4; i++) {
    buf = REG16(DataAddr) ;
    buf = ((0xffff & REG16(DataAddr))<<16) | buf ;
    tmp[i] = buf ;
    j= REG32(0xb341013c) ;
  }
clr_drs_dws();
  //counter_clr() ;
  //tg_clr_pn() ;

  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  //REG32(0xb3410050) = tg_en ;
  #endif

  for(i=0; i<(page_size+sa)/4; i++ ){
    //printd(i) ;
    //printx(tmp[i]);
    //printx(page[i]);
    if(page[i]!=tmp[i]) {printd(i) ;printx(tmp[i]);printx(page[i]);printx(0xdead0001); return 1 ;}
  }
  printd(1234);
  return 0 ;
}


unsigned int tg_dma_page_read(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3,  unsigned int addr4, unsigned int *page,
                unsigned int bank,unsigned int sa,
//for pdma
unsigned int des_addr    ,
unsigned int tc ,
unsigned int sur_port_width , //source port width. 00:32bit ; 01:8bit ; 10:16bit
unsigned int des_port_width   //destination port width. 00:32bit ; 01:8bit ; 10:16bit

)
{
  unsigned int i;
  //unsigned int tmp [(8192+1024)/4] ;

unsigned int * des_p = (unsigned int *)(des_addr | 0xa0000000);

unsigned int sur_addr       = DataAddr ^ 0xa0000000;//source address
//unsigned int des_addr       = (int)page^ 0xa0000000;//target address
//unsigned int tc             = 8192 ; //transfer count, Byte
unsigned int req_type       = 0x8  ; //request types

unsigned int sur_id         = 1; //source identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
unsigned int des_id         = 0; //destination identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
unsigned int sur_addr_inc   = 0; //source address increment. 0:no increment ; 1:increment
unsigned int des_addr_inc   = 1; //destination address increment. 0:no increment ; 1:increment
//unsigned int sur_port_width = 0; //source port width. 00:32bit ; 01:8bit ; 10:16bit
//unsigned int des_port_width = 0; //destination port width. 00:32bit ; 01:8bit ; 10:16bit

  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  #endif
#endif
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();
#ifndef TG_CDQSH
  #ifdef TG_NAND
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ;
  #endif
#endif
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
#ifdef EN_PNDR
  tg_set_pn() ;
#endif

#ifdef EN_BITCNT
  counter_set() ;
#endif
  tg_timing_set( bank ,0) ;
  pdma_cfg(
       sur_addr       , //source address
       des_addr       , //target address
       tc             , //transfer count
       req_type       , //request types

       sur_id         , //source identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       des_id         , //destination identification. 0:TCSM ; 1:BCH/NEMC ; 2:DDR ;
       sur_addr_inc   , //source address increment. 0:no increment ; 1:increment
       des_addr_inc   , //destination address increment. 0:no increment ; 1:increment
       sur_port_width , //source port width. 00:32bit ; 01:8bit ; 10:16bit
       des_port_width   //destination port width. 00:32bit ; 01:8bit ; 10:16bit
  );
  pdma_check_tt() ;
clr_drs_dws();
#ifdef EN_BITCNT
  if(REG32(0xb3410108) != (tc)*8){
      printx(0xdeaddead);
      return 1;
  }
  counter_clr() ;
#endif

#ifdef EN_PNDR
  tg_clr_pn() ;
#endif

  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  #ifdef TG_NAND
  tg_ce_done_check() ;
  #endif

  for(i=0; i<(tc)/4; i++ ){
    //printd(i) ;
    //printx(tmp[i]);
    //printx(page[i]);
    if(page[i]!=des_p[i]) {printd(i) ;printx(des_p[i]);printx(page[i]);printx(0xdead0001); return 1 ;}
  }

  printd(112233) ;
  return 0 ;
}


unsigned int tg_two_plane_page_read(
                unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3,  unsigned int addr4,
                unsigned int *page0, unsigned int *page1,
                unsigned int bank,unsigned int block_size, unsigned int sa)
{
unsigned int i;
unsigned int tmp0 [(8192+1024)/4] ;
unsigned int tmp1 [(8192+1024)/4] ;
unsigned int addr2_plane2 = addr2 | 0x80 ;
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 0 )
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x32 ;
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 1 )
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2_plane2 ;
//  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 0 )
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x05 ;

  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(CmdAddr)  = 0xe0 ;
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
  //tg_timing_set( bank ,0) ;
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (i= 0; i<(block_size+sa)/4; i++) {
    tmp0[i] = REG32(DataAddr) ;
  }
  clr_drs_dws();
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ;
  #endif

  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 1 )
  //-------------------------------------------------------------------
  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2_plane2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x05 ;

  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(CmdAddr)  = 0xe0 ;
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (i= 0; i<(block_size+sa)/4; i++) {
    tmp1[i] = REG32(DataAddr) ;
  }
  clr_drs_dws();
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  #endif

  //check plane0
  for(i=0; i<(block_size+sa)/4; i++ ){
    if(page0[i]!=tmp0[i]) {printd(i) ; printx(tmp0[i]); printx(page0[i]) ; printx(0xdead0001); return 1 ;}
  }

  //check plane1
  for(i=0; i<(block_size+sa)/4; i++ ){
    if(page1[i]!=tmp1[i]) {printd(i) ; printx(tmp1[i]); printx(page1[i]) ; printx(0xdead0002); return 1 ;}
  }

}

unsigned int tg_two_plane_secondary_page_read(
                unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3,  unsigned int addr4,
                unsigned int *page0, unsigned int *page1,
                unsigned int bank,unsigned int sa)
{
unsigned int i;
//unsigned int tmp0 [(8192+1024)/4] ;
unsigned int tmp1 [(8192+1024)/4] ;
unsigned int addr2_plane2 = addr2 | 0x80 ;
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 0 )
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x60 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x60 ;
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 1 )
  //-------------------------------------------------------------------
  REG8(AddrAddr) = addr2_plane2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;
  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 0 )
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2_plane2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x05 ;

  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(CmdAddr)  = 0xe0 ;
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
  tg_timing_set( bank ,0) ;
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (i= 0; i<(8192+sa)/4; i++) {
    tmp1[i] = REG32(DataAddr) ;
  }
  clr_drs_dws();
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  #endif

/*
#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ;
  #endif
#endif
  //-------------------------------------------------------------------
  // read toggle nand flash ( plane 1 )
  //-------------------------------------------------------------------
  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2_plane2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x05 ;

  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(CmdAddr)  = 0xe0 ;
  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------
  for (i= 0; i<(8192+sa)/4; i++) {
    tmp1[i] = REG32(DataAddr) ;
  }
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  #endif
  //check plane0
  for(i=0; i<(8192+sa)/4; i++ ){
    printd(i) ;
    printx(tmp0[i]);
    printx(page0[i]);
    if(page0[i]!=tmp0[i]) {printx(0xdead0001); return 1 ;}
  }
*/
  //check plane1
  for(i=0; i<(8192+sa)/4; i++ ){
    printd(i) ;
    printx(tmp1[i]);
    printx(page1[i]);
    if(page1[i]!=tmp1[i]) {printx(0xdead0002); return 1 ;}
  }
}


unsigned int tg_random_page_read(unsigned int addr0, unsigned int addr1, unsigned int addr2, unsigned int addr3,  unsigned int addr4, unsigned int *page,
                unsigned int bank,unsigned int sa)
{
unsigned int i ;
unsigned int tmp [(8192+1024)/4] ;

  //-------------------------------------------------------------------
  // read toggle nand flash
  //-------------------------------------------------------------------

  REG8(CmdAddr)  = 0x00 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(AddrAddr) = addr2 ;
  REG8(AddrAddr) = addr3 ;
  REG8(AddrAddr) = addr4 ;
  REG8(CmdAddr)  = 0x30 ;

  //-------------------------------------------------------------------
  // wait toggle nand flash read done
  //-------------------------------------------------------------------
  wait_nf_busy();
  wait_nf_ready();
  //-------------------------------------------------------------------
  // read toggle nand flash spare area
  //-------------------------------------------------------------------
  //-------------------------------------------------------------------
  //Random Read spare area
  //-------------------------------------------------------------------
  REG8(CmdAddr)  = 0x05 ;
  REG8(AddrAddr) = 0x00 ;
  REG8(AddrAddr) = 0x20 ;
  REG8(CmdAddr)  = 0xe0 ;
  tg_timing_set( bank ,0) ;
  #ifdef TG_NAND
  fidle_check() ;
  set_drs();
  #endif

  for (i= 8192/4; i<(8192+sa)/4; i++) {
    tmp[i] = REG32(DataAddr) ;
  }
  clr_drs_dws();
//#ifndef TG_CDQSH
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  REG32(0xb3410050) = tg_en ;
  #endif
//#endif
  //-------------------------------------------------------------------
  //Random Read
  //-------------------------------------------------------------------
  REG8(CmdAddr)  = 0x05 ;
  REG8(AddrAddr) = addr0 ;
  REG8(AddrAddr) = addr1 ;
  REG8(CmdAddr)  = 0xe0 ;
  #ifdef TG_NAND
	fidle_check() ;
  set_drs();
  #endif

  for (i= 0; i<(8192)/4; i++) {
    tmp[i] = REG32(DataAddr) ;
  }
  clr_drs_dws();
  #ifdef TG_NAND
  REG32(0xb3410050) = tg_dis_en ;//disable toggle nand
  tg_ce_done_check() ;
  #endif
  for(i=0; i<(8192+sa)/4; i++ ){
    printd(i) ;
    printx(tmp[i]);
    printx(page[i]);
    if(page[i]!=tmp[i]) {printx(0xdead0001); return 1 ;}
  }
  //printd(1234);
  return 0 ;
}

#endif
//
