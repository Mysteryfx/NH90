#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "Definitions.h"

namespace Radio
{


enum EnvironmentFlags
{
	FLAG_ZERO			= 0,
	FLAG_DIFFRACTION	= 1 << 0,
	FLAG_INTERFERENTION	= 1 << 1,
	FLAG_ALL			= FLAG_DIFFRACTION | FLAG_INTERFERENTION
};

class Environment
{
public:
	typedef EnvironmentFlags Flags;
	virtual ~Environment() {;}
	virtual Gain getGain(const Scene & scene) const = 0; // V = Er / sqrt(Pt * Gt(...) * Gr(...))
};

}

#endif _ENVIRONMENT_H_