
#include <stdio.h>
#include <stdlib.h>

#include <base.h>
#include <init.h>
#include <mem.h>
#include <cache.h>
#include <mips.h>
#include <test/fw.h>

#include <iomap.h>
#include <irq.h>

#include <math.h>
#include <time.h>
#include "x2d.h"

#define SEED_CFG
#define SEED_CORE 0x158f5

#define MAX_X2D_TEST_TIMES 1000

//#define X2D_DEBUG_M
#define X2D_SHOW

#define X2D_IRQ_ID 19
#define X2D_IRQ_MODE
//#define USER_CFG_OPT

#define LAY_SRC_SMAXW 520
#define LAY_SRC_SMAXH 520
#define LAY_SRC_DMAXW 520
#define LAY_SRC_DMAXH 520
#define LAY_DST_MAXW 300
#define LAY_DST_MAXH 300

#define random() rand() 

static volatile int *x2d_base;

#define write_x2d_reg(off, value) (*(x2d_base + (off)/4) = (value))

#define read_x2d_reg(off) (*(x2d_base + (off)/4))

struct x2d_data {
#define TEST_SIZE 200*1024
  int times;
  x2d_chain_info_p chn_ptr;
  x2d_tbench_verc_info_p verc_ptr;
};


#define sat_to_0_255(a)  ( (int)(a) > 255 ? 255 : (int)(a)<0? 0 : (a) )

uint32_t get_coef(uint32_t x) //coef * 512
{
  
  int32_t a = -2;
  int32_t a_plus_3 = (a + 3*4);
  int32_t a_plus_2 = (a + 2*4);
  //double x_point = (double)x/512;
  //double x_result;

  if ( x < 512 ){
    int32_t x2 = x*x; // coef*coef*512*512
    int32_t x3 = x*x*x; //coef*coef*512*512*coef*512
    int32_t x2_result = ( x2 * a_plus_3 );    
    x2_result = x2_result >> 9;
    
    int32_t x3_result = a_plus_2 * x3 ;
    x3_result = x3_result >> 18;

    int32_t one_min_x2_result = 512*4 - x2_result;

    int32_t final_result = x3_result + one_min_x2_result;

    final_result = final_result >> 2;
    return final_result;
  }
  else if ( x <= 1024 ){
    int32_t x2 = x*x; // coef*coef*512*512
    int32_t x3 = x*x*x; //coef*coef*512*512*coef*512
    int32_t a4 = 4*a*512;
    int32_t x_result = ( 8 * a * x );

    int32_t x_result_min_4a = x_result - a4;

    int32_t x2_result =  ( 5*a*x2 ) >> 9;
    int32_t x2_result_min_xresult = x2_result - x_result_min_4a;

    int32_t x3_result = ( a*x3 ) >> 18;
    int32_t x3_result_final = x3_result - x2_result_min_xresult;
    x3_result_final = x3_result_final >> 2;
    return x3_result_final;
  }
  else
    return 0;
}


void blk_copyw_fm(
		   uint32_t *src, uint32_t src_w, uint32_t src_h, uint32_t src_str,
		   uint32_t *dst, uint32_t dst_w, uint32_t dst_h, uint32_t dst_str,
		   uint32_t bk_argb, uint32_t tile
		   )
{
  uint32_t x;
  uint32_t y;
  for ( y = 0; y < dst_h; y++)
    for ( x = 0; x < dst_w; x++)
      {
	if ( tile == 0 ){ 
	  if ( x >= src_w || y >= src_h )
	    dst[x+y*(dst_str/(sizeof(uint32_t)))] = bk_argb;
	  else
	    dst[x+y*(dst_str/(sizeof(uint32_t)))] = src[x+y*(src_str/(sizeof(uint32_t)))];
	}else{
	  uint32_t x_new;
	  x_new = ((x/16)*256) + x%16 + (y%16)*16;
	  uint32_t y_new;
	  y_new = y/16;
	  if ( x >= src_w || y >= src_h )
	    dst[x_new+y_new*(dst_str/(sizeof(uint32_t)))] = bk_argb;
	  else
	    dst[x_new+y_new*(dst_str/(sizeof(uint32_t)))] = src[x+y*(src_str/(sizeof(uint32_t)))];
	}
      }
}

void recovery_bk_fm(uint32_t *src, uint32_t *dst, uint32_t size)
{
  uint32_t i;
  for( i = 0; i< (size/4) ; i++ ){
    dst[i] = src[i];
  }

}



uint32_t blk_cmp_fm(
		   uint32_t *src, uint32_t src_w, uint32_t src_h, uint32_t src_str,
		   uint32_t *dst, uint32_t dst_w, uint32_t dst_h, uint32_t dst_str, 
		   uint32_t dst_tile_en
		   )
{
  uint32_t x;
  uint32_t y;
  for ( y = 0; y < dst_h; y++)
    for ( x = 0; x < dst_w; x++)
      {
	if ( dst_tile_en == 1 ){
	  uint32_t new_x;
	  uint32_t new_y;
	  new_x = (x/16)*256 + (x%16) + (y%16)*16;
	  new_y = y/16;
	  if ( dst[new_x+new_y*(dst_str/(sizeof(uint32_t)))] != src[x+y*(src_str/(sizeof(uint32_t)))] )
	    {
	    x2d_err_printf("+++++++++++++++++FM CMP ERROR: [0x%x,0x%x]   ref-src++++\n", x, y);
	    x2d_err_printf("ref:0x%x-0x%x, src:0x%x\n", (uint32_t)&src[x+y*(src_str/(sizeof(uint32_t)))], src[x+y*(src_str/(sizeof(uint32_t)))],
			   dst[new_x+new_y*(dst_str/(sizeof(uint32_t)))]
			   );
	    return 1;
	  }	  
	}else {
	  if ( dst[x+y*(dst_str/(sizeof(uint32_t)))] != src[x+y*(src_str/(sizeof(uint32_t)))] )
	    {
	    x2d_err_printf("+++++++++++++++++FM CMP ERROR: [0x%x,0x%x]   ref-src++++\n", x, y);
	    x2d_err_printf("ref:0x%x-0x%x, src:0x%x\n", (uint32_t)&src[x+y*(src_str/(sizeof(uint32_t)))], src[x+y*(src_str/(sizeof(uint32_t)))],
			   dst[x+y*(dst_str/(sizeof(uint32_t)))]
			   );
	    return 1;
	  }
	}
      }
  return 0;
}

uint32_t resize_pix( uint32_t src[4][4],
		     uint32_t xcoef[4],
		     uint32_t ycoef[4]
		     )
{
  uint32_t i;
  uint32_t yrsz_result[4];
  int32_t tmp = 0;
  for ( i = 0; i<4; i++){
    tmp = src[0][i]*ycoef[0] + src[1][i]*ycoef[1] + src[2][i]*ycoef[2] + src[3][i]*ycoef[3];
    tmp = tmp < 0 ? 0: tmp;
    tmp = (tmp + 256) >> 9 ;
    tmp = tmp > 255? 255: tmp;
    yrsz_result[i] = tmp;
  }

  tmp = yrsz_result[0] * xcoef[0] + yrsz_result[1] * xcoef[1] + yrsz_result[2] * xcoef[2] + yrsz_result[3] * xcoef[3];
  tmp = tmp < 0 ? 0: tmp;
  tmp = (tmp + 256) >> 9;
  tmp = tmp > 255? 255: tmp;
  return tmp;
}

uint32_t resize_pix_unit( uint32_t src[4],
			  uint32_t coef[4]
			  )
{
  //uint32_t yrsz_result[4];
  int32_t tmp = 0;

  if ( src[0] == src[1] && src[2] == src[3] && src[1] == src[2] )
    return src[0];
  tmp = src[0]*coef[0] + src[1]*coef[1] + src[2]*coef[2] + src[3]*coef[3];
  tmp = tmp < 0 ? 0: tmp;
  tmp = (tmp + 256) >> 9 ;
  tmp = tmp > 255? 255: tmp;
  return tmp;
}
		 

