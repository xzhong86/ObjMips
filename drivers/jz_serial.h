


#define	UART0_BASE	0xB0030000
#define	UART1_BASE	0xB0031000
#define	UART2_BASE	0xB0032000
#define	UART3_BASE	0xB0033000



/*************************************************************************
 * UART
 *************************************************************************/

#define IRDA_BASE	UART0_BASE
#define UART_BASE	UART0_BASE
#define UART_OFF	0x1000

/* Register Offset */
#define OFF_RDR		(0x00)	/* R  8b H'xx */
#define OFF_TDR		(0x00)	/* W  8b H'xx */
#define OFF_DLLR	(0x00)	/* RW 8b H'00 */
#define OFF_DLHR	(0x04)	/* RW 8b H'00 */
#define OFF_IER		(0x04)	/* RW 8b H'00 */
#define OFF_ISR		(0x08)	/* R  8b H'01 */
#define OFF_FCR		(0x08)	/* W  8b H'00 */
#define OFF_LCR		(0x0C)	/* RW 8b H'00 */
#define OFF_MCR		(0x10)	/* RW 8b H'00 */
#define OFF_LSR		(0x14)	/* R  8b H'00 */
#define OFF_MSR		(0x18)	/* R  8b H'00 */
#define OFF_SPR		(0x1C)	/* RW 8b H'00 */
#define OFF_SIRCR	(0x20)	/* RW 8b H'00, UART0 */
#define OFF_UMR		(0x24)	/* RW 8b H'00, UART M Register */
#define OFF_UACR	(0x28)	/* RW 8b H'00, UART Add Cycle Register */

/* Register Address */
#define UART0_RDR	(UART0_BASE + OFF_RDR)
#define UART0_TDR	(UART0_BASE + OFF_TDR)
#define UART0_DLLR	(UART0_BASE + OFF_DLLR)
#define UART0_DLHR	(UART0_BASE + OFF_DLHR)
#define UART0_IER	(UART0_BASE + OFF_IER)
#define UART0_ISR	(UART0_BASE + OFF_ISR)
#define UART0_FCR	(UART0_BASE + OFF_FCR)
#define UART0_LCR	(UART0_BASE + OFF_LCR)
#define UART0_MCR	(UART0_BASE + OFF_MCR)
#define UART0_LSR	(UART0_BASE + OFF_LSR)
#define UART0_MSR	(UART0_BASE + OFF_MSR)
#define UART0_SPR	(UART0_BASE + OFF_SPR)
#define UART0_SIRCR	(UART0_BASE + OFF_SIRCR)
#define UART0_UMR	(UART0_BASE + OFF_UMR)
#define UART0_UACR	(UART0_BASE + OFF_UACR)

/*
 * Define macros for UART_IER
 * UART Interrupt Enable Register
 */
#define UART_IER_RIE	(1 << 0)	/* 0: receive fifo "full" interrupt disable */
#define UART_IER_TIE	(1 << 1)	/* 0: transmit fifo "empty" interrupt disable */
#define UART_IER_RLIE	(1 << 2)	/* 0: receive line status interrupt disable */
#define UART_IER_MIE	(1 << 3)	/* 0: modem status interrupt disable */
#define UART_IER_RTIE	(1 << 4)	/* 0: receive timeout interrupt disable */

/*
 * Define macros for UART_ISR
 * UART Interrupt Status Register
 */
#define UART_ISR_IP	(1 << 0)	/* 0: interrupt is pending  1: no interrupt */
#define UART_ISR_IID	(7 << 1)	/* Source of Interrupt */
#define UART_ISR_IID_MSI		(0 << 1)	/* Modem status interrupt */
#define UART_ISR_IID_THRI	(1 << 1)	/* Transmitter holding register empty */
#define UART_ISR_IID_RDI		(2 << 1)	/* Receiver data interrupt */
#define UART_ISR_IID_RLSI	(3 << 1)	/* Receiver line status interrupt */
#define UART_ISR_FFMS	(3 << 6)	/* FIFO mode select, set when UART_FCR.FE is set to 1 */
#define UART_ISR_FFMS_NO_FIFO	(0 << 6)
#define UART_ISR_FFMS_FIFO_MODE	(3 << 6)

