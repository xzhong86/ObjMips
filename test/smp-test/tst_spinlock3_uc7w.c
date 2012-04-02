

#define COUNTER_NUM 6
#define ADD_TIMES   8

#define TEST_SIZE   (1024*1024)	/* L2 d-cache size * 2 */
#define LCNT_SIZE   ((COUNTER_NUM+1)*4)
#define LCNTS_NUM   (TEST_SIZE/LCNT_SIZE)

#define ATTR_UNCAC 1
#define ATTR_USR   2
//#define CNT_ATTR { 0, 1, 2, 0, 1, 2 }
#define CNT_ATTR { 0, 1, 1, 0, 1, 0 }

#define SET_START 0
#define SET_END   0

#define TEST_FUN  test_spinlock3_uc7w

#include "tst_spinlock3_model.c"
