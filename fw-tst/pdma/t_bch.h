#ifndef __BCH_H__
#define __BCH_H__

#include "server.h"
#include <stdio.h>
#include <stdlib.h>

#define REG_ONE      0XFFFFFFFF
#define REG_ZERO     0X00000000
//define by mark
#define set_bhcr() REG32((BCH_BASE + 0x0)) = 0x000103c5 
#define set_bhcsr() REG32(BCH_BASE + 0x4) = 0x8
#define set_bhcnt() REG32((BCH_BASE + 0xc)) = (0x69<<16)|0x400
#define set_bhdr() REG32(BCH_BASE + 0x10) = 0x12345678

volatile unsigned int INT_SYN , INT_DEC , INT_ENC ;
volatile unsigned int BCHINT ,BCH_DEC_INT;
//#define rd_bhint()  return (REG32(BCH_BASE + 0x184)) 
#define disable_int() REG32(BCH_BASE + 0x190) = 0x0

#define check_bch_reg_reset_value()			\
({							\
  if (REG32(BHCR)   != REG_ZERO  ||			\
      REG32(BHCNT)  != (REG_ZERO | ENC_CNT_INIT | DEC_CNT_INIT) ||			\
      REG32(BHPAR0)   != REG_ZERO  ||			\
      REG32(BHPAR1)   != REG_ZERO  ||			\
      REG32(BHPAR2)   != REG_ZERO  ||			\
      REG32(BHPAR3)   != REG_ZERO  ||			\
      REG32(BHPAR4)   != REG_ZERO  ||			\
      REG32(BHPAR5)   != REG_ZERO  ||			\
      REG32(BHPAR6)   != REG_ZERO  ||			\
      REG32(BHINT)   != REG_ZERO  ||			\
      REG32(BHINTE)   != REG_ZERO  ||			\
      REG32(BHERR0)   != REG_ZERO  ||			\
      REG32(BHERR1)   != REG_ZERO  ||			\
      REG32(BHERR2)   != REG_ZERO  ||			\
      REG32(BHERR3)   != REG_ZERO  ||			\
      REG32(BHERR4)   != REG_ZERO  ||			\
      REG32(BHERR5)   != REG_ZERO  ||			\
      REG32(BHERR6)   != REG_ZERO  ||			\
      REG32(BHERR7)   != REG_ZERO) return 1;})

#define set_bhcr_f_value()             REG32(BHCSR)   = REG_ONE
#define set_bhcr_0_value()             REG32(BHCCR)   = REG_ONE
#define check_bhcr_f_value()           if(REG32(BHCR)!= BHCR_SET_F)  return 1
#define check_bhcr_0_value()           if(REG32(BHCR)!= REG_ZERO)  return 1

#define set_bhcnt_f_value()             REG32(BHCNT)   = REG_ONE
#define set_bhcnt_0_value()             REG32(BHCNT)   = REG_ZERO
#define check_bhcnt_f_value()           if(REG32(BHCNT)!= BHCNT_SET_F)  return 1
#define check_bhcnt_0_value()           if(REG32(BHCNT)!= REG_ZERO)  return 1

#define set_bhinte_f_value()            REG32(BHINTES)   = REG_ONE
#define set_bhinte_0_value()            REG32(BHINTEC)   = REG_ONE
#define check_bhinte_f_value()          if(REG32(BHINTE)!= BHINTE_SET_F)  return 1
#define check_bhinte_0_value()          if(REG32(BHINTE)!= REG_ZERO)  return 1

#define check_bhint_decf(val)          if(REG32(BHINT)!= val)  return 1

#define load_bch_byte(s_addr, d_addr, count)	\
({									\
  int i, j, tmp;		\
  for(i = 0; i<count; i++)      \
   { \
    tmp = i_l8u(s_addr + i,0);  \
    i_s8(tmp, d_addr, 0);	\
   } \
})

#define check_bch_byte(d_addr, val1, val2, val3, val4)	\
({									\
  int i, j, val, tmp, tmp1, tmp2;								\
  for(i = 1; i<5; i++)						\
   { if (i == 1) val = val1;           \
     else if (i == 2) val = val2;           \
     else if (i == 3) val = val3;           \
     else if (i == 4) val = val4;           \
     else val = 0; \
     for(j = 0; j <4; j++) \
      {tmp = i_l8u(d_addr + j + (i-1)*4,0);   \
       tmp1 = (val >> j*8);                 \
       tmp2 = tmp1 & 0xff;                  \
       if(tmp != tmp2) {show_long(d_addr + j + (i-1)*4); show_long(j); show_long(tmp); show_long(tmp2); return 1;}}} \
})

