#ifndef __PistonEngineOptions_h__
#define __PistonEngineOptions_h__

#include "Base.h"

namespace Lua {
	class Config;
}

namespace EagleFM { 

class FMBASE_API PistonEngOptions
{
public:
	static void	init(Lua::Config& config);
	static double Tau_Res;
};

extern PistonEngOptions PistonEng_Options;
}

#endif
