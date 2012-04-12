/* $Id: instructions.h,v 1.1 2012/03/27 07:44:59 qzhou Exp $ */
#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

// B_TYPE-->Jump type
// R3_TYPE three reigster operation
// R2_TYPE two reigster operation
// I_TYPE  immediate operand type
// S_TYPE  shift amount  TYPE
#define R1_TYPE "%0"
#define R2_TYPE "%0,%1"
#define R3_TYPE	"%0,%1,%2"
#define MEM_TYPE "%0,%2(%1)"

#define B_TYPE	R1_TYPE
#define B2_TYPE R2_TYPE
#define B3_TYPE	R3_TYPE

#define TI_TYPE	R2_TYPE
#define M_TYPE	R2_TYPE

#define	I_TYPE	R3_TYPE 
#define S_TYPE	R3_TYPE
#define T_TYPE	R3_TYPE

//#define C_U21 ""
//#define C_S16 "L"
//#define C_0 "O"
#define C_S16 "X"  /* instead of "K" */
#define C_U16 "X"  /* "X" is used for all constants */
#define C_S5  "i"
#define C_U5  "i"
#define C_S10 "i"
#define C_U10 "i"

#define i_ehb

/* ALU Instructions */
#ifdef TEST_ADD_EXP
#define i_add(src0,src1)                                                                	\
(	{unsigned int _dst_;                                                                  	\
  	__asm__ __volatile__("add\t"R3_TYPE"\t#i_add":"=&r"(_dst_):"r" (src0), "r" (src1));    	\
 	_dst_;})										
#else
#define i_add(src0,src1)                                                                	\
(	{unsigned int _dst_;                                                                  	\
  	__asm__ __volatile__("add\t"R3_TYPE"\t#i_add":"=r"(_dst_):"r" (src0), "r" (src1));    	\
 	_dst_;})										
#endif

#define i_addu(src0,src1)                                                               	\
(	{unsigned int _dst_;                                                                  	\
	__asm__ __volatile__("addu\t"R3_TYPE"\t#i_addu":"=r"(_dst_) :"r" (src0), "r" (src1));  	\
 	_dst_;})

#ifdef TEST_SUB_EXP
#define i_sub(src0,src1)                                                                	\
(	{unsigned int _dst_;                                                                  	\
  	__asm__ __volatile__("sub\t"R3_TYPE"\t#i_sub":"=&r"(_dst_):"r" (src0), "r" (src1));    	\
 	_dst_;})
#else
#define i_sub(src0,src1)                                                                	\
(	{unsigned int _dst_;                                                                  	\
  	__asm__ __volatile__("sub\t"R3_TYPE"\t#i_sub":"=r"(_dst_):"r" (src0), "r" (src1));    	\
 	_dst_;})
#endif

#define i_subu(src0,src1)                                                               	\
(	{unsigned int _dst_;                                                                  	\
	__asm__ __volatile__("subu\t"R3_TYPE"\t#i_subu":"=r"(_dst_) :"r" (src0), "r" (src1));  	\
 	_dst_;})

#ifdef TEST_ADDI_EXP
#define i_addi(src0,src1)                                                               	\
(	{unsigned int _dst_;                                                                  	\
   	__asm__ __volatile__("addi\t"I_TYPE"\t#i_addi":"=&r"(_dst_) :"r" (src0),C_S16 (src1));	\
 	_dst_;})
#else
#define i_addi(src0,src1)                                                               	\
(	{unsigned int _dst_;                                                                  	\
   	__asm__ __volatile__("addi\t"I_TYPE"\t#i_addi":"=r"(_dst_) :"r" (src0),C_S16 (src1));	\
 	_dst_;})
#endif

#define i_addiu(src0,src1)                                                               	\
(	{unsigned int _dst_;                                                                  	\
   	__asm__ __volatile__("addiu\t"I_TYPE"\t#i_addiu":"=r"(_dst_) :"r" (src0),C_S16(src1));	\
 	_dst_;})
/*Logical Instruction*/
#define i_and(src0,src1)									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("and\t"R3_TYPE"\t#i_and":"=r"(_dst_):"r"(src0),"r"(src1));		\
	_dst_;})

#define i_or(src0,src1)										\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("or \t"R3_TYPE"\t#i_or ":"=r"(_dst_):"r"(src0),"r"(src1));		\
	_dst_;})

#define i_xor(src0,src1)									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("xor\t"R3_TYPE"\t#i_xor":"=r"(_dst_):"r"(src0),"r"(src1));		\
	_dst_;})

#define i_nor(src0,src1)									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("nor\t"R3_TYPE"\t#i_nor":"=r"(_dst_):"r"(src0),"r"(src1));		\
	_dst_;})

#define i_andi(src0,src1)									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("andi\t"I_TYPE"\t#i_andi":"=r"(_dst_):"r"(src0),C_S16(src1));	\
	_dst_;})

#define i_ori(src0,src1)									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("ori \t"I_TYPE"\t#i_ori ":"=r"(_dst_):"r"(src0),C_S16(src1));	\
	_dst_;})

#define i_xori(src0,src1)									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("xori\t"I_TYPE"\t#i_xori":"=r"(_dst_):"r"(src0),C_S16(src1));	\
	_dst_;})
