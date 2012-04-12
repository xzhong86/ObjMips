#ifndef __T_PDMA_NAND_H__
#define __T_PDMA_NAND_H__


#define DMA_BASE	(IO_BASE + 0x03420000) 
#define IO_BASE	        0xB0000000

#ifdef PDMA
#define DMAC       (DMA_BASE + 0x1000) 
#define DIRQP      (DMA_BASE + 0x1004)  
#define DDR        (DMA_BASE + 0x1008)
#define DDS        (DMA_BASE + 0x100c)
#define DCKE       (DMA_BASE + 0x1010)
#define DCKES      (DMA_BASE + 0x1014)
#define DCKEC      (DMA_BASE + 0x1018)
#define DMACP      (DMA_BASE + 0x101c)
#define DSIRQP     (DMA_BASE + 0x1020)
#define DSIRQM     (DMA_BASE + 0x1024)
#define DCIRQP     (DMA_BASE + 0x1028)
#define DCIRQM     (DMA_BASE + 0x102C)
#define DMCS       (DMA_BASE + 0x1030)
#define DMNMB      (DMA_BASE + 0x1034)
#define DMSMB      (DMA_BASE + 0x1038)
#define DMINT      (DMA_BASE + 0x103C)

#define DSA0      (DMA_BASE + 0x000)
#define DTA0      (DMA_BASE + 0x004)
#define DTC0      (DMA_BASE + 0x008)
#define DRT0      (DMA_BASE + 0x00c)
#define DCS0      (DMA_BASE + 0x010)
#define DCM0      (DMA_BASE + 0x014)
#define DDA0      (DMA_BASE + 0x018)
#define DSD0      (DMA_BASE + 0x01c)

#define DSA1      (DMA_BASE + 0x020)  
#define DTA1      (DMA_BASE + 0x024) 
#define DTC1      (DMA_BASE + 0x028)
#define DRT1      (DMA_BASE + 0x02c)  
#define DCS1      (DMA_BASE + 0x030) 
#define DCM1      (DMA_BASE + 0x034)
#define DDA1      (DMA_BASE + 0x038)
#define DSD1      (DMA_BASE + 0x03c)

#define DSA2      (DMA_BASE + 0x040)  
#define DTA2      (DMA_BASE + 0x044)  
#define DTC2      (DMA_BASE + 0x048) 
#define DRT2      (DMA_BASE + 0x04c)
#define DCS2      (DMA_BASE + 0x050)  
#define DCM2      (DMA_BASE + 0x054)
#define DDA2      (DMA_BASE + 0x058)
#define DSD2      (DMA_BASE + 0x05c)

#define DSA3      (DMA_BASE + 0x060)  
#define DTA3      (DMA_BASE + 0x064)  
#define DTC3      (DMA_BASE + 0x068) 
#define DRT3      (DMA_BASE + 0x06c)
#define DCS3      (DMA_BASE + 0x070)  
#define DCM3      (DMA_BASE + 0x074)
#define DDA3      (DMA_BASE + 0x078)
#define DSD3      (DMA_BASE + 0x07c)

#define DSA4      (DMA_BASE + 0x080)  
#define DTA4      (DMA_BASE + 0x084) 
#define DTC4      (DMA_BASE + 0x088)
#define DRT4      (DMA_BASE + 0x08c)  
#define DCS4      (DMA_BASE + 0x090) 
#define DCM4      (DMA_BASE + 0x094)
#define DDA4      (DMA_BASE + 0x098)
#define DSD4      (DMA_BASE + 0x09c)

#define DSA5      (DMA_BASE + 0x0a0)
#define DTA5      (DMA_BASE + 0x0a4)
#define DTC5      (DMA_BASE + 0x0a8)
#define DRT5      (DMA_BASE + 0x0ac)
#define DCS5      (DMA_BASE + 0x0b0)
#define DCM5      (DMA_BASE + 0x0b4)
#define DDA5      (DMA_BASE + 0x0b8)
#define DSD5      (DMA_BASE + 0x0bc)

#define DSA6      (DMA_BASE + 0x0c0)
#define DTA6      (DMA_BASE + 0x0c4)
#define DTC6      (DMA_BASE + 0x0c8)
#define DRT6      (DMA_BASE + 0x0cc)
#define DCS6      (DMA_BASE + 0x0d0)
#define DCM6      (DMA_BASE + 0x0d4)
#define DDA6      (DMA_BASE + 0x0d8)
#define DSD6      (DMA_BASE + 0x0dc)

