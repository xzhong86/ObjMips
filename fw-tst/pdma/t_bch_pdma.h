//CH0 for BCH64
#define PDMA_BCH 0xb3420000
#define DIRQP 0xb3421004
#define BHTT1  (0x134d0198|0xa0000000)
#define BHTT2  (0x134d019c|0xa0000000)
volatile unsigned int PDMA_SPC0_TT ;

void pdma_special_ch_int_handler()
{
	unsigned int tmp = REG32(PDMA_BCH + 0x10) ; //check spe ch0 TT
	if( (tmp&0x9) == 0x9){
		REG32(PDMA_BCH + 0x10) = tmp ^ 0x9;//clear ch0 TT
		printx(REG32(DIRQP));
		REG32(DIRQP) = REG32(DIRQP) & 0xfffffffe ;//clear PDMA Interrupt Pending
		printx(REG32(DIRQP));
		PDMA_SPC0_TT = 1 ;
		printd(898989);
	}

}

void bch_pdma_cfg(
    unsigned int sur_addr       , //source address
    unsigned int des_addr       , //target address
    unsigned int tc             , //transfer count
    unsigned int req_type       , //request types

    unsigned int sur_id         , //source identification. 0:TCSM ; 1:BCH ; 2:DDR ; 
    unsigned int des_id         , //destination identification. 0:TCSM ; 1:BCH ; 2:DDR ;
    unsigned int sur_addr_inc   , //source address increment. 0:no increment ; 1:increment
    unsigned int des_addr_inc   , //destination address increment. 0:no increment ; 1:increment
    unsigned int sur_port_width , //source port width. 00:32bit ; 01:8bit ; 10:16bit
    unsigned int des_port_width   //destination port width. 00:32bit ; 01:8bit ; 10:16bit
)
{
	unsigned int tmp = (sur_id<<26) | (des_id<<24) |
                       (sur_addr_inc<<23) | (des_addr_inc<<22) |
                       (sur_port_width<<14) | (des_port_width<<12) |
                       (0x00000700);//TSZ=autonomy
    //pdma control
    REG32(0xb3421000) = 0x3 ;

    //pdma source address
    REG32(PDMA_BCH) = sur_addr ;
    //pdma target address
    REG32(PDMA_BCH + 0x04) = des_addr ;
    //pdma transfer count
    REG32(PDMA_BCH + 0x08) = tc & 0x00ffffff ;
    //request types
    REG32(PDMA_BCH + 0x0C) = req_type & 0x3f ;
    //pdma channel cmd, source increment, destination NOT incriment
#ifndef PDMA_INT_ENABLE
    REG32(PDMA_BCH + 0x14) = tmp ; 
#else
	REG32(PDMA_BCH + 0x14) = tmp | 2 ; //enabel INT
#endif
    printd(115) ;
    printx(tmp);
    printx(sur_id<<26);
    printx(des_id<<24) ;
    printx(sur_addr_inc<<23) ; 
    printx(des_addr_inc<<22) ;
    printx(sur_port_width<<14) ; 
    printx(des_port_width<<12) ;

//0x01800700 ; //0x01c00700 ;

    //last ,pdma channel control
    REG32(PDMA_BCH + 0x10) = 0x80000001;//Non-descriptor, channel enable

}

#define BCH_AHB_TEST

#ifndef PDMA_INT_ENABLE
#ifdef BCH_AHB_TEST
#define random_single() \
({           tmp++;                      \
        if((rand() % 2) == 1){   \
            tmp_f = tmp ;    \
            REG32(BHTT1) = tmp ;   \
            tmp = REG32(BHTT1) ;   \
            if(tmp_f != tmp) {printd(9991);printx(tmp);printx(tmp_f);printx(0xdead1000|i);}  \
        }else{    \
            tmp_f = tmp ;   \
            REG32(BHTT2) = tmp ;   \
            tmp = REG32(BHTT2) ;   \
            if(tmp_f != tmp) {printd(9991);printx(tmp);printx(tmp_f);printx(0xdead2000|i);}   \
        }   \
})


unsigned int SEED ;
void bch_pdma_check_tt()
{
    unsigned int tmp , tmp_f , i=0;
    tmp = REG32(PDMA_BCH + 0x10) ;
	SEED++;
	srand(SEED);
    while( (tmp & 0x8) != 0x8 ){
//#ifdef DISTURB_BCH_SLAVE
		tmp = 0xa5a5a500  ;
		for(i=0;i<10;i++)
			random_single() ;
//#endif
        tmp = REG32(PDMA_BCH + 0x10) ;
	}
    REG32(PDMA_BCH + 0x10) = tmp ^ 0x9;//clear TT, disable ch0
    printd(1);
    printx(tmp) ;
}
#else
void bch_pdma_check_tt()
{
    unsigned int tmp , tmp_f;
    tmp = REG32(PDMA_BCH + 0x10) ;
    while( (tmp & 0x8) != 0x8 ){
//#ifdef DISTURB_BCH_SLAVE
        tmp = 0xa5a5a500  ;
        tmp_f = tmp ;
        REG32(BHTT1) = tmp ;
        tmp = REG32(BHTT1) ;
        if(tmp_f != tmp) {printd(9991);printx(tmp);printx(tmp_f);printx(0xdead0001);}
        tmp = tmp + 1 ;//1
        tmp_f = tmp ;
        REG32(BHTT2) = tmp ;
        tmp = REG32(BHTT2) ;
        if(tmp_f != tmp) {printd(9991);printx(tmp);printx(tmp_f);printx(0xdead0002);}
        tmp = tmp + 1 ;//2
        tmp_f = tmp ;
        REG32(BHTT1) = tmp ;
        tmp = REG32(BHTT1) ;
        if(tmp_f != tmp) {printd(9991);printx(tmp);printx(tmp_f);printx(0xdead0003);}
        tmp = REG32(BHTT1) ;
        if(tmp_f != tmp) {printd(9991);printx(tmp);printx(tmp_f);printx(0xdead0004);}
        tmp = REG32(BHTT1) ;
        if(tmp_f != tmp) {printd(9991);printx(tmp);printx(tmp_f);printx(0xdead0005);}
        tmp = tmp + 1 ;
        tmp_f = tmp ;
        REG32(BHTT2) = tmp ;
        REG32(BHTT2) = tmp ;
        REG32(BHTT1) = tmp+1 ;
        REG32(BHTT2) = tmp ;
        tmp = REG32(BHTT2) ;
        if(tmp_f != tmp) {printd(9991);printx(tmp);printx(tmp_f);printx(0xdead0006);}
        tmp = REG32(BHTT1) ;
        if((tmp_f+1) != (tmp)) {printd(9991);printx(tmp);printx(tmp_f);printx(0xdead0007);}
//#endif
        tmp = REG32(PDMA_BCH + 0x10) ;
    }
    REG32(PDMA_BCH + 0x10) = tmp ^ 0x9;//clear TT, disable ch0
    printd(1);
    printx(tmp) ;
}
#endif
#else

void bch_pdma_check_tt()
{
    //unsigned int tmp ;
    //tmp = REG32(PDMA_BCH + 0x10) ;
    while( PDMA_SPC0_TT == 0 ){
    }
	PDMA_SPC0_TT = 0 ;
    //REG32(PDMA_BCH + 0x10) = tmp ^ 0x9;//clear TT, disable ch0
    printd(1);
    //printx(tmp) ;
}

#endif
