#ifndef __INGENIC_CIM_H__
#define __INGENIC_CIM_H__

#define CIMCFG                  (CIM_BASE + 0x0000)
#define CIMCTRL                 (CIM_BASE + 0x0004)
#define CIMSTATE                (CIM_BASE + 0x0008)
#define CIMIID                  (CIM_BASE + 0x000C)
#define CIMRXFIFO               (CIM_BASE + 0x0010)
#define CIMDA                   (CIM_BASE + 0x0020)
#define CIMFA                   (CIM_BASE + 0x0024)
#define CIMFID                  (CIM_BASE + 0x0028)
#define CIMCMD                  (CIM_BASE + 0x002C)
#define CIMSIZE                 (CIM_BASE + 0x0030)
#define CIMOFFSET               (CIM_BASE + 0x0034)
#define CIMYFA                  (CIM_BASE + 0x0038)
#define CIMYCMD                 (CIM_BASE + 0x003C)
#define CIMUFA                  (CIM_BASE + 0x0040)
#define CIMUCMD                 (CIM_BASE + 0x0044)
#define CIMVFA                  (CIM_BASE + 0x0048)
#define CIMVCMD                 (CIM_BASE + 0x004C)
#define CIMCTRL2                (CIM_BASE + 0x0050)
#define CIMFS                   (CIM_BASE + 0x0054)
#define CIMIMR                  (CIM_BASE + 0x0058)
#define CIMTC                   (CIM_BASE + 0x005C)
#define CIMTINX                 (CIM_BASE + 0x0060)
#define CIMTCNT                 (CIM_BASE + 0x0064)

#ifndef FPGA_CIM
#define DVDMR                   (CIM_DRV_BASE + 0x000)
#define DVCDR                   (CIM_DRV_BASE + 0x004)
#define DVCCR                   (CIM_DRV_BASE + 0x008)
#define DVISR                   (CIM_DRV_BASE + 0x00c)
#define DVHSR                   (CIM_DRV_BASE + 0x010)
#define DVHER                   (CIM_DRV_BASE + 0x014)
#define DVVSR                   (CIM_DRV_BASE + 0x018)
#define DVVER                   (CIM_DRV_BASE + 0x01c)
#define DVTLR                   (CIM_DRV_BASE + 0x020)
#define DVTFR                   (CIM_DRV_BASE + 0x024)
#define DVAHS                   (CIM_DRV_BASE + 0x028)
#define DVAHE                   (CIM_DRV_BASE + 0x02c)
#define DVAVS                   (CIM_DRV_BASE + 0x030)
#define DVAVE                   (CIM_DRV_BASE + 0x034)
#define D2HSR                   (CIM_DRV_BASE + 0x038)
#define D2HER                   (CIM_DRV_BASE + 0x03c)
#define D2VSR                   (CIM_DRV_BASE + 0x040)
#define D2VER                   (CIM_DRV_BASE + 0x044)
#define D2TLR                   (CIM_DRV_BASE + 0x048)
#define D2TFR                   (CIM_DRV_BASE + 0x04c)
#define D2AHS                   (CIM_DRV_BASE + 0x050)
#define D2AHE                   (CIM_DRV_BASE + 0x054)
#define D2AVS                   (CIM_DRV_BASE + 0x058)
#define D2AVE                   (CIM_DRV_BASE + 0x05c)
#define DVFCNT                  (CIM_DRV_BASE + 0x060)
#define DVLCNT                  (CIM_DRV_BASE + 0x064)
#define DVHCNT                  (CIM_DRV_BASE + 0x068)
#define DVVCNT                  (CIM_DRV_BASE + 0x06c)
#define DVIDA                   (CIM_DRV_BASE + 0x070)
#define DVISA                   (CIM_DRV_BASE + 0x074)
#define DVIFID                  (CIM_DRV_BASE + 0x078)
#define DVICMD                  (CIM_DRV_BASE + 0x07c)
#define DVIBR                   (CIM_DRV_BASE + 0x080)
#define DVBAR                   (CIM_DRV_BASE + 0x084)
#define DVBDA                   (CIM_DRV_BASE + 0x090)
#define DVBSA                   (CIM_DRV_BASE + 0x094)
#define DVBFID                  (CIM_DRV_BASE + 0x098)
#define DVBCMD                  (CIM_DRV_BASE + 0x09c)
#endif

#define SAV_FIRST 0x80
#define EAV_FIRST 0
#define CFG_EEOF                (1<<31)
#define CFG_EEOF_ENB            (1<<31)
#define CFG_EEOF_DIS            0
#define CFG_EXP                 (1<<30)
#define CFG_EXP_DIS             0
#define CFG_EXP_ENB             (1<<30)
#define CFG_BW                  (7 << 21)
#define CFG_SEP                 (1 << 20)
#define CFG_ORDER               (3 << 18)
#define CFG_DF                  (3 << 16)
#define CFG_DF_RAW              (0 << 16)
#define CFG_DF_YCBCR444         (1 << 16)
#define CFG_DF_YCBCR422         (2 << 16)
#define CFG_DF_ITU656           (3 << 16)
#define CFG_INV_DAT_DIS   	0
#define CFG_INV_DAT_EN    	(1 << 15)
#define CFG_VSP_H               0
#define CFG_VSP_L               (1 << 14)
#define CFG_HSP_H               0
#define CFG_HSP_L               (1 << 13)
#define CFG_PCP_R               0
#define CFG_PCP_F               (1 << 12)
#define CFG_BURST_A              10
#define CFG_BURST              (3 << CFG_BURST_A)
#define BURST_INCR4            (0)
#define BURST_INCR8            (1)
#define BURST_INCR16           (2)
#define BURST_INCR32           (3)
#define CFG_DUMMY_DIS   	0
#define CFG_DUMMY_EN    	(1 << 9)
#define CFG_EXT_VSYNC_DIS   	0
#define CFG_EXT_VSYNC_EN    	(1 << 8)
#define CFG_LM                  (1 << 7)
#define CFG_LM_EAV_FIRST        0
#define CFG_LM_SAV_FIRST        (1 << 7)
#define CFG_PACK		(7 << 4)
#define CFG_FP                  (1 << 3)
#define CFG_FP_L                0
#define CFG_FP_H                (1 << 3)
#define CFG_DSM    		3
#define CFG_DSM_656_P    	0
#define CFG_DSM_656_I    	1
#define CFG_DSM_GATE  		2
#define CFG_DSM_NON_GATE   	3
#define CTRL_EOF_LINE           0xff000000
#define CTRL_FRC                0x000f0000
#define CTRL_SIZE_EN            0x00004000
#define CTRL_DMA_SYNC           0x00000080
#define CTRL_MBEN               (1<<6)
#define CTRL_SW_RST             (1 << 3)
#define CTRL_DMA_EN             0x00000004
#define CTRL_RXF_RST_EN         0x00000002
#define CTRL_ENA_EN    	        0x00000001

