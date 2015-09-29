#ifndef __CommonTypes_h__
#define __CommonTypes_h__

#include "_config.h"

#ifdef COMMON_DLL
#define COMMON_API ED_CORE_EXPORT
#elif defined COMMON_STATIC
#define COMMON_API
#else
#define COMMON_API ED_CORE_IMPORT
#endif

#endif