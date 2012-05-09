#ifndef __T_X2D_H__
#define __T_X2D_H__

#define X2D_V_BASE 0xB3030000
#define X2D_P_BASE 0x13030000

typedef unsigned int uint32_t;
typedef unsigned short uint16_t; 
typedef unsigned char uint8_t;
//typedef int int32_t;
//typedef short int16_t; 
//typedef char int8_t;

#define M0_YUV_CSC_C0  0x400
#define M0_YUV_CSC_C1  0x59C
#define M0_YUV_CSC_C2  0x161
#define M0_YUV_CSC_C3  0x2DC
#define M0_YUV_CSC_C4  0x718

#define M1_YUV_CSC_C0  0x4A8
#define M1_YUV_CSC_C1  0x662
#define M1_YUV_CSC_C2  0x341
#define M1_YUV_CSC_C3  0x190
#define M1_YUV_CSC_C4  0x812


#define X2D_SLV_GLB_CTRL   	0xF000
#define X2D_SLV_GLB_STATUS 	0xF004
#define X2D_SLV_GLB_TRIG 	0xF008
#define X2D_SLV_TLB_BASE 	0xF00C
#define X2D_SLV_DHA 		0xF010
#define X2D_SLV_WDOG_CNT 	0xF018
#define X2D_SLV_GLB_TIME        0xF01C

#define X2D_SLV_BWR_DATA 	0xF020
#define X2D_SLV_BWR_CYC 	0xF024
#define X2D_SLV_BRD_DATA 	0xF028
#define X2D_SLV_BRD_CYC 	0xF02C


#define X2D_SLV_LNUM		0xE000
#define X2D_SLV_DST_BASE	0xE004
#define X2D_SLV_DST_CTRL_STR	0xE008
#define X2D_SLV_DST_GS		0xE00C
#define X2D_SLV_DST_BARGB       0xE010

#define X2D_BANK_1K             0x1000
#define X2D_BANK_2K             0x2000
#define X2D_BANK_4K             0x3000


#define L0_CTRL			0x0000
#define L0_YADDR		0x0004
#define L0_UADDR		0x0008
#define L0_VADDR		0x000C
#define L0_SGS			0x0010
#define L0_STR			0x0014
#define L0_OGS			0x0018
#define L0_OOFT			0x001C
#define L0_RSZ_COEF		0x0020

#define L1_CTRL			0x1000
#define L1_YADDR		0x1004
#define L1_UADDR		0x1008
#define L1_VADDR		0x100C
#define L1_SGS			0x1010
#define L1_STR			0x1014
#define L1_OGS			0x1018
#define L1_OOFT			0x101C
#define L1_RSZ_COEF		0x1020

#define L2_CTRL			0x2000
#define L2_YADDR		0x2004
#define L2_UADDR		0x2008
#define L2_VADDR		0x200C
#define L2_SGS			0x2010
#define L2_STR			0x2014
#define L2_OGS			0x2018
#define L2_OOFT			0x201C
#define L2_RSZ_COEF		0x2020

#define L3_CTRL			0x3000
#define L3_YADDR		0x3004
#define L3_UADDR		0x3008
#define L3_VADDR		0x300C
#define L3_SGS			0x3010
#define L3_STR			0x3014
#define L3_OGS			0x3018
#define L3_OOFT			0x301C
#define L3_RSZ_COEF		0x3020

/*===============glb ctrl===================*/
#define IRQ_EN 0x1
#define DST_TLB_EN 0x2
#define WDOG_EN 0x4
#define DESP_EN 0x8
#define CMD_TLB_EN 0x10

#define L0_TLB_EN 0x20
#define L1_TLB_EN 0x40
#define L2_TLB_EN 0x80
#define L3_TLB_EN 0x100

/*===============glb status===================*/
#define 	GIDLE       0
#define 	GCMD_UL     1
#define 	GFHEAD_REQ  2
#define 	GFHEAD      3
#define 	GFL_REQ     4
#define 	GFL         5
#define 	GFL_END     6
#define 	GCMD_LOCK   7
#define 	GKER_UL     8
#define 	GKER_W      9
#define 	GKER_END_LOCK  10

#define GLB_FSM_MSK 0xF
#define GLB_FSM_SFT 0

#define GLB_IRQ_ERR 0x10
#define GLB_WDOG_ERR 0x20

/*===============trigger =====================*/
#define X2D_START 0x1
#define X2D_STOP  0x2
#define X2D_RST   0x4
#define X2D_IRQ_CLR 0x8

/**==============destination control and strind====*/
#define DST_STR_MSK      0xFFFFF
#define BACK_EN          0x100000
#define DST_GLOBAL_ALPHA 0x200000
#define DST_PREM         0x400000
#define DST_MSK_EN       0x800000

