#ifndef _ED_TO_STRING_H_
#define _ED_TO_STRING_H_

#include <malloc.h>
#include <stdlib.h>

/// Warning! Return value is allocated on stack!
#define ed_itoa_stack(v, base) (_itoa(v, (char*)_alloca(sizeof(int) * 8 + 1), base))

#endif