void fm_resize ( uint32_t *src, uint32_t src_w, uint32_t src_h, uint32_t src_str,
		 uint32_t *dst, uint32_t dst_w, uint32_t dst_h, uint32_t dst_str,
		 uint32_t bk_argb, uint32_t xrsz_coef, uint32_t yrsz_coef, 
		 uint32_t csc_en, uint32_t csc_mod, uint32_t src_fmt
		 )
{
  uint32_t rxpos;
  uint32_t rypos;

  uint32_t yrsz_pos;
  uint32_t xrsz_pos;
  
  yrsz_pos = 0;
  for ( rypos = 0; rypos <dst_h; rypos++)
    {
      xrsz_pos = 0;
      uint32_t yrsz_coef_m = yrsz_pos & 0x1FF;
      
      uint32_t ycoef_pos0 = yrsz_coef_m + 1*512;
      uint32_t ycoef_pos1 = yrsz_coef_m        ;
      uint32_t ycoef_pos2 = 512 - yrsz_coef_m  ;
      uint32_t ycoef_pos3 = 1024 - yrsz_coef_m ;

      uint32_t ycoef[4];
      ycoef[0] = get_coef( ycoef_pos0 );
      ycoef[1] = get_coef( ycoef_pos1 );
      ycoef[2] = get_coef( ycoef_pos2 );
      ycoef[3] = get_coef( ycoef_pos3 );     

      for( rxpos = 0; rxpos <dst_w; rxpos++)
	{
	  uint32_t xrsz_coef_m = xrsz_pos & 0x1FF;
	  
	  uint32_t xcoef_pos0 = xrsz_coef_m + 1*512;
	  uint32_t xcoef_pos1 = xrsz_coef_m        ;
	  uint32_t xcoef_pos2 = 512 - xrsz_coef_m  ;
	  uint32_t xcoef_pos3 = 1024 - xrsz_coef_m ;
	  

	  uint32_t xcoef[4];
	  xcoef[0] = get_coef( xcoef_pos0 );

	  xcoef[1] = get_coef( xcoef_pos1 );

	  xcoef[2] = get_coef( xcoef_pos2 );

	  xcoef[3] = get_coef( xcoef_pos3 );


	  /*------------------ fetch 4x4 src pix ------------------------------*/	  
	  uint32_t a_pos[4];
	  uint32_t r_pos[4];
	  uint32_t g_pos[4];
	  uint32_t b_pos[4];
	  int32_t src_ypos;
	  int32_t src_xpos;
	  int x;
	  int y;
	  y = 0;
	  x = 0;

	  uint32_t a_pos_tmp[4];
	  uint32_t r_pos_tmp[4];
	  uint32_t g_pos_tmp[4];
	  uint32_t b_pos_tmp[4];
	  x = 0;
	  for ( src_xpos = ((int32_t)(xrsz_pos >> 9) -1); src_xpos <= (int32_t)((xrsz_pos >> 9) + 2); src_xpos++)
	    {
	      src_ypos = (int32_t)(yrsz_pos >> 9)-1;	  
	      y = 0;
	      for ( ; src_ypos <= (int32_t)((yrsz_pos >> 9) + 2); src_ypos++){
		uint32_t argb_src = bk_argb;		
		if (src_ypos >= 0 && src_ypos < src_h && src_xpos >=0 && src_xpos < src_w )
		  {
		    uint32_t *src_addr = src + src_ypos * (src_str/(sizeof(uint32_t))) + src_xpos;
		    argb_src = *src_addr;
		  }


		a_pos_tmp[y] = (argb_src >> 24) & 0xff;
		r_pos_tmp[y] = (argb_src >> 16) & 0xff;
		g_pos_tmp[y] = (argb_src >> 8 ) & 0xff;
		b_pos_tmp[y] = (argb_src >> 0 ) & 0xff;
		
		y++;
	    }
	      if ( src_xpos <0 || src_xpos >= src_w )
		{
		  a_pos[x] = (bk_argb >> 24) & 0xFF;
		  r_pos[x] = (bk_argb >> 16) & 0xFF;
		  g_pos[x] = (bk_argb >> 8) & 0xFF;
		  b_pos[x] = (bk_argb >> 0) & 0xFF;
		}else
		  {
		    if ( a_pos_tmp[0] == a_pos_tmp[1] && a_pos_tmp[2] == a_pos_tmp[3] && a_pos_tmp[1] == a_pos_tmp[2])
		      a_pos[x] = a_pos_tmp[0];
		    else
		      a_pos[x] = resize_pix_unit(a_pos_tmp,ycoef);

		    if ( r_pos_tmp[0] == r_pos_tmp[1] && r_pos_tmp[2] == r_pos_tmp[3] && r_pos_tmp[1] == r_pos_tmp[2])
		      r_pos[x] = r_pos_tmp[0];
		    else
		      r_pos[x] = resize_pix_unit(r_pos_tmp,ycoef);

		    if ( src_fmt == SRC_RGB555 || src_fmt == SRC_RGB565 )
		      {
			r_pos[x] = r_pos[x] & 0xf8;
		      }

		    


		    if ( g_pos_tmp[0] == g_pos_tmp[1] && g_pos_tmp[2] == g_pos_tmp[3] && g_pos_tmp[1] == g_pos_tmp[2])
		      g_pos[x] = g_pos_tmp[0];
		    else
		      g_pos[x] = resize_pix_unit(g_pos_tmp,ycoef);

		    if ( src_fmt == SRC_RGB555 || src_fmt == SRC_RGB565 )
		      {
			g_pos[x] = g_pos[x] & 0xFC;
		      }


		    if ( b_pos_tmp[0] == b_pos_tmp[1] && b_pos_tmp[2] == b_pos_tmp[3] && b_pos_tmp[1] == b_pos_tmp[2])
		      b_pos[x] = b_pos_tmp[0];
		    else
		      b_pos[x] = resize_pix_unit(b_pos_tmp,ycoef);

		    if ( src_fmt == SRC_RGB555 || src_fmt == SRC_RGB565 )
		      {
			b_pos[x] = b_pos[x] & 0xf8;
		      }

		  }
	      x++;
	  }
	  /*------------------ caculate pix ------------------------------*/	  
	  uint32_t A_result;
	  uint32_t R_result;
	  uint32_t G_result;
	  uint32_t B_result;
	  A_result = resize_pix_unit(a_pos, xcoef);
	  R_result = resize_pix_unit(r_pos, xcoef);
	  G_result = resize_pix_unit(g_pos, xcoef);
	  B_result = resize_pix_unit(b_pos, xcoef);

	  if ( csc_en == 1){
	    int32_t csc_c0;
	    int32_t csc_c1;
	    int32_t csc_c2;
	    int32_t csc_c3;
	    int32_t csc_c4;

	    int32_t tmp;
	    int32_t tmpr;
	    int32_t tmpg;
	    int32_t tmpb;
	    if ( csc_mod == 0 ){
	      csc_c0 = M0_YUV_CSC_C0;
	      csc_c1 = M0_YUV_CSC_C1;
	      csc_c2 = M0_YUV_CSC_C2;
	      csc_c3 = M0_YUV_CSC_C3;
	      csc_c4 = M0_YUV_CSC_C4;
	    }else
	      {
	      csc_c0 = M1_YUV_CSC_C0;
	      csc_c1 = M1_YUV_CSC_C1;
	      csc_c2 = M1_YUV_CSC_C2;
	      csc_c3 = M1_YUV_CSC_C3;
	      csc_c4 = M1_YUV_CSC_C4;
	      }
	    tmp = csc_c0 * R_result;
	    tmpr = tmp + csc_c1 * (int)(B_result - 128);

	    tmpg = tmp - csc_c2 * (int)(G_result - 128) - csc_c3 * (int)(B_result - 128);

	    tmpb = tmp + csc_c4 * (int)(G_result - 128);

	    tmpr = (sat_to_0_255(tmpr>>10) & 0x000000FF);
	    tmpg = (sat_to_0_255(tmpg>>10) & 0x000000FF);
	    tmpb = (sat_to_0_255(tmpb>>10) & 0x000000FF);

	    R_result = tmpr;
	    G_result = tmpg;
	    B_result = tmpb;
	  }
	  
	  uint32_t new_argb;
	  new_argb = (A_result<<24) | 
	             (R_result<<16) | 
	             (G_result<<8) | 
	             (B_result<<0)
	    ;
	  uint32_t * dst_pix_addr = dst + rypos * (dst_str/sizeof(uint32_t)) + rxpos;
	  *dst_pix_addr = new_argb;


	  xrsz_pos = xrsz_pos + xrsz_coef;
	}
      yrsz_pos = yrsz_pos + yrsz_coef;
    }
}

uint32_t x2d_osd_unit(uint32_t src_argb, uint32_t src_prem, 
		    uint32_t dst_argb, uint32_t dst_prem, 
		    uint32_t osd_mode,
		    uint32_t disp
		    )
{
  uint32_t src_a = (src_argb >> 24) & 0xff;
  uint32_t src_r = (src_argb >> 16) & 0xff;
  uint32_t src_g = (src_argb >> 8) & 0xff;
  uint32_t src_b = (src_argb >> 0) & 0xff;
  uint32_t fs = 0;
  uint32_t dst_a = (dst_argb >> 24) & 0xff;
  uint32_t dst_r = (dst_argb >> 16) & 0xff;
  uint32_t dst_g = (dst_argb >> 8) & 0xff;
  uint32_t dst_b = (dst_argb >> 0) & 0xff;
  uint32_t fd = 0 ;
  if ( disp == 1 )
    {
      x2d_err_printf("+++ [A, R, G, B]: src-0x%x,0x%x,0x%x,0x%x\n", src_a, src_r, src_g, src_b);
      x2d_err_printf("+++ [A, R, G, B]: dst-0x%x,0x%x,0x%x,0x%x\n", dst_a, dst_r, dst_g, dst_b);
    }
  switch ( osd_mode )
    {
    case X2D_OSD_MSRC_OVER: 
      fs = 255; 
      fd = 255 - src_a;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MSRC: 
      fs = 255; 
      fd = 0;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      


    case X2D_OSD_MDST: 
      fs = 0; 
      fd = 255;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MCLEAR: 
      fs = 0; 
      fd = 0;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MDST_OVER: 
      fs = 255 - dst_a; 
      fd = 255;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MSRC_IN: 
      fs = dst_a; 
      fd = 0;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MDST_IN: 
      fs = 0; 
      fd = src_a;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MSRC_OUT: 
      fs = 255 - dst_a; 
      fd = 0;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MDST_OUT: 
      fs = 0; 
      fd = 255 - src_a;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MSRC_ATOP: 
      fs = dst_a; 
      fd = 255 - src_a;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MDST_ATOP: 
      fs = 255 - dst_a; 
      fd = src_a;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      

    case X2D_OSD_MXOR: 
      fs = 255 - dst_a;  
      fd = 255 - src_a;
      if ( disp == 1 ){
	x2d_err_printf("+++ fs 0x%x\n", fs);
	x2d_err_printf("+++ fd 0x%x\n", fd);
      }
      break;      
    }
  uint32_t src_cs_r;
  uint32_t src_cs_g;
  uint32_t src_cs_b;

  uint32_t dst_cd_r;
  uint32_t dst_cd_g;
  uint32_t dst_cd_b;


  src_a = (src_a == 255)? 256: src_a;
  dst_a = (dst_a == 255)? 256: dst_a;
  fs = ( fs == 255 )? 256: fs;
  fd = ( fd == 255 )? 256: fd;
  

  if ( src_prem == 1){
    src_cs_r = src_r * 256; 
    src_cs_g = src_g * 256; 
    src_cs_b = src_b * 256; 
  }else
    {
      src_cs_r = src_r * src_a; 
      src_cs_g = src_g * src_a; 
      src_cs_b = src_b * src_a;
    }


  if ( dst_prem == 1){
    dst_cd_r = dst_r * 256; 
    dst_cd_g = dst_g * 256; 
    dst_cd_b = dst_b * 256; 
  }else
    {
      dst_cd_r = dst_r * dst_a; 
      dst_cd_g = dst_g * dst_a; 
      dst_cd_b = dst_b * dst_a;
    }

  if ( disp == 1 ){
    x2d_err_printf("+++ Cs[R, G, B]: 0x%x,0x%x,0x%x\n", src_cs_r, src_cs_g, src_cs_b);
    x2d_err_printf("+++ Cd[R, G, B]: 0x%x,0x%x,0x%x\n", dst_cd_r, dst_cd_g, dst_cd_b);
  }

  uint32_t result_r = src_cs_r * fs + dst_cd_r * fd;
  uint32_t result_g = src_cs_g * fs + dst_cd_g * fd;
  uint32_t result_b = src_cs_b * fs + dst_cd_b * fd;
  uint32_t result_a = src_a    * fs + dst_a    * fd;

  if ( disp == 1 ){
    x2d_err_printf("+++ S0[A, R, G, B]: 0x%x,0x%x,0x%x,0x%x\n", src_a * fs, src_cs_r * fs, src_cs_g * fs, src_cs_b * fs);
    x2d_err_printf("+++ D0[A, R, G, B]: 0x%x,0x%x,0x%x,0x%x\n", dst_a * fd, dst_cd_r * fd, dst_cd_g * fd, dst_cd_b * fd);
  }

  result_a = ((result_a + (128)) >> 8 );
  result_r = ((result_r + (1<<15)) >> 16 ) ;
  result_g = ((result_g + (1<<15))>> 16 ) ;
  result_b = ((result_b + (1<<15)) >> 16 );

  result_a = result_a > 255? 255: result_a;
  result_r = result_r > 255? 255: result_r;
  result_g = result_g > 255? 255: result_g;
  result_b = result_b > 255? 255: result_b;

  
  if ( disp == 1 ){
    x2d_err_printf("+++ RE[A, R, G, B]: 0x%x,0x%x,0x%x,0x%x\n", result_a, result_r, result_g, result_b);
  }
  uint32_t result ;
  result = (result_a << 24 ) | 
           (result_r << 16 ) | 
           (result_g << 8 ) | 
           (result_b << 0 )
           ;
  if ( disp == 1 ){
    x2d_err_printf("+++ RE[ARGB]: 0x%x\n", result);
  }
  return result; 

}

