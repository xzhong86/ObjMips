
#include <stdio.h>
#include <stdlib.h>

#include <base.h>
#include <init.h>
#include <mem.h>
#include <cache.h>
#include <mips.h>
#include <test/fw.h>


#include <time.h>
#include "t_pdma.h"
#include "t_bch.h"
#include "server.h"
#include "irq.h"
#include "mipsop.h"



#define TG_NAND
#define tgt_addr    ((unsigned int)0x1b000000)
#define NEMC_BANK   1
#include "t_tg_nand.h"
#define COM_MAIl_BOX 0xB3421034

volatile static unsigned int ddr2nemc_done ;
static unsigned int nemc_nand_flash_bank;
static unsigned int use_toggle_nand;
static unsigned int mcu_code[2012];

static void pdma_mailbox_int_handler();//interrupt handler
static void bch_cfg(unsigned int,unsigned int);
static void single_test_nand(unsigned int,unsigned int *,unsigned int *);

struct my_data {
	int times, n;
#define TEST_SIZE 100*1024
	unsigned int *mem;
	unsigned int phy;
};

static int my_prepare(struct fw_dev *dev)
{
	struct my_data *md;
	unsigned int i;
	
	nemc_nand_flash_bank = NEMC_BANK;
	use_toggle_nand = 1 ;//1:use toggle nand flash; 0:use common nand flash;
	/* the mcu must be reset at first */
	REG32(0xb3421030) = REG32(0xb3421030) | 0x1;//reset mcu

	printk("download the mcu code %d bytes\n",sizeof(mcu_code));
	for(i=0;i<sizeof(mcu_code)/4;i++)
		REG32(0xb3422000 + i*4) = mcu_code[i];
  
	register_irqfun(32+8+29,pdma_mailbox_int_handler,"pdma_mailbox",NULL);
	REG32(0xb000102c) = 0x20000000;
	
	
	fast_mode_config(0);     //not fast mode
	mcu_boot_up();           //boot up mcu
	REG32(DMAC) = 1;
	printk("the initial dmac is %x\n",REG32(DMAC));

	md = malloc(sizeof(*md));
	if (!md)
		return -1;
	md->times = 0;
	md->mem = mem_alloc(20*1024);//20K,10K is for source data, teh other is for read-back data
	if (!md->mem) {
		free(md);
		return -1;
	}
	md->phy = mem_get_phy(md->mem);
	dev->priv = md;
	return 0;
}

static int my_start(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	unsigned int *ddr_data = md->mem;
	unsigned int *ddr_data_readback = (unsigned int *)((unsigned int)md->mem + 10*1024);
	
	single_test_nand(md->times,ddr_data,ddr_data_readback);
	//md->times++;
	dev->priv = md;
	
	return 0;
}

static void my_ask(struct fw_dev *dev)
{
	if(ddr2nemc_done)
		fw_finish(dev);
}

static chk_t my_check(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	unsigned int *ddr_data = md->mem;
	unsigned int *ddr_data_readback = (unsigned int *)((unsigned int)md->mem + 10*1024);
	int i;
	unsigned int error = 0;
	
	if(md->times%2 == 0)
		nemc_write_ck(nemc_nand_flash_bank) ;
	else
	{
		nemc_read_ck();
		for(i=0; i<10*1024/4; i++){
			if(ddr_data[i] != ddr_data_readback[i]) 
			{
				printk("read-back %d is %08x but need %08x\n",
				       i,ddr_data_readback[i],ddr_data[i]);
				if (error ++ > 10)
					break;
			}    
		}
		ddr2nemc_done = 0;
		if(error)
			printk("there have been %d errors\n",error);
	}

	md->times++;
	dev->priv = md;

	if(error)
		return CHK_FINISHED;
	return CHK_PASSED;
}

static void my_stop(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;

	if (md->times <= 10)
		printk("no, i stopped at %d\n", md->times);
	else
		printk("o, i stopped\n");
	mem_free(md->mem);
	free(md);
}

static struct fw_ops mydev_ops = {
	.prepare = my_prepare,
	.start = my_start,
	.ask = my_ask,
	.check = my_check,
	.stop = my_stop,
};

static int init_dev(void)
{
	if (!fwdev_register("pdma-nand", &mydev_ops))
		return -1;
	return 0;
}
fw_initcall(init_dev,100);



static void pdma_mailbox_int_handler()//interrupt handler
{
	pr_dbg("enter the pdma_mailbox interrupt handler\n");
	clear_pdma_mailbox_int();
	if(((REG32(COM_MAIl_BOX) & 0xff) == 0x1) || ((REG32(COM_MAIl_BOX) & 0xff) == 0x2))
	{
		ddr2nemc_done = 1;
		if((REG32(COM_MAIl_BOX) & 0xff) == 0x1)
			pr_dbg("the nand write has completed\n");
		else
			pr_dbg("the nand read has completed\n");
	}
	else if((REG32(COM_MAIl_BOX) & 0xff) == 0xff)
	{
		pr_dbg("there is one nand block is un-corrected\n");
		pr_dbg("the un_corrected block is %d\n",(unsigned int)((REG32(COM_MAIl_BOX) >> 24) & 0xff));
	}
}

static void bch_cfg(unsigned int bch_level,unsigned int block)
{
	unsigned int blocksize     = block ;
	unsigned int paritysize    = bch_level*14/8;
	unsigned int sel_ecc_level = bch_level ;
	pr_dbg("prepare to bch_cfg\n");
	bch_set_size(
		blocksize ,
		paritysize
		) ;
	REG32(BCH_BASE + 0x0) = 0x5|((sel_ecc_level&0x7f)<<4) ;
	pr_dbg("bch_cfg has been completed\n");
}