#define DST_GALPHA_SFT 24
#define DST_GALPHA_MSK 0xFF

/*========================= DST GS==========*/
#define DST_WID_MSK 0xFFFF
#define DST_WID_SFT 0
#define DST_HEIGHT_MSK 0xFFFF
#define DST_HEIGHT_SFT 16

/*================ LAYX CTRL ===========*/
#define SRC_ARGB 0
#define SRC_RGB555 1
#define SRC_RGB565 2
#define SRC_YUV_SEP 3
#define SRC_YUV_TILE 4
#define SRC_YUV_NV12 5
#define SRC_YUV_NV21 6
#define SRC_FM_MSK 0x7
#define LAY_GLB_ALPHA_EN 0x08
#define SRC_PREM 0x10
#define SRC_CSCM0 0x0
#define SRC_CSCM1 0x20
#define SRC_MSK_EN 0x40


#define ROTATE_0 0x0
#define ROTATE_90 0x1
#define ROTATE_180 0x2
#define ROTATE_270 0x3
#define ROTA_MSK 0x3

#define HMIRROR_EN 0x04
#define VMIRROR_EN 0x08

#define LAY_GLB_ALPHA_MSK 0xFF
#define LAY_GLB_ALPHA_SFT 8

#define SRC_RGBM_MSK 0x7
#define SRC_FMT_RGB 0
#define SRC_FMT_BGR 1
#define SRC_FMT_GRB 2
#define SRC_FMT_BRG 3
#define SRC_FMT_RBG 4
#define SRC_FMT_GBR 5

#define SRC_ARGB_SEL 0x8 
#define SRC_RGBA_SEL 0
/*============== LAYX SGS==============*/
#define IN_FM_W_MSK 0xFFFF
#define IN_FM_W_SFT 0
#define IN_FM_H_MSK 0xFFFF
#define IN_FM_H_SFT 16

/*=============LAYX STR ===============*/
#define Y_S_MSK 0xFFFF
#define Y_S_SFT 0
#define UV_S_MSK 0xFFFF
#define UV_S_SFT 16

/*=========== LAYX OGS ===============*/
#define LAY_OW_MSK 0xFFFF
#define LAY_OW_SFT 0
#define LAY_OH_MSK 0xFFFF
#define LAY_OH_SFT 16

/*========== LAYX OOSFT =============*/
#define LAY_OWFST_MSK 0xFFFF
#define LAY_OWFST_SFT 0
#define LAY_OHFST_MSK 0xFFFF
#define LAY_OHFST_SFT 16

/*========= LAYX SCALER COEF =======*/
#define LAY_HRSZ_C_MSK 0xFFFF
#define LAY_HRSZ_C_SFT 0
#define LAY_VRSZ_C_MSK 0xFFFF
#define LAY_VRSZ_C_SFT 16

#define SHARPL0 0
#define SHARPL1 1
#define SHARPL2 2
#define SHARPL3 3

#define DDR_BANK_1K 0
#define DDR_BANK_2K 1
#define DDR_BANK_4K 2

#define DST_ARGB 0
#define DST_XRGB888 1
#define DST_RGB555 2
#define DST_RGB565 3

typedef struct x2d_lay_info
{ // Order Cannot be Changed!
  uint8_t lay_ctrl;
  uint8_t lay_galpha;
  uint8_t rom_ctrl; //rotate and mirror control
  uint8_t RGBM;

  uint32_t y_addr;

  uint32_t u_addr;
  uint32_t v_addr;

  uint16_t swidth;
  uint16_t sheight;
  uint16_t ystr;
  uint16_t uvstr;

  uint16_t owidth;
  uint16_t oheight;
  uint16_t oxoffset;
  uint16_t oyoffset;

  uint16_t rsz_hcoef;
  uint16_t rsz_vcoef;
  uint32_t bk_argb;
}x2d_lay_info, *x2d_lay_info_p;

typedef struct x2d_chain_info
{ // Order Cannot be Changed!
  uint16_t   overlay_num;
  uint16_t   dst_tile_en;
  uint32_t   dst_addr;
  uint32_t   dst_ctrl_str;
  uint16_t   dst_width;
  uint16_t   dst_height;
  uint32_t   dst_argb;
  uint32_t   dst_fmt;
  x2d_lay_info x2d_lays[4];
}x2d_chain_info, *x2d_chain_info_p;

