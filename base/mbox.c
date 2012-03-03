
#include "smp.h"

//static
void mbox_ipi_entry(unsigned int mbox)
{
	unsigned int mnum;
	mnum = (mbox & 0xff) >>2;
	
}