uint32_t x2d_osd_5layer_pix(uint32_t lb_argb, uint32_t lb_prem,
		      uint32_t l0_argb, uint32_t l0_prem,
		      uint32_t l1_argb, uint32_t l1_prem,
		      uint32_t l2_argb, uint32_t l2_prem,
		      uint32_t l3_argb, uint32_t l3_prem,
		      uint32_t osd_mode_l0, 
		      uint32_t osd_mode_l1, 
		      uint32_t osd_mode_l2, 
		      uint32_t osd_mode_l3, 
		      uint32_t dips
		   )
{
  uint32_t  L01_result;
  uint32_t  L23_result;
  uint32_t  L0123_result;
  //uint32_t  LB0123_result;
  if ( dips == 1 )
    x2d_err_printf("++++++++++++lay 2 3 0x%x-0x%x++++++++++++++++\n", l2_argb, l3_argb);
  
  L23_result = x2d_osd_unit(
			    l3_argb, l3_prem,
			    l2_argb, l2_prem,
			    osd_mode_l3,
			    dips
                           );

  if ( dips == 1 )
    x2d_err_printf("++++++++++++lay 0 1+++0x%x-0x%x+++++++++++++\n", l0_argb, l1_argb);

  L01_result = x2d_osd_unit(
			    l1_argb, l1_prem,
			    l0_argb, l0_prem,
			    osd_mode_l1,
			    dips
                           );


  if ( dips == 1 )
    x2d_err_printf("++++++++++++lay 0 1 2 3++++++++++++++++\n");

  L0123_result = x2d_osd_unit(
			    L23_result, 1,
			    L01_result, 1,
			    osd_mode_l2,
			    dips
                           );

  if ( dips == 1 )
    x2d_err_printf("++++++++++++lay all bak 0 1 2 3++++++++++++++++\n");

  L0123_result = x2d_osd_unit(
			    L0123_result, 1,
			    lb_argb, lb_prem,
			    osd_mode_l0,
			    dips
                           );

  return L0123_result;
}


void x2d_osd_5layer(
			uint32_t ilayb_en,
			uint32_t ilayb_pm,
			uint32_t ilayb_w,
			uint32_t ilayb_h,
			uint32_t ilayb_galpha_en,
			uint32_t ilayb_galpha,
			uint32_t* ilayb_addr,
			uint32_t ilayb_str,
			uint32_t ilayb_mask_en,
			uint32_t ilayb_bak_argb,

			uint32_t ilay0_en,
			uint32_t ilay0_pm,
			uint32_t ilay0_w,
			uint32_t ilay0_h,
			uint32_t ilay0_xoft,
			uint32_t ilay0_yoft,
			uint32_t ilay0_galpha_en,
			uint32_t ilay0_galpha,			
			uint32_t* ilay0_addr,
			uint32_t ilay0_str,
			uint32_t ilay0_mask_en,
			uint32_t ilay0_bak_argb,
			uint32_t ilay0_osdm,

			uint32_t ilay1_en,
			uint32_t ilay1_pm,
			uint32_t ilay1_w,
			uint32_t ilay1_h,
			uint32_t ilay1_xoft,
			uint32_t ilay1_yoft,
			uint32_t ilay1_galpha_en,
			uint32_t ilay1_galpha,			
			uint32_t* ilay1_addr,
			uint32_t ilay1_str,
			uint32_t ilay1_mask_en,
			uint32_t ilay1_bak_argb,
			uint32_t ilay1_osdm,


			uint32_t ilay2_en,
			uint32_t ilay2_pm,
			uint32_t ilay2_w,
			uint32_t ilay2_h,
			uint32_t ilay2_xoft,
			uint32_t ilay2_yoft,
			uint32_t ilay2_galpha_en,
			uint32_t ilay2_galpha,			
			uint32_t* ilay2_addr,
			uint32_t ilay2_str,
			uint32_t ilay2_mask_en,
			uint32_t ilay2_bak_argb,
			uint32_t ilay2_osdm,

			uint32_t ilay3_en,
			uint32_t ilay3_pm,
			uint32_t ilay3_w,
			uint32_t ilay3_h,
			uint32_t ilay3_xoft,
			uint32_t ilay3_yoft,
			uint32_t ilay3_galpha_en,
			uint32_t ilay3_galpha,			
			uint32_t* ilay3_addr,
			uint32_t ilay3_str,
			uint32_t ilay3_mask_en,
			uint32_t ilay3_bak_argb,
			uint32_t ilay3_osdm

                       )
{
  int xpos, ypos;
  
  for ( ypos = 0;  ypos < ilayb_h; ypos++)
    {
      for ( xpos = 0;  xpos < ilayb_w; xpos++){
	uint32_t lb_argb = 0;
	uint32_t l0_argb= 0;
	uint32_t l1_argb= 0;
	uint32_t l2_argb= 0;
	uint32_t l3_argb= 0;  
	uint32_t dips = 0;
#ifdef X2D_DEBUG_M
	if ( xpos == 0 && (ypos == 0x288 || ypos == 0x289) )
	  dips = 1;
#endif

	/*==========================lay 0 ===================*/
	if ( xpos >= ilay0_xoft &&
	     xpos < (ilay0_xoft + ilay0_w) &&
	     ypos >= ilay0_yoft &&
	     ypos < (ilay0_yoft + ilay0_h) && ilay0_en
	     ){
	  uint32_t *ptr = ilay0_addr + (xpos-ilay0_xoft) + (ypos-ilay0_yoft)*(ilay0_str/sizeof(uint32_t));
	  l0_argb = *ptr;
	  if ( ilay0_mask_en )
	    l0_argb = ilay0_bak_argb;
	  else
	    if ( ilay0_galpha_en )
	      {
		l0_argb = (l0_argb & 0xFFFFFF) | (ilay0_galpha << 24);
	      }
	}
	/*==========================lay 0 ===================*/

	/*==========================lay1 ===================*/
	if ( xpos >= ilay1_xoft &&
	     xpos < (ilay1_xoft + ilay1_w) &&
	     ypos >= ilay1_yoft &&
	     ypos < (ilay1_yoft + ilay1_h) && ilay1_en
	     ){
	  uint32_t *ptr = ilay1_addr + (xpos-ilay1_xoft) + (ypos-ilay1_yoft)*(ilay1_str/sizeof(uint32_t));
	  l1_argb = *ptr;	  
	  if ( ilay1_mask_en )
	    l1_argb = ilay1_bak_argb;
	  else
	  if ( ilay1_galpha_en )
	    {
	      l1_argb = (l1_argb & 0xFFFFFF) | (ilay1_galpha << 24);
	    }
	}
	/*==========================lay1 ===================*/

	/*==========================lay2 ===================*/
	if ( xpos >= ilay2_xoft &&
	     xpos < (ilay2_xoft + ilay2_w) &&
	     ypos >= ilay2_yoft &&
	     ypos < (ilay2_yoft + ilay2_h) && ilay2_en
	     ){
	  uint32_t *ptr = ilay2_addr + (xpos-ilay2_xoft) + (ypos-ilay2_yoft)*(ilay2_str/sizeof(uint32_t));
	  l2_argb = *ptr;	  
	  if ( ilay2_mask_en )
	    l2_argb = ilay2_bak_argb;
	  else
	  if ( ilay2_galpha_en )
	    {
	      l2_argb = (l2_argb & 0xFFFFFF) | (ilay2_galpha << 24);
	    }

	}
	/*==========================lay2 ===================*/

	/*==========================lay3 ===================*/
	if ( xpos >= ilay3_xoft &&
	     xpos < (ilay3_xoft + ilay3_w) &&
	     ypos >= ilay3_yoft &&
	     ypos < (ilay3_yoft + ilay3_h) && ilay3_en
	     ){
	  uint32_t *ptr = ilay3_addr + (xpos-ilay3_xoft) + (ypos-ilay3_yoft)*(ilay3_str/sizeof(uint32_t));
	  l3_argb = *ptr;	  
	  if ( ilay3_mask_en )
	    l3_argb = ilay3_bak_argb;
	  else
	  if ( ilay3_galpha_en )
	    {
	      l3_argb = (l3_argb & 0xFFFFFF) | (ilay3_galpha << 24);
	    }

	}
	/*==========================lay3 ===================*/
	if ( ilayb_en )
	{
	  uint32_t *ptr = ilayb_addr + xpos + ypos*(ilayb_str/sizeof(uint32_t));
	  lb_argb = *ptr;
	  if ( dips == 1)
	    printf("lay b: x-0x%x, y-0x%x, pix:0x%x\n", xpos, ypos, lb_argb);
	  if ( ilayb_mask_en )
	    lb_argb = ilayb_bak_argb;
	  else
	  if ( ilayb_galpha_en )
	    {
	      lb_argb = (lb_argb & 0xFFFFFF) | (ilayb_galpha << 24);
	    }
	}
	uint32_t osd_result_pix;
	osd_result_pix = x2d_osd_5layer_pix(
					    lb_argb, ilayb_pm,
					    l0_argb, ilay0_pm,
					    l1_argb, ilay1_pm,
					    l2_argb, ilay2_pm,
					    l3_argb, ilay3_pm,
					    ilay0_osdm, 
					    ilay1_osdm, 
					    ilay2_osdm, 
					    ilay3_osdm, 
					    dips
					    );
	
	/* give back */
	{
	  uint32_t *ptr = ilayb_addr + xpos + ypos*(ilayb_str/sizeof(uint32_t));
	  *ptr = osd_result_pix;
	}
      }
    }
}

void fm_rotate(
	      uint32_t *src, uint32_t src_w, uint32_t src_h, uint32_t src_str,
	      uint32_t *dst, uint32_t dst_w, uint32_t dst_h, uint32_t dst_str,
	      uint32_t rota_m, uint32_t hmirr, uint32_t vmirr
              )
{
  int x = 0;
  int y = 0;
  int src_xpos = 0;
  int src_ypos = 0;
  int dst_xpos = 0;
  int dst_ypos = 0;
  /* printk(" > rota fm now rota-%d\n", rota_m); */
  /* printk(" > src: w-%d, h-%d, str-%d\n", src_w, src_h, src_str); */
  /* printk(" > dst: w-%d, h-%d, str-%d\n", dst_w, dst_h, dst_str); */
  /* printk(" dst: %p, src: %p\n", dst, src); */
  for ( y = 0; y < dst_h; y++){
    for ( x = 0; x < dst_w; x++){
      dst_xpos = x;
      dst_ypos = y;
      if ( rota_m == 0)
	{
	  src_xpos = x;
	  src_ypos = y;	  
	}
      else if ( rota_m == 1 ) //90
	{
	  src_xpos = y;
	  src_ypos = (src_h - 1) - x;
	}else if ( rota_m == 2 ) //180
	  {
	    src_xpos = (src_w - 1) -x;
	    src_ypos = (src_h - 1) -y;
	  }
      else if ( rota_m == 3 )
	{
	  src_xpos = (src_w - 1) - y;
	  src_ypos = x;
	}

      if ( hmirr == 1 )
	{
	  dst_xpos = (dst_w - 1) - dst_xpos;
	}

      if ( vmirr ==  1 )
	{
	  dst_ypos = (dst_h - 1) - dst_ypos;
	}
      /* printk("--dst: [x,y]-[%d, %d]\n", dst_xpos, dst_ypos); */
      /* printk("--src: [x,y]-[%d, %d]\n", src_xpos, src_ypos); */
      dst[dst_xpos+dst_ypos*(dst_str/(sizeof(uint32_t)))] 
	= src[src_xpos + src_ypos * (src_str/(sizeof(uint32_t)))];
    }
  }      

}

