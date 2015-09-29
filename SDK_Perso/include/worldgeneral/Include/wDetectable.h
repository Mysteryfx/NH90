#ifndef __wDetectable__
#define __wDetectable__

//Interface of object that can be detected visually, by optics (TV, IR) and by radar 

#include "WorldGeneral.h"

#include "cLinear.h"
#include "wTime.h"
#include "Math/Box.h"

class MovingObject;

static const float T_Basis = 400.0; //For airplane Kmax = 1.0;

class WORLDGENERAL_API wDetectable
{
public:
	wDetectable();
	virtual ~wDetectable();
	
	//Returns bounding box of the volume effect: smoke, dust tail, vapour tail
	virtual bool  getVolumeEffectBox(Math::Box2f * box = NULL) const; 

	//Returns strength of object lights
	virtual float getIllumination(const cVector & vec) const;

	//Returns strength of illumination from outside: fires, candle bombs
	virtual float getOuterSourceIllumination(MovingObject * pTarget) const;

	//Returns object temperature
	virtual float getTemperature(const cVector & vec) const;
	virtual float getTemperature(float cos_target_aspect_angle) const;

	//Returns object radar cross-section for given wavelength and local vector from
	virtual float getRCS(float lambda, const cVector & vec) const;

	//Returns minimal radar resolution at the point where the target is located to separate the target from clutter and other objects
	virtual float getTargetResolutionDistance(MovingObject * pTarget) const;
private:
	//Cached values with last update times
	mutable float outerSourceIllumination_;
	mutable wModelTime outerSourceIlluminationTime_;
	mutable float resolutionDistance_;
	mutable wModelTime resolutionDistanceTime_;
	mutable cPoint resolutionPoint_;
};

#endif __wDetectable__
