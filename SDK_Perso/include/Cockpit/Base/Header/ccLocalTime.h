#pragma once

#include "CockpitBase.h"
#include "wTime.h"

namespace cockpit {

class COCKPITBASE_API ccLocalTime
{
public:
	static void initialize();
	static wModelTime getLocalModelTime();
	static wModelTime getDeltaLocalTime();
    static void setLocalTime(wModelTime time);
	static void getMissionDate(unsigned int& day,unsigned int& month, unsigned int& year);

private:
	static wModelTime DeltaLocalTime;
};

}
