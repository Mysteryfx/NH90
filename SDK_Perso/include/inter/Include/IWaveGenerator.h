#ifndef IWAVEGENERATOR_H
#define IWAVEGENERATOR_H

#include "Inter.h"
#include <osg/Vec2f>
#include <osg/Vec3f>

namespace PondsMath
{

const int phase = 500;
const double phaseDT = 0.02;

class IWaveGenerator
{
public:
    virtual void init(float lambda, unsigned int x_power, unsigned int y_power, float a, const osg::Vec2f &wind, float dx, float dy) = 0;

	virtual float *getH(double t) const = 0;
	virtual float *getX(double t) const = 0;
	virtual float *getZ(double t) const = 0;

    virtual osg::Vec3f getNormalAtPoint(double t, float x, float z) = 0;
	virtual float getHeightAtPoint(double t, float x, float z) = 0;
};

extern INTER_API IWaveGenerator *globalWaveGenerator;

}

#endif
