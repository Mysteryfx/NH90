#ifndef RENDERER_FOG_H
#define RENDERER_FOG_H

#include "osg/Vec4f"
#include "osg/Vec3d"
#include <stdint.h>

namespace edcg
{

class Fog
{
public:
	// fmb support
	static const uint32_t packetType = 3;//fmb::RT_SETFOG;
	static const uint32_t packetVersion = 0;
protected:
	double _distance;
	double _layerHeight, _layerDist;
	
	/// Holds coefficents for distance fog.
	/// Coefficients are alpha, beta for fog by distance and alpha, beta for layered fog.
	osg::Vec4f _coefs;

	inline void calcCoefs();

	inline double calcAtt(double a, double b, double camHeight, double dist, double cosEta)const;
public:
	inline Fog();
	inline ~Fog();

	/// Sets distance at which attenuation equals to 0.001. 
	inline void setDistance(double dist);
	
	/// Sets layered fog parameters.
	inline void setLayer(double height, double dist);

	/// Sets layered fog higher boundary.
	inline void setLayerHeight(double height);
	
	/// Sets layered fog strength.
	inline void setLayerDistance(double dist);

	/// Returns distance at which attenuation equals to 0.00001.
	inline double getDistance()const;
	
	/// Returns layered fog parameters.
	inline void getLayer(double& height, double& dist)const;
	/// Returns layered fog parameters.
	inline void getLayer(float& height, float& dist)const;

	/// Returns height of layered fog.
	inline double getLayerHeight()const;

	/// Coefficients are Br.r, Br.g, Br.b, Bm for both distance and layered fogs.
	inline const osg::Vec4f& getCoefficients()const;

	/// Calculates attenuation coefficient due to fog.
	/// Useful to detect render object or not.
	inline double calculateAttenuation(const osg::Vec3d &cameraPos, const osg::Vec3d &objPos)const;
};

}

#include "Fog.inl"

#endif
