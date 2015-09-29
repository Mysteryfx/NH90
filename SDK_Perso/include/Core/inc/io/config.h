#ifndef EDSIM_CONFIG_H
#define EDSIM_CONFIG_H

#if defined(_WIN32) || defined(_WIN64)
	#define EDSIM_DLL_EXPORT __declspec(dllexport)
	#define EDSIM_DLL_IMPORT __declspec(dllimport)
	#define ED_DLL_EXPORT __declspec(dllexport)
	#define ED_DLL_IMPORT __declspec(dllimport)
#else
	#define EDSIM_DLL_EXPORT
	#define EDSIM_DLL_IMPORT
	#define ED_DLL_EXPORT
	#define ED_DLL_IMPORT
#endif

//#if defined(_WIN32) || defined(_WIN64)
//	#include <malloc.h>
//	#define STACK_ALLOC(type, var, count) type *var = (type *)_alloca((count) * sizeof(type))
//#else
#ifndef STACK_ALLOC
	#define STACK_ALLOC(type, var, count) type var[count]
#endif

#endif // EDSIM_CONFIG_H

