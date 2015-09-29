#pragma once
#include "_config.h"

class ISpline
{
public:
	virtual ~ISpline() {}
	virtual void	set(float knot0, float tangent0, float knot1, float tangent1) = 0;
	virtual float	getValue(float t) = 0;
	virtual float	getSpeed(float t) = 0;	
};

//Катмул-Ром
class CatmullRomSpline: public ISpline
{
	float a,b,c,d;
public:
	CatmullRomSpline(){}
	ED_CORE_EXTERN CatmullRomSpline(float knot0, float tangent0, float knot1, float tangent1);
	ED_CORE_EXTERN virtual void	set(float knot0, float tangent0, float knot1, float tangent1);
	ED_CORE_EXTERN virtual float	getValue(float t);
	ED_CORE_EXTERN virtual float	getSpeed(float t);
};