#define set_bch_enc_vbr()           \
({                      \
  setvbr(TP_INT | INTC_BCH, bch_enc_handler);   \
})

#endif
unsigned int align_datablock(unsigned int *din , unsigned int blocksize , unsigned int *pin , unsigned int paritysize )
{
  if((blocksize % 4) != 2) 
    return 1 ;
  if((blocksize % 4) == 0) 
    return 0 ;
}

void bch_set_ctrl(
	unsigned int tag ,
	unsigned int zero_cnt ,
	unsigned int bypass ,
	unsigned int dma_enable ,
	unsigned int sel_ecc_level ,
	unsigned int fast_dec ,
	unsigned int init ,
	unsigned int enc_enable ,
	unsigned int bch_enable
)
{
	unsigned int tmp ;
	tmp = (tag<<16)|(zero_cnt<<13)|(bypass<<12)|(dma_enable<<11)|(sel_ecc_level<<4)|(fast_dec<<3)|(enc_enable<<2)|(init<<1)|(bch_enable<<0) ;
    printd(1010);
    printx(tmp);
	REG32(BCH_BASE + 0x0) = tmp ;
}

void bch_set_init()
{
	REG32(BCH_BASE + 0x4) = 0x2 ;	
}

void bch_set_size(
	unsigned int blocksize ,
	unsigned int paritysize 
)
{
	unsigned int tmp ;
	tmp = (paritysize << 16)|blocksize ;
	REG32(BCH_BASE + 0xc) = tmp ;
}

void bch_data_input(
	unsigned int data 
)
{
#ifdef BCH_HSIZE_B
REG8(BCH_BASE + 0x10) = data&0xff ;
REG8(BCH_BASE + 0x10) = (data>>8)&0xff ;
REG8(BCH_BASE + 0x10) = (data>>16)&0xff ;
REG8(BCH_BASE + 0x10) = (data>>24)&0xff ;
#endif

#ifdef BCH_HSIZE_HW
REG16(BCH_BASE + 0x10) = (data)&0xffff ;
REG16(BCH_BASE + 0x10) = (data>>16)&0xffff ;
#endif

#ifndef BCH_HSIZE_B
#ifndef BCH_HSIZE_HW
	REG32(BCH_BASE + 0x10) = data ;
#endif
#endif
}

unsigned int bch_check_finish_int()  
{
	unsigned int tmp ;
	tmp = REG32(BCH_BASE + 0x184) ;
	return tmp ;
}

void bch_set_int_enable(
    unsigned int set_int
)
{
	REG32(BCH_BASE + 0x190) = set_int ;
}

void bch_clear_finish_int(
	unsigned int clear_int
)
{
    REG32(BCH_BASE + 0x184) = clear_int ;
}


unsigned int bch_rd_parity(unsigned int num)
{
	unsigned int tmp ;
	tmp = REG32(BCH_BASE + 0x14 + num*4) ;
	return tmp ;
}

unsigned int bch_rd_ecc(unsigned int num)
{
	unsigned int tmp ;
	tmp = REG32(BCH_BASE + 0x84 + num*4) ;
    return tmp ;
}

unsigned int bch_rd_tag()
{
	//printx(0xdead0001);
	unsigned int tmp ;
	tmp = REG32(BCH_BASE + 0x194) ;
	return tmp ;
}

/*-------------------------------
void bch_pdma_check_tt()
{
    unsigned int tmp ;
    tmp = REG32(PDMA_BCH + 0x10) ;
    while( (tmp & 0x8) != 0x8 )
        tmp = REG32(PDMA_BCH + 0x10) ;
    printd(1);
    printx(tmp) ;
}
---------------------------------*/


