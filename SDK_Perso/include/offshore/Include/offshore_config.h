#ifndef _OFFSHORE_CONFIG_H__
#define _OFFSHORE_CONFIG_H__

#if defined(OFFSHORE_EXPORTS)
	#define OFFSHORE_API __declspec(dllexport)
#elif defined(OFFSHORE_STATIC)
	#define OFFSHORE_API
#else
	#define OFFSHORE_API __declspec(dllimport)
#endif


//#include "edterrain.h"

#include "io/File.h"

#include "ed/vector.h"
#include "ed/map.h"
#include "ed/list.h"

#endif