void init_pix(
	      uint32_t *dst, uint32_t dst_w, uint32_t dst_h, uint32_t dst_str,
	      uint32_t bk_argb, uint32_t smod, uint32_t msk_en,
	      uint32_t raw_en, uint32_t *src_y, uint32_t *src_u, uint32_t *src_v ,uint32_t y_str,
	      uint32_t rgbm, uint32_t apos
              )
{
  int x;
  int y;
  uint16_t *hf_ptr;
  uint8_t *df_ptr;

  uint32_t src_pix;
  for ( y = 0; y< dst_h; y++){
    for ( x = 0 ; x < dst_w; x++){
      src_pix = random();
      
      if ( smod == SRC_RGB555 ){
	  src_pix = (src_pix & 0x00F8F8F8) | 0xFF000000;
      }else if ( smod == SRC_RGB565 ){
	  src_pix = (src_pix & 0x00F8FCF8) | 0xFF000000;
      }else if ( smod != 0 ){
	if ( ((x%2)==0) && (y%2==0) ){
	  src_pix = src_pix | 0xFF000000;
	}
	else{
	  src_pix = ((src_pix | 0xFF000000) & 0xFFFF0000) | 
	    ((dst[((int)(x/2))*2 + ((int)(y/2))*2*(dst_str/sizeof(uint32_t))]) & 
	     0x0000FFFF
	     );
	}
      }
      
      if(raw_en){
	if(smod == SRC_ARGB){
	  uint32_t tmpa = (src_pix >> 24)&0xFF;
	  uint32_t tmpr = (src_pix >> 16)&0xFF;
	  uint32_t tmpg = (src_pix >> 8)&0xFF;
	  uint32_t tmpb = (src_pix >> 0)&0xFF;
	  uint32_t new_rgb = 0;
	  switch ( rgbm ){
	  case SRC_FMT_RGB: new_rgb = (tmpr << 16) | (tmpg << 8) | (tmpb << 0); break;
	  case SRC_FMT_BGR: new_rgb = (tmpb << 16) | (tmpg << 8) | (tmpr << 0); break;
	  case SRC_FMT_RBG: new_rgb = (tmpr << 16) | (tmpb << 8) | (tmpg << 0); break;
	  case SRC_FMT_GBR: new_rgb = (tmpg << 16) | (tmpb << 8) | (tmpr << 0); break;
	  case SRC_FMT_GRB: new_rgb = (tmpg << 16) | (tmpr << 8) | (tmpb << 0); break;
	  case SRC_FMT_BRG: new_rgb = (tmpb << 16) | (tmpr << 8) | (tmpg << 0); break;
	  }
	  uint32_t new_argb;
	  if ( apos == 0 )
	    new_argb = (new_rgb<<8) | tmpa;
	  else
	    new_argb = (new_rgb) | (tmpa << 24);

	  src_y[x + y * (y_str/sizeof(uint32_t))] = new_argb;//src_pix;
/* 	  if ( x==0 && y== 0) */
/* 	  printf("new_argb:%x,", new_argb); */
	}else if(smod == SRC_RGB555){
	  uint32_t tmpb = ( (src_pix & 0xF8) >>3) & 0x1F;
	  uint32_t tmpg = ( (src_pix & 0xF800) >>11) & 0x1F;
	  uint32_t tmpr = ( (src_pix & 0xF80000) >>19) & 0x1F;
	  //uint32_t new_pix = tmpb | (tmpg << 5) | ( tmpr << 10 );
	  hf_ptr = (uint16_t *)src_y;
	  uint32_t new_rgb = 0;
	  switch ( rgbm ){
	  case SRC_FMT_RGB: new_rgb = (tmpr << 10) | (tmpg << 5) | (tmpb << 0); break;
	  case SRC_FMT_BGR: new_rgb = (tmpb << 10) | (tmpg << 5) | (tmpr << 0); break;
	  case SRC_FMT_RBG: new_rgb = (tmpr << 10) | (tmpb << 5) | (tmpg << 0); break;
	  case SRC_FMT_GBR: new_rgb = (tmpg << 10) | (tmpb << 5) | (tmpr << 0); break;
	  case SRC_FMT_GRB: new_rgb = (tmpg << 10) | (tmpr << 5) | (tmpb << 0); break;
	  case SRC_FMT_BRG: new_rgb = (tmpb << 10) | (tmpr << 5) | (tmpg << 0); break;
	  }
	  hf_ptr[x + y * (y_str/sizeof(uint16_t))] = new_rgb;
	  
	}else if(smod == SRC_RGB565){
	  uint32_t tmpb = ( (src_pix & 0xF8) >>3) & 0x1F;
	  uint32_t tmpg = ( (src_pix & 0xFC00) >>10) & 0x3F;
	  uint32_t tmpr = ( (src_pix & 0xF80000) >>19) & 0x1F;
	  //uint32_t new_pix = tmpa | (tmpb << 5) | ( tmpc << 11 );
	  uint32_t new_rgb = 0;
	  switch ( rgbm ){
	  case SRC_FMT_RGB: new_rgb = (tmpr << 11) | (tmpg << 5) | (tmpb << 0); break;
	  case SRC_FMT_BGR: new_rgb = (tmpb << 11) | (tmpg << 5) | (tmpr << 0); break;
	  case SRC_FMT_RBG: new_rgb = (tmpr << 11) | (tmpb << 6) | (tmpg << 0); break;
	  case SRC_FMT_GBR: new_rgb = (tmpg << 10) | (tmpb << 5) | (tmpr << 0); break;
	  case SRC_FMT_GRB: new_rgb = (tmpg << 10) | (tmpr << 5) | (tmpb << 0); break;
	  case SRC_FMT_BRG: new_rgb = (tmpb << 11) | (tmpr << 6) | (tmpg << 0); break;
	  }
	  hf_ptr = (uint16_t *)src_y;
	  
	  hf_ptr[x + y * (y_str/sizeof(uint16_t))] = new_rgb;//new_pix;

	}else if(smod == SRC_YUV_SEP){
	  uint32_t tmpa = (src_pix & 0xFF) & 0xFF;
	  uint32_t tmpb = ( (src_pix & 0xFF00) >>8) & 0xFF;
	  uint32_t tmpc = ( (src_pix & 0xFF0000) >>16) & 0xFF;
	  df_ptr = (uint8_t *)src_y;
	  df_ptr[x + y * (y_str/sizeof(uint8_t))] = tmpc;

	  df_ptr = (uint8_t *)src_u;
	  df_ptr[x/2 + y/2 * (y_str/2/sizeof(uint8_t))] = tmpb;

	  df_ptr = (uint8_t *)src_v;
	  df_ptr[x/2 + y/2 * (y_str/2/sizeof(uint8_t))] = tmpa;
	  
	  
	}else if(smod == SRC_YUV_TILE){
	  uint32_t tmpa = (src_pix & 0xFF) & 0xFF;
	  uint32_t tmpb = ( (src_pix & 0xFF00) >>8) & 0xFF;
	  uint32_t tmpc = ( (src_pix & 0xFF0000) >>16) & 0xFF;

	  uint32_t blk_x = x/16;
	  uint32_t blk_y = y/16;

	  uint32_t ofst_x = x%16;
	  uint32_t ofst_y = y%16;
	  //printf("blk (%x ,%x) (%x ,%x) : pix %x %x %x \n",blk_x,blk_y ,ofst_x,ofst_y , tmpc , tmpb, tmpa);

	  df_ptr = (uint8_t *)src_y;
	  df_ptr[blk_x*256 + ofst_x + ofst_y*16 + blk_y*y_str] = tmpc;

	  df_ptr = (uint8_t *)src_u;
	  df_ptr[blk_x*128 + (ofst_x/2) + (ofst_y/2)*16 + blk_y*y_str/2] = tmpb;
	  df_ptr[blk_x*128 + (ofst_x/2) + 8 + (ofst_y/2)*16 + blk_y*y_str/2] = tmpa;

	}else if(smod == SRC_YUV_NV12){
	  uint32_t tmpa = (src_pix & 0xFF) & 0xFF;
	  uint32_t tmpb = ( (src_pix & 0xFF00) >>8) & 0xFF;
	  uint32_t tmpc = ( (src_pix & 0xFF0000) >>16) & 0xFF;
	  df_ptr = (uint8_t *)src_y;
	  df_ptr[x + y * (y_str/sizeof(uint8_t))] = tmpc;

	  uint32_t new_pix = tmpb | (tmpa << 8);
	  hf_ptr = (uint16_t *)src_u;
	  hf_ptr[x/2 + y/2 * (y_str/sizeof(uint16_t))] = new_pix;

	}else if(smod == SRC_YUV_NV21){
	  uint32_t tmpa = (src_pix & 0xFF) & 0xFF;
	  uint32_t tmpb = ( (src_pix & 0xFF00) >>8) & 0xFF;
	  uint32_t tmpc = ( (src_pix & 0xFF0000) >>16) & 0xFF;
	  df_ptr = (uint8_t *)src_y;
	  df_ptr[x + y * (y_str/sizeof(uint8_t))] = tmpc;

	  uint32_t new_pix = tmpa | (tmpb << 8);
	  hf_ptr = (uint16_t *)src_u;
	  hf_ptr[x/2 + y/2 * (y_str/sizeof(uint16_t))] = new_pix;

	}
      }
      
      dst[x + y * (dst_str/sizeof(uint32_t))] = src_pix ;
    }
  }
}