void encoding(
unsigned int tag ,
unsigned int zero_cnt ,
unsigned int bypass ,
unsigned int dma_enable ,
unsigned int sel_ecc_level ,
unsigned int fast_dec ,
unsigned int init ,
unsigned int enc_enable ,
unsigned int bch_enable ,

unsigned int blocksize ,
unsigned int paritysize ,

unsigned int *parity ,
unsigned int *din
)
{
    unsigned int data_word ;
    unsigned int i ;
    unsigned int bchint ;
    bch_set_ctrl(
         tag ,
         zero_cnt ,
         bypass ,
         dma_enable ,
         sel_ecc_level ,
         fast_dec ,
         init ,
         enc_enable ,
         bch_enable
    );

    data_word = blocksize/4 ;
    if( blocksize%4 != 0 )
        data_word++ ;

    for( i=0 ; i < data_word ; i++ ){
        bch_data_input(din[i]);
    }

	//input more data word , for test
    bch_data_input(0xa5a5a5a5);
    bch_data_input(0xa5a5a5a5);
    bch_data_input(0xa5a5a5a5);

#ifndef BCH_INT_ENABLE
    bchint = bch_check_finish_int() ; //read status reg
    while((bchint & 0x00000004) != 0x00000004)
    {
        bchint = bch_check_finish_int() ;
    }

#else
    while(INT_ENC != 1) printd(1015);
    INT_ENC = 0 ;
	bchint = BCHINT ;
#endif

	printd(2);
	printx(bchint) ;
    //read parity
	data_word =paritysize/4 ;
    if( paritysize%4 != 0 )
        data_word++ ;

    for( i = 0 ; i < data_word ; i++ ){
        parity[i] = bch_rd_parity( i ) ;
    }

}

void int_encoding(
unsigned int tag ,
unsigned int zero_cnt ,
unsigned int bypass ,
unsigned int dma_enable ,
unsigned int sel_ecc_level ,
unsigned int fast_dec ,
unsigned int init ,
unsigned int enc_enable ,
unsigned int bch_enable ,

unsigned int blocksize ,
unsigned int paritysize ,

unsigned int *parity ,
unsigned int *din
)
{
    unsigned int data_word ;
    unsigned int i ;
    //unsigned int bchint ;
    bch_set_ctrl(
         tag ,
         zero_cnt ,
         bypass ,
         dma_enable ,
         sel_ecc_level ,
         fast_dec ,
         init ,
         enc_enable ,
         bch_enable
    );

    data_word = blocksize/4 ;
    if( blocksize%4 != 0 )
        data_word++ ;

    for( i=0 ; i < data_word ; i++ ){
        bch_data_input(din[i]);
    }

    //input more data word , for test
    bch_data_input(0xa5a5a5a5);
    bch_data_input(0xa5a5a5a5);
    bch_data_input(0xa5a5a5a5);

    while(INT_ENC != 1) printd(1015);
    INT_ENC = 0 ;

    //read parity
    data_word =paritysize/4 ;
    if( paritysize%4 != 0 )
        data_word++ ;

    for( i = 0 ; i < data_word ; i++ ){
        parity[i] = bch_rd_parity( i ) ;
    }
}


void dma_encoding(
unsigned int tag ,
unsigned int zero_cnt ,
unsigned int bypass ,
unsigned int dma_enable ,
unsigned int sel_ecc_level ,
unsigned int fast_dec ,
unsigned int init ,
unsigned int enc_enable ,
unsigned int bch_enable ,

unsigned int blocksize ,
unsigned int paritysize ,

unsigned int *parity ,
unsigned int *din ,

//for PDMA.DCM0
    unsigned int sur_addr ,//va_2_pa((unsigned int)din) ;//source address
    unsigned int des_addr ,//target address
    unsigned int tc       ,//transfer count
    unsigned int req_type ,//request types

    unsigned int sur_id         , //source identification
    unsigned int des_id         , //destination identification
    unsigned int sur_addr_inc   , //source address increment
    unsigned int des_addr_inc   , //destination address increment
    unsigned int sur_port_width , //source port width
    unsigned int des_port_width   //destination port width

)
{
    unsigned int data_word ;
    //unsigned int i ;
    unsigned int bchint ;

    //unsigned int sur_addr = 0x13423000 ;//va_2_pa((unsigned int)din) ;//source address
    //unsigned int des_addr = 0x134d0010 ;//target address
    //unsigned int tc       = blocksize ;//transfer count
    //unsigned int req_type = 0x8 ;//request types

    bch_set_ctrl(
         tag ,
         zero_cnt ,
         bypass ,
         dma_enable ,
         sel_ecc_level ,
         fast_dec ,
         init ,
         enc_enable ,
         bch_enable
    );

    data_word = blocksize/4 ;
    if( blocksize%4 != 0 )
        data_word++ ;


    //bch_pdma_check_tt();
    //input more data word , for test
    bch_data_input(0xa5a5a5a5);
    bch_data_input(0xa5a5a5a5);
    bch_data_input(0xa5a5a5a5);

#ifndef BCH_INT_ENABLE
    bchint = bch_check_finish_int() ; //read status reg
    while((bchint & 0x00000004) != 0x00000004)
    {
        bchint = bch_check_finish_int() ;
    }
#else
    while(INT_ENC != 1) printd(1015);
    INT_ENC = 0 ;
#endif
    printd(2);
    printx(bchint) ;
    //read parity
/*
    data_word =paritysize/4 ;
    if( paritysize%4 != 0 )
        data_word++ ;

    for( i = 0 ; i < data_word ; i++ ){
        parity[i] = bch_rd_parity( i ) ;
    }
*/
}