/*Shift Logical*/
#define i_sll(src0,src1)                                                               		\
(	{unsigned int _dst_;                                                                  	\
   	__asm__ __volatile__("sll\t"S_TYPE"\t#i_sll":"=r"(_dst_) :"r" (src0), C_U5 (src1));	\
 	_dst_;})

#define i_srl(src0,src1)                                                               		\
(	{unsigned int _dst_;                                                                  	\
   	__asm__ __volatile__("srl\t"S_TYPE"\t#i_srl":"=r"(_dst_) :"r" (src0), C_U5 (src1));	\
 	_dst_;})

#define i_sra(src0,src1)                                                               		\
(	{unsigned int _dst_;                                                                  	\
   	__asm__ __volatile__("sra\t"S_TYPE"\t#i_sra":"=r"(_dst_) :"r" (src0), C_U5 (src1));	\
 	_dst_;})

#define i_sllv(src0,src1)                                                                	\
(	{unsigned int _dst_;                                                                  	\
   	__asm__ __volatile__("sllv\t"R3_TYPE"\t#i_sllv":"=r"(_dst_) :"r" (src0),"r"(src1));	\
 	_dst_;})

#define i_srlv(src0,src1)                                                                	\
(	{unsigned int _dst_;                                                               	\
   	__asm__ __volatile__("srlv\t"R3_TYPE"\t#i_srlv":"=r"(_dst_) :"r" (src0),"r" (src1));   	\
 	_dst_;})

#define i_srav(src0,src1)                                                                	\
(	{unsigned int _dst_;                                                               	\
   	__asm__ __volatile__("srav\t"R3_TYPE"\t#i_srav":"=r"(_dst_) :"r" (src0),"r" (src1));   	\
 	_dst_;})

#define i_sarv(src0,src1) i_srav(src0,src1)      
#define i_sar(src0,src1)  i_sra(src0,src1)      
#define i_slrv(src0,src1) i_srlv(src0,src1)      
#define i_slr(src0,src1)  i_srl(src0,src1)      

#define i_slli(src0,src1)   i_sll(src0, src1)
#define i_slri(src0,src1)   i_srl(src0, src1)
#define i_sari(src0,src1)   i_sra(src0, src1)

/*Compare Instruction */
#define i_slt(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("slt\t"R3_TYPE"\t#i_slt":"=r"(_dst_):"r"(src0),"r"(src1)); 	\
	_dst_;})

#define i_sltu(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("sltu\t"R3_TYPE"\t#i_sltu":"=r"(_dst_):"r"(src0),"r"(src1)); 	\
	_dst_;})

#define i_slti(src0,src1)									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("slti\t"I_TYPE"\t#i_slti":"=r"(_dst_):"r"(src0),C_S16(src1));	\
	_dst_;})

#define i_sltiu(src0,src1)									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("sltiu\t"I_TYPE"\t#i_sltiu":"=r"(_dst_):"r"(src0),C_S16(src1));	\
	_dst_;})
/* Move Instruction */
#ifndef TEST_MOVZ
#define i_movz(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("movz\t"R3_TYPE"\t#i_movz":"=&r"(_dst_):"r"(src0),"r"(src1)); 	\
	_dst_;})
#else
#define i_movz(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("add %0,$0,%3;movz\t"R3_TYPE"\t#i_movz":				\
	"=&r"(_dst_):"r"(src0),"r"(src1),"r"(0x3c3c3c3c));					\
	_dst_;})
#endif

#ifndef TEST_MOVN
#define i_movn(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("movn\t"R3_TYPE"\t#i_movn":"=&r"(_dst_):"r"(src0),"r"(src1)); 	\
	_dst_;})
#else
#define i_movn(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("add %0,$0,%3;movn\t"R3_TYPE"\t#i_movn":				\
	"=&r"(_dst_):"r"(src0),"r"(src1),"r"(0x3c3c3c3c));					\
	_dst_;})
#endif
/*Multiply and Division Instruction*/
#define i_mul(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("mul\t"R3_TYPE"\t#i_mul":"=r"(_dst_):"r"(src0),"r"(src1)); 	\
	_dst_;})

#define i_mult(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("mult\t"R2_TYPE"\t#i_mult"::"r"(src0),"r"(src1));    	\
	/*_dst_;*/})

#define i_multu(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("multu\t"M_TYPE"\t#i_multu"::"r"(src0),"r"(src1));    	\
	/*_dst_;*/})

#define i_madd(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("madd\t"M_TYPE"\t#i_madd"::"r"(src0),"r"(src1));    	\
	/*_dst_;*/})

#define i_maddu(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("maddu\t"M_TYPE"\t#i_maddu"::"r"(src0),"r"(src1));    	\
	/*_dst_;*/})

#define i_msub(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("msub\t"M_TYPE"\t#i_msub"::"r"(src0),"r"(src1));    	\
	/*_dst_;*/})

#define i_msubu(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("msubu\t"M_TYPE"\t#i_msubu"::"r"(src0),"r"(src1));    	\
	/*_dst_;*/})

/* Important: Actually, assembler will treat this as a macro inst, and expand it 
   into more than 1 inst. To generate one inst, use i_div0 please. */
#define i_div(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("div\t"M_TYPE"\t#i_div"::"r"(src0),"r"(src1));    		\
	/*_dst_;*/})