#define DSA7      (DMA_BASE + 0x0e0)  
#define DTA7      (DMA_BASE + 0x0e4) 
#define DTC7      (DMA_BASE + 0x0e8)
#define DRT7      (DMA_BASE + 0x0ec)  
#define DCS7      (DMA_BASE + 0x0f0) 
#define DCM7      (DMA_BASE + 0x0f4)
#define DDA7      (DMA_BASE + 0x0f8)
#define DSD7      (DMA_BASE + 0x0fc)

#define DSA8      (DMA_BASE + 0x100)  
#define DTA8      (DMA_BASE + 0x104)  
#define DTC8      (DMA_BASE + 0x108) 
#define DRT8      (DMA_BASE + 0x10c)
#define DCS8      (DMA_BASE + 0x110)  
#define DCM8      (DMA_BASE + 0x114)
#define DDA8      (DMA_BASE + 0x118)
#define DSD8      (DMA_BASE + 0x11c)

#define DSA9      (DMA_BASE + 0x120)  
#define DTA9      (DMA_BASE + 0x124)  
#define DTC9      (DMA_BASE + 0x128) 
#define DRT9      (DMA_BASE + 0x12c)
#define DCS9      (DMA_BASE + 0x130)  
#define DCM9      (DMA_BASE + 0x134)
#define DDA9      (DMA_BASE + 0x138)
#define DSD9      (DMA_BASE + 0x13c)

#define DSA10      (DMA_BASE + 0x140)  
#define DTA10      (DMA_BASE + 0x144) 
#define DTC10      (DMA_BASE + 0x148)
#define DRT10      (DMA_BASE + 0x14c)  
#define DCS10      (DMA_BASE + 0x150) 
#define DCM10      (DMA_BASE + 0x154)
#define DDA10      (DMA_BASE + 0x158)
#define DSD10      (DMA_BASE + 0x15c)

#define DSA11      (DMA_BASE + 0x160)
#define DTA11      (DMA_BASE + 0x164)
#define DTC11      (DMA_BASE + 0x168)
#define DRT11      (DMA_BASE + 0x16c)
#define DCS11      (DMA_BASE + 0x170)
#define DCM11      (DMA_BASE + 0x174)
#define DDA11      (DMA_BASE + 0x178)
#define DSD11      (DMA_BASE + 0x17c)

#define DSA12      (DMA_BASE + 0x180)
#define DTA12      (DMA_BASE + 0x184)
#define DTC12      (DMA_BASE + 0x188)
#define DRT12      (DMA_BASE + 0x18c)
#define DCS12      (DMA_BASE + 0x190)
#define DCM12      (DMA_BASE + 0x194)
#define DDA12      (DMA_BASE + 0x198)
#define DSD12      (DMA_BASE + 0x19c)

#define DSA13      (DMA_BASE + 0x1a0)
#define DTA13      (DMA_BASE + 0x1a4)
#define DTC13      (DMA_BASE + 0x1a8)
#define DRT13      (DMA_BASE + 0x1ac)
#define DCS13      (DMA_BASE + 0x1b0)
#define DCM13      (DMA_BASE + 0x1b4)
#define DDA13      (DMA_BASE + 0x1b8)
#define DSD13      (DMA_BASE + 0x1bc)

#define DSA14      (DMA_BASE + 0x1c0)
#define DTA14      (DMA_BASE + 0x1c4)
#define DTC14      (DMA_BASE + 0x1c8)
#define DRT14      (DMA_BASE + 0x1cc)
#define DCS14      (DMA_BASE + 0x1d0)
#define DCM14      (DMA_BASE + 0x1d4)
#define DDA14      (DMA_BASE + 0x1d8)
#define DSD14      (DMA_BASE + 0x1dc)

#define DSA15      (DMA_BASE + 0x1e0)
#define DTA15      (DMA_BASE + 0x1e4)
#define DTC15      (DMA_BASE + 0x1e8)
#define DRT15      (DMA_BASE + 0x1ec)
#define DCS15      (DMA_BASE + 0x1f0)
#define DCM15      (DMA_BASE + 0x1f4)
#define DDA15      (DMA_BASE + 0x1f8)
#define DSD15      (DMA_BASE + 0x1fc)

