#ifndef _ed_assert_h_
#define _ed_assert_h_

#include "ed_log.h"
#include <assert.h>

#define ed_assert(a) {\
	assert(a);\
	if(!(a)){\
		ED_DEBUG("Failed assert at %s:%d", __FILE__, __LINE__);\
	}\
}

#endif
