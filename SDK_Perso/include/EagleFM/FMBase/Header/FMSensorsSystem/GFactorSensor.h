#pragma once

#include "Base.h"
#include "FMMathematics/math.h"

namespace EagleFM
{

class DynamicBody;

class FMBASE_API GFactorSensor
{
public:
	GFactorSensor(DynamicBody*);

	double			calcGFactor() const;


    Vec3			Pos_l;		// Координаты 

private:
	DynamicBody		*pDBody;
};


}