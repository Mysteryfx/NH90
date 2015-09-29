#ifndef _CannonLight_h_
#define _CannonLight_h_

#include "ObjectLight.h"


namespace Effects
{

// источник света для скорострельных пушек
class CannonLight : public ObjectLight
{
public:

	// выставляет все параметры источника в зависимости от мощности пушки
	virtual void SetCannonPower(float power) = 0;

	// время угасания
	virtual void SetFading(float t) = 0;

	// сигнал о выстреле
	virtual void Shot() = 0;


};

} // namespace Effects

#endif // _CannonLight_h_