static void single_test_nand(unsigned int loop_cnt,
			     unsigned int *ddr_data,
			     unsigned int *ddr_data_readback
	)
{


	unsigned int parity_temp; 
	unsigned int  config_loop;
	unsigned short block_size;
	unsigned int bch_level;
	unsigned short page_size;
	unsigned int trans_bytes;
	unsigned int block_cnt;
	unsigned int block_cnt_temp;

	static unsigned int addr0     = 0;
	static unsigned int addr1     = 0;
	static unsigned int addr2     = 0;
	static unsigned int local_new_page_flag = 0;
	local_new_page_flag++;
  
 
	block_size  = (16 * (loop_cnt + 1))%1024;
	block_size    = 64;
	if(block_size == 0)
		block_size = 1024;      
	pr_dbg("the current block_size is %d\n",block_size);

	bch_level      = 4 * (loop_cnt%16 + 1);
	bch_level      = 4;
	pr_dbg("the current bch_level is %d\n",bch_level);
      
	if((bch_level*14/8)%4)
		parity_temp = ((bch_level*14/8)/4 + 1)*4;
	else
		parity_temp = bch_level*14/8;
      
	block_cnt      = (4096 + 128)/(block_size + parity_temp);
	block_cnt_temp = 2048/parity_temp;
	if(block_cnt > block_cnt_temp)
		block_cnt = block_cnt_temp;
	pr_dbg("the current transfer-cnt is %d\n",block_cnt);
      
	trans_bytes    = block_cnt * block_size;
	pr_dbg("the current total transfer bytes is %d\n",trans_bytes);
      
	page_size      = trans_bytes;
   
	unsigned int i, src_addr;

	
	pr_dbg("the nand source data has been ready\n");
	bch_cfg(bch_level,block_size) ;
  
	if(loop_cnt%2 == 0)
	{
		for(i=0; i<10*1024/4; i++)
		{
			ddr_data[i] = i + loop_cnt * 10*1024/4;
			ddr_data_readback[i] = 0;
		}
		for(i=trans_bytes/4;i<10*1024/4; i++)
			ddr_data[i] = 0;
		blast_dcache();
		jz_sync();
	
		pr_dbg("the nand source data has been ready\n");
		//---------------------------------------
		//nemc erase a block
		//---------------------------------------
		nemc_cfg_tg(addr0, addr1, addr2, ((addr2&0xff00)>>8), ((addr2&0xff0000)>>16), nemc_nand_flash_bank);
		nemc_write_ini(addr0, addr1, addr2, ((addr2&0xff00)>>8), ((addr2&0xff0000)>>16), nemc_nand_flash_bank);
		src_addr = (unsigned int)ddr_data & 0x1fffffff;
  
		ddr2nemc_done = 0 ;
		config_ddr_nemc_trans(src_addr, tgt_addr, trans_bytes, 1,addr0, addr1, addr2, 
				      ((addr2&0xff00)>>8), ((addr2&0xff0000)>>16), 
				      nemc_nand_flash_bank,use_toggle_nand,page_size,block_size);
		pr_dbg("nand write transfer start\n");
	}
	else
	{
		//-----------------------------------------------------------------
		//read nand flash
		//-----------------------------------------------------------------
		unsigned int read_nemc_tgt_addr = (unsigned int)ddr_data_readback & 0x1fffffff;
		src_addr = tgt_addr ;
		nemc_read_ini(addr0, addr1, addr2, ((addr2&0xff00)>>8), ((addr2&0xff0000)>>16), nemc_nand_flash_bank);
		config_ddr_nemc_trans(src_addr, read_nemc_tgt_addr, trans_bytes, 2, addr0, addr1, addr2, 
				      ((addr2&0xff00)>>8), ((addr2&0xff0000)>>16), 
				      nemc_nand_flash_bank,use_toggle_nand,page_size,block_size);
		pr_dbg("nand read transfer start\n");
	}

	if(local_new_page_flag%2 == 0)
		addr2++;
	pr_dbg("the current nand page address is %d\n",addr2);
}