#define DSA16      (DMA_BASE + 0x200)
#define DTA16      (DMA_BASE + 0x204)
#define DTC16      (DMA_BASE + 0x208)
#define DRT16      (DMA_BASE + 0x20c)
#define DCS16      (DMA_BASE + 0x210)
#define DCM16      (DMA_BASE + 0x214)
#define DDA16      (DMA_BASE + 0x218)
#define DSD16      (DMA_BASE + 0x21c)

#define DSA17      (DMA_BASE + 0x220)
#define DTA17      (DMA_BASE + 0x224)
#define DTC17      (DMA_BASE + 0x228)
#define DRT17      (DMA_BASE + 0x22c)
#define DCS17      (DMA_BASE + 0x230)
#define DCM17      (DMA_BASE + 0x234)
#define DDA17      (DMA_BASE + 0x238)
#define DSD17      (DMA_BASE + 0x23c)

#define DSA18      (DMA_BASE + 0x240)
#define DTA18      (DMA_BASE + 0x244)
#define DTC18      (DMA_BASE + 0x248)
#define DRT18      (DMA_BASE + 0x24c)
#define DCS18      (DMA_BASE + 0x250)
#define DCM18      (DMA_BASE + 0x254)
#define DDA18      (DMA_BASE + 0x258)
#define DSD18      (DMA_BASE + 0x25c)

#define DSA19      (DMA_BASE + 0x260)
#define DTA19      (DMA_BASE + 0x264)
#define DTC19      (DMA_BASE + 0x268)
#define DRT19      (DMA_BASE + 0x26c)
#define DCS19      (DMA_BASE + 0x270)
#define DCM19      (DMA_BASE + 0x274)
#define DDA19      (DMA_BASE + 0x278)
#define DSD19      (DMA_BASE + 0x27c)

#define DSA20      (DMA_BASE + 0x280)
#define DTA20      (DMA_BASE + 0x284)
#define DTC20      (DMA_BASE + 0x288)
#define DRT20      (DMA_BASE + 0x28c)
#define DCS20      (DMA_BASE + 0x290)
#define DCM20      (DMA_BASE + 0x294)
#define DDA20      (DMA_BASE + 0x298)
#define DSD20      (DMA_BASE + 0x29c)

#define DSA21      (DMA_BASE + 0x2a0)
#define DTA21      (DMA_BASE + 0x2a4)
#define DTC21      (DMA_BASE + 0x2a8)
#define DRT21      (DMA_BASE + 0x2ac)
#define DCS21      (DMA_BASE + 0x2b0)
#define DCM21      (DMA_BASE + 0x2b4)
#define DDA21      (DMA_BASE + 0x2b8)
#define DSD21      (DMA_BASE + 0x2bc)

#define DSA22      (DMA_BASE + 0x2c0)
#define DTA22      (DMA_BASE + 0x2c4)
#define DTC22      (DMA_BASE + 0x2c8)
#define DRT22      (DMA_BASE + 0x2cc)
#define DCS22      (DMA_BASE + 0x2d0)
#define DCM22      (DMA_BASE + 0x2d4)
#define DDA22      (DMA_BASE + 0x2d8)
#define DSD22      (DMA_BASE + 0x2dc)

#define DSA23      (DMA_BASE + 0x2e0)
#define DTA23      (DMA_BASE + 0x2e4)
#define DTC23      (DMA_BASE + 0x2e8)
#define DRT23      (DMA_BASE + 0x2ec)
#define DCS23      (DMA_BASE + 0x2f0)
#define DCM23      (DMA_BASE + 0x2f4)
#define DDA23      (DMA_BASE + 0x2f8)
#define DSD23      (DMA_BASE + 0x2fc)

#define DSA24      (DMA_BASE + 0x300)
#define DTA24      (DMA_BASE + 0x304)
#define DTC24      (DMA_BASE + 0x308)
#define DRT24      (DMA_BASE + 0x30c)
#define DCS24      (DMA_BASE + 0x310)
#define DCM24      (DMA_BASE + 0x314)
#define DDA24      (DMA_BASE + 0x318)
#define DSD24      (DMA_BASE + 0x31c)