void init_verc(x2d_tbench_verc_info_p verc_ptr)
{

  verc_ptr->lay_num = random()%5;
#ifdef USER_CFG_OPT
  verc_ptr->lay_num = 4;
#endif  
  verc_ptr->lay_num = 4;
  uint32_t max_xpos = 0;
  uint32_t max_ypos = 0;  
  int i;

  verc_ptr->cmd_tlb_en = random()%2;
  verc_ptr->dst_tlb_en = random()%2;

  verc_ptr->cmd_tlb_en = 0;
  verc_ptr->dst_tlb_en = 0;

  for ( i = 0 ; i<verc_ptr->lay_num; i++)
    {
      { int ii;
      for ( ii = 0 ; ii<10000; ii++);
      }
      verc_ptr->layx_ver[i].ilay_en = 1; //here not modify

      verc_ptr->layx_ver[i].ilay_pm = random()%2;
      verc_ptr->layx_ver[i].ilay_bak_argb = (uint32_t)(random());//|0xFF000000;
      verc_ptr->layx_ver[i].ilay_galpha_en = random()%2;
      verc_ptr->layx_ver[i].ilay_galpha  = random()%256;
      verc_ptr->layx_ver[i].ilay_smode = random()%7;
      verc_ptr->layx_ver[i].ilay_csc_mod = random()%2;
      verc_ptr->layx_ver[i].ilay_osdm = random()%12;
      verc_ptr->layx_ver[i].ilay_rgbm = random()%6;
      verc_ptr->layx_ver[i].ilay_mask_en = random()%2;

      verc_ptr->layx_ver[i].ilay_rota = random()%4;

      verc_ptr->layx_ver[i].ilay_hmirror = random()%2;
      verc_ptr->layx_ver[i].ilay_vmirror = random()%2;
      verc_ptr->layx_ver[i].ilay_apos = random()%2;
      verc_ptr->layx_ver[i].ilayer_tlb_en = random()%2;
      verc_ptr->layx_ver[i].ilayer_tlb_en = 0;
      if ( i == 0 )
	verc_ptr->layx_ver[i].ilay_osdm = 0;



#ifdef USER_CFG_OPT
      verc_ptr->layx_ver[i].ilay_pm = 0;
      verc_ptr->layx_ver[i].ilay_bak_argb = 0x72a20d7f;//|0xFF000000;
      verc_ptr->layx_ver[i].ilay_galpha_en = 0;
      verc_ptr->layx_ver[i].ilay_galpha  = 0xFF;
      verc_ptr->layx_ver[i].ilay_bak_argb = 0 ;

      verc_ptr->layx_ver[i].ilay_smode = SRC_YUV_TILE;

      verc_ptr->layx_ver[i].ilay_csc_mod = 0;
      verc_ptr->layx_ver[i].ilay_osdm = 0;
      verc_ptr->layx_ver[i].ilay_rgbm = 0;
      verc_ptr->layx_ver[i].ilay_apos = 1;
      verc_ptr->layx_ver[i].ilay_mask_en = 0;//random()%2;

      if ( i == 2 )
	{
	  verc_ptr->layx_ver[i].ilay_osdm = X2D_OSD_MSRC_OVER;
	}
#endif   

      if ( verc_ptr->layx_ver[i].ilay_smode == SRC_RGB565 )
	{
	  verc_ptr->layx_ver[i].ilay_bak_argb =  verc_ptr->layx_ver[i].ilay_bak_argb & 0xFFF8FCF8;
	}
      else if ( verc_ptr->layx_ver[i].ilay_smode == SRC_RGB555 )
	{
	  verc_ptr->layx_ver[i].ilay_bak_argb =  verc_ptr->layx_ver[i].ilay_bak_argb & 0xFFF8F8F8;
	}
   
      if ( verc_ptr->layx_ver[i].ilay_smode != 0 )
	verc_ptr->layx_ver[i].ilay_galpha_en = 1;
      
      double ratio = 0;
      do{
	verc_ptr->layx_ver[i].ilay_src_width = random()%LAY_SRC_SMAXW + 4;
	verc_ptr->layx_ver[i].ilay_dst_width = random()%LAY_SRC_DMAXW + 4;


	if ( verc_ptr->layx_ver[i].ilay_smode == SRC_YUV_TILE ){
	  verc_ptr->layx_ver[i].ilay_src_width = (verc_ptr->layx_ver[i].ilay_src_width + 15) & 0xFFFFFFF0;
	}

	ratio = (double)verc_ptr->layx_ver[i].ilay_dst_width/(double)verc_ptr->layx_ver[i].ilay_src_width;	
      }while( ratio >= 128 || ratio <= 0.0078125);
      ratio = 0;
      do{
	verc_ptr->layx_ver[i].ilay_src_height = random()%LAY_SRC_SMAXH+4;

	if ( verc_ptr->layx_ver[i].ilay_smode == SRC_YUV_TILE )
	  verc_ptr->layx_ver[i].ilay_src_height = ( verc_ptr->layx_ver[i].ilay_src_height + 15) & 0xFFFFFFF0;
	
	verc_ptr->layx_ver[i].ilay_dst_height = random()%LAY_SRC_DMAXH + 4;
	ratio = (double)verc_ptr->layx_ver[i].ilay_dst_height/(double)verc_ptr->layx_ver[i].ilay_src_height;	
      }while( ratio >= 128 || ratio <= 0.0078125);
      
      
      //verc_ptr->layx_ver[i].ilay_dst_xoft = random()%LAY_SRC_DMAXW;
      //verc_ptr->layx_ver[i].ilay_dst_yoft = random()%LAY_SRC_DMAXH;
      verc_ptr->layx_ver[i].ilay_dst_xoft = random()%16;
      verc_ptr->layx_ver[i].ilay_dst_yoft = random()%16;

#ifdef USER_CFG_OPT
      verc_ptr->layx_ver[i].ilay_src_width  = 32;//(random()%10+1)*16;
      verc_ptr->layx_ver[i].ilay_src_height = 32;//(random()%10+1)*16;

      verc_ptr->layx_ver[i].ilay_dst_width  = 32;//(random()%720 + 16);
      verc_ptr->layx_ver[i].ilay_dst_height = 32;//(random()%720 + 16);
      verc_ptr->layx_ver[i].ilay_dst_xoft = 0;//random()%16;
      verc_ptr->layx_ver[i].ilay_dst_yoft = 0;//random()%16;

      verc_ptr->layx_ver[i].ilay_rota = 0;
      verc_ptr->layx_ver[i].ilay_hmirror = 0;
      verc_ptr->layx_ver[i].ilay_vmirror = 0;      
#endif

 
      int format = verc_ptr->layx_ver[i].ilay_smode;
      int format_yuv_nv12 = (format == SRC_YUV_NV12);
      int format_yuv_nv21 = (format == SRC_YUV_NV21) ;
      int format_yuv_nv = format_yuv_nv12 ||format_yuv_nv21;
      int format_yuvs = (format == SRC_YUV_SEP);
      int format_yuvt = (format == SRC_YUV_TILE);
      int format_yuv = format_yuv_nv || format_yuvs || format_yuvt;
      int format_rgb888 = (format == SRC_ARGB);
      int format_rgb565 = (format == SRC_RGB565);
      int format_rgb555 = (format == SRC_RGB555);
      int format_rgb2 = format_rgb565 || format_rgb555;
      //int format_rgb = format_rgb888 || format_rgb2;
      
      int  fm_rot = verc_ptr->layx_ver[i].ilay_rota == ROTATE_90 || verc_ptr->layx_ver[i].ilay_rota == ROTATE_270;
      //printf("fm_rot %x -- rot %x \n",fm_rot,verc_ptr->layx_ver[i].ilay_rota);
      
      uint32_t one_pix_unit = ( format_rgb888 ? 4 :
				format_rgb2 ? 2 : 1);
				
      if ( format_yuv )
	{
	  verc_ptr->layx_ver[i].ilay_src_width = (verc_ptr->layx_ver[i].ilay_src_width + 1)&0xFFFFFFFE;
	  verc_ptr->layx_ver[i].ilay_src_height = (verc_ptr->layx_ver[i].ilay_src_height + 1) & 0xFFFFFFFE;
	}

      //uint32_t rnd_h_len = verc_ptr->layx_ver[i].ilay_src_height + verc_ptr->layx_ver[i].ilay_src_height%2;
      uint32_t rnd_w_len = verc_ptr->layx_ver[i].ilay_src_width + verc_ptr->layx_ver[i].ilay_src_width%2;

      verc_ptr->layx_ver[i].ilay_argb_str = rnd_w_len*(sizeof(uint32_t));
      

      uint32_t w_len = rnd_w_len * one_pix_unit;
      uint32_t fix_w_len = (w_len%16 == 0) ? w_len : ( (w_len/16 + 1)*16); // DW align

      if(format_yuvt && (w_len%16 !=0) )
	printf("Error : TileYUV width error : %x \n",w_len);

      verc_ptr->layx_ver[i].ilay_ystr = format_yuvt ? (w_len/16 * 256) : fix_w_len;


    
      double hresiz_ratio;
      double vresiz_ratio;

      if ( (random()%9) != 0){
	hresiz_ratio = (double)random()/1000 ;
	do{
	  if ( (random()%9) != 1){
	    hresiz_ratio = (double)(random()%10000+1)/100000;
	  }else{
	    hresiz_ratio = (double)(random()%128)+((double)(random()%100000)/100000);
	  }
	  //}while(hresiz_ratio>=128 || hresiz_ratio <= 0.0078125);
	}while(hresiz_ratio>=127.5 || hresiz_ratio <= 0.007875);

	vresiz_ratio = (double)random()/10000 ;
	do{
	  if ( (random()%9) != 1)
	    vresiz_ratio = (double)(random()%10000+1)/100000;
	  else
	    vresiz_ratio = (double)(random()%128)+((double)(random()%100000)/100000);
	  //}while(vresiz_ratio>=128 || vresiz_ratio <= 0.0078125);
	}while(vresiz_ratio>=127.5 || vresiz_ratio <= 0.007875);
      }else
	{
	  hresiz_ratio = ((double)verc_ptr->layx_ver[i].ilay_dst_width / verc_ptr->layx_ver[i].ilay_src_width);
	  vresiz_ratio = (double)verc_ptr->layx_ver[i].ilay_dst_height / verc_ptr->layx_ver[i].ilay_src_height;
	  int tmp_w;
	  do{
	    tmp_w= verc_ptr->layx_ver[i].ilay_dst_width + (random()%100 - 50);
	  }while( tmp_w <= 4 || tmp_w >= 4096 );
	  verc_ptr->layx_ver[i].ilay_dst_width = tmp_w;

	  int tmp_h;
	  do{
	    tmp_h= verc_ptr->layx_ver[i].ilay_dst_height + (random()%100 - 50);
	  }while( tmp_h <= 4 || tmp_h >= 4096 );
	  verc_ptr->layx_ver[i].ilay_dst_height = tmp_h;
	}

      max_xpos = (verc_ptr->layx_ver[i].ilay_dst_xoft + verc_ptr->layx_ver[i].ilay_dst_width) > max_xpos? 
	         (verc_ptr->layx_ver[i].ilay_dst_xoft + verc_ptr->layx_ver[i].ilay_dst_width):
	         max_xpos;
      
      max_ypos = (verc_ptr->layx_ver[i].ilay_dst_yoft + verc_ptr->layx_ver[i].ilay_dst_height) > max_ypos? 
	         (verc_ptr->layx_ver[i].ilay_dst_yoft + verc_ptr->layx_ver[i].ilay_dst_height):
	         max_ypos;
      
#ifdef USER_CFG_OPT
      hresiz_ratio = (fm_rot ? 
		      ((double)verc_ptr->layx_ver[i].ilay_dst_height / verc_ptr->layx_ver[i].ilay_src_width):
		      ((double)verc_ptr->layx_ver[i].ilay_dst_width / verc_ptr->layx_ver[i].ilay_src_width) );
      vresiz_ratio = (fm_rot ? 
		      ((double)verc_ptr->layx_ver[i].ilay_dst_width / verc_ptr->layx_ver[i].ilay_src_height):
		      (double)verc_ptr->layx_ver[i].ilay_dst_height / verc_ptr->layx_ver[i].ilay_src_height );
#endif


      //printf(" >hratio: %f, vratio:%f\n", hresiz_ratio, vresiz_ratio);
      verc_ptr->layx_ver[i].ilay_hrsz_coef = (uint32_t)(((double)1/hresiz_ratio)*512);
      verc_ptr->layx_ver[i].ilay_vrsz_coef = (uint32_t)(((double)1/vresiz_ratio)*512);
      
      uint32_t rnd_rsz_w_len = verc_ptr->layx_ver[i].ilay_dst_width + verc_ptr->layx_ver[i].ilay_dst_width%2;

      verc_ptr->layx_ver[i].ilay_rsz_str =  rnd_rsz_w_len * sizeof(uint32_t); 

      init_pix(
	       verc_ptr->layx_ver[i].ilay_argb_addr,//src
	       verc_ptr->layx_ver[i].ilay_src_width, //src_w
	       verc_ptr->layx_ver[i].ilay_src_height, //src_w
	       verc_ptr->layx_ver[i].ilay_argb_str,   //src_str


	       verc_ptr->layx_ver[i].ilay_bak_argb,
	       verc_ptr->layx_ver[i].ilay_smode,
	       0,
	       1,
	       verc_ptr->layx_ver[i].ilay_yaddr,
	       verc_ptr->layx_ver[i].ilay_uaddr,
	       verc_ptr->layx_ver[i].ilay_vaddr,
	       verc_ptr->layx_ver[i].ilay_ystr,

	       verc_ptr->layx_ver[i].ilay_rgbm,
	       verc_ptr->layx_ver[i].ilay_apos
               );
      /* -------------------- here should have the rotate ---------------------*/
      if ( fm_rot ){
	
	verc_ptr->layx_ver[i].ilay_rota_w = verc_ptr->layx_ver[i].ilay_src_height;	    
	verc_ptr->layx_ver[i].ilay_rota_h = verc_ptr->layx_ver[i].ilay_src_width;

	verc_ptr->layx_ver[i].ilay_rota_xcoef = verc_ptr->layx_ver[i].ilay_vrsz_coef;
	verc_ptr->layx_ver[i].ilay_rota_ycoef = verc_ptr->layx_ver[i].ilay_hrsz_coef;
      }else{
	verc_ptr->layx_ver[i].ilay_rota_w = verc_ptr->layx_ver[i].ilay_src_width;
	verc_ptr->layx_ver[i].ilay_rota_h = verc_ptr->layx_ver[i].ilay_src_height;  

	verc_ptr->layx_ver[i].ilay_rota_xcoef = verc_ptr->layx_ver[i].ilay_hrsz_coef;
	verc_ptr->layx_ver[i].ilay_rota_ycoef = verc_ptr->layx_ver[i].ilay_vrsz_coef;
      }
      
      uint32_t rnd_rot_w_len = verc_ptr->layx_ver[i].ilay_rota_w + verc_ptr->layx_ver[i].ilay_rota_w%2;
      //uint32_t rnd_rot_h_len = verc_ptr->layx_ver[i].ilay_rota_h + verc_ptr->layx_ver[i].ilay_rota_h%2;

      verc_ptr->layx_ver[i].ilay_rota_str = rnd_rot_w_len * sizeof(uint32_t); 
      

      verc_ptr->layx_ver[i].ilay_uvstr = format_yuv_nv ? verc_ptr->layx_ver[i].ilay_ystr : (verc_ptr->layx_ver[i].ilay_ystr/2);

      
      fm_rotate(
		verc_ptr->layx_ver[i].ilay_argb_addr, //src
		verc_ptr->layx_ver[i].ilay_src_width, //src_w
		verc_ptr->layx_ver[i].ilay_src_height, //src_w
		verc_ptr->layx_ver[i].ilay_argb_str,   //src_str
		
		verc_ptr->layx_ver[i].ilay_rota_addr, //dst
		verc_ptr->layx_ver[i].ilay_rota_w, //dst_w
		verc_ptr->layx_ver[i].ilay_rota_h, //dst_w
		verc_ptr->layx_ver[i].ilay_rota_str,   //dst_str
		
		verc_ptr->layx_ver[i].ilay_rota, //rota
		verc_ptr->layx_ver[i].ilay_hmirror, //hmir
		verc_ptr->layx_ver[i].ilay_vmirror //hmir
                );

      uint32_t csc_en;
      if ( format_yuv )
	csc_en = 1;
      else
	csc_en = 0;

      fm_resize ( 
		 verc_ptr->layx_ver[i].ilay_rota_addr, //src
		 verc_ptr->layx_ver[i].ilay_rota_w, //src_w
		 verc_ptr->layx_ver[i].ilay_rota_h, //src_w
		 verc_ptr->layx_ver[i].ilay_rota_str,   //src_str
		  
		 verc_ptr->layx_ver[i].ilay_rsz_addr, //dst
		 verc_ptr->layx_ver[i].ilay_dst_width, //dst_w
		 verc_ptr->layx_ver[i].ilay_dst_height, //dst_h
		 verc_ptr->layx_ver[i].ilay_rsz_str, //dst_str
		 
		 verc_ptr->layx_ver[i].ilay_bak_argb,	//bk_argb
		 verc_ptr->layx_ver[i].ilay_rota_xcoef, //xrsz_coef
		 verc_ptr->layx_ver[i].ilay_rota_ycoef,  //yrsz_coef		  

		 csc_en, //csc_en
		 verc_ptr->layx_ver[i].ilay_csc_mod,
		 verc_ptr->layx_ver[i].ilay_smode		 
		 );      


    }  

  /* =================== gen bak ground ====================*/
  verc_ptr->layb_ver.ilay_en = random()%2; 
  verc_ptr->layb_ver.ilay_pm = random()%2; 
  verc_ptr->layb_ver.ilay_galpha_en = random()%2; 
  verc_ptr->layb_ver.ilay_galpha = random()%256;     
  verc_ptr->layb_ver.ilay_mask_en = random()%2;
  verc_ptr->layb_ver.ilay_bak_argb = random();
  verc_ptr->layb_ver.ilay_tile_en = random()%2;


#ifdef USER_CFG_OPT
  verc_ptr->layb_ver.ilay_en = 1; 
  verc_ptr->layb_ver.ilay_pm = 0x0; 
  verc_ptr->layb_ver.ilay_tile_en = 0;
  verc_ptr->layb_ver.ilay_galpha_en = 0; 
  verc_ptr->layb_ver.ilay_galpha = 0x2a;     
  verc_ptr->layb_ver.ilay_mask_en = 0;
  verc_ptr->layb_ver.ilay_bak_argb = 0x15be0bbd; 
#endif
  
  if ( verc_ptr->layb_ver.ilay_pm == 1 )
    verc_ptr->layb_ver.ilay_galpha_en = 0;

  if ( verc_ptr->layb_ver.ilay_mask_en == 1){
    verc_ptr->layb_ver.ilay_galpha_en = 0;
    verc_ptr->layb_ver.ilay_pm = 0 ;
  }


  if ( verc_ptr->layb_ver.ilay_pm ==1 && verc_ptr->lay_num == 0)
    verc_ptr->layb_ver.ilay_en = 0;
  

  verc_ptr->layb_ver.ilay_width = random()%LAY_DST_MAXW+1;
  verc_ptr->layb_ver.ilay_height = random()%LAY_DST_MAXH+1;

#ifdef USER_CFG_OPT
  verc_ptr->layb_ver.ilay_width = 0x20;
  verc_ptr->layb_ver.ilay_height = 0x20;
#endif


  verc_ptr->layb_ver.ilay_width = verc_ptr->layb_ver.ilay_width > max_xpos? 
                                  verc_ptr->layb_ver.ilay_width :
                                  max_xpos;

  verc_ptr->layb_ver.ilay_height = verc_ptr->layb_ver.ilay_height > max_ypos? 
                                   verc_ptr->layb_ver.ilay_height :
                                   max_ypos;

  if ( verc_ptr->layb_ver.ilay_tile_en )
    {
      verc_ptr->layb_ver.ilay_width = (verc_ptr->layb_ver.ilay_width + 15) & 0xFFFFFFF0;
      verc_ptr->layb_ver.ilay_height = (verc_ptr->layb_ver.ilay_height + 15) & 0xFFFFFFF0;
    }


  verc_ptr->layb_ver.ilay_argb_str = (verc_ptr->layb_ver.ilay_width + (verc_ptr->layb_ver.ilay_width%2))*(sizeof(uint32_t));




  if ( verc_ptr->layb_ver.ilay_tile_en == 1)
    {
      verc_ptr->layb_ver.ilay_std_str = verc_ptr->layb_ver.ilay_width*sizeof(uint32_t) * 16;
      
    }else{
      verc_ptr->layb_ver.ilay_std_str = verc_ptr->layb_ver.ilay_width*sizeof(uint32_t) + 
	(verc_ptr->layb_ver.ilay_width%2)*sizeof(uint32_t);
    }

  if ( verc_ptr->layb_ver.ilay_tile_en == 1)
    {
      verc_ptr->layb_ver.ilay_raw_str = verc_ptr->layb_ver.ilay_width*sizeof(uint32_t) * 16;
    }
  else {
    verc_ptr->layb_ver.ilay_raw_str = verc_ptr->layb_ver.ilay_width*sizeof(uint32_t) 
                                    + (verc_ptr->layb_ver.ilay_width%2)*sizeof(uint32_t)
                                      ;
  }


  if ( verc_ptr->layb_ver.ilay_en ){
      init_pix(
	       verc_ptr->layb_ver.ilay_argb_addr,//dst
	       verc_ptr->layb_ver.ilay_width, //dst_w
	       verc_ptr->layb_ver.ilay_height, //dst_w
	       verc_ptr->layb_ver.ilay_argb_str,   //dst_str

	       verc_ptr->layb_ver.ilay_bak_argb,
	       0,
	       0,
	       0,0,0,0,0, 0, 1
               );    
      
  }

  blk_copyw_fm( 
	       verc_ptr->layb_ver.ilay_argb_addr,//src
	       verc_ptr->layb_ver.ilay_width, //src_w
	       verc_ptr->layb_ver.ilay_height, //src_w
	       verc_ptr->layb_ver.ilay_argb_str,   //src_str
	       
	       verc_ptr->layb_ver.ilay_raw_addr,
	       verc_ptr->layb_ver.ilay_width, //dst_w
	       verc_ptr->layb_ver.ilay_height, //dst_w
	       verc_ptr->layb_ver.ilay_raw_str,   //dst_str
	       
	       verc_ptr->layb_ver.ilay_bak_argb,
	       verc_ptr->layb_ver.ilay_tile_en
	       );

  x2d_osd_5layer(
		 verc_ptr->layb_ver.ilay_en,		 // uint32_t ilayb_en,
		 verc_ptr->layb_ver.ilay_pm,             //uint32_t ilayb_pm,
		 verc_ptr->layb_ver.ilay_width,      // uint32_t ilayb_w,
		 verc_ptr->layb_ver.ilay_height,     //uint32_t ilayb_h,
		 verc_ptr->layb_ver.ilay_galpha_en, //uint32_t ilayb_galpha_en,
		 verc_ptr->layb_ver.ilay_galpha, //uint32_t ilayb_galpha,
		 verc_ptr->layb_ver.ilay_argb_addr, //uint32_t ilayb_addr
		 verc_ptr->layb_ver.ilay_argb_str, //uint32_t ilayb_str,
		 verc_ptr->layb_ver.ilay_mask_en, //uint32_t ilayb_str,
		 verc_ptr->layb_ver.ilay_bak_argb,

		 verc_ptr->layx_ver[0].ilay_en,//uint32_t ilay0_en,
		 verc_ptr->layx_ver[0].ilay_pm,//uint32_t ilay0_pm,
		 verc_ptr->layx_ver[0].ilay_dst_width,//uint32_t ilay0_w,
		 verc_ptr->layx_ver[0].ilay_dst_height,//uint32_t ilay0_h,
		 verc_ptr->layx_ver[0].ilay_dst_xoft,//ilay0_xoft,
		 verc_ptr->layx_ver[0].ilay_dst_yoft,//ilay0_yoft,
		 verc_ptr->layx_ver[0].ilay_galpha_en,//ilay0_galpha_en,
		 verc_ptr->layx_ver[0].ilay_galpha,//ilay0_galpha,			
		 verc_ptr->layx_ver[0].ilay_rsz_addr,//ilay0_addr
		 verc_ptr->layx_ver[0].ilay_rsz_str,//ilay0_str,
		 verc_ptr->layx_ver[0].ilay_mask_en,//ilay0_str,
		 verc_ptr->layx_ver[0].ilay_bak_argb,//ilay0_str,
		 verc_ptr->layx_ver[0].ilay_osdm,

		 verc_ptr->layx_ver[1].ilay_en,//uint32_t ilay1_en,
		 verc_ptr->layx_ver[1].ilay_pm,//uint32_t ilay1_pm,
		 verc_ptr->layx_ver[1].ilay_dst_width,//uint32_t ilay1_w,
		 verc_ptr->layx_ver[1].ilay_dst_height,//uint32_t ilay1_h,
		 verc_ptr->layx_ver[1].ilay_dst_xoft,//ilay1_xoft,
		 verc_ptr->layx_ver[1].ilay_dst_yoft,//ilay1_yoft,
		 verc_ptr->layx_ver[1].ilay_galpha_en,//ilay1_galpha_en,
		 verc_ptr->layx_ver[1].ilay_galpha,//ilay1_galpha,			
		 verc_ptr->layx_ver[1].ilay_rsz_addr,//ilay1_addr
		 verc_ptr->layx_ver[1].ilay_rsz_str,//ilay1_str,
		 verc_ptr->layx_ver[1].ilay_mask_en,//ilay0_str,
		 verc_ptr->layx_ver[1].ilay_bak_argb,//ilay0_str,
		 verc_ptr->layx_ver[1].ilay_osdm,

		 verc_ptr->layx_ver[2].ilay_en,//uint32_t ilay2_en,
		 verc_ptr->layx_ver[2].ilay_pm,//uint32_t ilay2_pm,
		 verc_ptr->layx_ver[2].ilay_dst_width,//uint32_t ilay2_w,
		 verc_ptr->layx_ver[2].ilay_dst_height,//uint32_t ilay2_h,
		 verc_ptr->layx_ver[2].ilay_dst_xoft,//ilay2_xoft,
		 verc_ptr->layx_ver[2].ilay_dst_yoft,//ilay2_yoft,
		 verc_ptr->layx_ver[2].ilay_galpha_en,//ilay2_galpha_en,
		 verc_ptr->layx_ver[2].ilay_galpha,//ilay2_galpha,			
		 verc_ptr->layx_ver[2].ilay_rsz_addr,//ilay2_addr
		 verc_ptr->layx_ver[2].ilay_rsz_str,//ilay2_str,
		 verc_ptr->layx_ver[2].ilay_mask_en,//ilay0_str,
		 verc_ptr->layx_ver[2].ilay_bak_argb,//ilay0_str,
		 verc_ptr->layx_ver[2].ilay_osdm,


		 verc_ptr->layx_ver[3].ilay_en,//uint32_t ilay3_en,
		 verc_ptr->layx_ver[3].ilay_pm,//uint32_t ilay3_pm,
		 verc_ptr->layx_ver[3].ilay_dst_width,//uint32_t ilay3_w,
		 verc_ptr->layx_ver[3].ilay_dst_height,//uint32_t ilay3_h,
		 verc_ptr->layx_ver[3].ilay_dst_xoft,//ilay3_xoft,
		 verc_ptr->layx_ver[3].ilay_dst_yoft,//ilay3_yoft,
		 verc_ptr->layx_ver[3].ilay_galpha_en,//ilay3_galpha_en,
		 verc_ptr->layx_ver[3].ilay_galpha,//ilay3_galpha,			
		 verc_ptr->layx_ver[3].ilay_rsz_addr,//ilay3_addr
		 verc_ptr->layx_ver[3].ilay_rsz_str, //ilay3_str,
		 verc_ptr->layx_ver[3].ilay_mask_en,//ilay0_str,
		 verc_ptr->layx_ver[3].ilay_bak_argb,//ilay0_str,
		 verc_ptr->layx_ver[3].ilay_osdm
		 );

  blk_copyw_fm( 
	       verc_ptr->layb_ver.ilay_argb_addr,//src
	       verc_ptr->layb_ver.ilay_width, //src_w
	       verc_ptr->layb_ver.ilay_height, //src_w
	       verc_ptr->layb_ver.ilay_argb_str,   //src_str

	       verc_ptr->layb_ver.ilay_std_addr,
	       verc_ptr->layb_ver.ilay_width, //dst_w
	       verc_ptr->layb_ver.ilay_height, //dst_w
	       verc_ptr->layb_ver.ilay_std_str,   //dst_str
	       verc_ptr->layb_ver.ilay_bak_argb,
	       verc_ptr->layb_ver.ilay_tile_en
	       );



}

