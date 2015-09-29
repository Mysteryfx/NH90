#include "ed/map.h"

namespace edterrain
{


inline const char* edterrain::getSurfaceTypeName(uint64_t surfaceType)
{
	switch (surfaceType)
	{
	case ST_LAND  :	return "Land";
	case ST_RUNWAY: return "Runway";
	case ST_SEA   : return "Sea";
	case ST_RIVER : return "River";
	case ST_LAKE  : return "Lake";
	case ST_ROAD  : return "Road";
	case ST_RAILWAY:return "Railway";
	case ST_TOWN  : return "Town";
	case ST_FIELD : return "Field";
	case ST_BEACH : return "Beach";
	}
	return "unknown";
}

inline ed::string edterrain::getSurfaceTypesName(uint64_t surfaceTypes)
{
	ed::string str;

	for (uint64_t i = 1; i < ST_MAX; i <<= 1)
	{
		if ((surfaceTypes&i) == 0)
			continue;
		if (!str.empty())
			str += ", ";
		str += getSurfaceTypeName(i);
	}
	return str;
}


inline PointOnSurface::PointOnSurface()
{
	terrain = nullptr;
	t = 0;
}

// ок?
inline bool PointOnSurface::isValid() const
{
	return terrain != nullptr;
}


// задать запрос
inline void PointOnSurface::set(const osg::Vec3d& query)
{
	if (terrain != nullptr)
		*this = terrain->getPointOnSurface(query);
	else
		*this = ITerrain::GetDefaultTerrain()->getPointOnSurface(query);
}
inline void PointOnSurface::set(double x, double z)
{
	set(osg::Vec3d(x, DBL_MAX, z));
}

// точка
inline double PointOnSurface::height() const
{
	return p.y();
}

// точка
inline osg::Vec3d PointOnSurface::point() const
{
	return p;
}
// нормаль
inline osg::Vec3d PointOnSurface::normal() const
{
	return n;
}

// маска типов, см. enSurfaceTypes 
inline uint64_t PointOnSurface::types() const
{
	return t;
}
// hasType, см. enSurfaceTypes 
inline bool PointOnSurface::hasType(uint64_t mask) const
{
	return t & mask;
}

} // namespace edterrain
