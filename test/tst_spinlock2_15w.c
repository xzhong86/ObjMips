

#define COUNTER_NUM 14
#define ADD_TIMES   3

#define TEST_SIZE   (1024*1024)	/* L2 d-cache size * 2 */
#define LCNT_SIZE   ((COUNTER_NUM+1)*4)
#define LCNTS_NUM   (TEST_SIZE/LCNT_SIZE)

#define TEST_FUN  test_spinlock2_15w

#include "tst_spinlock2_model.c"