#define CIMIMR_DEEOFM       (1 << 11)
#define CIMIMR_DSTPM        (1 << 10)
#define CIMIMR_DEOFM        (1 << 9)
#define CIMIMR_DSOFM        (1 << 8)
#define CIMIMR_TLBEM        (1 << 4)
#define CIMIMR_FSEM         (1 << 3)
#define CIMIMR_RFOFM        (1 << 2)
#define CIMIMR_VDDM         (1)

#define CIMIMR_INT             (CIMIMR_DEEOFM | \
                                CIMIMR_DSTPM | \
				CIMIMR_DEOFM | \
				CIMIMR_DSOFM | \
                                CIMIMR_TLBEM | \
                                CIMIMR_FSEM | \
                                CIMIMR_RFOFM | \
				CIMIMR_VDDM)
#define CIMIMR_INT_ENB          0
#define CIMIMR_INT_DIS          CIMIMR_INT

#define CIMCR2_FRAGHE             (1 << 25)
#define CIMCR2_FRAGVE             (1 << 24)

#define CIMCR2_FSC             (1 << 23)
#define CIMCR2_FSC_ENB          CIMCR2_FSC
#define CIMCR2_FSC_DIS          0

#define CIMCR2_ARIF            (1 << 22)
#define CIMCR2_ARIF_ENB         CIMCR2_ARIF
#define CIMCR2_ARIF_DIS         0

#define CIMCR2_HRZ_A            20
#define CIMCR2_HRZ_M           (3 << CIMCR2_HRZ_A)
#define CIM_HRZ_1               0
#define CIM_HRZ_2               1
#define CIM_HRZ_4               2
#define CIM_HRZ_8               3

#define CIMCR2_VRZ_A            18
#define CIMCR2_VRZ_M           (3 << CIMCR2_VRZ_A)
#define CIM_VRZ_1               0
#define CIM_VRZ_2               1
#define CIM_VRZ_4               2
#define CIM_VRZ_8               3

#define CTRL2_CSC                (3 << 16)
#define CTRL2_CSC_DIS            (0 << 16)
#define CTRL2_CSC_422            (2 << 16)
#define CTRL2_CSC_420            (3 << 16)

#define CTRL2_OP_MSK            (0x3 << 4)
#define CTRL2_OPE               (1 << 2)
#define CTRL2_EME               (1 << 1)
#define CTRL2_APM               (1 << 0)

#define STATE_EEOF              0x00000800
#define STATE_STOP              0x00000400
#define STATE_EOF               0x00000200
#define STATE_SOF               0x00000100
#define STATE_TLBE              0x00000010
#define STATE_FSE               0x00000008
#define STATE_OF                0x00000004
#define STATE_EMPTY             0x00000002
#define STATE_VDD               0x00000001

#define CMD_SOF_INT_EN          0x80000000
#define CMD_EOF_INT_EN          0x40000000
#define CMD_EEOF_INT_EN         0x20000000
#define CMD_STOP_EN             0x10000000
#define CMD_OFRCV_EN            0x08000000
#define CMD_FRAME_LEN           0x00ffffff
#define CMD_STOP_DIS            0x00000000

#define CIMFS_FVS_A             16
#define CIMFS_FVS_M             (0x1fff << CIMFS_FVS_A)

#define CIMFS_BPP_A             14
#define CIMFS_BPP_M             (0x3 << CIMFS_BPP_A)
#define CIMFS_BPP_1             (1)
#define CIMFS_BPP_2             (2)
#define CIMFS_BPP_3             (3)
#define CIMFS_BPP_4             (4)

#define CIMFS_FHS_A             0
#define CIMFS_FHS_M             (0x1fff << CIMFS_FVS_A)

#define CIM_FHS                  0x1fff

#define CIMTC_TBA_A            (2)
#define CIMTC_TBA_M            (0x3fffffff << CIMTC_TBA_A)
#define CIMTC_RST              (1 << 1)
#define CIMTC_ENB               1

#define CIMTINX_INX            (7)

#ifndef CIMCDR
#define CIMCDR                  0xB000007C
#endif

// CIMFS


#define set_cim_tlb_ctrl(val)           i_s32(val, CIMTC,  0x0)
#define get_cim_tlb_ctrl()              i_l32(CIMTC, 0x0)

#define set_cim_tlb_tba(val)   set_cim_tlb_ctrl(get_cim_tlb_ctrl() & ~CIMTC_TBA_M | (val << CIMTC_TBA_A))

#define set_cim_tlb_vld()      ({;})
#define set_cim_tlb_invld()    ({;})

#define enb_cim_tlb_rst()   set_cim_tlb_ctrl(get_cim_tlb_ctrl() |  CIMTC_RST)
#define dis_cim_tlb_rst()   set_cim_tlb_ctrl(get_cim_tlb_ctrl() & ~CIMTC_RST)

#define enb_cim_tlb() set_cim_tlb_ctrl(get_cim_tlb_ctrl() |  CIMTC_ENB)
#define dis_cim_tlb() set_cim_tlb_ctrl(get_cim_tlb_ctrl() & ~CIMTC_ENB)

#define set_cim_tlb_index(val)           i_s32(val, CIMTINX,  0x0)
#define get_cim_tlb_index()              i_l32(CIMTINX, 0x0)

#define get_cim_tlb_content()    i_l32(CIMTCNT, 0x0)