/*
 * Define macros for UART_FCR
 * UART FIFO Control Register
 */
#define UART_FCR_FE	(1 << 0)	/* 0: non-FIFO mode  1: FIFO mode */
#define UART_FCR_RFLS	(1 << 1)	/* write 1 to flush receive FIFO */
#define UART_FCR_TFLS	(1 << 2)	/* write 1 to flush transmit FIFO */
#define UART_FCR_DMS	(1 << 3)	/* 0: disable DMA mode */
#define UART_FCR_UUE	(1 << 4)	/* 0: disable UART */
#define UART_FCR_RTRG	(3 << 6)	/* Receive FIFO Data Trigger */
#define UART_FCR_RTRG_1	(0 << 6)
#define UART_FCR_RTRG_4	(1 << 6)
#define UART_FCR_RTRG_8	(2 << 6)
#define UART_FCR_RTRG_15	(3 << 6)

/*
 * Define macros for UART_LCR
 * UART Line Control Register
 */
#define UART_LCR_WLEN	(3 << 0)	/* word length */
#define UART_LCR_WLEN_5	(0 << 0)
#define UART_LCR_WLEN_6	(1 << 0)
#define UART_LCR_WLEN_7	(2 << 0)
#define UART_LCR_WLEN_8	(3 << 0)
#define UART_LCR_STOP	(1 << 2)	/* 0: 1 stop bit when word length is 5,6,7,8
					   1: 1.5 stop bits when 5; 2 stop bits when 6,7,8 */
#define UART_LCR_STOP_1	(0 << 2)	/* 0: 1 stop bit when word length is 5,6,7,8
					   1: 1.5 stop bits when 5; 2 stop bits when 6,7,8 */
#define UART_LCR_STOP_2	(1 << 2)	/* 0: 1 stop bit when word length is 5,6,7,8
					   1: 1.5 stop bits when 5; 2 stop bits when 6,7,8 */

#define UART_LCR_PE	(1 << 3)	/* 0: parity disable */
#define UART_LCR_PROE	(1 << 4)	/* 0: even parity  1: odd parity */
#define UART_LCR_SPAR	(1 << 5)	/* 0: sticky parity disable */
#define UART_LCR_SBRK	(1 << 6)	/* write 0 normal, write 1 send break */
#define UART_LCR_DLAB	(1 << 7)	/* 0: access UART_RDR/TDR/IER  1: access UART_DLLR/DLHR */

/*
 * Define macros for UART_LSR
 * UART Line Status Register
 */
#define UART_LSR_DR	(1 << 0)	/* 0: receive FIFO is empty  1: receive data is ready */
#define UART_LSR_ORER	(1 << 1)	/* 0: no overrun error */
#define UART_LSR_PER	(1 << 2)	/* 0: no parity error */
#define UART_LSR_FER	(1 << 3)	/* 0; no framing error */
#define UART_LSR_BRK	(1 << 4)	/* 0: no break detected  1: receive a break signal */
#define UART_LSR_TDRQ	(1 << 5)	/* 1: transmit FIFO half "empty" */
#define UART_LSR_TEMT	(1 << 6)	/* 1: transmit FIFO and shift registers empty */
#define UART_LSR_RFER	(1 << 7)	/* 0: no receive error  1: receive error in FIFO mode */

/*
 * Define macros for UART_MCR
 * UART Modem Control Register
 */
#define UART_MCR_DTR	(1 << 0)	/* 0: DTR_ ouput high */
#define UART_MCR_RTS	(1 << 1)	/* 0: RTS_ output high */
#define UART_MCR_OUT1	(1 << 2)	/* 0: UART_MSR.RI is set to 0 and RI_ input high */
#define UART_MCR_OUT2	(1 << 3)	/* 0: UART_MSR.DCD is set to 0 and DCD_ input high */
#define UART_MCR_LOOP	(1 << 4)	/* 0: normal  1: loopback mode */
#define UART_MCR_MCE	(1 << 7)	/* 0: modem function is disable */

/*
 * Define macros for UART_MSR
 * UART Modem Status Register
 */