#define i_div0(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("div\t$0,"M_TYPE"\t#i_div"::"r"(src0),"r"(src1));    	\
	/*_dst_;*/})

/* Important: Actually, assembler will treat this as a macro inst, and expand it 
   into more than 1 inst. To generate one inst, use i_divu0 please. */
#define i_divu(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("divu\t"M_TYPE"\t#i_divu"::"r"(src0),"r"(src1));    	\
	/*_dst_;*/})

#define i_divu0(src0,src1)  								\
(	{/*unsigned int _dst_;*/							\
	__asm__ __volatile__("divu\t$0,"M_TYPE"\t#i_divu"::"r"(src0),"r"(src1));    	\
	/*_dst_;*/})

#define i_clo(src)                                                              	\
(	{unsigned int __dst__;                                                  	\
   	__asm__ __volatile__("clo\t"R2_TYPE"\t#i_clo":"=r"(__dst__) :"r" (src));       	\
 	__dst__;})

#define i_clz(src)                                                              	\
(	{unsigned int __dst__;                                                  	\
   	__asm__ __volatile__("clz\t"R2_TYPE"\t#i_clz":"=r"(__dst__) :"r" (src));       	\
 	__dst__;})

#define i_mfhi()									\
(	{unsigned int __dst__;								\
	__asm__	__volatile__("mfhi\t%0\t#i_mfhi":"=r"(__dst__));			\
	__dst__;})

#define i_mflo()									\
(	{unsigned int __dst__;								\
	__asm__	__volatile__("mflo\t%0\t#i_mflo":"=r"(__dst__));			\
	__dst__;})

#define i_mthi(src)									\
({ __asm__ __volatile__("mthi\t%0\t#i_mthi"::"r"(src));})

#define i_mtlo(src)									\
({ __asm__ __volatile__("mtlo\t%0\t#i_mtlo"::"r"(src));})

#define i_lui(src)                                               			\
({  	unsigned int _dst_;                           					\
	__asm__ __volatile__("lui\t"R2_TYPE"\t#i_lui":"=r"(_dst_) :C_S16 (src)); 	\
	_dst_;})

/* Macro instruction "la reg,sym" */
#define i_la(src)                                               			\
({  	unsigned int _dst_;                           					\
	__asm__ __volatile__("la\t"R2_TYPE"\t#i_la macro":"=r"(_dst_) :"X" (src));      \
	_dst_;})

/*Trap trap TRAP*/
#define i_teq(src0,src1,src2)	\
({__asm__ __volatile__("teq\t"T_TYPE"\t#i_teq"::"r"(src0),"r"(src1),C_U10(src2));})

#define i_tne(src0,src1,src2)	\
({__asm__ __volatile__("tne\t"T_TYPE"\t#i_tne"::"r"(src0),"r"(src1),C_U10(src2));})

#define i_tge(src0,src1,src2)	\
({__asm__ __volatile__("tge\t"T_TYPE"\t#i_tge"::"r"(src0),"r"(src1),C_U10(src2));})

#define i_tgeu(src0,src1,src2)	\
({__asm__ __volatile__("tgeu\t"T_TYPE"\t#i_tgeu"::"r"(src0),"r"(src1),C_U10(src2));})

#define i_tlt(src0,src1,src2)	\
({__asm__ __volatile__("tlt\t"T_TYPE"\t#i_tlt"::"r"(src0),"r"(src1),C_U10(src2));})

#define i_tltu(src0,src1,src2)	\
({__asm__ __volatile__("tltu\t"T_TYPE"\t#i_tltu"::"r"(src0),"r"(src1),C_U10(src2));})

#define i_teqi(src0,src1)	\
({__asm__ __volatile__("teqi\t"TI_TYPE"\t#i_teqi"::"r"(src0),C_S16(src1));})

#define i_tnei(src0,src1)	\
({__asm__ __volatile__("tnei\t"TI_TYPE"\t#i_tnei"::"r"(src0),C_S16(src1));})

#define i_tgei(src0,src1)	\
({__asm__ __volatile__("tgei\t"TI_TYPE"\t#i_tgei"::"r"(src0),C_S16(src1));})

#define i_tgeiu(src0,src1)	\
({__asm__ __volatile__("tgeiu\t"TI_TYPE"\t#i_tgeiu"::"r"(src0),C_S16(src1));})

#define i_tlti(src0,src1)	\
({__asm__ __volatile__("tlti\t"TI_TYPE"\t#i_tlti"::"r"(src0),C_S16(src1));})

#define i_tltiu(src0,src1)	\
({__asm__ __volatile__("tltiu\t"TI_TYPE"\t#i_tltiu"::"r"(src0),C_S16(src1));})


#define i_bgez(src0,offset)	\
({ __asm__ __volatile__("bgez\t"B2_TYPE"\t#i_bgez"::"r"(src0),C_S16(offset):"memory");})

#define i_bgezl(src0,offset)	\
({ __asm__ __volatile__("bgezl\t"B2_TYPE"\t#i_bgezl"::"r"(src0),C_S16(offset):"memory");})

#define i_bgtz(src0,offset)	\
({ __asm__ __volatile__("bgtz\t"B2_TYPE"\t#i_bgtz"::"r"(src0),C_S16(offset):"memory");})

#define i_bgtzl(src0,offset)	\
({ __asm__ __volatile__("bgtzl\t"B2_TYPE"\t#i_bgtzl"::"r"(src0),C_S16(offset):"memory");})