#define set_cimfs(val)           i_s32(val, CIMFS,  0x0)
#define get_cimfs()              i_l32(CIMFS, 0x0)

#define set_cimfhs(val)          set_cimfs((get_cimfs() & ~CIM_FHS) | (val - 1))
#define set_cim_fhs(val)         set_cimfhs(val)
#define set_cim_fvs(val)         set_cimfs((get_cimfs() & ~CIMFS_FVS_M) | ((val - 1) << CIMFS_FVS_A))
#define set_cim_bpp(val)         set_cimfs((get_cimfs() & ~CIMFS_BPP_M) | ((val - 1) << CIMFS_BPP_A))

#define set_cimmclk_rate(N)      i_s32(N, CIMCDR, 0x0)
#define set_cimmode(val)         i_s32(val, CIMCFG,  0x0)
#define set_cimctrl(val)         i_s32(val, CIMCTRL,  0x0)
#define set_cimstate(val)        i_s32(val, CIMSTATE, 0x0)
#define set_cimda(val)           i_s32(val, CIMDA,    0x0)
#define set_cimsize(val)         i_s32(val, CIMSIZE,  0x0)
#define set_cimoffset(val)       i_s32(val, CIMOFFSET,  0x0)
#define set_cimctrl2(val)        i_s32(val, CIMCTRL2,  0x0)
#define set_cimimr(val)          i_s32(val, CIMIMR,  0x0)

#define get_cimmode()            i_l32(CIMCFG,   0x0)
#define get_cimctrl()            i_l32(CIMCTRL,  0x0)
#define get_cimctrl2()           i_l32(CIMCTRL2, 0x0)
#define get_cimimr()             i_l32(CIMIMR,   0x0)
#define get_cimstate()           i_l32(CIMSTATE, 0x0)
#define get_cimiid()             i_l32(CIMIID,   0x0)
#define get_cimrxfifo()          i_l32(CIMRXFIFO,0x0)
#define get_cimda()              i_l32(CIMDA,    0x0)
#define get_cimfa()              i_l32(CIMFA,    0x0)
#define get_cimfid()             i_l32(CIMFID,   0x0)
#define get_cimcmd()             i_l32(CIMCMD,   0x0)
#define get_cimsize()            i_l32(CIMSIZE,  0x0)
#define get_cimoffset()          i_l32(CIMOFFSET,  0x0)
#define get_cimyfa()             i_l32(CIMYFA,   0x0)
#define get_cimycmd()            i_l32(CIMYCMD,  0x0)
#define get_cimufa()             i_l32(CIMUFA,   0x0)
#define get_cimucmd()            i_l32(CIMUCMD,  0x0)
#define get_cimvfa()             i_l32(CIMVFA,   0x0)
#define get_cimvcmd()            i_l32(CIMVCMD,  0x0)
#define get_cim_data()           get_cimrxfifo()
#define dis_eeof_cim()           set_cimmode((get_cimmode() & ~CFG_EEOF) | CFG_EEOF_DIS)
#define enb_eeof_cim()           set_cimmode((get_cimmode() & ~CFG_EEOF) | CFG_EEOF_ENB)
#define dis_exp_cim()            set_cimmode((get_cimmode() & ~CFG_EXP) | CFG_EXP_DIS)
#define enb_exp_cim()            set_cimmode((get_cimmode() & ~CFG_EXP) | CFG_EXP_ENB)
#define set_bw_cim(n)            set_cimmode((get_cimmode() & ~CFG_BW) | ((n-9) << 21))

#define enb_cim_hfrag()          set_cimctrl2(get_cimctrl2() & ~CIMCR2_FRAGHE)
#define dis_cim_hfrag()          set_cimctrl2(get_cimctrl2() | CIMCR2_FRAGHE)

#define enb_cim_vfrag()          set_cimctrl2(get_cimctrl2() & ~CIMCR2_FRAGVE)
#define dis_cim_vfrag()          set_cimctrl2(get_cimctrl2() | CIMCR2_FRAGVE)

#define enb_cim_fsc()            set_cimctrl2((get_cimctrl2() & ~CIMCR2_FSC) | CIMCR2_FSC_ENB)
#define dis_cim_fsc()            set_cimctrl2((get_cimctrl2() & ~CIMCR2_FSC) | CIMCR2_FSC_DIS)

#define enb_cim_arif()           set_cimctrl2((get_cimctrl2() & ~CIMCR2_ARIF) | CIMCR2_ARIF_ENB)
#define dis_cim_arif()           set_cimctrl2((get_cimctrl2() & ~CIMCR2_ARIF) | CIMCR2_ARIF_DIS)

#define set_cim_hrz(val)         set_cimctrl2((get_cimctrl2() & ~CIMCR2_HRZ_M) | ((val) << CIMCR2_HRZ_A))
#define get_cim_hrz(val)         ((get_cimctrl2() & CIMCR2_HRZ_M) >> CIMCR2_HRZ_A)

#define set_cim_vrz(val)         set_cimctrl2((get_cimctrl2() & ~CIMCR2_VRZ_M) | ((val) << CIMCR2_VRZ_A))
#define get_cim_vrz(val)         ((get_cimctrl2() & CIMCR2_VRZ_M) >> CIMCR2_VRZ_A)

