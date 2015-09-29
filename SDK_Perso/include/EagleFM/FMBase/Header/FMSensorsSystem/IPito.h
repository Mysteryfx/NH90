#ifndef IPITO_H
#define IPITO_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Base.h"

namespace EagleFM
{

class FMBASE_API IPitotStatic
{
public:

	virtual double	getAtmoPressure() const = 0;
	virtual double	getVerticalSpeed() const { return 0.0; }
	virtual ~IPitotStatic() {}
};
}

#endif
