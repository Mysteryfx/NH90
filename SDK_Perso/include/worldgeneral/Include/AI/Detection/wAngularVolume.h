#ifndef _wAngularVolume_h_
#define _wAngularVolume_h_

#include "cPosition.h"

namespace Lua
{
	class Config;
}

class wAngularVolume
{
public:
	virtual ~wAngularVolume();
	virtual void load(Lua::Config & config) = 0;
	virtual bool checkVectorPolar(float azimuth, float elevation) const = 0;
	virtual bool checkVectorLinear(const cVector & pos) const = 0;
	virtual float getAngularSquare() const = 0;
	inline float getScanTime(float scan_rate) const { return getAngularSquare() / scan_rate; }	
	//
	//wAngularVolume operator *(float magnification) const;
	//wAngularVolume operator /(float magnification) const;
	//float operator /(const wAngularVolume & angular_volume) const;
};

class wAngularVolumePyramid : public wAngularVolume
{
public:
	wAngularVolumePyramid();
	wAngularVolumePyramid(float halfAzimuthWidth, float halfElevationWidth);
	void load(Lua::Config & config);
	void set(float halfAzimuthWidth, float halfElevationWidth);
	//
	virtual bool checkVectorPolar(float azimuth, float elevation) const;
	virtual bool checkVectorLinear(const cVector & pos) const;
	virtual float getAngularSquare() const;
	//
	wAngularVolumePyramid operator *(float magnification) const;
	wAngularVolumePyramid operator /(float magnification) const;
	float operator /(const wAngularVolumePyramid & angular_volume) const;
protected:
	inline bool checkVector_(float cosAz, float sinElev) const { return cosAz > cosHalfAzimuthWidth && sinElev < sinHalfElevationWidth; }
	//virtual float getRelativePolarPos(float azimuth, float elevation, float * p_azimuth = NULL, float * p_elevation = NULL) const;		
	//virtual float getRelativeAngularPos(const cVector & pos, float * p_azimuth = NULL, float * p_elevation = NULL) const;
	float cosHalfAzimuthWidth;
	float sinHalfElevationWidth;
	float square;
};

class wAngularVolumePyramidRot : public wAngularVolumePyramid
{
public:
	wAngularVolumePyramidRot();
	void load(Lua::Config & config);
	void set(float azimuth0, float elevation0, float halfAzimuthWidth, float halfElevationWidth);
	//
	virtual bool checkVectorPolar(float azimuth, float elevation) const;
	virtual bool checkVectorLinear(const cVector & pos) const;
private:
	cPosition pos_;
};

class wAngularVolumeMinElev : public wAngularVolume
{
public:
	enum { SIZE = 5 };
	wAngularVolumeMinElev();
	void load(Lua::Config & config);
	void set(const float (&sinMinElevByCosAz)[SIZE]);
	//
	virtual bool checkVectorPolar(float azimuth, float elevation) const;
	virtual bool checkVectorLinear(const cVector & pos) const;
	virtual float getAngularSquare() const;
private:
	bool checkVector_(float cosAz, float sinElev) const;
	static const float cosAzimuthStep_;
	float sinMinElevByCosAz_[SIZE];
	float square_;
};

void getPosition(cPosition & pos, const cPoint & start_point, const cPoint & end_point);

#endif _wAngularVolume_h_