#define DSA25      (DMA_BASE + 0x320)
#define DTA25      (DMA_BASE + 0x324)
#define DTC25      (DMA_BASE + 0x328)
#define DRT25      (DMA_BASE + 0x32c)
#define DCS25      (DMA_BASE + 0x330)
#define DCM25      (DMA_BASE + 0x334)
#define DDA25      (DMA_BASE + 0x338)
#define DSD25      (DMA_BASE + 0x33c)

#define DSA26      (DMA_BASE + 0x340)
#define DTA26      (DMA_BASE + 0x344)
#define DTC26      (DMA_BASE + 0x348)
#define DRT26      (DMA_BASE + 0x34c)
#define DCS26      (DMA_BASE + 0x350)
#define DCM26      (DMA_BASE + 0x354)
#define DDA26      (DMA_BASE + 0x358)
#define DSD26      (DMA_BASE + 0x35c)

#define DSA27      (DMA_BASE + 0x360)
#define DTA27      (DMA_BASE + 0x364)
#define DTC27      (DMA_BASE + 0x368)
#define DRT27      (DMA_BASE + 0x36c)
#define DCS27      (DMA_BASE + 0x370)
#define DCM27      (DMA_BASE + 0x374)
#define DDA27      (DMA_BASE + 0x378)
#define DSD27      (DMA_BASE + 0x37c)

#define DSA28      (DMA_BASE + 0x380)
#define DTA28      (DMA_BASE + 0x384)
#define DTC28      (DMA_BASE + 0x388)
#define DRT28      (DMA_BASE + 0x38c)
#define DCS28      (DMA_BASE + 0x390)
#define DCM28      (DMA_BASE + 0x394)
#define DDA28      (DMA_BASE + 0x398)
#define DSD28      (DMA_BASE + 0x39c)

#define DSA29      (DMA_BASE + 0x3a0)
#define DTA29      (DMA_BASE + 0x3a4)
#define DTC29      (DMA_BASE + 0x3a8)
#define DRT29      (DMA_BASE + 0x3ac)
#define DCS29      (DMA_BASE + 0x3b0)
#define DCM29      (DMA_BASE + 0x3b4)
#define DDA29      (DMA_BASE + 0x3b8)
#define DSD29      (DMA_BASE + 0x3bc)

#define DSA30      (DMA_BASE + 0x3c0)
#define DTA30      (DMA_BASE + 0x3c4)
#define DTC30      (DMA_BASE + 0x3c8)
#define DRT30      (DMA_BASE + 0x3cc)
#define DCS30      (DMA_BASE + 0x3d0)
#define DCM30      (DMA_BASE + 0x3d4)
#define DDA30      (DMA_BASE + 0x3d8)
#define DSD30      (DMA_BASE + 0x3dc)

#define DSA31      (DMA_BASE + 0x3e0)
#define DTA31      (DMA_BASE + 0x3e4)
#define DTC31      (DMA_BASE + 0x3e8)
#define DRT31      (DMA_BASE + 0x3ec)
#define DCS31      (DMA_BASE + 0x3f0)
#define DCM31      (DMA_BASE + 0x3f4)
#define DDA31      (DMA_BASE + 0x3f8)
#define DSD31      (DMA_BASE + 0x3fc)

// for compatible

#define DSAR0      DSA0 
#define DTAR0      DTA0
#define DCTR0      DTC0
#define DRSR0      DRT0
#define DCCSR0     DCS0
#define DCMR0      DCM0
#define DDAR0      DDA0
#define DSTD0      DSD0

#define DSAR1      DSA1 
#define DTAR1      DTA1
#define DCTR1      DTC1
#define DRSR1      DRT1
#define DCCSR1     DCS1
#define DCMR1      DCM1
#define DDAR1      DDA1
#define DSTD1      DSD1

#define DSAR2      DSA2 
#define DTAR2      DTA2
#define DCTR2      DTC2
#define DRSR2      DRT2
#define DCCSR2     DCS2
#define DCMR2      DCM2
#define DDAR2      DDA2
#define DSTD2      DSD2

#define DSAR3      DSA3 
#define DTAR3      DTA3
#define DCTR3      DTC3
#define DRSR3      DRT3
#define DCCSR3     DCS3
#define DCMR3      DCM3
#define DDAR3      DDA3
#define DSTD3      DSD3

//#define DMAC       (DMA_BASE + 0x1000) 
//#define DIRQP      (DMA_BASE + 0x1004)  
//#define DDR        (DMA_BASE + 0x1008)
//#define DDS        (DMA_BASE + 0x100c)
//#define DCKE       (DMA_BASE + 0x1010)
//#define DCKES      (DMA_BASE + 0x1014)
//#define DCKEC      (DMA_BASE + 0x1018)

