#ifndef __Detect_Utils__
#define __Detect_Utils__

#include "WorldGeneral.h"
#include "wTime.h"

#include "cLinear.h"

class viObjectShape;

namespace Graphics
{
	class Model;
}

float WORLDGENERAL_API getSunLightFactor();

bool WORLDGENERAL_API checkLOS(const cPoint & from, const cPoint & to);

float WORLDGENERAL_API getWeatherVisibility(float fromHeight, float toHeight);

bool WORLDGENERAL_API isVisible(const cPoint & from, const cPoint & to, float maxDistance);

float WORLDGENERAL_API getVisibleObjectRadius(viObjectShape * pObjectShape);

cPoint WORLDGENERAL_API getObjectTopPoint(viObjectShape * pObjectShape, wModelTime time = wTime::GetModelTime());

cPoint WORLDGENERAL_API getObjectCenterPoint(viObjectShape * pObjectShape, wModelTime time = wTime::GetModelTime());

cPoint WORLDGENERAL_API getObjectCenterPoint2(viObjectShape * pObjectShape, wModelTime time = wTime::GetModelTime());

bool WORLDGENERAL_API isVisible(viObjectShape * from, viObjectShape * to, float tanAngSizeMax);

float WORLDGENERAL_API getVisibility(const cPoint & from, viObjectShape * pObjectShape, float tanAngSizeMax);

float WORLDGENERAL_API	getRCS(Graphics::Model * model);

#endif