#define i_blez(src0,offset)	\
({ __asm__ __volatile__("blez\t"B2_TYPE"\t#i_blez"::"r"(src0),C_S16(offset):"memory");})

#define i_blezl(src0,offset)	\
({ __asm__ __volatile__("blezl\t"B2_TYPE"\t#i_blezl"::"r"(src0),C_S16(offset):"memory");})

#define i_bltz(src0,offset)	\
({ __asm__ __volatile__("bltz\t"B2_TYPE"\t#i_bltz"::"r"(src0),C_S16(offset):"memory");})

#define i_bltzl(src0,offset)	\
({ __asm__ __volatile__("bltzl\t"B2_TYPE"\t#i_bltzl"::"r"(src0),C_S16(offset):"memory");})

#define i_beq(src0,src1,offset)	\
({ __asm__ __volatile__("beq\t"B3_TYPE"\t#i_beq"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_bne(src0,src1,offset)	\
({ __asm__ __volatile__("bne\t"B3_TYPE"\t#i_bne"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_beql(src0,src1,offset)	\
({ __asm__ __volatile__("beql\t"B3_TYPE"\t#i_beql"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_bnel(src0,src1,offset)	\
({ __asm__ __volatile__("bnel\t"B3_TYPE"\t#i_bnel"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_beqz(src0,offset)	\
({ __asm__ __volatile__("beqz\t"B2_TYPE"\t#i_beqz"::"r"(src0),C_S16(offset):"memory");})

#define i_bnez(src0,offset)	\
({ __asm__ __volatile__("bnez\t"B2_TYPE"\t#i_bnez"::"r"(src0),C_S16(offset):"memory");})

#define i_bgezal(src0,offset)	\
({ __asm__ __volatile__("bgezal\t"B2_TYPE"\t#i_bgezal"::"r"(src0),C_S16(offset):"memory");})

#define i_bgezall(src0,offset)	\
({ __asm__ __volatile__("bgezall\t"B2_TYPE"\t#i_bgezall"::"r"(src0),C_S16(offset):"memory");})

#define i_bltzal(src0,offset)	\
({ __asm__ __volatile__("bltzal\t"B2_TYPE"\t#i_bltzal"::"r"(src0),C_S16(offset):"memory");})

#define i_bltzall(src0,offset)	\
({ __asm__ __volatile__("bltzall\t"B2_TYPE"\t#i_bltzall"::"r"(src0),C_S16(offset):"memory");})

#define i_b(offset) \
({ __asm__ __volatile__("b\t"B_TYPE"\t#i_b"::C_S16(offset):"memory");})

#define i_bal(offset) \
({ __asm__ __volatile__("bal\t"B_TYPE"\t#i_bal"::C_S16(offset):"memory");})

#define i_j(label) \
({ __asm__ __volatile__("j\t"B_TYPE"\t#i_j"::"X"(label):"memory"); })

#define i_jal(label) \
({ __asm__ __volatile__("jal\t"B_TYPE"\t#i_jal"::"X"(label):"memory"); })

#define i_jr(src0) \
({ __asm__ __volatile__("jr\t"R1_TYPE"\t#i_jr"::"r"(src0):"memory"); })

#define i_jalr(src0) \
({ __asm__ __volatile__("jalr\t"R1_TYPE"\t#i_jalr"::"r"(src0):"memory"); })

#define i_jalr_reg(dst,src0) \
({ __asm__ __volatile__("jalr\t"R2_TYPE"\t#i_jalr":"=r"(dst):"r"(src0):"memory"); })

#define i_jump(label)    i_j(label)

#define i_ja(_dst_, src0, src1)\
({\
  unsigned int _tmp_;\
  _tmp_ = src0 + src1;\
  i_jalr_reg(_dst_, _tmp_);\
})

#define i_call(src)      i_jalr(src)


/*Load and Store */
#define i_lb(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("lb\t"MEM_TYPE"\t#i_lb":"=r"(_dst_):"r"(src0),C_S16(src1)); 	\
	_dst_;})

#define i_lbu(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("lbu\t"MEM_TYPE"\t#i_lbu":"=r"(_dst_):"r"(src0),C_S16(src1)); 	\
	_dst_;})

#define i_lh(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("lh\t"MEM_TYPE"\t#i_lh":"=r"(_dst_):"r"(src0),C_S16(src1)); 	\
	_dst_;})

#define i_lhu(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("lhu\t"MEM_TYPE"\t#i_lhu":"=r"(_dst_):"r"(src0),C_S16(src1)); 	\
	_dst_;})

#define i_lw(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("lw\t"MEM_TYPE"\t#i_lw":"=r"(_dst_):"r"(src0),C_S16(src1)); 	\
	_dst_;})

#define i_ll(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("ll\t"MEM_TYPE"\t#i_ll":"=r"(_dst_):"r"(src0),C_S16(src1)); 	\
	_dst_;})

#define i_lwl(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("lwl\t"MEM_TYPE"\t#i_lwl":"=r"(_dst_):"r"(src0),C_S16(src1)); 	\
	_dst_;})

#define i_lwr(src0,src1)  									\
(	{unsigned int _dst_;									\
	__asm__ __volatile__("lwr\t"MEM_TYPE"\t#i_lwr":"=r"(_dst_):"r"(src0),C_S16(src1)); 	\
	_dst_;})

