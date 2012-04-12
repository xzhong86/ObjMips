#ifndef _VCI_H_
#define _VCI_H_

//#include <instructions.h>
//#include <test.h>
//#include <exp_supp.h>
//#include <debug.h>
#ifdef AHB_TEST
#include "../../I_TEST/ddr.h"
#endif

#include "gpio.h"
#include "cim.h"

//-------------------------------------------------------------------------------
// macro def
//-------------------------------------------------------------------------------

#define CIM_FRAMW_HSIZE_B 300
#define CIM_FRAMW_VSIZE_B 400

#define CIM_FRAME0_SIZE_B (CIM_FRAMW_VSIZE_B*CIM_FRAMW_HSIZE_B)
#define CIM_FRAME1_SIZE_B (CIM_FRAMW_VSIZE_B*CIM_FRAMW_HSIZE_B)
#define CIM_FRAME2_SIZE_B (CIM_FRAMW_VSIZE_B*CIM_FRAMW_HSIZE_B)
#define CIM_FRAME3_SIZE_B (CIM_FRAMW_VSIZE_B*CIM_FRAMW_HSIZE_B)
#define CIM_FRAME4_SIZE_B (CIM_FRAMW_VSIZE_B*CIM_FRAMW_HSIZE_B)
#define CIM_FRAME5_SIZE_B (CIM_FRAMW_VSIZE_B*CIM_FRAMW_HSIZE_B)
                        
#define CIM_FRAME0_SIZE_W (CIM_FRAME0_SIZE_B/4)
#define CIM_FRAME1_SIZE_W (CIM_FRAME1_SIZE_B/4)
#define CIM_FRAME2_SIZE_W (CIM_FRAME2_SIZE_B/4)
#define CIM_FRAME3_SIZE_W (CIM_FRAME3_SIZE_B/4)
#define CIM_FRAME4_SIZE_W (CIM_FRAME4_SIZE_B/4)
#define CIM_FRAME5_SIZE_W (CIM_FRAME5_SIZE_B/4)

//-------------------------------------------------------------------------------
// type def
//-------------------------------------------------------------------------------

union U_w2b{
  unsigned int  word;
  unsigned char byte[4];
};

typedef union U_w2b UT_w2b;

typedef unsigned int uint;
typedef struct {
  uint DA, FID, FA, CMD;
} desptor;

//-------------------------------------------------------------------------------
// variables definition
//-------------------------------------------------------------------------------

static unsigned int * cim_frame0_ptr;
static unsigned int * cim_frame1_ptr;
static unsigned int * cim_frame2_ptr;
static unsigned int * cim_frame3_ptr;
static unsigned int * cim_frame4_ptr;
static unsigned int * cim_frame5_ptr;

static unsigned int cmd_len[6] __attribute__ ((aligned (32))) = {CIM_FRAME0_SIZE_W,
								 CIM_FRAME1_SIZE_W,
								 CIM_FRAME2_SIZE_W,
								 CIM_FRAME3_SIZE_W,
								 CIM_FRAME4_SIZE_W,
								 CIM_FRAME5_SIZE_W};

static desptor  vci_desp[6] __attribute__ ((aligned (32)));
static unsigned int vci_cmd[6]  __attribute__ ((aligned (32)));

//-------------------------------------------------------------------------------
// set cim DMA descriptor(s), here, use 6 frame buffers
//-------------------------------------------------------------------------------

static void set_cim_desp(void)
{                                                     
      unsigned int i;                                  

      for (i=0; i<6; i++) {                            
        vci_cmd[i] =  cmd_len[i];                      
      }                                                
                                                       
      vci_cmd[0] |= CMD_SOF_INT_EN | CMD_OFRCV_EN;     
      vci_cmd[5] |= CMD_EOF_INT_EN | CMD_STOP_EN | CMD_OFRCV_EN;
      vci_desp[0].DA  = va_2_pa((uint)&vci_desp[1]);   
      vci_desp[0].FA  = va_2_pa((uint)cim_frame0_ptr); 
      vci_desp[0].FID = 0;                             
      vci_desp[0].CMD = vci_cmd[0];                    
                                                       
      vci_desp[1].DA  = va_2_pa((uint)&vci_desp[2]);   
      vci_desp[1].FA  = va_2_pa((uint)cim_frame1_ptr); 
      vci_desp[1].FID = 1;
      vci_desp[1].CMD = vci_cmd[1];                    
                                                       
      vci_desp[2].DA  = va_2_pa((uint)&vci_desp[3]);   
      vci_desp[2].FA  = va_2_pa((uint)cim_frame2_ptr); 
      vci_desp[2].FID = 2;                             
      vci_desp[2].CMD = vci_cmd[2];                    
                                                       
      vci_desp[3].DA  = va_2_pa((uint)&vci_desp[4]);   
      vci_desp[3].FA  = va_2_pa((uint)cim_frame3_ptr); 
      vci_desp[3].FID = 3;                             
      vci_desp[3].CMD = vci_cmd[3];                    
                                                       
      vci_desp[4].DA  = va_2_pa((uint)&vci_desp[5]);   
      vci_desp[4].FA  = va_2_pa((uint)cim_frame4_ptr); 
      vci_desp[4].FID = 4;                             
      vci_desp[4].CMD = vci_cmd[4];                    
                                                       
      vci_desp[5].DA  = va_2_pa((uint)&vci_desp[0]);   
      vci_desp[5].FA  = va_2_pa((uint)cim_frame5_ptr); 
      vci_desp[5].FID = 5;                             
      vci_desp[5].CMD = vci_cmd[5];                    
                                                       
      set_cimda(va_2_pa((uint)&vci_desp[0]));          
}

