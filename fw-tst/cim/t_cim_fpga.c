#include <mem.h>
#include <mips.h>
#include <iomap.h>
#include <string.h>
#include <init.h>
#include <test/fw.h>

#define FPGA_CIM
#define DEV_REG_BASE  0
#define va_2_pa K0_TO_PHYS
#define i_s32(val, addr, offset)  (REG32(addr) = (val))
#define i_l32(addr, offset)       (REG32(addr))

#define CIM_BASE  ((unsigned)cim_base)
#define GPIO_BASE ((unsigned)gpio_base)
static void *cim_base, *gpio_base;

#include "t_cim_fpga.h"

typedef struct fw_dev Tfw_dw;

static int cim_prepare(Tfw_dw *dev)
{
  // get one memory space for frames 0-3 storage
  cim_frame0_ptr = (unsigned int *)mem_alloc(CIM_FRAME0_SIZE_B); // size: byte;  
  cim_frame1_ptr = (unsigned int *)mem_alloc(CIM_FRAME1_SIZE_B);
  cim_frame2_ptr = (unsigned int *)mem_alloc(CIM_FRAME2_SIZE_B);
  cim_frame3_ptr = (unsigned int *)mem_alloc(CIM_FRAME3_SIZE_B);
  cim_frame4_ptr = (unsigned int *)mem_alloc(CIM_FRAME4_SIZE_B);
  cim_frame5_ptr = (unsigned int *)mem_alloc(CIM_FRAME5_SIZE_B);

  if (!cim_frame0_ptr || !cim_frame1_ptr || !cim_frame2_ptr || !cim_frame3_ptr || !cim_frame4_ptr || !cim_frame5_ptr) {
    return -1;
  }

  // generate the cim register base address
  cim_base  = ioremap(0x13060000, 0x1000);
  gpio_base = ioremap(0x10010000, 0x1000);

  // need to config the gpio for pclk
  alter4vci();

  return 0;
}

static int cim_reset(Tfw_dw *dev)
{
  init_default_vci();    

  set_burst(BURST_INCR32);
  set_mode_bypass();
  set_vsync_h();
  set_hsync_h();
  set_pclk_r();                          
  set_frame_rate(1); 
  enb_dsm_gate();
  set_pack(0);
  dis_dummy(); 

  init_default_dvi();

  memset(cim_frame0_ptr, 0, CIM_FRAME0_SIZE_B);
  memset(cim_frame1_ptr, 0, CIM_FRAME1_SIZE_B);
  memset(cim_frame2_ptr, 0, CIM_FRAME2_SIZE_B);
  memset(cim_frame3_ptr, 0, CIM_FRAME3_SIZE_B);
  memset(cim_frame4_ptr, 0, CIM_FRAME4_SIZE_B);
  memset(cim_frame5_ptr, 0, CIM_FRAME5_SIZE_B);

  return 0;
}

static int cim_start(Tfw_dw *dev)
{
  enb_dvi();

  enb_rxf_rst();
  Dly_2(0xf);
  dis_rxf_rst();

  dis_vci_int();
  enb_vci_dma();
  enb_vci();

  return 0;
}

static void cim_ask(Tfw_dw *dev)
{
  unsigned int flag;

  flag = get_dma_stop();

  if (flag != 0) {
    fw_finish(dev);
  }
}

static void cim_stop(Tfw_dw *dev) 
{
  dis_vci_dma();
  dis_vci();
  dis_dvi();
}

static chk_t cim_check(Tfw_dw *dev)
{
  unsigned int err = 0;

  err += check_cim_frame(cim_frame0_ptr, 0, CIM_FRAME0_SIZE_W);
  err += check_cim_frame(cim_frame1_ptr, 0, CIM_FRAME1_SIZE_W);
  err += check_cim_frame(cim_frame2_ptr, 0, CIM_FRAME2_SIZE_W);
  err += check_cim_frame(cim_frame3_ptr, 0, CIM_FRAME3_SIZE_W);
  err += check_cim_frame(cim_frame4_ptr, 0, CIM_FRAME4_SIZE_W);
  err += check_cim_frame(cim_frame5_ptr, 0, CIM_FRAME5_SIZE_W);

  return err? CHK_FAILED: CHK_PASSED;
}

static struct fw_ops cim_ops = {
	.prepare = cim_prepare,
	.reset = cim_reset,
	.start = cim_start,
	.ask = cim_ask,
	.check = cim_check,
	.stop = cim_stop,
};

static int init_dev(void)
{
	if (!fwdev_register("cim", &cim_ops))
		return -1;
	return 0;
}
fw_initcall(init_dev,100);