#define UART_MSR_DCTS	(1 << 0)	/* 0: no change on CTS_ pin since last read of UART_MSR */
#define UART_MSR_DDSR	(1 << 1)	/* 0: no change on DSR_ pin since last read of UART_MSR */
#define UART_MSR_DRI	(1 << 2)	/* 0: no change on RI_ pin since last read of UART_MSR */
#define UART_MSR_DDCD	(1 << 3)	/* 0: no change on DCD_ pin since last read of UART_MSR */
#define UART_MSR_CTS	(1 << 4)	/* 0: CTS_ pin is high */
#define UART_MSR_DSR	(1 << 5)	/* 0: DSR_ pin is high */
#define UART_MSR_RI	(1 << 6)	/* 0: RI_ pin is high */
#define UART_MSR_DCD	(1 << 7)	/* 0: DCD_ pin is high */

/*
 * Define macros for SIRCR
 * Slow IrDA Control Register
 */
#define SIRCR_TSIRE	(1 << 0)	/* 0: transmitter is in UART mode  1: IrDA mode */
#define SIRCR_RSIRE	(1 << 1)	/* 0: receiver is in UART mode  1: IrDA mode */
#define SIRCR_TPWS	(1 << 2)	/* 0: transmit 0 pulse width is 3/16 of bit length
					   1: 0 pulse width is 1.6us for 115.2Kbps */
#define SIRCR_TXPL	(1 << 3)	/* 0: encoder generates a positive pulse for 0 */
#define SIRCR_RXPL	(1 << 4)	/* 0: decoder interprets positive pulse as 0 */


#define	CPM_BASE	0xB0000000
/*************************************************************************
 * CPM (Clock reset and Power control Management)
 *************************************************************************/
#define CPM_CPCCR		(CPM_BASE+0x00) /* Clock control register		*/
#define CPM_CPPCR		(CPM_BASE+0x10) /* PLL control register 0		*/
#define CPM_CPPSR		(CPM_BASE+0x14) /* PLL switch and status Register	*/
#define CPM_CPPCR1		(CPM_BASE+0x30) /* PLL control register 1		*/
#define CPM_CPSPR		(CPM_BASE+0x34) /* CPM scratch pad register		*/
#define CPM_CPSPPR		(CPM_BASE+0x38) /* CPM scratch protected register	*/
#define CPM_USBPCR		(CPM_BASE+0x3c) /* USB parameter control register	*/
#define CPM_USBRDT		(CPM_BASE+0x40) /* USB reset detect timer register	*/
#define CPM_USBVBFIL		(CPM_BASE+0x44) /* USB jitter filter register		*/
#define CPM_USBCDR		(CPM_BASE+0x50) /* USB OTG PHY clock divider register	*/
#define CPM_I2SCDR		(CPM_BASE+0x60) /* I2S device clock divider register	*/
#define CPM_LPCDR		(CPM_BASE+0x64) /* LCD pix clock divider register	*/
#define CPM_MSCCDR		(CPM_BASE+0x68) /* MSC clock divider register		*/
#define CPM_UHCCDR		(CPM_BASE+0x6C) /* UHC 48M clock divider register	*/
#define CPM_SSICDR		(CPM_BASE+0x74) /* SSI clock divider register		*/
#define CPM_CIMCDR		(CPM_BASE+0x7c) /* CIM MCLK clock divider register	*/
#define CPM_GPSCDR		(CPM_BASE+0x80) /* GPS clock divider register		*/
#define CPM_PCMCDR		(CPM_BASE+0x84) /* PCM device clock divider register	*/
#define CPM_GPUCDR		(CPM_BASE+0x88) /* GPU clock divider register		*/

#define CPM_LCR			(CPM_BASE+0x04)
#define CPM_PSWCST(n)		(CPM_BASE+0x4*(n)+0x90)
#define CPM_CLKGR0		(CPM_BASE+0x20) /* Clock Gate Register0 */
#define CPM_CLKGR1		(CPM_BASE+0x28) /* Clock Gate Register1 */
#define CPM_OPCR		(CPM_BASE+0x24) /* Oscillator and Power Control Register */

#define CPM_RSR			(CPM_BASE+0x08)