#ifdef X2D_IRQ_MODE
static void x2d_irq_handle(int irq, void *d)
{
	struct fw_dev *dev = d;
	write_x2d_reg(X2D_SLV_GLB_TRIG, X2D_IRQ_CLR);  
	fw_finish(dev);
}
#endif

static int x2d_prepare(struct fw_dev *dev)
{
  struct x2d_data *x2d_p;
#ifdef X2D_SHOW
  printk(" +X2D prepare ! \n");
#endif

#ifdef SEED_CFG
  int seed=SEED_CORE;
#else
  int seed=(int) time(NULL);
#endif
  srand(seed);

  x2d_base = ioremap(X2D_P_BASE , 0x10000);

#ifdef X2D_IRQ_MODE
  register_irqfun( IRQ_INTC_BASE + X2D_IRQ_ID,x2d_irq_handle,"X2D",dev);
#endif

  x2d_p = malloc(sizeof(*x2d_p));
  if (!x2d_p)
    return -1;
  x2d_p->times = 0;

  // Malloc X2D memory .
  x2d_p->chn_ptr = (x2d_chain_info_p) mem_alloc(TEST_SIZE);
  x2d_p->verc_ptr = (x2d_tbench_verc_info_p) mem_alloc(TEST_SIZE);
  x2d_p->verc_ptr->layb_ver.ilay_argb_addr = mem_alloc(TEST_SIZE *5);
  x2d_p->verc_ptr->layb_ver.ilay_std_addr = mem_alloc(TEST_SIZE *5);
  x2d_p->verc_ptr->layb_ver.ilay_raw_addr = mem_alloc(TEST_SIZE *5);

  if (!x2d_p->chn_ptr || !x2d_p->verc_ptr ||
      !x2d_p->verc_ptr->layb_ver.ilay_argb_addr ||
      !x2d_p->verc_ptr->layb_ver.ilay_std_addr ||
      !x2d_p->verc_ptr->layb_ver.ilay_raw_addr
      ) {
    printk("X2D malloc memory FAILED !\n");
    free(x2d_p);
    return -1;
  }

  int i;
  for(i = 0; i <4 ; i++){
    x2d_p->verc_ptr->layx_ver[i].ilay_argb_addr = mem_alloc(TEST_SIZE * 5);

    x2d_p->verc_ptr->layx_ver[i].ilay_yaddr = mem_alloc(TEST_SIZE * 5);
    x2d_p->verc_ptr->layx_ver[i].ilay_uaddr = mem_alloc(TEST_SIZE * 5);
    x2d_p->verc_ptr->layx_ver[i].ilay_vaddr = mem_alloc(TEST_SIZE * 5);

    x2d_p->verc_ptr->layx_ver[i].ilay_rota_addr = mem_alloc(TEST_SIZE * 5);
    x2d_p->verc_ptr->layx_ver[i].ilay_rsz_addr = mem_alloc(TEST_SIZE * 5);

	  
    if (!x2d_p->verc_ptr->layx_ver[i].ilay_argb_addr || 
	!x2d_p->verc_ptr->layx_ver[i].ilay_yaddr ||
	!x2d_p->verc_ptr->layx_ver[i].ilay_uaddr ||
	!x2d_p->verc_ptr->layx_ver[i].ilay_vaddr ||
	!x2d_p->verc_ptr->layx_ver[i].ilay_rota_addr ||
	!x2d_p->verc_ptr->layx_ver[i].ilay_rsz_addr 
	) {
      printk("X2D malloc Layer %d memory FAILED !\n",i);
      free(x2d_p);
      return -1;
    }

    // Get Physical Address for X2D hardware .
    x2d_p->chn_ptr->x2d_lays[i].y_addr = mem_get_phy(x2d_p->verc_ptr->layx_ver[i].ilay_yaddr);//Physical Address
    x2d_p->chn_ptr->x2d_lays[i].u_addr = mem_get_phy(x2d_p->verc_ptr->layx_ver[i].ilay_uaddr);//Physical Address
    x2d_p->chn_ptr->x2d_lays[i].v_addr = mem_get_phy(x2d_p->verc_ptr->layx_ver[i].ilay_vaddr);//Physical Address
  }


#ifdef X2D_SHOW
  printk(" +X2D Malloc over ! \n");
#endif
  // Initial X2D data and chain here!
  init_verc(x2d_p->verc_ptr);
#ifdef X2D_SHOW
  printk(" +X2D initial pixels over ! \n");
#endif
  x2d_p->chn_ptr->overlay_num = x2d_p->verc_ptr->lay_num;
  x2d_p->chn_ptr->dst_tile_en = x2d_p->verc_ptr->layb_ver.ilay_tile_en;
	
  if ( x2d_p->verc_ptr->dst_tlb_en == 1)
    x2d_p->chn_ptr->dst_addr = (uint32_t)x2d_p->verc_ptr->layb_ver.ilay_argb_addr;
  else
    x2d_p->chn_ptr->dst_addr = mem_get_phy(x2d_p->verc_ptr->layb_ver.ilay_argb_addr);//Physical Address

  x2d_p->chn_ptr->dst_ctrl_str = ( (x2d_p->verc_ptr->layb_ver.ilay_raw_str & DST_STR_MSK)
				   |(x2d_p->verc_ptr->layb_ver.ilay_en? BACK_EN: 0)
				   |(x2d_p->verc_ptr->layb_ver.ilay_pm? DST_PREM: 0)
				   |(x2d_p->verc_ptr->layb_ver.ilay_galpha_en? DST_GLOBAL_ALPHA: 0)
				   |(((x2d_p->verc_ptr->layb_ver.ilay_galpha & DST_GALPHA_MSK) 
				      << DST_GALPHA_SFT)
				     )
				   | (x2d_p->verc_ptr->layb_ver.ilay_mask_en? DST_MSK_EN:0)
				   );
  
  x2d_p->chn_ptr->dst_width = x2d_p->verc_ptr->layb_ver.ilay_width;
  x2d_p->chn_ptr->dst_height = x2d_p->verc_ptr->layb_ver.ilay_height;

  for ( i = 0 ; i < x2d_p->verc_ptr->lay_num; i++)
    {
      x2d_p->chn_ptr->x2d_lays[i].lay_ctrl = ( (x2d_p->verc_ptr->layx_ver[i].ilay_pm? SRC_PREM: 0) 
					       |(x2d_p->verc_ptr->layx_ver[i].ilay_galpha_en? LAY_GLB_ALPHA_EN: 0) 
					       |(x2d_p->verc_ptr->layx_ver[i].ilay_smode & SRC_FM_MSK) 
					       |(x2d_p->verc_ptr->layx_ver[i].ilay_csc_mod? SRC_CSCM1:SRC_CSCM0) 
					       |(x2d_p->verc_ptr->layx_ver[i].ilay_mask_en? SRC_MSK_EN:0) 
					       );
      x2d_p->chn_ptr->x2d_lays[i].lay_galpha = x2d_p->verc_ptr->layx_ver[i].ilay_galpha;
      x2d_p->chn_ptr->x2d_lays[i].rom_ctrl = ( (x2d_p->verc_ptr->layx_ver[i].ilay_rota & ROTA_MSK)
					       |(x2d_p->verc_ptr->layx_ver[i].ilay_hmirror? HMIRROR_EN:0)
					       |(x2d_p->verc_ptr->layx_ver[i].ilay_vmirror? VMIRROR_EN:0)	                          
					       );

      x2d_p->chn_ptr->x2d_lays[i].RGBM = ( (x2d_p->verc_ptr->layx_ver[i].ilay_rgbm & SRC_RGBM_MSK) 
					   | (x2d_p->verc_ptr->layx_ver[i].ilay_apos? SRC_ARGB_SEL:SRC_RGBA_SEL) 
					   | (x2d_p->verc_ptr->layx_ver[i].ilay_osdm<<4)
					   );


      x2d_p->chn_ptr->x2d_lays[i].bk_argb = x2d_p->verc_ptr->layx_ver[i].ilay_bak_argb;

      x2d_p->chn_ptr->x2d_lays[i].swidth = x2d_p->verc_ptr->layx_ver[i].ilay_src_width;
      x2d_p->chn_ptr->x2d_lays[i].sheight = x2d_p->verc_ptr->layx_ver[i].ilay_src_height;

      if ( x2d_p->verc_ptr->layx_ver[i].ilay_smode == SRC_YUV_TILE ){
	x2d_p->chn_ptr->x2d_lays[i].ystr = x2d_p->verc_ptr->layx_ver[i].ilay_ystr >> 4;
	x2d_p->chn_ptr->x2d_lays[i].uvstr = x2d_p->verc_ptr->layx_ver[i].ilay_uvstr >> 4;
      }else {
	x2d_p->chn_ptr->x2d_lays[i].ystr = x2d_p->verc_ptr->layx_ver[i].ilay_ystr;
	x2d_p->chn_ptr->x2d_lays[i].uvstr = x2d_p->verc_ptr->layx_ver[i].ilay_uvstr;
      }

      x2d_p->chn_ptr->x2d_lays[i].owidth = x2d_p->verc_ptr->layx_ver[i].ilay_dst_width;
      x2d_p->chn_ptr->x2d_lays[i].oheight = x2d_p->verc_ptr->layx_ver[i].ilay_dst_height;
      x2d_p->chn_ptr->x2d_lays[i].oxoffset = x2d_p->verc_ptr->layx_ver[i].ilay_dst_xoft;
      x2d_p->chn_ptr->x2d_lays[i].oyoffset = x2d_p->verc_ptr->layx_ver[i].ilay_dst_yoft;

      x2d_p->chn_ptr->x2d_lays[i].rsz_hcoef = x2d_p->verc_ptr->layx_ver[i].ilay_hrsz_coef;
      x2d_p->chn_ptr->x2d_lays[i].rsz_vcoef = x2d_p->verc_ptr->layx_ver[i].ilay_vrsz_coef;      
    }

  write_x2d_reg(X2D_SLV_GLB_TRIG, X2D_RST);
#ifdef X2D_SHOW
  printk(" +X2D chain over ! \n");
#endif

  dev->priv = x2d_p;
  return 0;
}

