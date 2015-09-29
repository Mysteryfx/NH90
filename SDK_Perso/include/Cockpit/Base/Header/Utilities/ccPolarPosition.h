#pragma once
#include "CockpitBase.h"
#include "dPosition.h"
#include "GraphMath/position.h"

namespace cockpit
{

template <class V3>
cVector  to_local_cs(const cPosition & m, const V3 & p)
{
	cVector r;
	mult_transp(r,m,cVector(p.x - m.p.x,
							p.y - m.p.y,
							p.z - m.p.z));
	return r;
}

template <class V3>
dVector  to_local_cs(const dPosition & m, const V3 & p)
{
	dVector r;
	mult_transp(r,m,dVector(p.x - m.p.x,
							p.y - m.p.y,
							p.z - m.p.z));
	return r;
}

template <class V3>
Vector3 to_local_cs(const Position3 & m, const V3 & p)
{
	return toVec3(p) * (~m);
}

class COCKPITBASE_API PolarPosition
{
private:
	template <class V3>	
	void set_from_vector(const V3 & r)
	{
		distance  = sqrt(r.x * r.x + r.y * r.y + r.z * r.z); 
		azimuth   = atan2(r.z,r.x);
		elevation = atan2(r.y,sqrt(r.x*r.x + r.z*r.z));
	}   

public:
    PolarPosition():distance(0),azimuth(0),elevation(0){};
    PolarPosition(double d,double a,double e):distance(d),azimuth(a),elevation(e){};
    PolarPosition(const PolarPosition &p) {	*this = p;};

	template <class M_XYZP,class V3>
    PolarPosition(const M_XYZP &m,const V3 &p)
    {
		set_from_vector(to_local_cs(m,p));
    };
   
	template <class V3>	
	PolarPosition(const V3 &r)
    {
		set_from_vector<V3>(r);
    };

	bool	operator ==	(const PolarPosition& p) const	
    {
        return  fabs(distance - p.distance) < DBL_EPSILON &&
                fabs(azimuth  - p.azimuth)  < DBL_EPSILON &&
                fabs(elevation- p.elevation)< DBL_EPSILON;
    }
    bool	operator !=	(const PolarPosition& v) const
    {
        return !((*this)== v);
    }
    PolarPosition& operator=(const PolarPosition &p)
    {
        if (this != &p)
        {
            distance  = p.distance;
            azimuth   = p.azimuth;
            elevation = p.elevation;
        }
        return *this;
    };
    friend PolarPosition operator+(const PolarPosition &p1,const PolarPosition &p2)
    {
        PolarPosition p;
        p.azimuth   = p1.azimuth   + p2.azimuth;
        p.elevation = p1.elevation + p2.elevation;
        p.distance  = p1.distance  + p2.distance;
        return p;
    };
    friend PolarPosition operator-(const PolarPosition &p1,const PolarPosition &p2)
    {
        PolarPosition p;
        p.azimuth   = p1.azimuth   - p2.azimuth;
        p.elevation = p1.elevation - p2.elevation;
        p.distance  = p1.distance  - p2.distance;
        return p;
    };
    PolarPosition& operator+=(const PolarPosition &p)
    {
        azimuth   += p.azimuth;
        elevation += p.elevation;
        distance  += p.distance;
        return *this;
    };
    PolarPosition& operator-=(const PolarPosition &p)
    {
        azimuth   -= p.azimuth;
        elevation -= p.elevation;
        distance  -= p.distance;
        return *this;
    };
    PolarPosition& operator/=(const double  k)
    {
        azimuth   /= k;
        elevation /= k;
        distance  /= k;
        return *this;
    };
    PolarPosition& operator*=(const double  k)
    {
        azimuth   *= k;
        elevation *= k;
        distance  *= k;
        return *this;
    };
    PolarPosition	operator *	(const double k) const 
    {
        return PolarPosition( (distance  * k),
            (azimuth   * k),
            (elevation * k)); 
    }
    PolarPosition	operator /	(const double k) const
    {
        return PolarPosition( (distance  / k),
            (azimuth   / k),
            (elevation / k)); 
    }
    inline friend Serializer& operator << (Serializer& serializer, PolarPosition &p)
    {
        serializer << p.distance
                   << p.azimuth
                   << p.elevation;
        return serializer;
    };
    double distance;
    double azimuth;
    double elevation;
};

}