#define REG_CPM_CPCCR		REG32(CPM_CPCCR)
#define REG_CPM_CPPCR		REG32(CPM_CPPCR)
#define REG_CPM_CPPSR		REG32(CPM_CPPSR)
#define REG_CPM_CPPCR1		REG32(CPM_CPPCR1)
#define REG_CPM_CPSPR		REG32(CPM_CPSPR)
#define REG_CPM_CPSPPR		REG32(CPM_CPSPPR)
#define REG_CPM_USBPCR		REG32(CPM_USBPCR)
#define REG_CPM_USBRDT		REG32(CPM_USBRDT)
#define REG_CPM_USBVBFIL	REG32(CPM_USBVBFIL)
#define REG_CPM_USBCDR		REG32(CPM_USBCDR)
#define REG_CPM_I2SCDR		REG32(CPM_I2SCDR)
#define REG_CPM_LPCDR		REG32(CPM_LPCDR)
#define REG_CPM_MSCCDR		REG32(CPM_MSCCDR)
#define REG_CPM_UHCCDR		REG32(CPM_UHCCDR)
#define REG_CPM_SSICDR		REG32(CPM_SSICDR)
#define REG_CPM_CIMCDR		REG32(CPM_CIMCDR)
#define REG_CPM_GPSCDR		REG32(CPM_GPSCDR)
#define REG_CPM_PCMCDR		REG32(CPM_PCMCDR)
#define REG_CPM_GPUCDR		REG32(CPM_GPUCDR)

#define REG_CPM_LCR	REG32(CPM_LCR)
#define REG_CPM_CLKGR0	REG32(CPM_CLKGR0)
#define REG_CPM_CLKGR1	REG32(CPM_CLKGR1)
#define REG_CPM_OPCR	REG32(CPM_OPCR)

#define REG_CPM_RSR	REG32(CPM_RSR)
/* Clock control register */
#define CPM_CPCCR_ECS			(0x01 << 31)
#define CPM_CPCCR_MEM			(0x01 << 30)
#define CPM_CPCCR_SDIV_BIT		24
#define CPM_CPCCR_SDIV_MASK		(0x0f << CPM_CPCCR_SDIV_BIT)
#define CPM_CPCCR_CE			(0x01 << 22)
#define CPM_CPCCR_PCS			(0x01 << 21)
#define CPM_CPCCR_H2DIV_BIT		16
#define CPM_CPCCR_H2DIV_MASK		(0x0f << CPM_CPCCR_H2DIV_BIT)
#define CPM_CPCCR_MDIV_BIT		12
#define CPM_CPCCR_MDIV_MASK		(0x0f << CPM_CPCCR_MDIV_BIT)
#define CPM_CPCCR_PDIV_BIT		8
#define CPM_CPCCR_PDIV_MASK		(0x0f << CPM_CPCCR_PDIV_BIT)
#define CPM_CPCCR_HDIV_BIT		4
#define CPM_CPCCR_HDIV_MASK		(0x0f << CPM_CPCCR_HDIV_BIT)
#define CPM_CPCCR_CDIV_BIT		0
#define CPM_CPCCR_CDIV_MASK		(0x0f << CPM_CPCCR_CDIV_BIT)

/* PLL control register 0 */
#define CPM_CPPCR_PLLM_BIT		24
#define CPM_CPPCR_PLLM_MASK		(0x7f << CPM_CPPCR_PLLM_BIT)
#define CPM_CPPCR_PLLN_BIT		18
#define CPM_CPPCR_PLLN_MASK		(0x0f << CPM_CPPCR_PLLN_BIT)
#define CPM_CPPCR_PLLOD_BIT		16
#define CPM_CPPCR_PLLOD_MASK		(0x03 << CPM_CPPCR_PLLOD_BIT)
#define CPM_CPPCR_LOCK0			(1 << 15)
#define CPM_CPPCR_ENLOCK		(1 << 14)
#define CPM_CPPCR_PLLS			(1 << 10)
#define CPM_CPPCR_PLLBP			(1 << 9)
#define CPM_CPPCR_PLLEN			(1 << 8)
#define CPM_CPPCR_PLLST_BIT		0
#define CPM_CPPCR_PLLST_MASK		(0xff << CPM_CPPCR_PLLST_BIT)