void decoding_ff(
unsigned int tag ,
unsigned int zero_cnt ,
unsigned int bypass ,
unsigned int dma_enable ,
unsigned int sel_ecc_level ,
unsigned int fast_dec ,
unsigned int init ,
unsigned int enc_enable ,
unsigned int bch_enable ,

unsigned int blocksize ,
unsigned int paritysize ,

unsigned int *parity ,
unsigned int *din

)
{
    unsigned int data_word ;
    unsigned int i ;
    //unsigned int bchint ;

    bch_set_ctrl(
         tag ,
         zero_cnt ,
         bypass ,
         dma_enable ,
         sel_ecc_level ,
         fast_dec ,
         init ,
         enc_enable ,
         bch_enable
    );

    bch_set_init() ; //start decoding

    data_word = ( blocksize ) / 4 ;
    if( ( blocksize )%4 != 0 )
        data_word++ ;
	bch_data_input( 0xfffffffe );
	bch_data_input( 0xfefffeff );
    for( i=0 ; i < data_word ; i++ )
        bch_data_input( 0xffffffff );//data input to bch

    data_word = ( paritysize ) / 4 ;
    if( ( paritysize )%4 != 0 )
        data_word++ ;

    for( i=0 ; i < data_word ; i++ )
        bch_data_input( 0xffffffff );//parity input to bch

    bch_data_input(0xa5a5a5a5);//input data over flw

}

void read_error(
	unsigned int *bherr ,
	unsigned int *cnt
)
{
    unsigned int bchint ;
    unsigned int error_halfword_cnt ;

#ifndef BCH_INT_ENABLE
    bchint = bch_check_finish_int() ; //read status reg
    while((bchint & 0x00000008) != 0x00000008)
    {
        bchint = bch_check_finish_int() ;
    }
#else
    while(INT_DEC != 1) printd(1016);
    INT_DEC = 0 ;
    bchint = BCH_DEC_INT;//BCHINT ;
#endif

	printd(3);
    printx(bchint);
	printd(4);
    printx(bch_rd_tag());

    //read error reg
	printd(5) ;
    error_halfword_cnt = (bchint & 0x7f000000)>>24 ;
	*cnt = error_halfword_cnt ;
    printx(*cnt) ;


    for( ; error_halfword_cnt > 0 ; error_halfword_cnt-- )
    {
        //printx(bch_rd_ecc(error_halfword_cnt-1)) ;
		bherr[error_halfword_cnt-1] = bch_rd_ecc(error_halfword_cnt-1) ;
    }

}

