#ifndef ATMOSPHERE_UTILITIEES_H
#define ATMOSPHERE_UTILITIEES_H

#include "Weather.h"
#include "Math/Math.h"

namespace Weather{

// double WEATHER_API pressureFromPascalsToMmHg(double pressureInPascales);
// double WEATHER_API pressureFromMmHgToPascals(double pressureInMmHg);

void WEATHER_API windVectorToVelocityAndDirection(Math::Vec3d wind, double& velocity, double& direction);

};

#endif//ATMOSPHERE_UTILITIEES_H