/* PLL control register 1 */
#define CPM_CPPCR1_PLL1M_BIT		24
#define CPM_CPPCR1_PLL1M_MASK		(0x7f << CPM_CPPCR1_PLL1M_BIT)
#define CPM_CPPCR1_PLL1N_BIT		18
#define CPM_CPPCR1_PLL1N_MASK		(0x0f << CPM_CPPCR1_PLL1N_BIT)
#define CPM_CPPCR1_PLL1OD_BIT		16
#define CPM_CPPCR1_PLL1OD_MASK		(0x03 << CPM_CPPCR1_PLL1OD_BIT)
#define CPM_CPPCR1_P1SCS		(1 << 15)
#define CPM_CPPCR1_P1SDIV_BIT		9
#define CPM_CPPCR1_P1SDIV_MASK		(0x3f << CPM_CPPCR1_P1SDIV_BIT)
#define CPM_CPPCR1_PLL1EN		(1 << 7)
#define CPM_CPPCR1_PLL1S		(1 << 6)
#define CPM_CPPCR1_LOCK1		(1 << 2)
#define CPM_CPPCR1_PLL1OFF		(1 << 1)
#define CPM_CPPCR1_PLL1ON		(1 << 0)

/* PLL switch and status Register */
#define CPM_CPPSR_PLLOFF		(1 << 31)
#define CPM_CPPSR_PLLBP			(1 << 30)
#define CPM_CPPSR_PLLON			(1 << 29)
#define CPM_CPPSR_PS			(1 << 28)
#define CPM_CPPSR_FS			(1 << 27)
#define CPM_CPPSR_CS			(1 << 26)
#define CPM_CPPSR_SM			(1 << 2)
#define CPM_CPPSR_PM			(1 << 1)
#define CPM_CPPSR_FM			(1 << 0)

/* CPM scratch protected register */
#define CPM_CPSPPR_BIT			0
#define CPM_CPSPPR_MASK			(0xffff << CPM_CPSPPR_BIT)

/* USB parameter control register */
#define CPM_USBPCR_USB_MODE		(1 << 31)  /* 1: OTG, 0: UDC*/
#define CPM_USBPCR_AVLD_REG		(1 << 30)
#define CPM_USBPCR_IDPULLUP_MASK_BIT	28
#define CPM_USBPCR_IDPULLUP_MASK_MASK	(0x02 << IDPULLUP_MASK_BIT)
#define CPM_USBPCR_INCR_MASK		(1 << 27)
#define CPM_USBPCR_CLK12_EN		(1 << 26)
#define CPM_USBPCR_COMMONONN		(1 << 25)
#define CPM_USBPCR_VBUSVLDEXT		(1 << 24)
#define CPM_USBPCR_VBUSVLDEXTSEL	(1 << 23)
#define CPM_USBPCR_POR			(1 << 22)
#define CPM_USBPCR_SIDDQ		(1 << 21)
#define CPM_USBPCR_OTG_DISABLE		(1 << 20)
#define CPM_USBPCR_COMPDISTUNE_BIT	17
#define CPM_USBPCR_COMPDISTUNE_MASK	(0x07 << COMPDISTUNE_BIT)
#define CPM_USBPCR_OTGTUNE_BIT		14
#define CPM_USBPCR_OTGTUNE_MASK		(0x07 << OTGTUNE_BIT)
#define CPM_USBPCR_SQRXTUNE_BIT		11
#define CPM_USBPCR_SQRXTUNE_MASK	(0x7x << SQRXTUNE_BIT)
#define CPM_USBPCR_TXFSLSTUNE_BIT	7
#define CPM_USBPCR_TXFSLSTUNE_MASK	(0x0f << TXFSLSTUNE_BIT)
#define CPM_USBPCR_TXPREEMPHTUNE	(1 << 6)
#define CPM_USBPCR_TXRISETUNE_BIT	4
#define CPM_USBPCR_TXRISETUNE_MASK	(0x03 << TXRISETUNE_BIT)
#define CPM_USBPCR_TXVREFTUNE_BIT	0
#define CPM_USBPCR_TXVREFTUNE_MASK	(0x0f << TXVREFTUNE_BIT)

/* USB reset detect timer register */
#define CPM_USBRDT_VBFIL_LD_EN		(1 << 25)
#define CPM_USBRDT_IDDIG_EN		(1 << 24)
#define CPM_USBRDT_IDDIG_REG		(1 << 23)
#define CPM_USBRDT_USBRDT_BIT		0
#define CPM_USBRDT_USBRDT_MASK		(0x7fffff << CPM_USBRDT_USBRDT_BIT)

