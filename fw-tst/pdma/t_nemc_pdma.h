//CH1 for NEMC 
#define CH1 1
#define PDMA_BASE (0xb3420000 + CH1*0x20)

#include "server.h"


void pdma_cfg(
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
    REG32(PDMA_BASE) = sur_addr ;
    printd(2323) ;
    printx(REG32(PDMA_BASE));
    printx(sur_addr);
    //pdma target address
    REG32(PDMA_BASE + 0x04) = des_addr ;
    //pdma transfer count
    REG32(PDMA_BASE + 0x08) = tc & 0x00ffffff ;
    //request types
    REG32(PDMA_BASE + 0x0C) = req_type & 0x3f ;
    //pdma channel cmd, source increment, destination NOT incriment
    REG32(PDMA_BASE + 0x14) = tmp ; 
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
    REG32(PDMA_BASE + 0x10) = 0x80000001;//Non-descriptor, channel enable

}

void pdma_check_tt()
{
    unsigned int tmp ;
    printd(88) ;
    tmp = REG32(PDMA_BASE + 0x10) ;
    while( (tmp & 0x8) != 0x8 )
        tmp = REG32(PDMA_BASE + 0x10) ;
    REG32(PDMA_BASE + 0x10) = tmp ^ 0x9;//clear TT, disable ch0
    printd(99);
    printx(tmp) ;
}