static unsigned int mcu_code[2012] = {
	0x10000041, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x10000006, 0x00000000, 0x3c1df400, 0x27bd3ff8, 0x3c1cf400, 0x100000ca, 0x279c1f20, 0x27bdff80,
	0xafa10004, 0xafa20008, 0xafa3000c, 0xafa40010, 0xafa50014, 0xafa60018, 0xafa7001c, 0xafa80020,
	0xafa90024, 0xafaa0028, 0xafab002c, 0xafac0030, 0xafad0034, 0xafae0038, 0xafaf003c, 0xafb00040,
	0xafb10044, 0xafb20048, 0xafb3004c, 0xafb40050, 0xafb50054, 0xafb60058, 0xafb7005c, 0xafb80060,
	0xafb90064, 0xafba0068, 0xafbb006c, 0xafbc0070, 0xafbd0074, 0xafbe0078, 0xafbf007c, 0x03a02021,
	0x27bdfff0, 0x0d000090, 0x00000000, 0x27bd0010, 0x8fa10004, 0x8fa20008, 0x8fa3000c, 0x8fa40010,
	0x8fa50014, 0x8fa60018, 0x8fa7001c, 0x8fa80020, 0x8fa90024, 0x8faa0028, 0x8fab002c, 0x8fac0030,
	0x8fad0034, 0x8fae0038, 0x8faf003c, 0x8fb00040, 0x8fb10044, 0x8fb20048, 0x8fb3004c, 0x8fb40050,
	0x8fb50054, 0x8fb60058, 0x8fb7005c, 0x8fb80060, 0x8fb90064, 0x8fba0068, 0x8fbb006c, 0x8fbc0070,
	0x8fbd0074, 0x8fbe0078, 0x8fbf007c, 0x27bd0080, 0x42000018, 0x00000000, 0x00000000, 0x00000000,
	0x40026001, 0x30420002, 0x1040007a, 0x3c021342, 0x34421028, 0x8c450000, 0x00001021, 0x40826001,
	0x30a20001, 0x10400014, 0x30a20002, 0x3c041342, 0x34830010, 0x3c028000, 0xac620000, 0x3c03f400,
	0x8c621f50, 0x34841028, 0x34420001, 0xac621f50, 0x8c820000, 0x2403fffe, 0x00431024, 0xac820000,
	0x3c03f400, 0x90621f45, 0x304200ff, 0x34420001, 0xa0621f45, 0x30a20002, 0x10400014, 0x30a20004,
	0x3c041342, 0x34830030, 0x3c028000, 0xac620000, 0x3c03f400, 0x8c621f50, 0x34841028, 0x34420002,
	0xac621f50, 0x8c820000, 0x2403fffd, 0x00431024, 0xac820000, 0x3c03f400, 0x90621f45, 0x304200ff,
	0x34420002, 0xa0621f45, 0x30a20004, 0x10400014, 0x3c06f400, 0x3c041342, 0x34830050, 0x3c028000,
	0xac620000, 0x3c03f400, 0x8c621f50, 0x34841028, 0x34420004, 0xac621f50, 0x8c820000, 0x2403fffb,
	0x00431024, 0xac820000, 0x3c03f400, 0x90621f45, 0x304200ff, 0x34420004, 0xa0621f45, 0x3c06f400,
	0x34c22fcc, 0x8c470000, 0x24e2ffff, 0x2c420002, 0x10400030, 0x24030007, 0x3c02f400, 0xa0431f45,
	0x34c22fc0, 0x8c430000, 0x3c02f400, 0xac431f54, 0x34c22fc4, 0x8c430000, 0x3c02f400, 0xac431f38,
	0x34c22fc8, 0x8c420000, 0x3c05f400, 0xaca21f64, 0x34c62fd8, 0x8ca31f64, 0x8cc20000, 0x3c04f400,
	0x00021402, 0x0062001b, 0x004001f4, 0x3c02f400, 0x24080001, 0x00001812, 0xac831f34, 0x8c831f34,
	0xac431f5c, 0x3c02f400, 0xa0401f68, 0x8cc20000, 0x00021402, 0xaca21f64, 0x14e80004, 0x3c02f400,
	0xac481f6c, 0x0900010b, 0x3c02f400, 0x24020002, 0x14e20006, 0x3c02f400, 0x3c02f400, 0xac471f6c,
	0x3c02f400, 0xa0481f4e, 0x3c02f400, 0x34422fcc, 0xac400000, 0x03e00008, 0x00000000, 0x00000000,
	0x27bdffe8, 0xafbf0010, 0x24020001, 0x40826000, 0x00001021, 0x40826001, 0x40826800, 0x3c021342,
	0x34422000, 0x4082f000, 0x3c031342, 0x3464103c, 0x24020003, 0xac820000, 0x3469102c, 0x34651034,
	0x34661038, 0x34671020, 0x34681024, 0x2402ffff, 0x34631028, 0xaca00000, 0xacc00000, 0xac800000,
	0xace00000, 0xad020000, 0xac600000, 0xad200000, 0x0d000584, 0x00000000, 0x0d000494, 0x00000000,
	0x0d0005bc, 0x00000000, 0x00001021, 0x40826000, 0x3c02f400, 0x90421f45, 0x24030007, 0x304200ff,
	0x10430003, 0x24020001, 0x42000020, 0x24020001, 0x40826000, 0x0d000758, 0x00000000, 0x0d0002da,
	0x00000000, 0x0d000688, 0x00000000, 0x0900012e, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x3c021341, 0x3442010c, 0x3c038000, 0xac430000, 0x8c420000, 0x03e00008, 0x00000000, 0x3c021341,
	0x3442010c, 0xac400000, 0x8c420000, 0x03e00008, 0x00000000, 0x3c021341, 0x3442013c, 0x8c430000,
	0x2405003f, 0x0900015c, 0x00402021, 0x8c830000, 0x3062003f, 0x1445fffd, 0x00001021, 0x03e00008,
	0x00000000, 0x03e00008, 0x00001021, 0x03e00008, 0x00001021, 0x3c07f400, 0x3c02134d, 0x34e72fd4,
	0x3442000c, 0x8ce30000, 0x8c420000, 0x3c0600ff, 0x00461024, 0x00021402, 0x24420400, 0x00031e02,
	0x70621802, 0x34c6ffff, 0x30623f00, 0x00021202, 0xa0830000, 0xa0a20000, 0x8ce40000, 0x8ce30000,
	0x3c02ff00, 0x00621824, 0x3c020100, 0x00862024, 0x00621821, 0x00832025, 0xace40000, 0x03e00008,
	0x00000000, 0x2484ffff, 0x24030001, 0x00831804, 0x3c028000, 0x00621825, 0x3c021341, 0x3442010c,
	0xac430000, 0x8c430000, 0x3c050001, 0x0900018e, 0x00402021, 0x8c830000, 0x00651024, 0x1040fffd,
	0x00000000, 0x03e00008, 0x00000000, 0x3c021341, 0x34420148, 0x2403006e, 0xac430000, 0x8c430000,
	0x00402821, 0x0900019c, 0x3c040001, 0x8ca30000, 0x00641024, 0x1040fffd, 0x00000000, 0x03e00008,
	0x00000000, 0x03e00008, 0x00000000, 0x27bdffc8, 0xafb50024, 0xafb40020, 0xafbf0030, 0xafb7002c,
	0xafb60028, 0xafb3001c, 0xafb20018, 0xafb10014, 0xafb00010, 0x00a0a021, 0x14a00005, 0x0080a821,
	0x00061202, 0x304400ff, 0x090001cf, 0x30c600ff, 0x24020001, 0x0006ba02, 0x14a20016, 0x30d600ff,
	0x00049840, 0x2482ffff, 0x2673fffe, 0x3c100001, 0x00508004, 0x02651004, 0x02021025, 0x3c121341,
	0x3c038000, 0x00431025, 0x36510050, 0xae220000, 0x0d000155, 0x00000000, 0x24020003, 0x02621004,
	0x02028025, 0xae300000, 0x3652010c, 0xae400000, 0x8e420000, 0x02c03021, 0x32e400ff, 0x24020085,
	0x3c031b40, 0xa0620000, 0x24020001, 0x1682000d, 0x3c021b80, 0x3c021341, 0x34420148, 0x2403006e,
	0xac430000, 0x8c430000, 0x00403821, 0x090001de, 0x3c050001, 0x8ce30000, 0x00651024, 0x1040fffd,
	0x3c021b80, 0xa0460000, 0xa0440000, 0x24020001, 0x1682000c, 0x8fbf0030, 0x02a02021, 0x8fb7002c,
	0x8fb60028, 0x8fb50024, 0x8fb40020, 0x8fb3001c, 0x8fb20018, 0x8fb10014, 0x8fb00010, 0x09000181,
	0x27bd0038, 0x8fb7002c, 0x8fb60028, 0x8fb50024, 0x8fb40020, 0x8fb3001c, 0x8fb20018, 0x8fb10014,
	0x8fb00010, 0x03e00008, 0x27bd0038, 0x03e00008, 0x00000000, 0x3c021341, 0x24070001, 0x14a70003,
	0x3442010c, 0xac400000, 0x8c420000, 0x24020005, 0x3c041b40, 0xa0820000, 0x3c021b80, 0xa0460000,
	0x00061a02, 0xa0430000, 0x240200e0, 0xa0820000, 0x3c021341, 0x14a70004, 0x3443010c, 0x3c028000,
	0xac620000, 0x8c620000, 0x03e00008, 0x00000000, 0x03e00008, 0x00001021, 0x27bdffd0, 0x00041040,
	0xafb20018, 0x2484ffff, 0x2452fffe, 0x3c020001, 0xafb00010, 0x00828004, 0x02451004, 0xafb50024,
	0xafb40020, 0x02021025, 0x3c141341, 0x3c038000, 0x24150001, 0xafb3001c, 0xafb10014, 0xafbf0028,
	0x36910050, 0x00a09821, 0x14b5000b, 0x00431025, 0xae220000, 0x0d000155, 0x00000000, 0x24020003,
	0x02421004, 0x02021025, 0xae220000, 0x3682010c, 0xac400000, 0x8c420000, 0x24020005, 0x3c041b40,
	0xa0820000, 0x00001821, 0x3c021b80, 0xa0430000, 0xa0430000, 0x240200e0, 0xa0820000, 0x3c021341,
	0x16750004, 0x3443010c, 0x3c028000, 0xac620000, 0x8c620000, 0x8fbf0028, 0x8fb50024, 0x8fb40020,
	0x8fb3001c, 0x8fb20018, 0x8fb10014, 0x8fb00010, 0x00001021, 0x03e00008, 0x27bd0030, 0x03e00008,
	0x00001021, 0x27bdffe0, 0x3c021341, 0xafb10014, 0x34510050, 0x00041040, 0xafb20018, 0x2484ffff,
	0x2452fffe, 0x3c020001, 0xafb00010, 0x00828004, 0x02451004, 0x02021025, 0x3c038000, 0x00431825,
	0x24020001, 0x14a20008, 0xafbf001c, 0xae230000, 0x0d000155, 0x00000000, 0x24020003, 0x02421004,
	0x02021025, 0xae220000, 0x8fbf001c, 0x8fb20018, 0x8fb10014, 0x8fb00010, 0x00001021, 0x03e00008,
	0x27bd0020, 0x3c02134d, 0x34420184, 0x8c420000, 0x00004821, 0x00021602, 0x304a007f, 0x09000288,
	0x3c0bffff, 0x8ca30000, 0x00031040, 0x30420ffc, 0x006b4024, 0x00823821, 0x00033402, 0x30630001,
	0x10600003, 0x8ce20000, 0x09000285, 0x01021826, 0x00c21826, 0xace30000, 0x25290001, 0x24a50004,
	0x152afff0, 0x00000000, 0x03e00008, 0x00000000, 0x3c02134d, 0x8c420000, 0x3c03f400, 0x90631f20,
	0x00021102, 0x3042007f, 0x0062001b, 0x004001f4, 0x00003021, 0x00001010, 0x090002a0, 0x304700ff,
	0x10a00003, 0x8c820000, 0x0900029d, 0x38420001, 0x38420080, 0xac820000, 0x24c60001, 0x24840004,
	0x14c7fff7, 0x30c50001, 0x24630001, 0x3c02f400, 0x03e00008, 0xa0431f20, 0x3c02134d, 0x34420184,
	0x8c430000, 0x30620001, 0x10400013, 0x3c057f00, 0x34a20003, 0x00621024, 0x24060001, 0x1046000e,
	0x30640003, 0x14860007, 0x24020003, 0x00651024, 0x10400007, 0x00000000, 0x34a20001, 0x03e00008,
	0x00621024, 0x14820002, 0x00000000, 0x24020002, 0x03e00008, 0x00000000, 0x03e00008, 0x00001021,
	0x3c021342, 0x34480034, 0x34430020, 0x34470024, 0x34420028, 0xac640000, 0xace50000, 0xac460000,
	0x3c020440, 0x34420702, 0xad020000, 0x03e00008, 0x00001021, 0x3c021342, 0x34480034, 0x34430020,
	0x34470024, 0x34420028, 0xac640000, 0xace50000, 0xac460000, 0x3c020180, 0x34420702, 0xad020000,
	0x03e00008, 0x00001021, 0x27bdffd8, 0xafbf0024, 0xafb40020, 0xafb3001c, 0xafb20018, 0xafb10014,
	0xafb00010, 0x3c02f400, 0x90421f4e, 0x1040003a, 0x3c06f400, 0x3c02f400, 0x34452fd8, 0x34422fd4,
	0x8c430000, 0x8c440000, 0x3c02134d, 0x3442000c, 0x8ca60000, 0x8c420000, 0x00031a02, 0x00021402,
	0x00042402, 0x30450003, 0x307300ff, 0x309400ff, 0x10a00003, 0x3043007f, 0x3062007c, 0x24430004,
	0x3c02f400, 0x8c511f5c, 0x30c6ffff, 0x02602021, 0x02802821, 0x0d0001fd, 0x70718802, 0x3c02f400,
	0x8c491f54, 0x3c101342, 0x36060020, 0x36070024, 0x36025000, 0xacc90000, 0xace20000, 0x3c020440,
	0x36030028, 0x36080034, 0x34420702, 0x3c12f400, 0xac710000, 0x24040001, 0x26451f45, 0xad020000,
	0x0d000484, 0x36100030, 0x3c028000, 0x34420001, 0xae020000, 0x92421f45, 0x30420002, 0x1040fffd,
	0x02602021, 0x0d000216, 0x02802821, 0x3c02f400, 0xa0401f4e, 0x3c06f400, 0x8cc21f6c, 0x3c03f400,
	0x2442ffff, 0x24651f3c, 0x00021080, 0x00451021, 0x90440000, 0x8cc21f6c, 0x2442ffff, 0x00021080,
	0x00451021, 0x90430001, 0x8cc21f6c, 0x00641825, 0x2442ffff, 0x00021080, 0x00451021, 0x90440002,
	0x8cc21f6c, 0x2442ffff, 0x00021080, 0x00451021, 0x90420003, 0x00441025, 0x00431025, 0x30420010,
	0x10400072, 0x8fbf0024, 0x8cc31f6c, 0x8cc21f6c, 0x2463ffff, 0x2442ffff, 0x00021080, 0x00451021,
	0x90420000, 0x00031880, 0x304200ef, 0x00651821, 0xa0620000, 0x8cc31f6c, 0x8cc21f6c, 0x2463ffff,
	0x2442ffff, 0x00021080, 0x00451021, 0x90420001, 0x00031880, 0x304200ef, 0x00651821, 0xa0620001,
	0x8cc31f6c, 0x8cc21f6c, 0x2463ffff, 0x2442ffff, 0x00021080, 0x00451021, 0x90420002, 0x00031880,
	0x304200ef, 0x00651821, 0xa0620002, 0x8cc31f6c, 0x8cc21f6c, 0x2463ffff, 0x2442ffff, 0x00021080,
	0x00451021, 0x90420003, 0x00031880, 0x304200ef, 0x00651821, 0xa0620003, 0x3c02f400, 0x8c421f34,
	0x14400042, 0x8fbf0024, 0x8cc31f6c, 0x24020001, 0x14620038, 0x24030002, 0x3c03f400, 0x34622fd4,
	0x8c440000, 0x8c420000, 0x34632fd8, 0x00021402, 0x8c660000, 0x00042202, 0x305200ff, 0x309300ff,
	0x02602021, 0x30c6ffff, 0x0d0001a3, 0x02402821, 0x3c02134d, 0x3442000c, 0x8c420000, 0x00021402,
	0x30430003, 0x10600003, 0x3044007f, 0x3082007c, 0x24440004, 0x3c02f400, 0x8c431f5c, 0x3c101342,
	0x70831802, 0x3c02f400, 0x8c4a1f38, 0x36080020, 0x36025000, 0xad020000, 0x3c020180, 0x36060024,
	0x36070028, 0x36090034, 0x34420702, 0x3c11f400, 0xacca0000, 0x24040001, 0xace30000, 0x26251f45,
	0xad220000, 0x0d000484, 0x36100030, 0x3c028000, 0x34420001, 0xae020000, 0x92221f45, 0x30420002,
	0x1040fffd, 0x02602021, 0x0d000251, 0x02402821, 0x24030001, 0x3c021342, 0x34421034, 0xac430000,
	0x0d000584, 0x00000000, 0x8fbf0024, 0x8fb40020, 0x8fb3001c, 0x8fb20018, 0x8fb10014, 0x8fb00010,
	0x00001021, 0x03e00008, 0x27bd0028, 0x00000000, 0x308400ff, 0x2c820007, 0x10400013, 0x24020001,
	0x00822004, 0x30820011, 0x14400006, 0x3c02f400, 0x30820066, 0x14400006, 0x3c03f400, 0x090003cb,
	0x3c04f400, 0x90421f68, 0x090003d6, 0x304300ff, 0x90621f68, 0x1040001b, 0x00000000, 0x90621f68,
	0x090003d5, 0x2442ffff, 0x3c04f400, 0x90821f68, 0x10400012, 0x00000000, 0x90821f68, 0x24030001,
	0x304200ff, 0x1043000f, 0x00000000, 0x90821f68, 0x2442fffe, 0x304300ff, 0x24020002, 0x10620007,
	0x24040003, 0x10640007, 0x00000000, 0x38620001, 0x0002200a, 0x03e00008, 0x00801021, 0x03e00008,
	0x24020001, 0x03e00008, 0x24020002, 0x10800004, 0x3c02f400, 0x3c02f400, 0xac451f54, 0x3c02f400,
	0x8c431f6c, 0x24020001, 0x14620030, 0x3c02f400, 0x3c02f400, 0x90421f4c, 0x10400020, 0x3c02f400,
	0x3c04f400, 0x90821f68, 0x304200ff, 0x1443000b, 0x3c02f400, 0x8c451f54, 0x3c02f400, 0x8c431f5c,
	0x3c02f400, 0x8c441f34, 0x3c02f400, 0x8c421f64, 0x00641823, 0x09000419, 0x00a00013, 0x90821f68,
	0x24030002, 0x304200ff, 0x1443001c, 0x00000000, 0x3c02f400, 0x8c441f54, 0x3c02f400, 0x8c431f5c,
	0x3c02f400, 0x8c421f34, 0x00621823, 0x3c02f400, 0x8c421f64, 0x09000418, 0x24630001, 0x8c441f54,
	0x3c02f400, 0x8c431f5c, 0x3c02f400, 0x8c421f34, 0x00621823, 0x3c02f400, 0x8c421f64, 0x24630002,
	0x00800013, 0x0900041d, 0x70620000, 0x8c421f54, 0x00400013, 0x00001012, 0x00400013, 0x03e00008,
	0x00001012, 0x10800004, 0x3c02f400, 0x3c02f400, 0xac451f38, 0x3c02f400, 0x8c431f6c, 0x24020002,
	0x1462000c, 0x3c04f400, 0x3c02f400, 0x8c851f38, 0x8c431f5c, 0x3c02f400, 0x8c441f34, 0x3c02f400,
	0x8c421f64, 0x00641823, 0x70622002, 0x03e00008, 0x00851021, 0x8c821f38, 0x03e00008, 0x00000000,
	0x308400ff, 0x2c820007, 0x1040002c, 0x30a500ff, 0x24020001, 0x00822004, 0x30820011, 0x14400005,
	0x30820066, 0x14400014, 0x00000000, 0x09000467, 0x00000000, 0x10a00007, 0x3c04f400, 0x3c03f400,
	0x90621f32, 0x24420001, 0x304200ff, 0xa0621f32, 0x3c04f400, 0x90821f32, 0x24030003, 0x304200ff,
	0x14430002, 0x00000000, 0xa0801f32, 0x90821f32, 0x09000477, 0x304300ff, 0x10a00007, 0x3c04f400,
	0x3c03f400, 0x90621f4d, 0x24420001, 0x304200ff, 0xa0621f4d, 0x3c04f400, 0x90821f4d, 0x24030003,
	0x304200ff, 0x14430002, 0x00000000, 0xa0801f4d, 0x90821f4d, 0x09000477, 0x304300ff, 0x10a00007,
	0x3c04f400, 0x3c03f400, 0x90621f30, 0x24420001, 0x304200ff, 0xa0621f30, 0x3c04f400, 0x90821f30,
	0x24030003, 0x304200ff, 0x14430002, 0x00000000, 0xa0801f30, 0x90821f30, 0x304300ff, 0x1060000a,
	0x3c021342, 0x24020001, 0x14620004, 0x00000000, 0x3c021342, 0x03e00008, 0x34424400, 0x3c021342,
	0x03e00008, 0x34424800, 0x03e00008, 0x34424000, 0x308400ff, 0x00001021, 0x40826000, 0x24020001,
	0x90a30000, 0x00821004, 0x00021027, 0x306300ff, 0x00621824, 0xa0a30000, 0x24020001, 0x40826000,
	0x03e00008, 0x00000000, 0x00000000, 0x00000000, 0x3c021342, 0x3442101c, 0x8c430000, 0x3c04f400,
	0x34630007, 0xac430000, 0x8c821f6c, 0x24050001, 0x10450005, 0x00000000, 0x8c831f6c, 0x24020002,
	0x146200df, 0x3c02f400, 0x3c04f400, 0x90821f68, 0x304300ff, 0x10650021, 0x00000000, 0x10600005,
	0x24020002, 0x14620047, 0x3c02f400, 0x090004d6, 0x00000000, 0x90821f68, 0x24420001, 0x304200ff,
	0xa0821f68, 0x3c02f400, 0x90421f4c, 0x304200ff, 0x10450003, 0x240a0006, 0x090004ba, 0x24080008,
	0x00005021, 0x00004021, 0x3c02f400, 0x90421f58, 0x14400005, 0x3c03f400, 0x00002021, 0x00004821,
	0x09000570, 0x24070001, 0x8c621f34, 0x14400038, 0x00000000, 0x09000575, 0x24090010, 0x90821f68,
	0x3c03f400, 0x24420001, 0x304200ff, 0xa0821f68, 0x3c02f400, 0x90421f4c, 0x90631f58, 0x304200ff,
	0x38420001, 0x240a0008, 0x146000a4, 0x0002500a, 0x0900056d, 0x00002021, 0x90821f68, 0x3c03f400,
	0x24420001, 0x304200ff, 0xa0821f68, 0x90621f4c, 0x10400002, 0x00000000, 0xa0601f4c, 0x3c03f400,
	0x90621f58, 0x14400007, 0x00000000, 0x00002021, 0x24090006, 0x00005021, 0x24080001, 0x09000570,
	0x24070008, 0x24040001, 0x24090006, 0x240a0010, 0x24080001, 0x24070008, 0xa0601f58, 0x09000571,
	0x24020001, 0xa0451f58, 0x3c03f400, 0xa0801f68, 0x8c621f34, 0x10400072, 0x00000000, 0x8c621f34,
	0x2442ffff, 0xac621f34, 0x09000569, 0x24090008, 0x8c621f34, 0x2442ffff, 0xac621f34, 0x09000575,
	0x24090010, 0x8c621f34, 0x24090001, 0x2442ffff, 0x24080010, 0x0900050c, 0x24070006, 0x8c621f34,
	0x10400005, 0x3c02f400, 0x8c621f34, 0x2442ffff, 0xac621f34, 0x3c02f400, 0x8c421f5c, 0x24030001,
	0x10430005, 0x24040002, 0x14440007, 0x3c02f400, 0x09000518, 0x00005021, 0x00004821, 0x00005021,
	0x09000543, 0x00004021, 0x8c421f34, 0x2c420003, 0x10400027, 0x3c02f400, 0x3c02f400, 0x90421f4c,
	0x14400023, 0x3c02f400, 0x3c02f400, 0x90421f68, 0x304200ff, 0x1043000d, 0x00000000, 0x10400005,
	0x00000000, 0x14440014, 0x3c03f400, 0x09000539, 0x00000000, 0x3c03f400, 0x90621f44, 0x00005021,
	0x304200ff, 0x09000542, 0x34420008, 0x3c03f400, 0x90621f44, 0x00003821, 0x304200ff, 0x09000542,
	0x34420001, 0x90621f44, 0x00004821, 0x304200ff, 0x09000542, 0x34420002, 0x90621f44, 0x00004021,
	0x304200ff, 0x34420004, 0xa0621f44, 0x3c02f400, 0x90451f44, 0x90431f44, 0x90441f44, 0x90421f44,
	0x3c0bf400, 0x30420008, 0x8d661f6c, 0x30a50001, 0x30630002, 0x30840004, 0x0002500b, 0x24020001,
	0x0005380b, 0x0003480b, 0x14c20009, 0x0004400b, 0x3c03f400, 0x24621f3c, 0xa0671f3c, 0xa0490001,
	0xa0480002, 0xa04a0003, 0x09000580, 0x3c02f400, 0x8d631f6c, 0x24020002, 0x14620020, 0x00000000,
	0x3c02f400, 0x24421f3c, 0xa0470004, 0xa0490005, 0xa0480006, 0xa04a0007, 0x09000580, 0x3c02f400,
	0x24090008, 0x240a0001, 0x24080006, 0x0900050d, 0x24070010, 0x24090001, 0x00004021, 0x24070006,
	0x24020001, 0x1082ff95, 0x3c03f400, 0x0900050e, 0x3c02f400, 0x0900050d, 0x24070001, 0x3c03f400,
	0x8c621f34, 0x1440ff87, 0x00000000, 0x24090001, 0x24080010, 0x0900050d, 0x24070006, 0x3c02f400,
	0xac401f50, 0x03e00008, 0x00001021, 0x00000000, 0x3c02f400, 0xac401f50, 0x3c02f400, 0xac401f48,
	0x3c02f400, 0xa0401f45, 0x3c02f400, 0xa0401f68, 0x3c02f400, 0xa0401f58, 0x3c02f400, 0xac401f6c,
	0x24030001, 0x3c02f400, 0xa0431f4c, 0x3c02f400, 0xa0401f44, 0x3c02f400, 0xa0401f31, 0x3c041342,
	0x3c0200c0, 0x24050008, 0x3486000c, 0x3487002c, 0x3488004c, 0x34890054, 0x34420702, 0xacc50000,
	0xace50000, 0xad050000, 0x3487101c, 0xad220000, 0x8ce20000, 0x3c05f400, 0x34420007, 0xace20000,
	0x24030002, 0x34841000, 0x24a61f3c, 0x3c02f400, 0xac830000, 0xaca01f3c, 0xacc00004, 0xa0431f32,
	0x3c02f400, 0xa0431f4d, 0x3c02f400, 0xa0431f30, 0x3c02f400, 0xa0401f4f, 0x3c02f400, 0xa0401f60,
	0x3c02f400, 0xa0401f4e, 0x03e00008, 0x00001021, 0x27bdffe0, 0x00002021, 0x00002821, 0xafbf001c,
	0xafb20018, 0xafb10014, 0x0d0003e3, 0xafb00010, 0x00002021, 0x00002821, 0x0d000421, 0x00408821,
	0x24040003, 0x0d0003b4, 0x00409021, 0x3c03f400, 0x24701f3c, 0x00501021, 0x90420000, 0x30420008,
	0x10400015, 0x24040003, 0x0d000438, 0x24050001, 0x3c03f400, 0x8c671f64, 0x3c09f400, 0x3c03f400,
	0x8d241f48, 0x34652d80, 0xaca20000, 0x3c020180, 0x34682d8c, 0x34662d84, 0x34840002, 0x34632d88,
	0x34420702, 0xac670000, 0xad020000, 0xad241f48, 0x09000600, 0xacd20000, 0x0d0003b4, 0x24040004,
	0x00501021, 0x90420004, 0x30420001, 0x10400014, 0x3c10f400, 0x36022d80, 0x24040004, 0x24050001,
	0x0d000438, 0xac510000, 0x3c07f400, 0x3c03f400, 0x8c651f64, 0x8ce41f48, 0x36032d84, 0xac620000,
	0x3c020440, 0x36062d8c, 0x34840002, 0x36102d88, 0x34420702, 0xae050000, 0xacc20000, 0xace41f48,
	0x0d0003b4, 0x00002021, 0x3c03f400, 0x24701f3c, 0x00501021, 0x90420000, 0x30420001, 0x10400012,
	0x00002021, 0x3c10f400, 0x36022e00, 0x24050001, 0x0d000438, 0xac510000, 0x3c05f400, 0x3c03f400,
	0x8c661f64, 0x8ca31f48, 0x36042e08, 0x34630004, 0x36102e04, 0xae020000, 0xac860000, 0xaca31f48,
	0x09000630, 0x00000000, 0x0d0003b4, 0x24040007, 0x00501021, 0x90420004, 0x30420008, 0x10400010,
	0x24040007, 0x0d000438, 0x24050001, 0x3c07f400, 0x3c03f400, 0x8c681f64, 0x8ce51f48, 0x3c03f400,
	0x34662e08, 0x34642e00, 0x34a50004, 0x34632e04, 0xac820000, 0xac720000, 0xacc80000, 0xace51f48,
	0x0d0003b4, 0x24040001, 0x3c03f400, 0x24701f3c, 0x00501021, 0x90420000, 0x30420002, 0x1040000a,
	0x24020004, 0x3c10134d, 0x36030004, 0xac620000, 0x24040001, 0x24020002, 0x24050001, 0xac620000,
	0x09000655, 0x00000000, 0x0d0003b4, 0x24040005, 0x00501021, 0x90420004, 0x30420002, 0x10400022,
	0x8fbf001c, 0x3c10134d, 0x24040005, 0x0d000438, 0x24050001, 0x24020004, 0x36030008, 0xac620000,
	0x36060004, 0x24020002, 0x24040005, 0x00002821, 0xacc20000, 0x0d000438, 0x00000000, 0x3c03f400,
	0x8c681f64, 0x3c09f400, 0x3c03f400, 0x8d261f48, 0x34642d00, 0xac820000, 0x3c020180, 0x34672d0c,
	0x34652d04, 0x36100010, 0x34c60001, 0x34632d08, 0x34420702, 0xacb00000, 0xac680000, 0xace20000,
	0xad261f48, 0x8fbf001c, 0x8fb20018, 0x8fb10014, 0x8fb00010, 0x00001021, 0x03e00008, 0x27bd0020,
	0x3c021342, 0xac440000, 0x34480014, 0x34430004, 0x34470008, 0x3c0204c0, 0x34420702, 0xac650000,
	0xace60000, 0xad020000, 0x03e00008, 0x00001021, 0x3c021342, 0xac440000, 0x34480014, 0x34430004,
	0x34470008, 0x3c020180, 0x34420702, 0xac650000, 0xace60000, 0xad020000, 0x03e00008, 0x00001021,
	0x27bdffe0, 0x3c02f400, 0xafb00010, 0xafbf0018, 0xafb10014, 0x34432fd8, 0x24040001, 0x34422fd4,
	0x8c450000, 0x8c420000, 0x8c620000, 0x0d0003b4, 0x00000000, 0x3c03f400, 0x24701f3c, 0x00501021,
	0x90420000, 0x30420002, 0x1040003f, 0x3c03f400, 0x8c621f50, 0x30420001, 0x1040fffd, 0x3c111342,
	0x36231030, 0x8c620000, 0x30420020, 0x1440fffd, 0x3c04134d, 0x34830184, 0x24020004, 0xac620000,
	0x3482000c, 0x8c420000, 0x00021402, 0x30430003, 0x10600003, 0x3048007f, 0x3102007c, 0x24480004,
	0x3c06f400, 0x90c21f4f, 0x36235000, 0x304200ff, 0x71022802, 0x36270014, 0x00a31021, 0x34830014,
	0x36240004, 0xae230000, 0xac820000, 0x3c0204c0, 0x36250008, 0x34420702, 0xaca80000, 0xace20000,
	0x90c21f4f, 0x3c10f400, 0x24420001, 0x304200ff, 0x00002021, 0x26051f45, 0xa0c21f4f, 0x0d000484,
	0x00000000, 0x3c028000, 0x36230010, 0x34420001, 0xac620000, 0x92021f45, 0x30420001, 0x14400082,
	0x8fbf0018, 0x090006cd, 0x00000000, 0x90421f31, 0x36231034, 0x00021600, 0x344200ff, 0xac620000,
	0x0900074e, 0x3c02134d, 0x0d0003b4, 0x24040005, 0x00501021, 0x90420004, 0x30420002, 0x10400071,
	0x3c03f400, 0x90621f31, 0x3c04f400, 0x24420001, 0x304200ff, 0xa0621f31, 0x8c821f50, 0x30420001,
	0x1040fffd, 0x3c05134d, 0x34a2000c, 0x8c420000, 0x00021402, 0x30430003, 0x10600003, 0x3048007f,
	0x3102007c, 0x24480004, 0x3c06f400, 0x90c31f4f, 0x3c101342, 0x306300ff, 0x71032002, 0x36025000,
	0x00821821, 0x34a50010, 0x36020004, 0xae030000, 0xac450000, 0x3c020180, 0x36040008, 0x36070014,
	0x34420702, 0xac880000, 0xace20000, 0x90c21f4f, 0x3c11f400, 0x24420001, 0x304200ff, 0x00002021,
	0x26251f45, 0xa0c21f4f, 0x0d000484, 0x36100010, 0x3c028000, 0x34420001, 0xae020000, 0x92221f45,
	0x30420001, 0x1040fffd, 0x3c02f400, 0x8c421f50, 0x30420001, 0x1040fffd, 0x3c02f400, 0x3c111342,
	0x36231030, 0x8c620000, 0x30420040, 0x1440fffd, 0x00000000, 0x0d0002a6, 0x00000000, 0x00401821,
	0x30440003, 0x24020001, 0x14820026, 0x24020002, 0x3c02134d, 0x34420084, 0xae220000, 0x00031d82,
	0x3c0204c0, 0x306301fc, 0x36280004, 0x36264c00, 0x36270008, 0x36290014, 0x34420702, 0x3c10f400,
	0xad060000, 0x00002021, 0xace30000, 0x26051f45, 0xad220000, 0x0d000484, 0x00000000, 0x3c028000,
	0x36230010, 0x34420001, 0xac620000, 0x92021f45, 0x30420001, 0x1040fffd, 0x24040005, 0x0d000438,
	0x00002821, 0x3c04e0bd, 0x3484e000, 0x3c05f400, 0x00442021, 0x0d000271, 0x34a52c00, 0x0900074e,
	0x3c02134d, 0x1082ff89, 0x3c02f400, 0x1480ffc7, 0x3c02f400, 0x3c02134d, 0x24030028, 0x34420184,
	0xac430000, 0x8fbf0018, 0x8fb10014, 0x8fb00010, 0x00001021, 0x03e00008, 0x27bd0020, 0x00000000,
	0x27bdffe8, 0xafbf0014, 0xafb00010, 0x3c02f400, 0x8c421f48, 0x30420002, 0x10400020, 0x3c02f400,
	0x3c02f400, 0x34472d8c, 0x344c2d80, 0x34432d84, 0x34462d88, 0x3c02f400, 0x8c421f34, 0x3c101342,
	0x3c05f400, 0x24a51f45, 0x24040001, 0x36080020, 0x36090024, 0x360a0028, 0x1040000f, 0x360b0034,
	0x8c630000, 0x8d820000, 0x8cc60000, 0x8ce70000, 0xad020000, 0xad230000, 0xad460000, 0xad670000,
	0x0d000484, 0x00000000, 0x3c028000, 0x36030030, 0x34420001, 0xac620000, 0x3c02f400, 0x8c421f48,
	0x30420001, 0x1040001f, 0x3c02f400, 0x3c02f400, 0x34472d0c, 0x344b2d00, 0x34432d04, 0x34462d08,
	0x3c02f400, 0x8c421f34, 0x3c101342, 0x3c05f400, 0x24a51f45, 0x00002021, 0x36080004, 0x36090008,
	0x1040000f, 0x360a0014, 0x8c630000, 0x8d620000, 0x8cc60000, 0x8ce70000, 0xae020000, 0xad030000,
	0xad260000, 0xad470000, 0x0d000484, 0x00000000, 0x3c028000, 0x36030010, 0x34420001, 0xac620000,
	0x3c02f400, 0x8c421f48, 0x30420004, 0x1040001a, 0x3c02f400, 0x34462e08, 0x344a2e00, 0x34432e04,
	0x3c02f400, 0x8c421f34, 0x3c101342, 0x3c05f400, 0x24a51f45, 0x36070040, 0x36080044, 0x36090048,
	0x1040000d, 0x24040002, 0x8c630000, 0x8d420000, 0x8cc60000, 0xace20000, 0xad030000, 0xad260000,
	0x0d000484, 0x00000000, 0x3c028000, 0x36030050, 0x34420001, 0xac620000, 0x3c02f400, 0xac401f48,
	0x8fbf0014, 0x8fb00010, 0x00001021, 0x03e00008, 0x27bd0018, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000
};
