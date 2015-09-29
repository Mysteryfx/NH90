#ifndef EDSIM_TIMER_H
#define EDSIM_TIMER_H

#include "_config.h"

#include <stdint.h>

#ifndef ED_CORE_EXPORT
#define ED_CORE_EXPORT EDSIM_DLL_IMPORT
#endif // ED_CORE_EXPORT

namespace ed
{

// fixed-point, 32b = one second
typedef uint64_t time_t;
const double TICKS_PER_SECOND = 1LL<<32;

// get seconds part from time_t
inline double getSeconds(time_t t)
{
	return t/TICKS_PER_SECOND;
}

// get real time (from some arbitrary starting point)
ED_CORE_EXPORT time_t getTime();

}

#endif // EDSIM_TIMER_H