/* USB OTG PHY clock divider register */
#define CPM_USBCDR_UCS			(1 << 31)
#define CPM_USBCDR_UPCS			(1 << 30)
#define CPM_USBCDR_OTGDIV_BIT		0
#define CPM_USBCDR_OTGDIV_MASK		(0x3f << CPM_USBCDR_OTGDIV_BIT)

/* I2S device clock divider register */
#define CPM_I2SCDR_I2CS			(1 << 31)
#define CPM_I2SCDR_I2PCS		(1 << 30)
#define CPM_I2SCDR_I2SDIV_BIT		0
#define CPM_I2SCDR_I2SDIV_MASK		(0x1ff << CPM_I2SCDR_I2SDIV_BIT)

/* LCD pix clock divider register */
#define CPM_LPCDR_LSCS			(1 << 31)
#define CPM_LPCDR_LTCS			(1 << 30)
#define CPM_LPCDR_LPCS			(1 << 29)
#define CPM_LPCDR_PIXDIV_BIT		0
#define CPM_LPCDR_PIXDIV_MASK		(0x7ff << CPM_LPCDR_PIXDIV_BIT)

/* MSC clock divider register */
#define CPM_MSCCDR_MCS			(1 << 31)
#define CPM_MSCCDR_MSCDIV_BIT		0
#define CPM_MSCCDR_MSCDIV_MASK		(0x3f << CPM_MSCCDR_PIXDIV_BIT)

/* UHC 48M clock divider register */
#define CPM_UHCCDR_UHPCS		(1 << 31)
#define CPM_UHCCDR_UHCDIV_BIT		0
#define CPM_UHCCDR_UHCDIV_MASK		(0xf << CPM_UHCCDR_UHCDIV_BIT)

/* SSI clock divider register */
#define CPM_SSICDR_SCS			(1 << 31)
#define CPM_SSICDR_SSIDIV_BIT		0
#define CPM_SSICDR_SSIDIV_MASK		(0x3f << CPM_SSICDR_SSIDIV_BIT)

/* CIM MCLK clock divider register */
#define CPM_CIMCDR_CIMDIV_BIT		0
#define CPM_CIMCDR_CIMDIV_MASK		(0xff << CPM_CIMCDR_CIMDIV_BIT)

/* GPS clock divider register */
#define CPM_GPSCDR_GPCS			(1 << 31)
#define CPM_GPSCDR_GPSDIV_BIT		0
#define CPM_GSPCDR_GPSDIV_MASK		(0xf << CPM_GPSCDR_GPSDIV_BIT)

/* PCM device clock divider register */
#define CPM_PCMCDR_PCMS			(1 << 31)
#define CPM_PCMCDR_PCMPCS		(1 << 30)
#define CPM_CPCCR1_P1SDIV_BIT		9
#define CPM_CPCCR1_P1SDIV_MASK		(0x3f << CPM_CPCCR1_P1SDIV_BIT)
#define CPM_PCMCDR_PCMDIV_BIT		0
#define CPM_PCMCDR_PCMDIV_MASK		(0x1ff << CPM_PCMCDR_PCMDIV_BIT)

/* GPU clock divider register */
#define CPM_GPUCDR_GPCS			(1 << 31)
#define CPM_GPUCDR_GPUDIV_BIT		0
#define CPM_GPUCDR_GPUDIV_MASK		(0x7 << CPM_GPUCDR_GPUDIV_BIT)

/* Low Power Control Register */
#define CPM_LCR_PD_AHB1		(1 << 30)
#define CPM_LCR_VBAT_IR		(1 << 29)
#define CPM_LCR_PD_GPS		(1 << 28)
#define CPM_LCR_PD_AHB1S	(1 << 26)
#define CPM_LCR_PD_GPSS		(1 << 24)
#define CPM_LCR_PST_BIT 	8
#define CPM_LCR_PST_MASK 	(0xfff << CPM_LCR_PST_BIT)
#define CPM_LCR_DOZE_DUTY_BIT 	3
#define CPM_LCR_DOZE_DUTY_MASK 	(0x1f << CPM_LCR_DOZE_DUTY_BIT)
#define CPM_LCR_DOZE_ON		(1 << 2)
#define CPM_LCR_LPM_BIT		0
#define CPM_LCR_LPM_MASK	(0x3 << CPM_LCR_LPM_BIT)
  #define CPM_LCR_LPM_IDLE	(0x0 << CPM_LCR_LPM_BIT)
  #define CPM_LCR_LPM_SLEEP	(0x1 << CPM_LCR_LPM_BIT)