#define enb_bypass()             set_cimctrl2((get_cimctrl2() & ~CTRL2_CSC) | CTRL2_CSC_DIS)
#define enb_csc_422()            set_cimctrl2((get_cimctrl2() & ~CTRL2_CSC) | CTRL2_CSC_422)
#define enb_csc_420()            set_cimctrl2((get_cimctrl2() & ~CTRL2_CSC) | CTRL2_CSC_420)
#define set_mode_bypass()        enb_bypass()
#define clr_mode_bypass()        enb_csc_422()
#define set_vsync_l()            set_cimmode(get_cimmode() |    CFG_VSP_L )
#define set_vsync_h()            set_cimmode(get_cimmode() &  (~CFG_VSP_L))
#define set_hsync_l()            set_cimmode(get_cimmode() |    CFG_HSP_L )
#define set_hsync_h()            set_cimmode(get_cimmode() &  (~CFG_HSP_L))
#define set_pclk_f()             set_cimmode(get_cimmode() |    CFG_PCP_F )
#define set_pclk_r()             set_cimmode(get_cimmode() &  (~CFG_PCP_F))
#define enb_inv_dat()            set_cimmode(get_cimmode() |    CFG_INV_DAT_EN)
#define dis_inv_dat()            set_cimmode(get_cimmode() &  (~CFG_INV_DAT_EN))
#define enb_dummy()              set_cimmode(get_cimmode() |    CFG_DUMMY_EN)
#define dis_dummy()              set_cimmode(get_cimmode() &  (~CFG_DUMMY_EN))
#define enb_ext_vsync()          set_cimmode(get_cimmode() |    CFG_EXT_VSYNC_EN)
#define dis_ext_vsync()          set_cimmode(get_cimmode() &  (~CFG_EXT_VSYNC_EN))
#define set_cimcfg_sep()         set_cimmode(get_cimmode() | CFG_SEP)
#define set_df(N)                set_cimmode((get_cimmode() & (~CFG_DF)) | (N<<16))
#define set_df_raw()             set_cimmode((get_cimmode() & (~CFG_DF)) | CFG_DF_RAW)
#define set_df_ycbcr444()        set_cimmode((get_cimmode() & (~CFG_DF)) | CFG_DF_YCBCR444)
#define set_df_ycbcr422()        set_cimmode((get_cimmode() & (~CFG_DF)) | CFG_DF_YCBCR422)
#define set_df_itu656()          set_cimmode((get_cimmode() & (~CFG_DF)) | CFG_DF_ITU656)
#define set_burst(N)             set_cimmode((get_cimmode() & (~CFG_BURST)) | ((N)<<CFG_BURST_A))
#define set_burst_incr4()        set_burst(0)
#define set_burst_incr8()        set_burst(1)
#define set_burst_incr16()       set_burst(2)
#define set_burst_incr32()       set_burst(3)
#define set_order(N)             set_cimmode((get_cimmode() & (~CFG_ORDER)) | ((N)<<18))
#define set_sav_first()          set_cimmode((get_cimmode() & (~CFG_LM)) | CFG_LM_SAV_FIRST)
#define set_eav_first()          set_cimmode((get_cimmode() & (~CFG_LM)) | CFG_LM_EAV_FIRST)
#define set_lm(N)                ({if (N != 0) set_sav_first(); else set_eav_first();})
#define set_pack(N)              set_cimmode((get_cimmode() & (~CFG_PACK))  | ((N)<<4))
#define set_fp_h()               set_cimmode((get_cimmode() & (~CFG_FP)  )  | CFG_FP_H)
#define set_fp_l()               set_cimmode((get_cimmode() & (~CFG_FP)  )  | CFG_FP_L)
#define enb_dsm_656_p()          set_cimmode((get_cimmode() & (~CFG_DSM) )  | CFG_DSM_656_P)
#define enb_dsm_656_i()          set_cimmode((get_cimmode() & (~CFG_DSM) )  | CFG_DSM_656_I)
#define enb_dsm_gate()           set_cimmode((get_cimmode() & (~CFG_DSM) )  | CFG_DSM_GATE)
#define enb_dsm_non_gate()       set_cimmode((get_cimmode() & (~CFG_DSM) )  | CFG_DSM_NON_GATE)
#define ROW_MASK 4095
#define COL_MASK (4095<<16)
#define set_row_offset(N)        set_cimoffset((get_cimoffset() & (~ROW_MASK)) | N) 
#define set_hor_offset(N)        set_row_offset(N)
#define set_col_offset(N)        set_cimoffset((get_cimoffset() & (~COL_MASK)) | (N<<16))  
#define set_vor_offset(N)        set_col_offset(N)
#define set_row_size(N)          set_cimsize((get_cimsize() & (~ROW_MASK)) | N) 
#define set_hor_size(N)          set_row_size(N)
#define set_col_size(N)          set_cimsize((get_cimsize() & (~COL_MASK)) | (N<<16))
#define set_vor_size(N)          set_col_size(N)
#define enb_vci()                set_cimctrl(get_cimctrl() |   CTRL_ENA_EN    )
#define dis_vci()                set_cimctrl(get_cimctrl() & (~CTRL_ENA_EN)   )
#define enb_cim_rst()            set_cimctrl(get_cimctrl() |   CTRL_SW_RST    )
#define dis_cim_rst()            set_cimctrl(get_cimctrl() & (~CTRL_SW_RST)   )
#define enb_vci_dma()            set_cimctrl(get_cimctrl() |   CTRL_DMA_EN    )
#define dis_vci_dma()            set_cimctrl(get_cimctrl() & (~CTRL_DMA_EN)   )
#define enb_rxf_rst()            set_cimctrl(get_cimctrl() |   CTRL_RXF_RST_EN)
#define dis_rxf_rst()            set_cimctrl(get_cimctrl() & (~CTRL_RXF_RST_EN))
#define enb_offset()             set_cimctrl(get_cimctrl() |   CTRL_OFFSET_EN)
#define dis_offset()             set_cimctrl(get_cimctrl() & (~CTRL_OFFSET_EN))
#define enb_size()               set_cimctrl(get_cimctrl() |   CTRL_SIZE_EN)
#define dis_size()               set_cimctrl(get_cimctrl() & (~CTRL_SIZE_EN))
#define enb_dma_sync()           set_cimctrl(get_cimctrl() | CTRL_DMA_SYNC)
#define dis_dma_sync()           set_cimctrl(get_cimctrl() & (~CTRL_DMA_SYNC))
#define get_dma_sync()          (get_cimctrl() & CTRL_DMA_SYNC)
#define enb_cim_mb()             set_cimctrl(get_cimctrl() | CTRL_MBEN)
#define dis_cim_mb()             set_cimctrl(get_cimctrl() & (~CTRL_MBEN))

#define dis_eeof_int()           set_cimimr(get_cimimr() |   CIMIMR_DEEOFM)
#define enb_eeof_int()           set_cimimr(get_cimimr() & (~CIMIMR_DEEOFM))
#define get_eeof_mask()         (get_cimimr() & CIMIMR_DEEOFM)