static int x2d_start(struct fw_dev *dev)
{
  struct x2d_data *x2d_p = dev->priv;
#ifdef X2D_SHOW
  printk(" +X2D start ! ..");
#endif

  recovery_bk_fm(x2d_p->verc_ptr->layb_ver.ilay_raw_addr , x2d_p->verc_ptr->layb_ver.ilay_argb_addr, TEST_SIZE * 5);

  //write_x2d_reg(X2D_SLV_TLB_BASE, (AP_TLB_BASE));

  write_x2d_reg(X2D_SLV_GLB_CTRL, DESP_EN | WDOG_EN | 

#ifdef X2D_IRQ_MODE 
		IRQ_EN | // X2D irq method
#endif
		(x2d_p->verc_ptr->cmd_tlb_en? CMD_TLB_EN : 0) | 
		(x2d_p->verc_ptr->dst_tlb_en? DST_TLB_EN : 0) |
		(x2d_p->verc_ptr->layx_ver[0].ilayer_tlb_en ? L0_TLB_EN : 0) |
		(x2d_p->verc_ptr->layx_ver[1].ilayer_tlb_en ? L1_TLB_EN : 0) |
		(x2d_p->verc_ptr->layx_ver[2].ilayer_tlb_en ? L2_TLB_EN : 0) |
		(x2d_p->verc_ptr->layx_ver[3].ilayer_tlb_en ? L3_TLB_EN : 0) |
		X2D_BANK_1K
		);

  write_x2d_reg(X2D_SLV_DHA, mem_get_phy(x2d_p->chn_ptr) );

  write_x2d_reg(X2D_SLV_WDOG_CNT, 13000000);

  write_x2d_reg(X2D_SLV_DST_BARGB, x2d_p->verc_ptr->layb_ver.ilay_bak_argb);

  write_x2d_reg(X2D_SLV_GLB_TRIG, X2D_START); // Start X2D here .

  blast_cache_all();

#ifdef X2D_SHOW
  printk(" .\n");
#endif

  return 0;
}