#define DMACR      DMAC  
#define DMAIPR     DIRQP  
//#define DDR        DDR  
#define DDRS       DDS  
//#define DCKE       DCKE 
//#define DCKES      DCKES
//#define DCKEC      DCKEC

#else

#define DMACR2     (DMA_BASE + 0x400) 
#define DMAIPR2    (DMA_BASE + 0x404)  
#define DDR2       (DMA_BASE + 0x408)
#define DDRS2      (DMA_BASE + 0x40c)
#define DCKE2      (DMA_BASE + 0x410)
#define DCKES2     (DMA_BASE + 0x414)
#define DCKEC2     (DMA_BASE + 0x418)

#define DMA_CORE1  (DMA_BASE + 0x000)
#define DMA_CORE2  (DMA_BASE + 0x100)

#define DNT0      (DMA_BASE + 0x0c0)
#define DNT1      (DMA_BASE + 0x0c4)
#define DNT2      (DMA_BASE + 0x0c8)
#define DNT3      (DMA_BASE + 0x0cc)
#define DNT4      (DMA_BASE + 0x0d0)
#define DNT5      (DMA_BASE + 0x0d4)

#endif

#define TCSM_BANK0  0xf4000000
#define TCSM_BANK1  0xf4000800
#define TCSM_BANK2  0xf4001000
#define TCSM_BANK3  0xf4001800
#define TCSM_BANK4  0xf4002000
#define TCSM_BANK5  0xf4002800

//DoorBell Set Value
#define DDS_CH0             0x00000001
#define DDS_CH1             0x00000002
#define DDS_CH2             0x00000004
#define DDS_CH3             0x00000008
#define DDS_CH4             0x00000010
#define DDS_CH5             0x00000020
#define DDS_CH6             0x00000040
#define DDS_CH7             0x00000080
#define DDS_CH8             0x00000100
#define DDS_CH9             0x00000200
#define DDS_CH10            0x00000400
#define DDS_CH11            0x00000800
#define DDS_CH12            0x00001000
#define DDS_CH13            0x00002000
#define DDS_CH14            0x00004000
#define DDS_CH15            0x00008000
#define DDS_CH16            0x00010000
#define DDS_CH17            0x00020000
#define DDS_CH18            0x00040000
#define DDS_CH19            0x00080000
#define DDS_CH20            0x00100000
#define DDS_CH21            0x00200000
#define DDS_CH22            0x00400000
#define DDS_CH23            0x00800000
#define DDS_CH24            0x01000000
#define DDS_CH25            0x02000000
#define DDS_CH26            0x04000000
#define DDS_CH27            0x08000000
#define DDS_CH28            0x10000000
#define DDS_CH29            0x20000000
#define DDS_CH30            0x40000000
#define DDS_CH31            0x80000000

#define DDR_CH0             DDS_CH0
#define DDR_CH1             DDS_CH1
#define DDR_CH2             DDS_CH2
#define DDR_CH3             DDS_CH3
#define DDR_CH4             DDS_CH4
#define DDR_CH5             DDS_CH5
#define DDR_CH6             DDS_CH0
#define DDR_CH7             DDS_CH1
#define DDR_CH8             DDS_CH2
#define DDR_CH9             DDS_CH3
#define DDR_CH10            DDS_CH4
#define DDR_CH11            DDS_CH5

//MASK of reserve bits
#ifdef PDMA
#define DTC_MASK           0x00ffffff
#define DRT_MASK           0x0000003f
#define DCS_MASK           0xffffff0b
#define DCM_MASK           0x0fcff707
#define DDA_MASK           0xfffffff0
#define DSD_MASK           0xffffffff

#define DMAC_MASK          0xf83f0003
#define DIRQP_MASK         0x00000000

#define DDR_MASK           0xffffffff
//#define DDS_MASK          0xffffffff   //CANT READ
#define DMACP_MASK         0xffffffff

#define DCIRQP_MASK        0x00000000
#define DMCS_MASK          0x00fffff9
#define DMINT_MASK         0x00000003