/* Test prepare setups:
  1. Orignal Source
  2. Fix Rotate Mirror RGBM NV21
  3. Expand UV 
*/
typedef struct  x2d_tbench_lay_verc{
  /*  Layer Base Information  */
  uint32_t  ilay_en;
  uint32_t  ilayer_tlb_en;
  uint32_t  ilay_pm;

  uint32_t  ilay_rota;
  uint32_t  ilay_vmirror;  
  uint32_t  ilay_hmirror;  
  uint32_t  ilay_csc_mod;

  uint32_t  ilay_bak_argb;
  uint32_t  ilay_galpha_en;
  uint32_t  ilay_galpha;
  uint32_t  ilay_mask_en;

  uint32_t  ilay_apos;

  uint32_t ilay_src_width;
  uint32_t ilay_src_height;
  uint32_t  ilay_smode;
  uint32_t ilay_osdm;
  uint32_t ilay_rgbm;

  uint32_t ilay_hrsz_coef;
  uint32_t ilay_vrsz_coef; 
  
  // Orignal Raw Pixels
  uint32_t* ilay_argb_addr;
  uint32_t  ilay_argb_str;

  // Raw Pixels for Hardware
  uint32_t* ilay_y_addr;
  uint32_t* ilay_u_addr;
  uint32_t* ilay_v_addr;
  uint32_t  ilay_y_str;
  uint32_t  ilay_uv_str;

  // Destination parameters
  uint32_t ilay_dst_width;
  uint32_t ilay_dst_height;
  uint32_t ilay_dst_xoft;
  uint32_t ilay_dst_yoft;


  /* Debug Use --> */
  uint32_t* ilay_rota_addr;
  uint32_t  ilay_rota_w;
  uint32_t  ilay_rota_h;
  uint32_t  ilay_rota_str;

  uint32_t  ilay_rota_xcoef;
  uint32_t  ilay_rota_ycoef;

  uint32_t  ilay_rot_y_str;
  uint32_t* ilay_rot_y_addr;
  uint32_t* ilay_rot_u_addr;
  uint32_t* ilay_rot_v_addr;

  uint32_t* ilay_ep_y_addr;
  uint32_t* ilay_ep_u_addr;
  uint32_t* ilay_ep_v_addr;

  uint32_t * ilay_rsz_addr;
  uint32_t   ilay_rsz_str;
  /* <-- Debug Use */
  
}x2d_tbench_lay_verc, *x2d_tbench_lay_verc_p;

typedef struct  x2d_tbench_layb_verc{
  uint32_t  ilay_en;
  uint32_t  ilay_pm;
  uint32_t  ilay_galpha_en;
  uint32_t  ilay_galpha;
  uint32_t  ilay_mask_en;
  uint32_t  ilay_tile_en;

  uint32_t  ilay_width;
  uint32_t  ilay_height;

  uint32_t  ilay_argb_str;
  uint32_t* ilay_argb_addr; // Temp value , Software's Orignal pixels

  uint32_t   ilay_std_str;
  uint32_t * ilay_std_addr; // Software result
  
  uint32_t   ilay_raw_str;  // Backup raw pixels
  uint32_t * ilay_raw_addr; // 

  //stride and base address must to be DW aligned !
  uint32_t   ilay_hw_str;  // the stripe is diff from ilay_argb_str when DST_FMT is { RGB555 or RGB565 }
  uint32_t * ilay_hw_addr; // Hardware result

  uint32_t  ilay_bak_argb;
  uint32_t  ilay_apos;
  uint32_t  ilay_fmt;
  uint32_t  ilay_rgb_mode;

}x2d_tbench_layb_verc, *x2d_tbench_layb_verc_p;


typedef struct x2d_tbench_verc_info{
  uint16_t lay_num;
  uint16_t cmd_tlb_en;
  uint16_t dst_tlb_en;
  uint16_t ddr_bank_sel;
  uint8_t  shart_level;
  x2d_tbench_layb_verc layb_ver;
  x2d_tbench_lay_verc  layx_ver[4];
}x2d_tbench_verc_info, *x2d_tbench_verc_info_p;

typedef struct x2d_pic_info{
  uint32_t *pic_buf;
  uint32_t pic_w;
  uint32_t pic_h;
  uint32_t pic_str;  
  uint32_t pic_en;
  uint32_t chns;
}x2d_pic_info, *x2d_pic_info_p;


#define X2D_OSD_MSRC_OVER 0
#define X2D_OSD_MSRC      1
#define X2D_OSD_MDST	  2
#define X2D_OSD_MCLEAR	  3
#define X2D_OSD_MDST_OVER 4
#define X2D_OSD_MSRC_IN	  5
#define X2D_OSD_MDST_IN   6
#define X2D_OSD_MSRC_OUT  7
#define X2D_OSD_MDST_OUT  8
#define X2D_OSD_MSRC_ATOP 9
#define X2D_OSD_MDST_ATOP 10
#define X2D_OSD_MXOR      11


#define X2D_ERR_OPT
#ifdef X2D_ERR_OPT
#define x2d_err_printf  printk
#else
void x2d_err_printf (const char* ptr, ...)
{  
}
#endif

//#define do_get_phy_addr(addr) (get_phy_addr(addr))
#define do_get_vaddr(addr) (addr)

#endif

