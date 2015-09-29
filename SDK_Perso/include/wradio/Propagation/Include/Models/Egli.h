#ifndef _EGLI_H_
#define _EGLI_H_

#include "Environment.h"

//The Egli Model is a terrain model for radio frequency propagation
//http://en.wikipedia.org/wiki/Egli_Model

namespace Radio
{

class Egli: public Environment
{
public:
	Egli() {;}
	virtual Gain getGain(const Vec3 & from, const Vec3 & to, const Signal & signal) const
	{
		Height h1 = from.y();
		Height h2 = to.y();
		Distance d = (Vec2(to.x(), to.z()) - Vec2(from.x(), from.z())).length();
		Frequency fMhz = lambdaToFrequency(signal.lambda) / 1.0e6;
		return pow(h1 * h2 / d / d, 2.0) * pow(40.0 / fMhz, 2.0);
	}

};

}

#endif _EGLI_H_