#ifndef _FREE_SPACE_H_
#define _FREE_SPACE_H_

#include "Definitions.h"

namespace Radio
{

inline Real getPhase(Distance distance, Lambda lambda)
{
	return 2.0 * PI * fmod(distance, lambda) / lambda;
}

inline Gain getFreeSpaceGain(Distance distance)
{
	const Real sqrt30 = 5.477;
	return sqrt30 / distance;
}

inline ComplexGain getComplexGain(Distance distance, Lambda lambda)
{
	return std::polar(getFreeSpaceGain(distance), getPhase(distance, lambda));
}

}

#endif _FREE_SPACE_H_