#define dis_dma_stop_int()       set_cimimr(get_cimimr() |   CIMIMR_DSTPM)
#define enb_dma_stop_int()       set_cimimr(get_cimimr() & (~CIMIMR_DSTPM))
#define get_dma_stop_mask()     (get_cimimr() & CIMIMR_DSTPM)

#define dis_eof_int()            set_cimimr(get_cimimr() |   CIMIMR_DEOFM)
#define enb_eof_int()            set_cimimr(get_cimimr() & (~CIMIMR_DEOFM))
#define get_eof_mask()          (get_cimimr() & CIMIMR_DEOFM)

#define dis_sof_int()            set_cimimr(get_cimimr() |   CIMIMR_DSOFM)
#define enb_sof_int()            set_cimimr(get_cimimr() & (~CIMIMR_DSOFM))
#define get_sof_mask()          (get_cimimr() & CIMIMR_DSOFM)

#define dis_tlbe_int()           set_cimimr(get_cimimr() |   CIMIMR_TLBEM)
#define enb_tlbe_int()           set_cimimr(get_cimimr() & (~CIMIMR_TLBEM))
#define get_tlbe_mask()         (get_cimimr() & CIMIMR_TLBEM)

#define dis_fse_int()            set_cimimr(get_cimimr() |   CIMIMR_FSEM)
#define enb_fse_int()            set_cimimr(get_cimimr() & (~CIMIMR_FSEM))
#define get_fse_mask()          (get_cimimr() & CIMIMR_FSEM)

#define dis_fifo_of_int()        set_cimimr(get_cimimr() |   CIMIMR_RFOFM)
#define enb_fifo_of_int()        set_cimimr(get_cimimr() & (~CIMIMR_RFOFM))
#define get_fifo_of_mask()      (get_cimimr() & CIMIMR_RFOFM)

#define dis_vdd_int()            set_cimimr(get_cimimr() |   CIMIMR_VDDM)
#define enb_vdd_int()            set_cimimr(get_cimimr() & (~CIMIMR_VDDM))
#define get_vdd_mask()          (get_cimimr() & CIMIMR_VDDM)

#define dis_vci_int()            set_cimimr((get_cimimr() & (~CIMIMR_INT)) | CIMIMR_INT_DIS)
#define enb_vci_int()            set_cimimr((get_cimimr() & (~CIMIMR_INT)) | CIMIMR_INT_ENB)

#define set_op(n)                set_cimctrl2((get_cimctrl2() & ~CTRL2_OP_MSK) | (n << 4))
#define get_op()               ((get_cimctrl2() & CTRL2_OP_MSK) >> 4)
#define enb_ope()                set_cimctrl2(get_cimctrl2() | CTRL2_OPE)
#define dis_ope()                set_cimctrl2(get_cimctrl2() & (~CTRL2_OPE))
#define get_ope()               (get_cimctrl2() & CTRL2_OPM)
#define enb_emergency_mode()     set_cimctrl2(get_cimctrl2() | CTRL2_EME)
#define dis_emergency_mode()     set_cimctrl2(get_cimctrl2() & (~CTRL2_EME))
#define get_emergency_mode()    (get_cimctrl2() & CTRL2_EME)
#define enb_apm()                set_cimctrl2(get_cimctrl2() | CTRL2_APM)
#define dis_apm()                set_cimctrl2(get_cimctrl2() & (~CTRL2_APM))
#define get_apm()               (get_cimctrl2() & CTRL2_APM)
#define set_frame_rate(N)        set_cimctrl((get_cimctrl() & (~CTRL_FRC))      | ( N<<16)     )
#define set_eof_line(N)          set_cimctrl((get_cimctrl() & (~CTRL_EOF_LINE))  | ( N<<20))
#define get_dis_done()          (get_cimstate() & STATE_VDD)
#define get_fifo_empty()        (get_cimstate() & STATE_EMPTY)
#define get_cim_tlbe()          (get_cimstate() & STATE_TLBE)
#define get_cim_fse()           (get_cimstate() & STATE_FSE)
#define get_fifo_of()           (get_cimstate() & STATE_OF)
#define get_dma_stop()          (get_cimstate() & STATE_STOP)
#define get_dma_sof()           (get_cimstate() & STATE_SOF)
#define get_dma_eof()           (get_cimstate() & STATE_EOF)
#define get_dma_eeof()          (get_cimstate() & STATE_EEOF)
#define clr_dma_stop()           set_cimstate((~STATE_STOP))
#define clr_dma_sof()            set_cimstate((~STATE_SOF))
#define clr_dma_eof()            set_cimstate((~STATE_EOF))
#define clr_dma_eeof()           set_cimstate((~STATE_EEOF))
#define clr_cim_tlbe()           set_cimstate((~STATE_TLBE))
#define clr_cim_fse()            set_cimstate((~STATE_FSE))
#define clr_fifo_of()            set_cimstate((~STATE_OF))
#define clr_fifo_empty()         set_cimstate((~STATE_EMPTY))
#define clr_dis_done()           set_cimstate((~STATE_VDD))
#define get_frame_len()         (get_cimcmd() & CMD_FRAME_LEN )