#define DIRQP_MASK_CH0     0x00000001
#define DIRQP_MASK_CH1     0x00000002
#define DIRQP_MASK_CH2     0x00000004
#define DIRQP_MASK_CH3     0x00000008
#define DIRQP_MASK_CH4     0x00000010
#define DIRQP_MASK_CH5     0x00000020
#define DIRQP_MASK_CH6     0x00000040
#define DIRQP_MASK_CH7     0x00000080
#define DIRQP_MASK_CH8     0x00000100
#define DIRQP_MASK_CH9     0x00000200
#define DIRQP_MASK_CH10    0x00000400
#define DIRQP_MASK_CH11    0x00000800
#define DIRQP_MASK_CH12    0x00001000
#define DIRQP_MASK_CH13    0x00002000
#define DIRQP_MASK_CH14    0x00004000
#define DIRQP_MASK_CH15    0x00008000
#define DIRQP_MASK_CH16    0x00010000
#define DIRQP_MASK_CH17    0x00020000
#define DIRQP_MASK_CH18    0x00040000
#define DIRQP_MASK_CH19    0x00080000
#define DIRQP_MASK_CH20    0x00100000
#define DIRQP_MASK_CH21    0x00200000
#define DIRQP_MASK_CH22    0x00400000
#define DIRQP_MASK_CH23    0x00800000
#define DIRQP_MASK_CH24    0x01000000
#define DIRQP_MASK_CH25    0x02000000
#define DIRQP_MASK_CH26    0x04000000
#define DIRQP_MASK_CH27    0x08000000
#define DIRQP_MASK_CH28    0x10000000
#define DIRQP_MASK_CH29    0x20000000
#define DIRQP_MASK_CH30    0x40000000
#define DIRQP_MASK_CH31    0x80000000

#else

#define DCTR_MASK           0x00ffffff
#define DCCSR_MASK          0xc0000001
#define DRSR_MASK           0x3f
#define DCMR06_MASK         0xf8cff722
#define DCMR_MASK           0x00cff722
#define DCMR01_MASK         0xf8cff722
#define DDAR_MASK           0xfffffff0
#define DMACR_MASK          0xf8000301
#define DDR_MASK            0x0000003f
#define DMACR6_MASK         0xf8000301

#ifdef DMA_6CH
#define DDR6_MASK           0x0000003f
#else
#define DDR6_MASK           0x0000000f
#endif

#define DMAIPR_MASK_CH0     0x00000001
#define DMAIPR_MASK_CH1     0x00000002
#define DMAIPR_MASK_CH2     0x00000004
#define DMAIPR_MASK_CH3     0x00000008
#define DMAIPR_MASK_CH4     0x00000010
#define DMAIPR_MASK_CH5     0x00000020
#define DMAIPR_MASK_CH6     0x00000040
#define DMAIPR_MASK_CH7     0x00000080
#define DMAIPR_MASK_CH8     0x00000100
#define DMAIPR_MASK_CH9     0x00000200
#define DMAIPR_MASK_CH10    0x00000400
#define DMAIPR_MASK_CH11    0x00000800
#endif

// dcmr control bits
//#define EREQ_CH             2
#define EREQ_CORE           1

#define EACK_ACTIVE_L       0x80000000
#define EACK_ACTIVE_H       0x00000000

#define EACK_WRITE          0x40000000
#define EACK_READ           0x00000000

#define EREQ_RISE_EDGE      0x30000000
#define EREQ_HIGH_LEVEL     0x20000000
#define EREQ_FALL_EDGE      0x10000000
#define EREQ_LOW_LEVEL      0x00000000

#define EREQ_RISE           EREQ_RISE_EDGE
#define EREQ_HIGH           EREQ_HIGH_LEVEL
#define EREQ_FALL           EREQ_FALL_EDGE
#define EREQ_LOW            EREQ_LOW_LEVEL

#define ERDM_R              0x3
#define ERDM_H              0x2
#define ERDM_F              0x1
#define ERDM_L              0x0

#define END_ACTIVE_L        0x08000000
#define END_ACTIVE_H        0x00000000

#define BLAST_MASK          0x02000000
#define BLAST_ENB           0x02000000

#define SADDR_FIX           0x00000000
#define SADDR_INC           0x00800000
#define DADDR_FIX           0x00000000
#define DADDR_INC           0x00400000

#define NF_DRD              1
#define NF_NDR              0
#define NF_DWR              1
#define NF_NDW              0

