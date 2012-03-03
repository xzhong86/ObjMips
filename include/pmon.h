#ifndef PMON_H
#define PMON_H

#include "mipsop.h"

#define get_pmon_csr()		__read_32bit_c0_register($16, 7)
#define set_pmon_csr(val)	__write_32bit_c0_register($16, 7, val)

#define get_pmon_high()		__read_32bit_c0_register($16, 4)
#define set_pmon_high(val)	__write_32bit_c0_register($16, 4, val)
#define get_pmon_lc()		__read_32bit_c0_register($16, 5)
#define set_pmon_lc(val)	__write_32bit_c0_register($16, 5, val)
#define get_pmon_rc()		__read_32bit_c0_register($16, 6)
#define set_pmon_rc(val)	__write_32bit_c0_register($16, 6, val)

#define pmon_clear_cnt() do {			\
		set_pmon_high(0);		\
		set_pmon_lc(0);			\
		set_pmon_rc(0);			\
	} while(0)

#define pmon_start() do {			\
		unsigned int csr;		\
		csr = get_pmon_csr();		\
		csr |= 0x100;			\
		set_pmon_csr(csr);		\
	} while(0)
#define pmon_stop() do {			\
		unsigned int csr;		\
		csr = get_pmon_csr();		\
		csr &= ~0x100;			\
		set_pmon_csr(csr);		\
	} while(0)

#define PMON_EVENT_CYCLE 0
#define PMON_EVENT_CACHE 1
#define PMON_EVENT_INST  2

#define pmon_prepare(event) do {		\
		unsigned int csr;		\
		pmon_stop();			\
		pmon_clear_cnt();		\
		csr = get_pmon_csr();		\
		csr &= ~0xf000;			\
		csr |= (event)<<12;		\
		set_pmon_csr(csr);		\
	} while(0)

int _pmon_report(int event);
#define pmon_report() do {			\
		unsigned int event;		\
		event = get_pmon_csr();		\
		event = event>>12 & 0xf;	\
		_pmon_report(event);		\
	} while(0)


#define pmon_get_cnt32(lc,rc) do {		\
		lc = get_pmon_lc();		\
		rc = get_pmon_rc();		\
	} while(0)
#define pmon_get_cnt64(lc,rc) do {			\
		unsigned int h,l,r;			\
		h = get_pmon_high();			\
		l = get_pmon_lc();			\
		r = get_pmon_rc();			\
		lc = ((long long)h>>16)<<48;		\
		rc = ((long long)h&0xffff)<<32;		\
		lc += l;				\
		rc += r;				\
	} while(0)




#define pmon_btb_invalid() do {			\
		unsigned int csr;		\
		csr = get_pmon_csr();		\
		csr |= 0x2;			\
		set_pmon_csr(csr);		\
	} while(0)

#define pmon_btb_enable() do {			\
		unsigned int csr;		\
		csr = get_pmon_csr();		\
		csr &= ~0x1;			\
		set_pmon_csr(csr);		\
	} while(0)

#define pmon_btb_disable() do {			\
		pmon_btb_invalid();		\
		unsigned int csr;		\
		csr = get_pmon_csr();		\
		csr |= 0x1;			\
		set_pmon_csr(csr);		\
	} while(0)

#endif