static void init_default_vci(void)                    
{
	int i;
  enb_rxf_rst();                              
  //loop_for_waiting(100, 1);
  for (i = 0; i < 10000; i++) ;
  dis_rxf_rst();

  set_burst(BURST_INCR32);
  set_mode_bypass();
  set_vsync_h();
  set_hsync_h();

  set_pclk_f();                               
  set_cim_desp();

#ifdef EAV_SAV
  set_eav_first();
#else
  set_sav_first();
#endif

#ifdef FIELD_1
  set_fp_h();
#endif                                  
}

#define Dly_2(times) ({ uint i; for (i=0; i<times; i++);})

/********************************************************************************
 * prepare for cim driver
 *******************************************************************************/

#define CIM_ENV_WIDTH_REG     (CIM_BASE+0xFF00)
#define CIM_ENV_HEIGH_REG     (CIM_BASE+0xFF04)
#define CIM_ENV_VSYN_T_REG    (CIM_BASE+0xFF08)
#define CIM_ENV_USLE_HSYN_REG (CIM_BASE+0xFF0C)
#define CIM_ENV_TRIG_REG      (CIM_BASE+0xFFF0)
#define CIM_ENV_STA_REG       (CIM_BASE+0xFFF4)

#define set_cim_env_width(val)      i_s32(val, CIM_ENV_WIDTH_REG, 0)
#define set_cim_env_high(val)       i_s32(val, CIM_ENV_HEIGH_REG, 0)
#define set_cim_env_vsync_hold(val) i_s32(val, CIM_ENV_VSYN_T_REG, 0)
#define set_cim_env_hsync_sep(val)  i_s32(val, CIM_ENV_USLE_HSYN_REG, 0)
#define set_cim_env_trig(val)       i_s32(val, CIM_ENV_TRIG_REG, 0)
#define set_cim_env_sta(val)        i_s32(val, CIM_ENV_STA_REG, 0)

static void init_default_dvi()
{
  set_cim_env_width(CIM_FRAMW_HSIZE_B);
  set_cim_env_high(CIM_FRAMW_VSIZE_B);

  set_cim_env_vsync_hold(5);
  set_cim_env_hsync_sep(5);
}

#define enb_dvi() set_cim_env_trig(1)
#define dis_dvi() set_cim_env_trig(1)

/********************************************************************************
 * prepare for check
 *******************************************************************************/

//-------------------------------------------------------------------------------
// pack for bypass
//-------------------------------------------------------------------------------

static unsigned int ypack[8][4] = {
  {3, 2, 1, 0},
  {0, 3, 2, 1},
  {1, 0, 3, 2},
  {2, 1, 0, 3},
  {0, 1, 2, 3},
  {3, 0, 1, 2},
  {2, 3, 0, 1},
  {1, 2, 3, 0}};

static unsigned int pack_ycbcr422(unsigned int mode, unsigned int data)
{
  UT_w2b       ut_w2b_s, ut_w2b_d;
  unsigned int i, result;

  ut_w2b_s.word = data;

  for (i=0; i<4; i++) {
    ut_w2b_d.byte[i] = ut_w2b_s.byte[ypack[mode][i]];
  }

  result = ut_w2b_d.word;

  return result;
}

static unsigned int bp_pack[8][4] = {
  {3, 2, 1, 0},
  {2, 1, 0, 3},
  {1, 0, 3, 2},
  {0, 3, 2, 1},
  {0, 1, 2, 3},
  {1, 2, 3, 0},
  {2, 3, 0, 1},
  {3, 0, 1, 2}};

static unsigned int pack(unsigned int mode, unsigned int data) 
{
  UT_w2b       ut_w2b_s, ut_w2b_d;
  unsigned int i, result;

  ut_w2b_s.word = data;

  for (i=0; i<4; i++) {
    ut_w2b_d.byte[i] = ut_w2b_s.byte[bp_pack[mode][3-i]];
  }

  result = ut_w2b_d.word;
  return result;
}

//-------------------------------------------------------------------------------
// order test for ycbcr 422
//-------------------------------------------------------------------------------

static unsigned int ycbcr422_order[4][4] = {
  {3, 2, 1, 0},
  {3, 0, 1, 2},
  {2, 3, 0, 1},
  {2, 1, 0, 3}};

static unsigned int order_ycbcr422(unsigned int order, unsigned int data)
{
  UT_w2b       ut_w2b_s, ut_w2b_d;
  unsigned int i, result;

  ut_w2b_s.word = data;

  for (i=0; i<4; i++) {
    ut_w2b_d.byte[i] = ut_w2b_s.byte[ycbcr422_order[order][i]];
  }

  result = ut_w2b_d.word;
  return result;
}

/********************************************************************************
 * check
 *******************************************************************************/

static unsigned int  check_cim_frame(unsigned int * rtl_ptr, unsigned int pack_mode, unsigned int len)
{                                               
  unsigned int i;
  volatile unsigned int tmp_data, rtl_data, itc_data;
  UT_w2b ut_w2b;
  
  for(i=0; i <len; i++) {
    ut_w2b.byte[0] = 0x13;
    ut_w2b.byte[1] = 0x06;
    tmp_data = (i & 0x0000ff00) >> 8;
    ut_w2b.byte[2] = tmp_data;
    tmp_data = i & 0x000000ff;
    ut_w2b.byte[3] = tmp_data;

    itc_data = ut_w2b.word;
    itc_data = pack(pack_mode, itc_data);

    rtl_data = *(rtl_ptr + i);

    if (rtl_data != itc_data)
      return -1;
  }

  return 0;                                            
}

#endif




