#pragma once

#include "Inter.h"
#include "cLinear.h"
//#include "dPosition.h"

class MovingObject;
enum SteeringPointMode
{
	//spmTarget = 0,
    spmGunner = 0,
    spmDriver
};
enum SteeringPointSubMode
{
    spsmTarget = 0,
    spsmHoldScreen,
    spsmPath
};

class INTER_API ICameraSteeringPoint
{
public:
	virtual ~ICameraSteeringPoint() {}

	//virtual dPosition getWorldPos() = 0;
    virtual dPoint getWorldPos() = 0;
	virtual dVector getWorldDir() = 0;
	
	virtual void getDiffAngles(const dVector& targDir, float& dYAngle, float& dZAngle) const = 0;

	virtual void reset(const cVector &dir, const SteeringPointMode spm = spmGunner, const SteeringPointSubMode spsm = spsmTarget) = 0;
    virtual void setWorldDir(const cVector &dir) = 0;
	virtual void move(float angleY, float angleZ) = 0;

	virtual void setHost(MovingObject*)= 0;
	virtual MovingObject* host() = 0;

	virtual SteeringPointMode mode() const = 0;
	virtual void setMode(SteeringPointMode)= 0;

    virtual SteeringPointSubMode subMode() const = 0;
    virtual void setSubMode(SteeringPointSubMode)= 0;
};

extern INTER_API const float maxSteeringIntersectionDist;
extern INTER_API ICameraSteeringPoint* globalCameraSteeringPoint;
