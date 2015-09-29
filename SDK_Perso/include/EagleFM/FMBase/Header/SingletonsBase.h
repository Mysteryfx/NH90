#ifndef __SingletonsBase_h__
#define __SingletonsBase_h__

#include "Base.h"

#include "FMSpace/IBasicAtmosphere.h"

struct lua_State;

//singletons of Base

namespace EagleFM
{

class FMBASE_API VersionControl
{
public:
	VersionControl() { VersionNum = 0; }
	void			setVersion(unsigned int V) { VersionNum = V; }
	unsigned int	getVersion() const { return VersionNum; }
	
private:
	unsigned int	VersionNum;
};

class FMBASE_API RandomizerSeed
{
public:
	RandomizerSeed() { Seed = 0; }
	void			setSeed(unsigned int S) { Seed = S; }
	unsigned int	getSeed() const { return Seed; }

private:
	unsigned int	Seed;
};

extern FMBASE_API VersionControl	*sVersion;
extern FMBASE_API RandomizerSeed	*sRandomSeed;
extern FMBASE_API IBasicAtmosphere	*sAtmosphere;
extern FMBASE_API lua_State			*sLuaState;

}

#endif