void dma_read_error(
    unsigned int des_addr ,
    unsigned int *cnt 
)
{
    unsigned int bchint ;
    unsigned int error_halfword_cnt ;

//PDMA
    //unsigned int sur_addr       = 0x134d0084;//source address, err report0
    unsigned int tc             ;//transfer count
    //unsigned int req_type       = 0x8;//request types

    //unsigned int sur_id          = 1    ; //source identification. 0:TCSM ; 1:BCH ; 2:DDR ;
    //unsigned int des_id          = 0    ; //destination identification. 0:TCSM ; 1:BCH ; 2:DDR ;
    //unsigned int sur_addr_inc    = 1    ; //source address increment. 0:no increment ; 1:increment
    //unsigned int des_addr_inc    = 1    ; //destination address increment. 0:no increment ; 1:increment
    //unsigned int sur_port_width  = 0    ; //source port width. 00:32bit ; 01:8bit ; 10:16bit
    //unsigned int des_port_width  = 0    ; //destination port width. 00:32bit ; 01:8bit ; 10:16bit

#ifndef BCH_INT_ENABLE
    bchint = bch_check_finish_int() ; //read status reg
    while((bchint & 0x00000008) != 0x00000008)
    {
        bchint = bch_check_finish_int() ;
    }
#else
    while(INT_DEC != 1) printd(1016);
    INT_DEC = 0 ;
    bchint = BCH_DEC_INT ;
#endif

    printd(3);
    printx(bchint);
    printd(4);
    printx(bch_rd_tag());

    //read error reg
    printd(5) ;
    error_halfword_cnt = (bchint & 0x7f000000)>>24 ;
    *cnt = error_halfword_cnt ;
    printx(*cnt) ;
    tc = error_halfword_cnt*4 ;
if(error_halfword_cnt != 0){

  //bch_pdma_check_tt();
}
/*
    for( error_halfword_cnt ; error_halfword_cnt > 0 ; error_halfword_cnt-- )
    {
        //printx(bch_rd_ecc(error_halfword_cnt-1)) ;
        bherr[error_halfword_cnt-1] = bch_rd_ecc(error_halfword_cnt-1) ;
    }
*/
}


void int_read_error(
    unsigned int *bherr ,
    unsigned int *cnt
)
{
  unsigned int bchint ;
    unsigned int error_halfword_cnt ;
    
	while(INT_DEC != 1)printd(1010);
	INT_DEC = 0 ;
    printd(3);
    //printx(bchint);
    printd(4);
    printx(bch_rd_tag());

    //read error reg
    printd(5) ;
    error_halfword_cnt = (bchint & 0x7f000000)>>24 ;
    *cnt = error_halfword_cnt ;
    printx(*cnt) ;


    for(; error_halfword_cnt > 0 ; error_halfword_cnt-- )
    {
        //printx(bch_rd_ecc(error_halfword_cnt-1)) ;
        bherr[error_halfword_cnt-1] = bch_rd_ecc(error_halfword_cnt-1) ;
    }

}


unsigned int read_syndrome_done()
{
    unsigned int bchint ;

	//because of the software write is very very slow, so if we check syndrome finish bit here, we can check only once.
    bchint = bch_check_finish_int() ; //read status reg
	/*
    while((bchint & 0x00000020) != 0x00000020)
    {
        bchint = bch_check_finish_int() ;
    }
	*/
	return (bchint & 0x00000020) == 0x00000020 ; 
}

void decoding(
unsigned int tag ,
unsigned int zero_cnt ,
unsigned int bypass ,
unsigned int dma_enable ,
unsigned int sel_ecc_level ,
unsigned int fast_dec ,
unsigned int init ,
unsigned int enc_enable ,
unsigned int bch_enable ,

unsigned int blocksize ,
unsigned int paritysize ,

unsigned int *parity ,
unsigned int *din

)
{
    unsigned int data_word ;
    unsigned int i ;
    //unsigned int bchint ;

    bch_set_ctrl(
         tag ,
		 zero_cnt ,
         bypass ,
         dma_enable ,
         sel_ecc_level ,
		 fast_dec ,
         init ,
         enc_enable ,
         bch_enable
    );
    //bch_set_init() ; //start decoding

    data_word = ( blocksize ) / 4 ;
    if( ( blocksize )%4 != 0 )
        data_word++ ;

    bch_data_input( din[0] );
    bch_data_input( din[1] );
    bch_data_input( din[2] );

    for( i=3 ; i < data_word ; i++ )
        bch_data_input( din[i] );//data input to bch

    data_word = ( paritysize ) / 4 ;
    if( ( paritysize )%4 != 0 )
        data_word++ ;

    for( i=0 ; i < data_word ; i++ )
        bch_data_input( parity[i] );//parity input to bch

    bch_data_input(0xa5a5a5a5);//input data over flw
    bch_data_input(0xa5a5a5a5);
    bch_data_input(0xa5a5a5a5);

}