#define i_sb(src0,src1,offset)	\
(	{ __asm__ __volatile__("sb\t"MEM_TYPE"\t#i_sb"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_sh(src0,src1,offset)	\
(	{ __asm__ __volatile__("sh\t"MEM_TYPE"\t#i_sh"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_sw(src0,src1,offset)	\
(	{ __asm__ __volatile__("sw\t"MEM_TYPE"\t#i_sw"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_sc(src0,src1,offset)	\
(	{ __asm__ __volatile__("sc\t"MEM_TYPE"\t#i_sc"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_swl(src0,src1,offset)	\
(	{ __asm__ __volatile__("swl\t"MEM_TYPE"\t#i_swl"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_swr(src0,src1,offset)	\
(	{ __asm__ __volatile__("swr\t"MEM_TYPE"\t#i_swr"::"r"(src0),"r"(src1),C_S16(offset):"memory");})

#define i_l8(src0, src1) 	i_lb(src0, src1)
#define i_l8u(src0, src1) 	i_lbu(src0, src1)
#define i_l16(src0, src1)	i_lh(src0, src1)
#define i_l16u(src0, src1)	i_lhu(src0, src1)
#define i_l32(src0, src1)	i_lw(src0, src1)
#define i_s8(src0,src1,src2)	i_sb(src0,src1,src2)
#define i_s16(src0,src1,src2)	i_sh(src0,src1,src2)
#define i_s32(src0,src1,src2)	i_sw(src0,src1,src2)

#define i_swap(src0,src1,src2)			\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_lw(src1, src2);			\
  i_sw(src0, src1, src2);			\
  src0 = _tmp_;					\
})

#define i_lx8(src0, src1)			\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_addu(src0, src1);				\
  i_lb(_tmp_, 0);				\
})

#define i_lx8u(src0, src1)			\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_addu(src0, src1);				\
  i_lbu(_tmp_, 0);				\
})

#define i_lx16(src0, src1)			\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_addu(src0, src1);				\
  i_lh(_tmp_, 0);				\
})

#define i_lx16u(src0, src1)			\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_addu(src0, src1);				\
  i_lhu(_tmp_, 0);				\
})

#define i_lx32(src0, src1)			\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_addu(src0, src1);				\
  i_lw(_tmp_, 0);				\
})

#define i_sx8(src0,src1,src2)						\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_addu(src1, src2);				\
  i_sb(src0, _tmp_, 0);				\
})

#define i_sx16(src0,src1,src2)						\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_addu(src1, src2);				\
  i_sh(src0, _tmp_, 0);				\
})

#define i_sx32(src0,src1,src2)						\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_addu(src1, src2);				\
  i_sw(src0, _tmp_, 0);				\
})

#define i_lx16s(src0, src1)			\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_sll(src1, 1);\
  _tmp_ = i_addu(src0, _tmp_);				\
  i_lh(_tmp_, 0);				\
})

#define i_lx16su(src0, src1)			\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_sll(src1, 1);\
  _tmp_ = i_addu(src0, _tmp_);				\
  i_lhu(_tmp_, 0);				\
})

#define i_lx32s(src0, src1)			\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_sll(src1, 2);\
  _tmp_ = i_addu(src0, _tmp_);				\
  i_lw(_tmp_, 0);				\
})

#define i_sx16s(src0,src1,src2)						\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_sll(src2, 1);\
  _tmp_ = i_addu(src1, _tmp_);				\
  i_sh(src0, _tmp_, 0);				\
})

#define i_sx32s(src0,src1,src2)						\
({						\
  unsigned int _tmp_;				\
  _tmp_ = i_sll(src2, 2);\
  _tmp_ = i_addu(src1, _tmp_);				\
  i_sw(src0, _tmp_, 0);				\
})

/*System Instruction*/
#define i_mtc0(src0,src1)	\
({ __asm__ __volatile__ ("mtc0\t%0,$%1"::"r"(src0),"i"(src1));})

#define i_mfc0(src0)							\
(	{unsigned int _dst_; 						\
 	__asm__ __volatile__ ("mfc0\t%0,$%1":"=r"(_dst_):"i"(src0));	\
	_dst_;})	

#define i_mtc0_2(src0,src1,src2)	\
({ __asm__ __volatile__ ("mtc0\t%0,$%1,%2"::"r"(src0),"i"(src1),"i"(src2));})

#define i_mfc0_2(src0,src1)								\
(	{unsigned int _dst_; 								\
 	__asm__ __volatile__ ("mfc0\t%0,$%1,%2":"=r"(_dst_):"i"(src0),"i"(src1));	\
	_dst_;})	

// test for CpU
#define i_mtc1(src0,src1)	\
({ __asm__ __volatile__ ("mtc1\t%0,$%1"::"r"(src0),"i"(src1));})

#define i_mfc1(src0)							\
(	{unsigned int _dst_; 						\
 	__asm__ __volatile__ ("mfc1\t%0,$%1":"=r"(_dst_):"i"(src0));	\
	_dst_;})	

#define i_mtc2(src0,src1)	\
({ __asm__ __volatile__ ("mtc2\t%0,$%1"::"r"(src0),"i"(src1));})