#ifndef X2D_IRQ_MODE
static void x2d_ask(struct fw_dev *dev)
{
  struct x2d_data *x2d_p = dev->priv;
#ifdef X2D_SHOW
  printk("A");
#endif
  
  if ((read_x2d_reg(X2D_SLV_GLB_STATUS) & GLB_FSM_MSK) != GIDLE)
	  return;

  fw_finish(dev);
}
#endif

static chk_t x2d_check(struct fw_dev *dev)
{
  struct x2d_data *x2d_p = dev->priv;

  if ( (read_x2d_reg(X2D_SLV_GLB_STATUS) & GLB_WDOG_ERR) != 0)
    printk(" +X2D : Watch dog error !\n");
  uint32_t consum_time = read_x2d_reg(X2D_SLV_GLB_TIME);
  double cycle_per_pixel = ((double)consum_time)/(x2d_p->verc_ptr->layb_ver.ilay_width*x2d_p->verc_ptr->layb_ver.ilay_height+1);
  double taget_cycle_per_pixel = (300*1000000)/(1920*1280*30);  
  
  printk(" ++consum_time:%x\n", consum_time);
  printk(" ++GKER_W --cycle: %d time:%fns/per_pix   target: %fns   %f%% \n", consum_time, 
	 cycle_per_pixel,  taget_cycle_per_pixel, cycle_per_pixel*100/taget_cycle_per_pixel
	 ); 

  write_x2d_reg(X2D_SLV_GLB_TRIG, X2D_IRQ_CLR);

  blast_cache_all();

  int value = blk_cmp_fm(
			 x2d_p->verc_ptr->layb_ver.ilay_argb_addr, 
			 x2d_p->verc_ptr->layb_ver.ilay_width, 
			 x2d_p->verc_ptr->layb_ver.ilay_height, 
			 x2d_p->verc_ptr->layb_ver.ilay_argb_str,
			 
			 x2d_p->verc_ptr->layb_ver.ilay_raw_addr,
			 x2d_p->verc_ptr->layb_ver.ilay_width,
			 x2d_p->verc_ptr->layb_ver.ilay_height, 
			 x2d_p->verc_ptr->layb_ver.ilay_raw_str,
			 
			 x2d_p->verc_ptr->layb_ver.ilay_tile_en
			 );

  if ( value ){
    return CHK_FAILED;
  }else {
    if (x2d_p->times ++ > MAX_X2D_TEST_TIMES )
      return CHK_FINISHED;
    return CHK_PASSED;
  }
}

static void x2d_halt(struct fw_dev *dev)
{
  struct x2d_data *x2d_p = dev->priv;
  
  if (x2d_p->times <= MAX_X2D_TEST_TIMES )
    printk(" > X2D stopped at %d .\n", x2d_p->times);
  else
    printk(" > X2D stopped !\n");

  mem_free(x2d_p->verc_ptr->layb_ver.ilay_argb_addr);
  mem_free(x2d_p->verc_ptr->layb_ver.ilay_std_addr);
  mem_free(x2d_p->verc_ptr->layb_ver.ilay_raw_addr);

  int i;
  for(i = 0; i <4 ; i++){
    mem_free(x2d_p->verc_ptr->layx_ver[i].ilay_argb_addr);

    mem_free(x2d_p->verc_ptr->layx_ver[i].ilay_yaddr);
    mem_free(x2d_p->verc_ptr->layx_ver[i].ilay_uaddr);
    mem_free(x2d_p->verc_ptr->layx_ver[i].ilay_vaddr);
    mem_free(x2d_p->verc_ptr->layx_ver[i].ilay_rota_addr);
    mem_free(x2d_p->verc_ptr->layx_ver[i].ilay_rsz_addr);
  }

  mem_free(x2d_p->chn_ptr);
  mem_free(x2d_p->verc_ptr);
  free(x2d_p);
}

static struct fw_ops x2d_dev_ops = {
	.prepare = x2d_prepare,
	.start = x2d_start,
#ifndef X2D_IRQ_MODE
	.ask = x2d_ask,
#endif
	.check = x2d_check,
	.halt = x2d_halt,
};

static int init_dev(void)
{
	if (!fwdev_register("x2d", &x2d_dev_ops))
		return -1;
	return 0;
}

fw_initcall(init_dev,0);
