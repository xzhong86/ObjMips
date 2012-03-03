
#ifndef __CUI_H__
#define __CUI_H__

#include <config.h>

#ifdef CONFIG_CUI
int cui_shell(void);
#else
static inline int cui_shell(void)
{
	return 0;
}
#endif

#endif
