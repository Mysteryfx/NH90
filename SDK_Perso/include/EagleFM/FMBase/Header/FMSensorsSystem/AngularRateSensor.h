#pragma once

#include "Base.h"
#include "FMMathematics/math.h"

namespace EagleFM
{

class DynamicBody;

//Датчик угловой скорости
class FMBASE_API AngularRateSensor
{
public:
	AngularRateSensor(DynamicBody*, double Azimuth_l = 0.0,  double Elevation_l = 0.0);

	void			setDirection(double Azimuth_l, double Elevation_l);

	double			calcAngularRate() const;


private:
	DynamicBody		*pDBody;
	Vec3			Direction; //направление
};


}