/* Clock Gate Register0 */
#define CPM_CLKGR0_EMC		(1 << 31)
#define CPM_CLKGR0_DDR  	(1 << 30)
#define CPM_CLKGR0_IPU    	(1 << 29)
#define CPM_CLKGR0_LCD		(1 << 28)
#define CPM_CLKGR0_TVE  	(1 << 27)
#define CPM_CLKGR0_CIM    	(1 << 26)
#define CPM_CLKGR0_MDMA    	(1 << 25)
#define CPM_CLKGR0_UHC    	(1 << 24)
#define CPM_CLKGR0_MAC    	(1 << 23)
#define CPM_CLKGR0_GPS    	(1 << 22)
#define CPM_CLKGR0_DMAC    	(1 << 21)
#define CPM_CLKGR0_SSI2    	(1 << 20)
#define CPM_CLKGR0_SSI1    	(1 << 19)
#define CPM_CLKGR0_UART3    	(1 << 18)
#define CPM_CLKGR0_UART2    	(1 << 17)
#define CPM_CLKGR0_UART1    	(1 << 16)
#define CPM_CLKGR0_UART0	(1 << 15)
#define CPM_CLKGR0_SADC		(1 << 14)
#define CPM_CLKGR0_KBC		(1 << 13)
#define CPM_CLKGR0_MSC2		(1 << 12)
#define CPM_CLKGR0_MSC1		(1 << 11)
#define CPM_CLKGR0_OWI		(1 << 10)
#define CPM_CLKGR0_TSSI		(1 << 9)
#define CPM_CLKGR0_AIC		(1 << 8)
#define CPM_CLKGR0_SCC		(1 << 7)
#define CPM_CLKGR0_I2C1		(1 << 6)
#define CPM_CLKGR0_I2C0		(1 << 5)
#define CPM_CLKGR0_SSI0		(1 << 4)
#define CPM_CLKGR0_MSC0		(1 << 3)
#define CPM_CLKGR0_OTG		(1 << 2)
#define CPM_CLKGR0_BCH		(1 << 1)
#define CPM_CLKGR0_NEMC		(1 << 0)

/* Clock Gate Register1 */
#define CPM_CLKGR1_GPU		(1 << 9)
#define CPM_CLKGR1_PCM		(1 << 8)
#define CPM_CLKGR1_AHB1		(1 << 7)
#define CPM_CLKGR1_CABAC	(1 << 6)
#define CPM_CLKGR1_SRAM		(1 << 5)
#define CPM_CLKGR1_DCT		(1 << 4)
#define CPM_CLKGR1_ME		(1 << 3)
#define CPM_CLKGR1_DBLK		(1 << 2)
#define CPM_CLKGR1_MC		(1 << 1)
#define CPM_CLKGR1_BDMA		(1 << 0)

/* Oscillator and Power Control Register */
#define CPM_OPCR_O1ST_BIT	8
#define CPM_OPCR_O1ST_MASK	(0xff << CPM_OPCR_O1ST_BIT)
#define CPM_OPCR_SPENDN		(1 << 7)
#define CPM_OPCR_GPSEN		(1 << 6)
#define CPM_OPCR_SPENDH		(1 << 5)
#define CPM_OPCR_O1SE		(1 << 4) /* */
#define CPM_OPCR_ERCS           (1 << 2) /* 0: select EXCLK/512 clock, 1: RTCLK clock */
#define CPM_OPCR_USBM           (1 << 0) /* 0: select EXCLK/512 clock, 1: RTCLK clock */


/* Reset Status Register */
#define CPM_RSR_P0R		(1 << 2)
#define CPM_RSR_WR		(1 << 1)
#define CPM_RSR_PR		(1 << 0)
