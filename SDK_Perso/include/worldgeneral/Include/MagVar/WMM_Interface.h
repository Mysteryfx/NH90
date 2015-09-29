#ifndef __WMM_INTERFACE_H__
#define __WMM_INTERFACE_H__

#include "WorldGeneral.h"

/************************************************************************/
/* World magnetic model       

	Functions for getting magnetic declination and inclination
	all over the world's ellipsoid.

	Before using please call the 'initialize' function. 
*/
/************************************************************************/

namespace magvar
{
	typedef float float_wmm;

	struct  WMM_Data
	{
		WMM_Data()
			: declination_(0.0), inclination_(0.0) {}

		WMM_Data(float_wmm declination, float_wmm inclination)
			: declination_(declination), inclination_(inclination) {}

		float_wmm declination_;
		float_wmm inclination_;
	};

	WORLDGENERAL_API float_wmm get_mag_decl(float_wmm latitude, float_wmm longitude);  /*latitude in degrees -180..180
																					   longitude in degrees -90..90*/
	WORLDGENERAL_API float_wmm get_mag_incl(float_wmm latitude, float_wmm longitude);  /*latitude in degrees -180..180  
																					   longitude in degrees -90..90*/
	WORLDGENERAL_API WMM_Data get_mag_decl_incl(float_wmm latitude, float_wmm longitude);  /*latitude in degrees -180..180  
																							longitude in degrees -90..90*/
	WORLDGENERAL_API void  initialize(int month, int year); /*month 1 - 12
															  year  'yyyy'*/
	WORLDGENERAL_API void  release();
}

#endif