#define i_mfc2(src0)							\
(	{unsigned int _dst_; 						\
 	__asm__ __volatile__ ("mfc2\t%0,$%1":"=r"(_dst_):"i"(src0));	\
	_dst_;})	

#define i_mtc3(src0,src1)	\
({ __asm__ __volatile__ ("mtc3\t%0,$%1"::"r"(src0),"i"(src1));})

#define i_mfc3(src0)							\
(	{unsigned int _dst_; 						\
 	__asm__ __volatile__ ("mfc3\t%0,$%1":"=r"(_dst_):"i"(src0));	\
	_dst_;})	


#define i_cache(opcode,base,offset)	\
({ __asm__ __volatile__("cache %0,%2(%1)"::"i"(opcode),"r"(base),"i"(offset):"memory");})

#define i_pref(hint,base,offset)	\
({ __asm__ __volatile__("pref %0,%2(%1)"::"i"(hint),"r"(base),"i"(offset):"memory");})

#define i_sdbbp(code) ({__asm__ __volatile__("sdbbp %0"::"X"(code):"memory");})
#define i_sync() ({__asm__ __volatile__("sync":::"memory");})
#define i_syscall(code) ({__asm__ __volatile__("syscall %0"::"X"(code):"memory");})

#define i_break() ({__asm__ __volatile__("break\t#i_break":::"memory");})
#define i_ssnop() ({__asm__ __volatile__("ssnop\t#i_ssnop":::"memory");})
#define i_deret() ({__asm__ __volatile__("deret\t#i_deret":::"memory");})
#define i_tlbwi() ({__asm__ __volatile__("tlbwi\t#i_tlbwi":::"memory");})
#define i_tlbwr() ({__asm__ __volatile__("tlbwr\t#i_tlbwr":::"memory");})
#define i_tlbp() ({__asm__ __volatile__("tlbp\t#i_tlbp":::"memory");})
#define i_tlbr() ({__asm__ __volatile__("tlbr\t#i_tlbr":::"memory");})
#define i_eret() ({__asm__ __volatile__("eret\t#i_eret":::"memory");})
#define i_rte   ({__asm__ __volatile__("eret\t#i_eret":::"memory");})
#define i_wait() ({__asm__ __volatile__("wait\t#i_wait":::"memory");})
#define i_nop  ({__asm__ __volatile__("nop\t#i_nop":::"memory");})
#define i_reserve() ({__asm__ __volatile__(".word 0x60010101":::"memory");})
#define i_sleep	i_wait()

#define i_set(_dst_) ({__asm__ __volatile__("#set %0":"=r"(_dst_)::"memory"); })
#define i_use(src) ({__asm__ __volatile__("#use %0"::"r"(src):"memory"); })
#define i_set_use(src__dst_) ({__asm__ __volatile__("#set & use %0":"+r"(src__dst_)::"memory"); })
#define i_label(id) ({  __asm__ __volatile__(".LL%0:"::C_U5(id)); })
#define i_mark(id) ({  __asm__ __volatile__("MARK%0:"::"X"(id)); })
#define i_align(num) ({  __asm__ __volatile__(".align\t%0"::C_U5(num)); })
#define i_long(op) ({ __asm__ __volatile__(".long\t%0"::"X"(op));}) 
#define i_long0(x) ({ __asm__ __volatile__(".long\t%0"::"X"(x));}) 
#define i_org(num) ({  __asm__ __volatile__(".org\t%0"::"X"(num)); })
#define i_comm 	   ({ __asm__ __volatile__("#comment"::);  })
#define i_barrier  ({  __asm__ __volatile__("":::"memory"); })
#define i_noreorder  ({  __asm__ __volatile__(".set noreorder"); })
#define i_reorder  ({  __asm__ __volatile__(".set reorder"); })
#define i_nomacro  ({  __asm__ __volatile__(".set nomacro"); })
#define i_macro  ({  __asm__ __volatile__(".set macro"); })

#ifdef MAU_V2
#define i_qld32(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qld32\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}
#define i_qlp32(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qlp32\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}
#define i_qsd32(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qsd32\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}
#define i_qsp32(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qsp32\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}

