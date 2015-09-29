#ifndef __FMOptions_h__
#define __FMOptions_h__

#include "Base.h"

namespace EagleFM
{ 

namespace BaseOptions
{

//extern FMBASE_API int		HelicopterTrimmerMethod;	//1 - aperiodic process, 2 - joystick will be disable after trim until it come to neutral zone
class FMBASE_API BaseFMOptions
{
public:
	static ed::string mod_path_;
};

}

}

#endif
