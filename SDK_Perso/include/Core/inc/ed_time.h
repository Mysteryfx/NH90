#ifndef _ed_time_h_
#define _ed_time_h_

#include "./_config.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Returns time in seconds since program start.
ED_CORE_EXTERN
double ED_get_time(void);

ED_CORE_EXTERN
int64_t ED_get_ticks(void);

ED_CORE_EXTERN
int64_t ED_get_tick_rate(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ed_time_h_ */