void dma_decoding(
unsigned int tag ,
unsigned int zero_cnt ,
unsigned int bypass ,
unsigned int dma_enable ,
unsigned int sel_ecc_level ,
unsigned int fast_dec ,
unsigned int init ,
unsigned int enc_enable ,
unsigned int bch_enable ,

unsigned int blocksize ,
unsigned int paritysize ,

unsigned int *parity ,
unsigned int *din ,

//for PDMA.DCM0
    unsigned int sur_addr ,//va_2_pa((unsigned int)din) ;//source address
    unsigned int des_addr ,//target address
    unsigned int tc       ,//transfer count
    unsigned int req_type ,//request types

    unsigned int sur_id         , //source identification
    unsigned int des_id         , //destination identification
    unsigned int sur_addr_inc   , //source address increment
    unsigned int des_addr_inc   , //destination address increment
    unsigned int sur_port_width , //source port width
    unsigned int des_port_width   //destination port width
)
{
  //unsigned int data_word ;
  //unsigned int i ;
  //unsigned int bchint ;

    bch_set_ctrl(
         tag ,
         zero_cnt ,
         bypass ,
         dma_enable ,
         sel_ecc_level ,
         fast_dec ,
         init ,
         enc_enable ,
         bch_enable
    );

 

    //bch_pdma_check_tt();
    
    bch_data_input(0xa5a5a5a5);//input data over flw
    bch_data_input(0xa5a5a5a5);
    bch_data_input(0xa5a5a5a5);
}

void err_inject(
unsigned int *din,
unsigned int size,
unsigned int err_num
)
{
	unsigned int i ;
	unsigned int tmp , tmp_0x1;
	for( i = 0 ; i < err_num ; i++ ){
		srand(din[1]+i) ;
        tmp = rand() % (size/4) ;
		tmp_0x1 = (0x1 << rand()%32) ;
		din[tmp] = din[tmp] ^ tmp_0x1 ;
	}
}

//---------------------------------------------------//
//                  for register test                //
//---------------------------------------------------//
void BHCR_wr( unsigned int data )
{
	REG32(BCH_BASE + 0x0) = data ;	
}

unsigned int BHCR_rd()
{
    return REG32(BCH_BASE + 0x0) ;
}

//---------------------------------------------------
void BHCSR_wr( unsigned int data )
{
    REG32(BCH_BASE + 0x4) = data ;
}

//---------------------------------------------------
void BHCCR_wr( unsigned int data )
{
    REG32(BCH_BASE + 0x8) = data ;
}

//---------------------------------------------------
void BHCNT_wr( unsigned int data )
{
    REG32(BCH_BASE + 0xc) = data ;
}

unsigned int BHCNT_rd( )
{
    return REG32(BCH_BASE + 0xc) ;
}

//--------------------------------------------------
void BHINTE_wr( unsigned int data )
{
    REG32(BCH_BASE + 0x190) = data ;
}

unsigned int BHINTE_rd( )
{
    return REG32(BCH_BASE + 0x190) ;
}

//--------------------------------------------------
void BHINTES_wr( unsigned int data )
{
    REG32(BCH_BASE + 0x188) = data ;
}

//--------------------------------------------------
void BHINTEC_wr( unsigned int data )
{
    REG32(BCH_BASE + 0x18c) = data ;
}


unsigned int err_corr(
	unsigned int error_halfword_cnt ,
	unsigned int *din ,
	unsigned int *bherr ,
	unsigned int blocksize ,
	unsigned int *din_bk ,
	unsigned int error_state
//0: no error occur;
//1: error occur, and can be correction;
//2: error can not be correction;
//3: all ff 
)
{
unsigned int tmp ;
unsigned int j ;

#ifndef BCH_INT_ENABLE
tmp = bch_check_finish_int() ;
#else
tmp = BCH_DEC_INT ;
printd(778899);
printx(tmp);
printx(error_halfword_cnt);
#endif
//error correction if need
if(((tmp&0x8)!=0) && ((tmp&0x2)==0) && ((tmp&0x1)!=0) ){
    for(j=0;j<error_halfword_cnt ; j++){
        din[(bherr[j]&0x000007ff)/2] = (bherr[j]&0x000007ff)%2!=0 ?
                                       (bherr[j]&0xffff0000)^din[(bherr[j]&0x000007ff)/2] :
                                       (bherr[j]&0xffff0000)>>16 ^din[(bherr[j]&0x000007ff)/2];
    }
}

if(error_state == 0x0){
	if((((tmp&0x8)!=0) && ((tmp&0x2)==0) && ((tmp&0x1)==0)) != 1 )//decoding done, no un-correction error, no error occur
	{
		printx(0xdead0001) ;
		return 1 ;
	}
}

if(error_state == 0x1){
	for(j=0 ;j<blocksize/4;j++){
    	if(din[j] != din_bk[j]){
        	printx(0xdead0002) ;
			printd(j);
			printx(din[j]);
			printx(din_bk[j]);
            return 1 ;
        }
    }
	if(( ((tmp&0x8)!=0) && ((tmp&0x2)==0) && ((tmp&0x1)!=0)) != 1 )//decoding done, no un-correction error, no error occur
	{
		printx(0xdead0003) ;
		return 1 ;
	}
} 

if(error_state == 0x2)
{ 
	//decoding done, un-correction error, no error occur(don't care this bit)
	if((((tmp&0x8)!=0) && ((tmp&0x2)!=0)) != 1 ){
	    printx(0xdead0004) ;
	    return 1 ;
	}
}

if(error_state == 0x3)
{
    //decoding done, un-correction error, no error occur(don't care this bit)
    if((((tmp&0x8)!=0) && ((tmp&0x10)!=0)) != 1 ){
        printx(0xdead0005) ;
        return 1 ;
    }
}
}