#ifndef FPGA_CIM
#define set_dvdmr(val)           i_s32(val, DVDMR,  0x0)
#define get_dvdmr(val)           i_l32(DVDMR,  0x0)
#define DVI_ENB                  0x80000000
#define enb_dvi()                set_dvdmr(get_dvdmr() |   DVI_ENB )
#define dis_dvi()                set_dvdmr(get_dvdmr() & (~DVI_ENB))
#define DVI_BPP                  0x70000000
#define DVI_BPP_8                0x30000000
#define DVI_BPP_16               0x40000000
#define set_dvi_bpp_8()          set_dvdmr(get_dvdmr() & (~DVI_BPP) | DVI_BPP_8 )
#define set_dvi_bpp_16()         set_dvdmr(get_dvdmr() & (~DVI_BPP) | DVI_BPP_16)
#define DVI_PDW                  0x0c000000
#define DVI_PDW_8                0x08000000
#define DVI_PDW_16               0x0c000000
#define set_dvi_pdw_8()          set_dvdmr(get_dvdmr() & (~DVI_PDW) | DVI_PDW_8 )
#define set_dvi_pdw_16()         set_dvdmr(get_dvdmr() & (~DVI_PDW) | DVI_PDW_16)
#define DVI_BEDN                 0x02000000
#define DVI_BEDN_SAME            0x00000000
#define DVI_BEDN_INVT            0x02000000
#define set_dvi_bedn_same()      set_dvdmr(get_dvdmr() & (~DVI_BEDN))
#define set_dvi_bedn_invt()      set_dvdmr(get_dvdmr() |   DVI_BEDN )
#define DVI_PEDN                 0x01000000
#define DVI_PEDN_BIG             0x00000000
#define DVI_PEDN_LITTLE          0x01000000
#define set_dvi_pend_b()         set_dvdmr(get_dvdmr() & (~DVI_PEDN))
#define set_dvi_pend_l()         set_dvdmr(get_dvdmr() |   DVI_PEDN )
#define DVI_RGB                  0x00800000
#define DVI_RGB_565              0x00000000
#define DVI_RGB_555              0x00800000
#define set_dvi_rgb_565()        set_dvdmr(get_dvdmr() & (~DVI_RGB))
#define set_dvi_rgb_555()        set_dvdmr(get_dvdmr() |   DVI_RGB )
#define DVI_VOE                  0x00400000
#define DVI_VOE_RISE             0x00000000
#define DVI_VOE_FALL             0x00400000
#define set_dvi_voe_r()          set_dvdmr(get_dvdmr() & (~DVI_VOE))
#define set_dvi_voe_f()          set_dvdmr(get_dvdmr() |   DVI_VOE )
#define DVI_HOE                  0x00200000
#define DVI_HOE_RISE             0x00000000
#define DVI_HOE_FALL             0x00200000
#define set_dvi_hoe_r()          set_dvdmr(get_dvdmr() & (~DVI_HOE))
#define set_dvi_hoe_f()          set_dvdmr(get_dvdmr() |   DVI_HOE )
#define DVI_DOE                  0x00100000
#define DVI_DOE_RISE             0x00000000
#define DVI_DOE_FALL             0x00100000
#define set_dvi_doe_r()          set_dvdmr(get_dvdmr() & (~DVI_DOE))
#define set_dvi_doe_f()          set_dvdmr(get_dvdmr() |   DVI_DOE )
#define DVI_CKOD                 0x00080000
#define DVI_CKOD_IN              0x00000000
#define DVI_CKOD_OUT             0x00080000
#define set_dvi_ck_in()          set_dvdmr(get_dvdmr() & (~DVI_CKOD))
#define set_dvi_ck_out()         set_dvdmr(get_dvdmr() |   DVI_CKOD )
#define DVI_VSD                  0x00040000
#define DVI_VSD_IN               0x00000000
#define DVI_VSD_OUT              0x00040000
#define set_dvi_vsync_in()       set_dvdmr(get_dvdmr() & (~DVI_VSD))
#define set_dvi_vsync_out()      set_dvdmr(get_dvdmr() |   DVI_VSD )
#define DVI_HSD                  0x00020000
#define DVI_HSD_IN               0x00000000
#define DVI_HSD_OUT              0x00020000
#define set_dvi_hsync_in()       set_dvdmr(get_dvdmr() & (~DVI_HSD))
#define set_dvi_hsync_out()      set_dvdmr(get_dvdmr() |   DVI_HSD )
#define DVI_DD                   0x00010000
#define DVI_DD_IN                0x00000000
#define DVI_DD_OUT               0x00010000
#define set_dvi_pd_in()          set_dvdmr(get_dvdmr() & (~DVI_DD))
#define set_dvi_pd_out()         set_dvdmr(get_dvdmr() |   DVI_DD )
#define DVI_VOP                  0x00008000
#define DVI_VOP_H                0x00000000
#define DVI_VOP_L                0x00008000
#define set_dvi_vsync_h()        set_dvdmr(get_dvdmr() & (~DVI_VOP))
#define set_dvi_vsync_l()        set_dvdmr(get_dvdmr() |   DVI_VOP )
#define DVI_HOP                  0x00004000
#define DVI_HOP_H                0x00000000
#define DVI_HOP_L                0x00004000
#define set_dvi_hsync_h()        set_dvdmr(get_dvdmr() & (~DVI_HOP))
#define set_dvi_hsync_l()        set_dvdmr(get_dvdmr() |   DVI_HOP )
#define DVI_XCKD                 0x00002000
#define DVI_XCKD_IN              0x00000000
#define DVI_XCKD_OUT             0x00002000
#define set_dvi_xck_in()         set_dvdmr(get_dvdmr() & (~DVI_XCKD))
#define set_dvi_xck_out()        set_dvdmr(get_dvdmr() |   DVI_XCKD )
#define DVI_EFD                  0x00001000
#define DVI_EFD_NORMAL           0x00000000
#define DVI_EFD_FIELD            0x00001000
#define set_dvi_efd_normal()     set_dvdmr(get_dvdmr() & (~DVI_EFD))
#define set_dvi_efd_field()      set_dvdmr(get_dvdmr() |   DVI_EFD )
#define DVI_IFM_INTERLACE        0x00000400
#define set_dvi_inter_lace()     set_dvdmr(get_dvdmr()  |   DVI_IFM_INTERLACE)
#define set_dvi_non_inter_lace() set_dvdmr(get_dvdmr()  & (~DVI_IFM_INTERLACE))
#define DVI_IFM_ITU656           0x00000200
#define set_dvi_non_itu656()     set_dvdmr(get_dvdmr()  & (~DVI_IFM_ITU656))
#define set_dvi_itu656()         set_dvdmr(get_dvdmr()  |   DVI_IFM_ITU656)
#define DVI_IFM_EXT_SYNC         0x00000100
#define set_dvi_non_ext_sync()   set_dvdmr(get_dvdmr()  & (~DVI_IFM_EXT_SYNC))
#define set_dvi_ext_sync()       set_dvdmr(get_dvdmr()  |   DVI_IFM_EXT_SYNC)
#define DVI_IFM_XCLK_GATE        0x00000080
#define set_dvi_xclk_gate()      set_dvdmr(get_dvdmr()  |   DVI_IFM_XCLK_GATE)
#define dis_dvi_xclk_gate()      set_dvdmr(get_dvdmr()  & (~DVI_IFM_XCLK_GATE))
#define DVI_BLANK_DAT            0x00000040
#define set_dvi_blank_data()     set_dvdmr(get_dvdmr()  |   DVI_BLANK_DAT)
#define dis_dvi_blank_data()     set_dvdmr(get_dvdmr()  & (~DVI_BLANK_DAT))
#define DVI_TDMK                 0x00000020
#define set_dvi_tidemark()       set_dvdmr(get_dvdmr()  |   DVI_TDMK)
#define dis_dvi_tidemark( )      set_dvdmr(get_dvdmr()  & (~DVI_TDMK))
#define DVI_PFM                  0x0000000C
#define DVI_PFM_NORMAL           0x00000000
#define DVI_PFM_INVERT           0x00000004
#define DVI_PFM_FIX0             0x00000008
#define DVI_PFM_FIX1             0x0000000C
#define set_dvi_field_normal()    set_dvdmr(get_dvdmr()  & (~DVI_PFM) | DVI_PFM_NORMAL)
#define set_dvi_field_invert()    set_dvdmr(get_dvdmr()  & (~DVI_PFM) | DVI_PFM_INVERT)
#define set_dvi_field_fix0()      set_dvdmr(get_dvdmr()  & (~DVI_PFM) | DVI_PFM_FIX0)
#define set_dvi_field_fix1()      set_dvdmr(get_dvdmr()  & (~DVI_PFM) | DVI_PFM_FIX1)
#define set_dvcdr(val)                i_s32(val, DVCDR,  0x0)
#define get_dvcdr(val)                i_l32(DVCDR,  0x0)
#define DVI_CLKOUT_R                  0xF0000000
#define DVI_CLKOUT_M                  0x0FFF0000
#define set_clkout_r(N)               set_dvcdr(get_dvcdr() & (~DVI_CLKOUT_R) | (N<<28))
#define set_clkout_m(N)               set_dvcdr(get_dvcdr() & (~DVI_CLKOUT_M) | (N<<16))
#define DVI_XCLK_R                    0x0000F000
#define DVI_XCLK_M                    0x00000FFF
#define set_xclk_r(N)                 set_dvcdr(get_dvcdr() & (~DVI_XCLK_R) | (N<<12))
#define set_xclk_m(N)                 set_dvcdr(get_dvcdr() & (~DVI_XCLK_M) | (N))
#define set_dvccr(val)                i_s32(val, DVCCR,  0x0)
#define get_dvccr(val)                i_l32(DVCCR,  0x0)
#define DVI_VSE                       0x00008000
#define DVI_VSE_EN                    0x00000000
#define DVI_VSE_DIS                   0x00008000
#define enb_chk_vsync()               set_dvccr(get_dvccr() & (~DVI_VSE))
#define dis_chk_vsync()               set_dvccr(get_dvccr() | ( DVI_VSE))
#define DVI_HSE                       0x00004000
#define DVI_HSE_EN                    0x00000000
#define DVI_HSE_DIS                   0x00004000
#define enb_chk_hsync()               set_dvccr(get_dvccr() & (~DVI_HSE))
#define dis_chk_hsync()               set_dvccr(get_dvccr() | ( DVI_HSE))
#define DVI_PDE                       0x00002000
#define DVI_PDE_EN                    0x00000000
#define DVI_PDE_DIS                   0x00002000
#define enb_chk_pd()                  set_dvccr(get_dvccr() & (~DVI_PDE))
#define dis_chk_pd()                  set_dvccr(get_dvccr() | ( DVI_PDE))
#define set_dvisr(val)                i_s32(val, DVISR,  0x0)
#define get_dvisr(val)                i_l32(DVISR,  0x0)
#define DVI_ERR                       0x80000000
#define get_dvi_error()              (get_dvisr() & DVI_ERR)
#define DVI_DATER                     0x40000000
#define get_dvi_data_err()           (get_dvisr() & DVI_DATER)
#define DVI_TIMER                     0x20000000
#define get_dvi_time_err()           (get_dvisr() & DVI_TIMER)
#define set_dvhsr(val)                i_s32(val, DVHSR,  0x0)
#define get_dvhsr(val)                i_l32(DVHSR,  0x0)
#define set_hsync_start_at(N)         set_dvhsr(N)
#define set_dvher(val)                i_s32(val, DVHER,  0x0)
#define get_dvher(val)                i_l32(DVHER,  0x0)
#define set_hsync_end_at(N)           set_dvher(N)
#define set_dvvsr(val)                i_s32(val, DVVSR,  0x0)
#define get_dvvsr(val)                i_l32(DVVSR,  0x0)
#define set_vsync_start_at(N)         set_dvvsr(N)
#define set_dvver(val)                i_s32(val, DVVER,  0x0)
#define get_dvver(val)                i_l32(DVVER,  0x0)
#define set_vsync_end_at(N)           set_dvver(N)
#define set_dvtlr(val)                i_s32(val, DVTLR,  0x0)
#define get_dvtlr(val)                i_l32(DVTLR,  0x0)
#define set_total_line(N)             set_dvtlr(N)
#define set_dvtfr(val)                i_s32(val, DVTFR,  0x0)
#define get_dvtfr(val)                i_l32(DVTFR,  0x0)
#define set_total_frame(N)            set_dvtfr(N)
#define set_dvahs(val)                i_s32(val, DVAHS,  0x0)
#define get_dvahs(val)                i_l32(DVAHS,  0x0)
#define set_act_hsync_start_at(N)     set_dvahs(N)
#define set_dvahe(val)                i_s32(val, DVAHE,  0x0)
#define get_dvahe(val)                i_l32(DVAHE,  0x0)
#define set_act_hsync_end_at(N)       set_dvahe(N)
#define set_dvavs(val)                i_s32(val, DVAVS,  0x0)
#define get_dvavs(val)                i_l32(DVAVS,  0x0)
#define set_act_vsync_start_at(N)     set_dvavs(N)
#define set_dvave(val)                i_s32(val, DVAVE,  0x0)
#define get_dvave(val)                i_l32(DVAVE,  0x0)
#define set_act_vsync_end_at(N)       set_dvave(N)
#define set_d2hsr(val)                i_s32(val, D2HSR,  0x0)
#define get_d2hsr(val)                i_l32(D2HSR,  0x0)
#define set_d2_hsync_start_at(N)      set_d2hsr(N)
#define set_d2her(val)                i_s32(val, D2HER,  0x0)
#define get_d2her(val)                i_l32(D2HER,  0x0)
#define set_d2_hsync_end_at(N)        set_d2her(N)
#define set_d2vsr(val)                i_s32(val, D2VSR,  0x0)
#define get_d2vsr(val)                i_l32(D2VSR,  0x0)
#define set_d2_vsync_start_at(N)      set_d2vsr(N)
#define set_d2ver(val)                i_s32(val, D2VER,  0x0)
#define get_d2ver(val)                i_l32(D2VER,  0x0)
#define set_d2_vsync_end_at(N)        set_d2ver(N)
#define set_d2tlr(val)                i_s32(val, D2TLR,  0x0)
#define get_d2tlr(val)                i_l32(D2TLR,  0x0)
#define set_d2_total_line(N)          set_d2tlr(N)
#define set_d2tfr(val)                i_s32(val, D2TFR,  0x0)
#define get_d2tfr(val)                i_l32(D2TFR,  0x0)
 
