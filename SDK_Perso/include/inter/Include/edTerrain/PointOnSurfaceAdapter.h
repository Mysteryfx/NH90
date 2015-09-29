#ifndef __PointOnSurfaceAdapter_H__
#define __PointOnSurfaceAdapter_H__

#include "ITerrain.h"
#include "IRoadNetworkAdapter.h"


INTER_API void assert_inter_edterrain_not_implemented(bool expression);

namespace edterrain
{

// PointOnSurfaceAdapter, только для миграции
// ! Не использовать в новом коде ! Удушу!
// 
struct PointOnSurfaceAdapter : public PointOnSurface
{
	/*/
	PointOnSurfaceAdapter():PointOnSurface()
	{
	}
	PointOnSurfaceAdapter(const PointOnSurfaceAdapter& arg):PointOnSurface()
	{
		this->terrain = terrain;
		this->p = p;
		this->n = n;
		this->t = t;
	}
	/*/
	template<class T>
	PointOnSurfaceAdapter& operator=(const T& t)
	{
		osg::Vec3d query(t.x, t.y, t.z);
		this->set(query);
		return *this;
	}
	template<class T>
	PointOnSurfaceAdapter& operator+=(const T& t)
	{
		osg::Vec3d query(t.x, t.y, t.z);
		this->set(query + p);
		return *this;
	}
	template<class T>
	PointOnSurfaceAdapter operator+(const T& t)
	{
		PointOnSurfaceAdapter other = *this;
		osg::Vec3d delta(t.x, t.y, t.z);
		other.set(this->point() + delta);
		return other;
	}

	// точка
	float getHeight() const {return height();};

	template<class T>
	void getNormal(T& n) const
	{
		auto nn = normal();
		n = T(nn.x(), nn.y(), nn.z());
	}

	template<>
	void getNormal<osg::Vec3d>(osg::Vec3d& n) const
	{
		n = normal();
	}

	template<class T, class V>
	bool intersectSurface(const T& a, const T& b, V *p) const
	{
		osg::Vec3d aa(a.x, a.y, a.z);
		osg::Vec3d bb(b.x, b.y, b.z);
		edterrain::PointOnSurface pons = ITerrain::GetDefaultTerrain()->intersection(aa, bb);
		if (!pons.isValid())
			return false;
		if (p)
		{
			auto pp = pons.point();
			*p = V(pp.x(), pp.y(), pp.z());
		}
		return true;
	}

	template<>
	bool intersectSurface<osg::Vec3d, osg::Vec3d>(const osg::Vec3d& a, const osg::Vec3d& b, osg::Vec3d *p) const
	{
		edterrain::PointOnSurface pons = ITerrain::GetDefaultTerrain()->intersection(a, b);
		if (!pons.isValid())
			return false;
		if (p)
			*p = pons.point();
		return true;
	}


	// Семантика
	unsigned int getTypeSurface() const;

	// helpers
	bool IsOnWater() const { return hasType(ST_WATER); }
	bool IsOnRunway() const { return hasType(ST_RUNWAY); }
	bool isTypeSurfaceWater() const
	{
		return IsOnWater();
	}

	template<class T>
	void getPoint(T& v) const
	{
		v.x = p.x(); v.y = 0; v.z = p.z();
	}

	bool IsShallowWater() const
	{
		return isWater();
	}

	float slopeAngle() const
	{
		const osg::Vec3f up = osg::Vec3f(0.f, 1.f, 0.f);
		return acos(n*up);
	}

	// =========================== TODO START =================================
	bool IsOnRoad(double height = 0) const
	{
		assert_inter_edterrain_not_implemented(0);
		return false;
	}

	PointOnRoadAdapter pointOnRoad() const
	{
		assert_inter_edterrain_not_implemented(0);
		return PointOnRoadAdapter();
	}

	bool getRiverDepth(float &depth) const
	{ 
		assert_inter_edterrain_not_implemented(0);
		depth = 1.5;
		return false;
	}
	// =========================== TODO END =================================
};

} // namespace edterrain


#include "inl/PointOnSurfaceAdapter.inl"

#endif //__PointOnSurfaceAdapter_H__