unsigned int int_err_corr(
    unsigned int error_halfword_cnt ,
    unsigned int *din ,
    unsigned int *bherr ,
    unsigned int blocksize ,
    unsigned int *din_bk ,
    unsigned int error_state
//0: no error occur;
//1: error occur, and can be correction;
//2: error can not be correction;
//3: all ff
)
{
unsigned int tmp ;
unsigned int j ;
tmp = bch_check_finish_int() ;

//error correction if need
//if(((tmp&0x8)!=0) && ((tmp&0x2)==0) && ((tmp&0x1)!=0) ){
    for(j=0;j<error_halfword_cnt ; j++){
        din[(bherr[j]&0x000007ff)/2] = (bherr[j]&0x000007ff)%2!=0 ?
                                       (bherr[j]&0xffff0000)^din[(bherr[j]&0x000007ff)/2] :
                                       (bherr[j]&0xffff0000)>>16 ^din[(bherr[j]&0x000007ff)/2];
    }
//}

if(error_state == 0x0){
/*
    if((((tmp&0x8)!=0) && ((tmp&0x2)==0) && ((tmp&0x1)==0)) != 1 )//decoding done, no un-correction error, no error occur
    {
        printx(0xdead0001) ;
        return 1 ;
    }
*/
}

if(error_state == 0x1){
    for(j=0 ;j<blocksize/4;j++){
        if(din[j] != din_bk[j]){
			printd(j) ;
			printx(din[j]) ;
			printx(din_bk[j]) ;
            printx(0xdead0002) ;
            return 1 ;
        }
    }
/*
    if(( ((tmp&0x8)!=0) && ((tmp&0x2)==0) && ((tmp&0x1)!=0)) != 1 )//decoding done, no un-correction error, no error occur
    {
        printx(0xdead0003) ;
        return 1 ;
    }
*/
}
/*
if(error_state == 0x2)
{
    //decoding done, un-correction error, no error occur(don't care this bit)
    if((((tmp&0x8)!=0) && ((tmp&0x2)!=0)) != 1 ){
        printx(0xdead0004) ;
        return 1 ;
    }
}

if(error_state == 0x3)
{
    //decoding done, un-correction error, no error occur(don't care this bit)
    if((((tmp&0x8)!=0) && ((tmp&0x10)!=0)) != 1 ){
        printx(0xdead0005) ;
        return 1 ;
    }
}
*/
}

