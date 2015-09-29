#ifndef __BLOCK_SIM_H__
#define __BLOCK_SIM_H__

#ifndef BLOCKSIM_LIB
	#ifdef BLOCKSIM_DLL
		#define BLOCKSIM_API __declspec(dllexport)
	#else
		#define BLOCKSIM_API __declspec(dllimport)
	#endif
#else
	#define BLOCKSIM_API
#endif

#endif