#define set_d2_total_frame(N)         set_d2tfr(N)
#define set_d2ahs(val)                i_s32(val, D2AHS,  0x0)
#define get_d2ahs(val)                i_l32(D2AHS,  0x0)
#define set_d2_act_hsync_start_at(N)  set_d2ahs(N)
#define set_d2ahe(val)                i_s32(val, D2AHE,  0x0)
#define get_d2ahe(val)                i_l32(D2AHE,  0x0)
#define set_d2_act_hsync_end_at(N)    set_d2ahe(N)
#define set_d2avs(val)                i_s32(val, D2AVS,  0x0)
#define get_d2avs(val)                i_l32(D2AVS,  0x0)
#define set_d2_act_vsync_start_at(N)  set_d2avs(N)
#define set_d2ave(val)                i_s32(val, D2AVE,  0x0)
#define get_d2ave(val)                i_l32(D2AVE,  0x0)
#define set_d2_act_vsync_end_at(N)    set_d2ave(N)
#define get_dvfcnt()                  i_l32(DVFCNT,  0x0)
#define get_frame_cnt()               get_dvfcnt()
#define set_frame_cnt(N)              i_s32(N, DVFCNT,  0x0)
#define get_dvlcnt()                  i_l32(DVLCNT,  0x0)
#define get_line_cnt()                get_dvlcnt()
#define set_line_cnt(N)               i_s32(N, DVLCNT,  0x0)
#define get_dvhcnt()                  i_l32(DVHCNT,  0x0)
#define get_hsync_cnt()               get_dvhcnt()
#define set_hsync_cnt(N)              i_s32(N, DVHCNT,  0x0)
#define get_dvvcnt()                  i_l32(DVVCNT,  0x0)
#define get_vsync_cnt()               get_dvvcnt()
#define set_vsync_cnt(N)              i_s32(N, DVVCNT,  0x0) 
#define set_dvida(val)                i_s32(val, DVIDA,  0x0)
#define get_dvida()                   i_l32(DVIDA,  0x0)
#define set_dvi_desp_addr(val)        set_dvida(val)
#define get_dvi_desp_addr()           get_dvida()
#define set_dvisa(val)                i_s32(val, DVISA,  0x0)
#define get_dvisa()                   i_l32(DVISA,  0x0)
#define get_dvi_src_addr()            get_dvisa()
#define set_dvifid(val)               i_s32(val, DVIFID,  0x0)
#define get_dvifid()                  i_l32(DVIFID,  0x0)
#define get_dvi_frame_id()            get_dvifid()
#define set_dvicmd(val)               i_s32(val, DVICMD,  0x0)
#define get_dvicmd()                  i_l32(DVICMD,  0x0)
#define get_dvi_buf_len()             get_dvicmd()
#define set_dvibr(val)                i_s32(val, DVIBR,  0x0)
#define get_dvibr()                   i_l32(DVIBR,  0x0)
#define DVI_BR_ENB                    0x80000000
#define enb_dvi_change_desp()         set_dvibr(get_dvibr() |   DVI_BR_ENB )
#define dis_dvi_change_desp()         set_dvibr(get_dvibr() & (~DVI_BR_ENB))
#define set_dvi_change_desp_num(N)    set_dvibr(get_dvibr() & (~0x7fffffff) | N)
#define get_dvi_change_desp_num()     get_dvibr()
#define set_dvbar(val)                i_s32(val, DVBAR,  0x0)
#define get_dvbar()                   i_l32(DVBAR,  0x0)
#define set_dvi_change_desp_addr(val) set_dvbar(val)
#define get_dvi_change_desp_addr()    get_dvbar()
#define set_dvbda(val)                i_s32(val, DVBDA,  0x0)
#define get_dvbda()                   i_l32(DVBDA,  0x0)
#define set_dvb_desp_addr(val)        set_dvbda(val)
#define get_dvb_desp_addr()           get_dvbda()
#define set_dvbsa(val)                i_s32(val, DVBSA,  0x0)
#define get_dvbsa()                   i_l32(DVBSA,  0x0)
#define get_dvb_src_addr()            get_dvbsa()
#define set_dvbfid(val)               i_s32(val, DVBFID,  0x0)
#define get_dvbfid()                  i_l32(DVBFID,  0x0)
#define get_dvb_frame_id()            get_dvbfid()
#define set_dvbcmd(val)               i_s32(val, DVBCMD,  0x0)
#define get_dvbcmd()                  i_l32(DVBCMD,  0x0)
#define get_dvb_buf_len()             get_dvbcmd()
#endif
#endif // __INGENIC_CIM_H__
