#ifndef __Base_h__
#define __Base_h__

#ifdef FMBASE_EXPORTS
#define FMBASE_API __declspec(dllexport)
#else
#define FMBASE_API __declspec(dllimport)
#endif

#include <ed/string.h>


namespace EagleFM
{
    class IBasicAtmosphere;

	FMBASE_API bool		initialize();
	FMBASE_API void		uninitialize();
	FMBASE_API bool		setExternalAtmosphere(IBasicAtmosphere* atmosphere);
}// EagleFM


#endif
