
#ifndef MIPS_INST_H
#define MIPS_INST_H

#define INST_LUI(reg,val)				\
	( (15<<26) | ((reg)<<16) | ((val) & 0xffff) )

#define INST_ORI(reg,val)						\
	( (13<<26) | ((reg)<<21) | ((reg)<<16) | ((val) & 0xffff) )

#define INST_JR(reg)				\
	( (((reg)&0x1f)<<21) + 8 )

#define INST_ADDIU(rd,rs,imm)						\
	( (9<<26) | ((rs)<<21) | ((rd)<<16) | ((imm) & 0xffff) )

#define INST_SW(rt,off,rb)						\
	( (43<<26) | ((rb)<<21) | ((rt)<<16) | ((off) & 0xffff) )

#define INST_LW(rt,off,rb)						\
	( (35<<26) | ((rb)<<21) | ((rt)<<16) | ((off) & 0xffff) )

#define INST_NOP	((unsigned)0)

#endif