void handler_bch_int()
{
	unsigned int tmp ;
	//close int
    REG32(BCH_BASE + 0x190) = 0x0 ;
    tmp = REG32(BCH_BASE + 0x184) ;
    //clear int
    REG32(BCH_BASE + 0x184) = 0x0c ;
    if((tmp&0x20) == 0x20){// syndrome done
    	printd(1111) ;
		INT_SYN = 1 ;
	}
	if((tmp&0x8) == 0x8){// decoding done
    	printd(1112) ;
		INT_DEC = 1 ;
		BCH_DEC_INT = tmp;//bch_check_finish_int() ;
		printx(BCH_DEC_INT);
	}
	if((tmp&0x4) == 0x4){//encoding done
    	printd(1113) ;
		INT_ENC = 1 ;
		BCHINT = tmp;// bch_check_finish_int() ;
		printx(BCHINT);
	}
	bch_set_int_enable(0x0c) ; //enable all int	
}
//--------------------------------------------
//     test regs with write 55AA
//--------------------------------------------
unsigned int test_all_reg_55AA()
{
//---------------------------------------------------//
//                  for register test                //
//---------------------------------------------------//
printd(0) ;
BHCR_wr( 0xaaaaaaaa ) ;//do not write 1 to the bit-feild of initial
printx(BHCR_rd());
if(BHCR_rd() != 0xaaaaaaa0 ) return 1 ;

printd(1) ;
BHCR_wr( 0x55555555 ) ;
if(BHCR_rd() != 0x55555555 ) return 1 ;


//---------------------------------------------------
printd(2) ;
BHCR_wr( 0x00000000 ) ;
BHCSR_wr( 0xaaaaaaaa ) ;
if(BHCR_rd() != 0xaaaaaaa0 ) return 1 ;

printd(3) ;
BHCR_wr( 0x00000000 ) ;
BHCSR_wr( 0x55555555 ) ;
if(BHCR_rd() != 0x55555555 ) return 1 ;

//---------------------------------------------------
printd(4) ;
BHCR_wr( 0x00000000 ) ;
BHCCR_wr( 0xaaaaaaaa ) ;
if(BHCR_rd() != 0x00000000 ) return 1 ;

printd(5) ;
BHCCR_wr( 0x55555555 ) ;
if(BHCR_rd() != 0x00000000 ) return 1 ;

printd(6) ;
BHCR_wr( 0xaaaaaaaa ) ;
BHCCR_wr( 0xaaaaaaaa ) ;
if(BHCR_rd() != 0x00000000 ) return 1 ;

printd(7) ;
BHCR_wr( 0x55555555 ) ;
BHCCR_wr( 0x55555555 ) ;
if(BHCR_rd() != 0x00000000 ) return 1 ;

printd(8) ;
BHCR_wr( 0xffffffff ) ;
BHCCR_wr( 0xaaaaaaaa ) ;
if(BHCR_rd() != 0x55555555 ) return 1 ;

printd(9) ;
BHCR_wr( 0xffffffff ) ;
BHCCR_wr( 0x55555555 ) ;
if(BHCR_rd() != 0xaaaaaaa0 ) return 1 ;

//---------------------------------------------------
printd(10) ;
BHCNT_wr( 0x002a02aa ) ;
if(BHCNT_rd() != 0x002a02aa) return 1 ;

printd(11) ;
BHCNT_wr( 0x00550555 ) ;
if(BHCNT_rd() != 0x00550555) return 1 ;

//disable bch module for INT test
BHCR_wr( 0x00000000 ) ;
//clear bch finish reg
REG32(0xb34d0184) = REG32(0xb34d0184) & 0x3f;
//--------------------------------------------------
printd(12) ;
BHINTE_wr( 0x2a ) ;
if(BHINTE_rd() != 0x2a) return 1 ;

printd(13) ;
BHINTE_wr( 0x15 ) ;
if(BHINTE_rd() != 0x15) {printx(BHINTE_rd());printx(0xdead1300);return 1 ;}

//--------------------------------------------------
printd(14) ;
BHINTE_wr( 0x00 ) ;
BHINTES_wr( 0x2a ) ;
if(BHINTE_rd() != 0x2a) return 1 ;

printd(15) ;
BHINTE_wr( 0x00 ) ;
BHINTES_wr( 0x15 ) ;
if(BHINTE_rd() != 0x15) return 1 ;

//--------------------------------------------------
printd(16) ;
BHINTE_wr( 0x00 ) ;
BHINTES_wr( 0x2a ) ;
BHINTEC_wr( 0x2a ) ;
if(BHINTE_rd() != 0x00) return 1 ;

printd(17) ;
BHINTE_wr( 0x00 ) ;
BHINTES_wr( 0x15 ) ;
BHINTEC_wr( 0x15 ) ;
if(BHINTE_rd() != 0x00) return 1 ;

return 0 ;
}

//----------------------not use

void bch_enc_handler() 
{
  unsigned int int_enb, int_stat;
  printd(123321);
  //int_enb  = get_bhinte();
  int_stat = REG32(BCHISR);

  if ((int_enb & 0x4) && (int_stat | 0x4)) {
    printd(123321);
    REG32(BCHISR)  = int_stat & ~0x04;
    REG32(BHINTEC) = 0x04;
  }
}
/*
#define set_bch_enc_vbr() 			\
({						\
  setvbr(TP_INT | INTC_BCH, bch_enc_handler);	\
})

#endif
*/
