
#ifndef INTC_H
#define INTC_H

int intc_init(void);
int intc_mask(unsigned int sr1,unsigned int sr0);
int intc_unmask(unsigned int sr1,unsigned int sr0);

int intc_get_no(void);

/* for tst_irq() */
int intc_tst_no(unsigned int sr1,unsigned int sr0);


#endif
