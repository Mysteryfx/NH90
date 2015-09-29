#pragma once

#include "CockpitBase.h"
#include <ed/SounderAPI.h>

namespace cockpit {

class COCKPITBASE_API SounderHolder
{
public:
	void setSounder(ed::SounderRef sndrIn);
	ed::SounderRef& getSounder();

private:
	ed::SounderRef sounder;
};

extern COCKPITBASE_API SounderHolder sndHolder;

}