#define NAC_ENB             1
#define NAC_DIS             0

#define NSTA_ENB             1
#define NSTA_DIS             0

#define EREQ_ENB             1
#define EREQ_DIS             0

#define DNT_ENB             1
#define DNT_DIS             0

#define RDIL_IGNORE      0x00000000
#define RDIL_1_UNIT      0x00010000
#define RDIL_2_UNIT      0x00020000
#define RDIL_3_UNIT      0x00030000
#define RDIL_4_UNIT      0x00040000
#define RDIL_8_UNIT      0x00050000
#define RDIL_16_UNIT     0x00060000
#define RDIL_32_UNIT     0x00070000
#define RDIL_64_UNIT     0x00080000
#define RDIL_128_UNIT    0x00090000

#define RDETECT_IGNORE   RDIL_IGNORE  
#define RDETECT_2_UNIT   RDIL_2_UNIT     
#define RDETECT_4_UNIT   RDIL_4_UNIT     
#define RDETECT_8_UNIT   RDIL_8_UNIT     
#define RDETECT_16_UNIT  RDIL_16_UNIT    
#define RDETECT_32_UNIT  RDIL_32_UNIT    
#define RDETECT_64_UNIT  RDIL_64_UNIT    
#define RDETECT_128_UNIT RDIL_128_UNIT   

#define SRC_WIDTH_4         0x00000000
#define SRC_WIDTH_1         0x00004000
#define SRC_WIDTH_2         0x00008000

#define DST_WIDTH_4         0x00000000
#define DST_WIDTH_1         0x00001000
#define DST_WIDTH_2         0x00002000

#define DATA_SIZE_4         0x00000000
#define DATA_SIZE_1         0x00000100 
#define DATA_SIZE_2         0x00000200
#define DATA_SIZE_16        0x00000300
#define DATA_SIZE_32        0x00000400
#define DATA_SIZE_64        0x00000500
#define DATA_SIZE_128       0x00000600
#define DATA_SIZE_AUTO      0x00000700

#define TMODE_SINGLE        0x00000000
#define TMODE_BLOCK         0x00000080

#define TEND_INT_ENB        0x00000002
#define TEND_INT_DIS        0x00000000

#define DES_VM_ENB          0x00000008
#define DES_VM_DIS          0x00000000

#define DES_V_ENB           0x00000010
#define DES_V_DIS           0x00000000

#define STDE_ENB            0x00000004
#define STDE_DIS            0x00000000

#define INV_INT_ENB         STDE_ENB   
#define INV_INT_DIS         STDE_DIS   

#define DES_LINK            0x00000001
#define DES_NLINK           0x00000000

// dcmr control bits
#define NDES_ENB            0x80000000
#define NDES_DIS            0x00000000 

#define DES8_ENB            0x40000000
#define DES8_DIS            0x00000000 

#define STD_ENB             0x00000004
#define STD_DIS             0x00000000 

#define CHANNEL_ENB         0x00000001
#define CHANNEL_DIS         0x00000000

//DRSR reg function bits definition
#define RS_ADDR_I2S1        0x00000004
#define RS_I2S1_ADDR        0x00000005
#define RS_ADDR_I2S0        0x00000006
#define RS_I2S0_ADDR        0x00000007
#define RS_ADDR_ADDR        0x00000008  // auto 
#define RS_SADC_ADDR        0x00000009

#define RS_ADDR_UART4       0x0000000c
#define RS_UART4_ADDR       0x0000000d
#define RS_ADDR_UART3       0x0000000e
#define RS_UART3_ADDR       0x0000000f
#define RS_ADDR_UART2       0x00000010
#define RS_UART2_ADDR       0x00000011
#define RS_ADDR_UART1       0x00000012
#define RS_UART1_ADDR       0x00000013
#define RS_ADDR_UART0       0x00000014
#define RS_UART0_ADDR       0x00000015

#define RS_ADDR_SSI0        0x00000016
#define RS_SSI0_ADDR        0x00000017
#define RS_ADDR_SSI1        0x00000018
#define RS_SSI1_ADDR        0x00000019

#if (defined(SSI0) | defined(SSI00) | defined(SSI000) | defined(SSI0000) | defined(SSI00000))
#define RS_ADDR_SSI         0x00000016
#define RS_SSI_ADDR         0x00000017
#endif

