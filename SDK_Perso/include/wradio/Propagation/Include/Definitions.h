#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <complex>

#include "Config.h"

#ifdef DOUBLE_PRECISION
#include "osg/Vec3d"
#include "osg/Vec2d"
#include "osg/Quatd"
#include "osg/Matrixd"
#else
#include "osg/Vec3f"
#include "osg/Vec2f"
#include "osg/Quatf"
#include "osg/Matrixf"
#endif

namespace Radio
{

#ifdef DOUBLE_PRECISION
typedef double Real;
typedef osg::Vec3d Vec3;
typedef osg::Vec2d Vec2;
typedef osg::Quatd Quat;
typedef osg::Matrixd Matrix;
#else
typedef float Real;
typedef osg::Vec3f Vec3;
typedef osg::Vec2f Vec2;
typedef osg::Quatf Quat;
typedef osg::Matrixf Matrix;
#endif

typedef Real Angle; //rad
typedef Real Frequency; //Hz
typedef Real Distance; //m
typedef Distance Lambda; //m
typedef Distance Dist;
typedef Distance Height; //m
typedef Real Power; //Watt
typedef Real Dimless;
typedef Dimless Gain;
typedef std::complex<Dimless> ComplexGain;

const Real PI = osg::PI;
const Real PI_2 = osg::PI_2;
const Real PI_4 = osg::PI_4;

const Distance speedOfLight = 3e8;

inline Distance frequencyToLambda(Frequency frequency)
{
	return speedOfLight / frequency;
}

inline Distance lambdaToFrequency(Lambda lambda)
{
	return speedOfLight / lambda;
}

struct Signal
{
	Real lambda;			//wavelength, m
	Real polarization;
};

struct Path2D
{
	Height		h1;			//Height of the transmitter, m
	Height		h2;			//Height of the receiver, m
	Distance	d;			//horizontal distance from transmitter to receiver, m
};

class Antenna;

struct Scene
{
	Vec3 posTx;
	Quat quatTx;
	const Antenna * antennaTx;

	Vec3 posRx;
	Quat quatRx;
	const Antenna * antennaRx;

	Signal signal;
};

inline void fillScene(const Vec3 & posTx, const Vec3 & posRx, const Signal & signal, Scene & scene)
{
	scene.posTx = posTx;
	scene.antennaTx = NULL;
	scene.posRx = posRx;
	scene.antennaRx = NULL;
	scene.signal = signal;
}

inline void fillScene(	const Vec3 & posTx, const Quat & quatTx, const Antenna * antennaTx,
						const Vec3 & posRx, const Quat & quatRx, const Antenna * antennaRx,
						const Signal & signal,
						Scene & scene)
{
	scene.posTx = posTx;
	scene.antennaTx = antennaTx;
	scene.quatTx = quatTx;
	scene.posRx = posRx;
	scene.antennaRx = antennaRx;
	scene.quatRx = quatRx;
	scene.signal = signal;
}

inline void fillScene(const Matrix & matTx, const Antenna * antennaTx,
					  const Matrix & matRx, const Antenna * antennaRx,
					  const Signal & signal,
					  Scene & scene)
{
	scene.posTx = matTx.getTrans();
	scene.antennaTx = antennaTx;
	scene.quatTx.set(matTx);
	scene.posRx = matRx.getTrans();
	scene.antennaRx = antennaRx;
	scene.quatRx.set(matRx);
	scene.signal = signal;
}

template<typename T>
inline T dimlessToDb(T dimless)
{
	return 20.0 * log10(dimless);
}

template<typename T>
inline T dBToDimless(T db)
{
	return pow(10.0, db / 20.0);
}

}

#endif _DEFINITIONS_H_