#define i_qmul(a,b,c,d,e)\
	__asm__	("qmul\t"#a","#b","#c","#d","#e"")
#define i_qpkmull(a,b,c,d)\
	__asm__	("qpkmull\t"#a","#b","#c","#d"")
#define i_qpkmulh(a,b,c,d)\
	__asm__	("qpkmulh\t"#a","#b","#c","#d"")
#define i_qmac(a,b,c,d,e,f)\
	__asm__	("qmac\t"#a","#b","#c","#d","#e","#f"")
#define i_qpkmacl(a,b,c,d,e,f)\
	__asm__	("qpkmacl\t"#a","#b","#c","#d","#e","#f"")
#define i_qpkmach(a,b,c,d,e,f)\
	__asm__	("qpkmach\t"#a","#b","#c","#d","#e","#f"")

#define i_qupka8(a,b,c,d)\
	__asm__	("qupka8\t"#a","#b","#c","#d"")
#define i_qupks8(a,b,c,d)\
	__asm__	("qupks8\t"#a","#b","#c","#d"")
#define i_qsum16(a,b,c,d)\
	__asm__	("qsum16\t"#a","#b","#c","#d"")
#define i_qpk16(a,b,c)\
	__asm__	("qpk16\t"#a","#b","#c"")

#define i_qavg8u(a,b,c)\
	__asm__	("qavg8u\t"#a","#b","#c"")
#define i_qavgr8u(a,b,c)\
	__asm__	("qavg8ru\t"#a","#b","#c"")

#define i_qsada8(a,b,c,d)\
	__asm__	("qsada8\t"#a","#b","#c","#d"")

#define i_qmax16(a,b,c)\
	__asm__	("qmax16\t"#a","#b","#c"")
#define i_qmin16(a,b,c)\
	__asm__	("qmin16\t"#a","#b","#c"")

#define i_qmix(a,b,c,d,e)\
	__asm__	("qmix\t"#a","#b","#c","#d","#e"")

#define i_qcps16(a,b,c)\
	__asm__	("qcps16\t"#a","#b","#c"")

#define i_qsll16(a,b,c)\
	__asm__	("qsll16\t"#a","#b","#c"")
#define i_qslr16(a,b,c)\
	__asm__	("qslr16\t"#a","#b","#c"")
#define i_qsar16(a,b,c)\
	__asm__	("qsar16\t"#a","#b","#c"")

#define i_qalign(a,b,c,d)\
	__asm__	("qalign\t"#a","#b","#c",%0"::"r"(d))

#else // MAU_V2

/************************ QLD instructions ******************/
/*
	valid data:
		qreg:		qr0-->qr7,gd0,gd1,scr
		base_addr:	any constant or int variable
		offset:		signed 10bits constant
*/
#define i_qld32(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qld32\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}

#define i_qld16h(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qld16h\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}

#define i_qld16l(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qld16l\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}
/************************ QLP instructions ******************/
#define i_qlp32(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qlp32\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}

#define i_qlp16h(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qlp16h\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}

#define i_qlp16l(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qlp16l\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}
/************************ QSD instructions ******************/
#define i_qsd32(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qsd32\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}

#define i_qsd16h(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qsd16h\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}

#define i_qsd16l(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qsd16l\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}
/************************ QSP instructions ******************/
#define i_qsp32(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qsp32\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}

#define i_qsp16h(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qsp16h\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}

#define i_qsp16l(qreg,base_addr,offset)	\
{\
	__asm__ __volatile__("qsp16l\t"#qreg",[%0,%1]"::"r"(base_addr),"K"(offset));\
}    

/************************ QMUL instructions ******************/
#define i_qmul(a,b,c,d,e)\
	__asm__	("qmul\t"#a","#b","#c","#d","#e"")
#define i_qmulr(a,b,c,d,e)\
	__asm__	("qmulr\t"#a","#b","#c","#d","#e"")
#define i_qpkmulh(a,b,c,d)\
	__asm__	("qpkmulh\t"#a","#b","#c","#d"")
#define i_qpkmull(a,b,c,d)\
	__asm__	("qpkmull\t"#a","#b","#c","#d"")
#define i_qpkmulrh(a,b,c,d)\
	__asm__	("qpkmulrh\t"#a","#b","#c","#d"")
#define i_qpkmulrl(a,b,c,d)\
	__asm__	("qpkmulrl\t"#a","#b","#c","#d"")
/************************ QMAC instructions ******************/
#define i_qmac(a,b,c,d,e)\
	__asm__	("qmac\t"#a","#b","#c","#d","#e"")
#define i_qmacr(a,b,c,d,e)\
	__asm__	("qmacr\t"#a","#b","#c","#d","#e"")
#define i_qmac_d(a,b,c,d,e,f)\
	__asm__	("qmac\t"#a","#b","#c","#d","#e","#f"")
#define i_qmacr_d(a,b,c,d,e,f)\
	__asm__	("qmacr\t"#a","#b","#c","#d","#e","#f"")
#define i_qpkmach(a,b,c,d,e,f)\
	__asm__	("qpkmach\t"#a","#b","#c","#d","#e","#f"")
#define i_qpkmacl(a,b,c,d,e,f)\
	__asm__	("qpkmacl\t"#a","#b","#c","#d","#e","#f"")
#define i_qpkmacrh(a,b,c,d,e,f)\
	__asm__	("qpkmacrh\t"#a","#b","#c","#d","#e","#f"")
#define i_qpkmacrl(a,b,c,d,e,f)\
	__asm__	("qpkmacrl\t"#a","#b","#c","#d","#e","#f"")
	
/************************ QSUM instructions ******************/
#define i_qsum8u(a,b,c,d)\
	__asm__	("qsum8u\t"#a","#b","#c","#d"")
#define i_qsum8u_d(a,b,c,d,e)\
	__asm__	("qsum8u\t"#a","#b","#c","#d","#e"")
#define i_qsum8(a,b,c,d)\
	__asm__	("qsum8\t"#a","#b","#c","#d"")
#define i_qsum8_d(a,b,c,d,e)\
	__asm__	("qsum8\t"#a","#b","#c","#d","#e"")
#define i_qsum16u(a,b,c,d)\
	__asm__	("qsum16u\t"#a","#b","#c","#d"")
#define i_qsum16u_d(a,b,c,d,e)\
	__asm__	("qsum16u\t"#a","#b","#c","#d","#e"")
#define i_qsum16(a,b,c,d)\
	__asm__	("qsum16\t"#a","#b","#c","#d"")
#define i_qsum16_d(a,b,c,d,e)\
	__asm__	("qsum16\t"#a","#b","#c","#d","#e"")
#define i_qsum32(a,b,c,d)\
	__asm__	("qsum32\t"#a","#b","#c","#d"")
#define i_qsum32_d(a,b,c,d,e)\
	__asm__	("qsum32\t"#a","#b","#c","#d","#e"")
	
/*********** QMAX/QMIN/QAVG/QSAD/QALIGN/QSAD4 *************/
#define i_qmax8u(a,b,c)\
	__asm__	("qmax8u\t"#a","#b","#c"")
#define i_qmax8(a,b,c)\
	__asm__	("qmax8\t"#a","#b","#c"")
#define i_qmax16u(a,b,c)\
	__asm__	("qmax16u\t"#a","#b","#c"")
#define i_qmax16(a,b,c)\
	__asm__	("qmax16\t"#a","#b","#c"")
#define i_qmin8u(a,b,c)\
	__asm__	("qmin8u\t"#a","#b","#c"")
#define i_qmin8(a,b,c)\
	__asm__	("qmin8\t"#a","#b","#c"")
#define i_qmin16u(a,b,c)\
	__asm__	("qmin16u\t"#a","#b","#c"")
#define i_qmin16(a,b,c)\
	__asm__	("qmin16\t"#a","#b","#c"")
#define i_qavg8u(a,b,c)\
	__asm__	("qavg8u\t"#a","#b","#c"")
#define i_qavg8(a,b,c)\
	__asm__	("qavg8\t"#a","#b","#c"")
#define i_qavgr8u(a,b,c)\
	__asm__	("qavgr8u\t"#a","#b","#c"")
#define i_qavgr8(a,b,c)\
	__asm__	("qavgr8\t"#a","#b","#c"")
#define i_qsad4(a,b,c)\
	__asm__	("qsad4\t"#a","#b","#c"")
#define i_qalign8(a,b,c)\
	__asm__	("qalign8\t"#a","#b","#c"")
#define i_qalign16(a,b,c)\
	__asm__	("qalign16\t"#a","#b","#c"")
#define i_qalign24(a,b,c)\
	__asm__	("qalign24\t"#a","#b","#c"")

/************ QMIX/QSFT/QMAP/QSAT instrucions ************/
#define i_qmix8h(a,b,c)\
	__asm__	("qmix8h\t"#a","#b","#c"")
#define i_qmix8l(a,b,c)\
	__asm__	("qmix8l\t"#a","#b","#c"")
#define i_qmix8(a,b,c,d)\
	__asm__	("qmix8\t"#a","#b","#c","#d"")
#define i_qmix16h(a,b,c)\
	__asm__	("qmix16h\t"#a","#b","#c"")
#define i_qmix16l(a,b,c)\
	__asm__	("qmix16l\t"#a","#b","#c"")
#define i_qmix16(a,b,c,d)\
	__asm__	("qmix16\t"#a","#b","#c","#d"")
#define i_qsll8(a,b,c)\
	__asm__	("qsll8\t"#a","#b","#c"")
#define i_qslr8(a,b,c)\
	__asm__	("qslr8\t"#a","#b","#c"")
#define i_qsar8(a,b,c)\
	__asm__	("qsar8\t"#a","#b","#c"")
#define i_qsll16(a,b,c)\
	__asm__	("qsll16\t"#a","#b","#c"")
#define i_qslr16(a,b,c)\
	__asm__	("qslr16\t"#a","#b","#c"")
#define i_qsar16(a,b,c)\
	__asm__	("qsar16\t"#a","#b","#c"")
#define i_qmap(a,b,c)\
	__asm__	("qmap\t"#a","#b","#c"")
#define i_qsat(a,b)\
	__asm__	("qsat\t"#a","#b"")
#endif // MAU_V2

#define i_mtqr(src0,src1)	\
({ __asm__ __volatile__ ("mtqr\t%0,%1"::"r"(src0),"i"(src1));})

#define i_mfqr(src0)							\
(	{unsigned int _dst_; 						\
 	__asm__ __volatile__ ("mfqr\t%0,%1":"=r"(_dst_):"i"(src0));	\
	_dst_;})	

#define gnu_exit(src)				\
    ({ __asm__ __volatile__(			\
    "lui $8, 0\n"				\
    "addiu $8, $8, 0\n"				\
    "addiu $4, $0, %0\n"			\
    "nop\n"					\
    "break 0x3ff\n"::"i"(src)); });

#define i_rcr_epc()  i_mfc0(CP0_EPC)
#define i_rcr_esr()  0
#define i_rcr_dpc()  i_mfc0(CP0_DEPC)
#define i_rcr_dsr()  0
#define i_rcr_sr()   i_mfc0(CP0_STATUS)

#define i_wcr_epc(x)  i_mtc0(x, CP0_EPC)
#define i_wcr_esr(x)
#define i_wcr_dpc(x)  i_mtc0(x, CP0_DEPC)
#define i_wcr_dsr(x)
#define i_wcr_sr(x)   i_mtc0(x, CP0_STATUS)

#define i_rcr_spc()  i_mfc0(CP0_EPC)
#define i_rcr_ssr()
#define i_wcr_spc(x)  i_mtc0(x, CP0_EPC)
#define i_wcr_ssr(x)

#endif