#if (defined(SSI1) | defined(SSI11) | defined(SSI111))
#define RS_ADDR_SSI         0x00000018
#define RS_SSI_ADDR         0x00000019
#endif

#define RS_ADDR_MSC0        0x0000001a
#define RS_MSC0_ADDR        0x0000001b
#define RS_ADDR_MSC1        0x0000001c
#define RS_MSC1_ADDR        0x0000001d
#define RS_ADDR_MSC2        0x0000001e
#define RS_MSC2_ADDR        0x0000001f

#if (defined(MSC0) | defined(MSC00))
#define RS_ADDR_MSC         0x0000001a
#define RS_MSC_ADDR         0x0000001b
#endif

#if (defined(MSC1) | defined(MSC11))
#define RS_ADDR_MSC         0x0000001c
#define RS_MSC_ADDR         0x0000001d
#endif

#if (defined(MSC2) | defined(MSC22))
#define RS_ADDR_MSC         0x0000001e
#define RS_MSC_ADDR         0x0000001f
#endif

#define RS_ADDR_PCM0        0x00000020
#define RS_PCM0_ADDR        0x00000021
#define RS_ADDR_PCM1        0x00000022
#define RS_PCM1_ADDR        0x00000023

#define RS_ADDR_I2C0        0x00000024
#define RS_I2C0_ADDR        0x00000025
#define RS_ADDR_I2C1        0x00000026
#define RS_I2C1_ADDR        0x00000027
#define RS_ADDR_I2C2        0x00000028
#define RS_I2C2_ADDR        0x00000029
#define RS_ADDR_I2C3        0x0000002a
#define RS_I2C3_ADDR        0x0000002b
#define RS_ADDR_I2C4        0x0000002c
#define RS_I2C4_ADDR        0x0000002d

#ifdef I2C0
#define RS_ADDR_I2C         RS_ADDR_I2C0 
#define RS_I2C_ADDR         RS_I2C0_ADDR 
#endif

#define RS_ADDR_DES         0x0000002e
#define RS_DES_ADDR         0x0000002f

//DMACR reg function bits definiton
#define PRIO_MODE_0123       0x00000000
#define ACC_MODE_0123        0xf8000000
#define PRIO_MODE_0231       0x00000100
#define PRIO_MODE_2013       0x00000200
#define PRIO_MODE_ROUND      0x00000300
#define ACC_MODE_ROUND       0xF8000300
#define DMAC_ENB             0x00000001
#define DMAC_DIS             0x00000000
#define CTE                  0x00000001


//STATUS bits
#define CHANNEL_ADDR_ERR     0x00000010 
#define CHANNEL_END          0x00000008
#define CHANNEL_HLT          0x00000004 
#define CHANNEL_NL_END       0x00000002
#define CHANNEL_DINV         0x00000040

#define DMAC_ADDR_ERR        0x00000004 
#define DMAC_HLT             0x00000008 

#define DMAC_BCH_ERR         0x00000f80 

#define DMAC_BCH_LASTMD_0    0x00000000 
#define DMAC_BCH_LASTMD_1    0x10000000 
#define DMAC_BCH_LASTMD_2    0x20000000 

#define DMAC_BAC_ENB         0x00000002 
#ifdef PDMA
#define DMAC_CH01            0x00000002
#endif

#define open_pdmac()       (REG32(DMAC) |=  DMAC_ENB)
#define close_pdmac()      (REG32(DMAC) &= ~DMAC_ENB) 

#define open_dmac()     open_pdmac()    
#define close_dmac()    close_pdmac()   

/* the fast_mode transfer config*/
#define fast_mode_config(fast_mode) \
({ \
  unsigned int val; \
  if(fast_mode) val  = 0xf8000000; \
  else          val  = 0x00000000; \
  REG32(DMAC)       = ((i_l32(DMAC,0)) | val); \
})

/*------------------------------------------------------
  mcu_boot_up    
  ------------------------------------------------------*/
#define mcu_boot_up() (REG32(DMCS) = 0x0)

/*------------------------------------------------------
  ddr_nemc    
  ------------------------------------------------------*/

#define INTC_PDMA_MAILBOX  (29+32)
#define set_pdma_mailbox_vbr()  setvbr(TP_INT | INTC_PDMA_MAILBOX, pdma_mailbox_int_handler)
#define clear_pdma_mailbox_int() (REG32(DMINT) = 0x0)

#endif
