#ifndef __IROADNETWORK_ADAPTER_H__
#define __IROADNETWORK_ADAPTER_H__

#include "IRoadNetwork.h"
#include <math.h>
#include <osg/Vec3d>
#include "ed/vector.h"


#ifdef USE_TERRAIN4

#include "cLinear.h"
#include "cPosition.h"


namespace edterrain
{

// ! Не использовать в новом коде ! Удушу!
typedef edterrain::PointOnRoad PointOnRoadAdapter;

/*/
struct PointOnRoadAdapter : public PointOnRoad
{
	// TODO START
	mutable dVector p;

	bool isTunnel() { return false; }
	float getHeigth() { return p.y; }
	dVector& GetPoint() const { return p; }
	// TODO END
};
/*/

typedef edterrain::PointOnRoad PointOnPathAdapter;

/*/
struct SegmentAdapter
{
	// TODO START
	
	mutable cVector p;
	cVector p1;		 // нач. точка
	cVector p2;		 // кон. точка
	float length;    // накопленная длина

	SegmentAdapter(): length(0) {}

	cVector getStartTangent() const { return cVector(); }
	const cVector& getStartPoint() const { return p; }
	const cVector& getEndPoint() const { return p; }
	float getLength() const { return 0; }
	// TODO END
};
/*/

struct RouteAdapter
{
public:
	edterrain::IRoute* route;
public:	
	int getSegmentsCount()const
	{
		return 1;
	}
	void getEndPoint(edterrain::PointOnRoad& pt) const 
	{
		pt = route->getEndPoint();
	}
	cVector getPoint(int ptn) const 
	{ 
		osg::Vec3d pt;
		if(ptn==0)
			pt = route->getPoint( route->getStartPoint());
		else
			pt = route->getPoint( route->getEndPoint());

		return cVector(pt.x(), pt.y(), pt.z()); 
	}
	dVector getPoint(const edterrain::PointOnRoad& ponr) const 
	{ 
		osg::Vec3d pt = route->getPoint( ponr);
		return dVector(pt.x(), pt.y(), pt.z()); 
	}
	cVector getStartPoint() const 
	{ 
		osg::Vec3d pt = route->getPoint( route->getStartPoint());
		return dVector(pt.x(), pt.y(), pt.z()); 
	}
	float getLength() const 
	{ 
		return route->getLength(); 
	}

    bool movePoint(PointOnRoad& ponr, double length)
    {
        return route->movePoint(ponr, length);
    }
	//SegmentAdapter* getSegment(int i) { return nullptr; }
	//SegmentAdapter* getStartSegment() { return nullptr; }
	//SegmentAdapter* getEndSegment() { return nullptr; }
	edterrain::PointOnRoad getStart() const 
	{ 
		return route->getStartPoint(); 
	}
	edterrain::PointOnRoad getEnd() const 
	{ 
		return route->getEndPoint(); 
	}
	bool isDead() const 
	{ 
		return false; 
	}
	bool isTunnel(const edterrain::PointOnRoad& ponr) const
	{
		return false;
	}
	double getHeigth(const edterrain::PointOnRoad& ponr) const
	{
		return route->getPoint( ponr).y();
	}
	// TODO END
};

struct PRouteAdapter
{
protected:
	RouteAdapter routeAdapter;
public:
	PRouteAdapter()
	{
		routeAdapter.route = nullptr;
	}
	PRouteAdapter& operator=(edterrain::IRoute* route)
	{
		routeAdapter.route = route;
		return *this;
	}
	RouteAdapter* operator->()
	{
		return &routeAdapter;
	}
	operator bool()
	{
		return routeAdapter.route!=nullptr;
	}
	bool operator !()
	{
		return routeAdapter.route==nullptr;
	}

};

/*/
struct RoadAnchorAdapter
{

};
/*/

struct SceneObjectAdapter
{
	// символьное имя типа объекта
	ed::string name;
	// Позиция объекта
	cPosition pos;
};

typedef edterrain::ICrossroad CrossroadAdapter;

/*/
struct CrossroadAdapter
{
protected:
	cPosition pos;			// Позиция перекрестка
	uint32_t flag;		// Флаги перекрестка

	// объекты сцены
	ed::vector< SceneObjectAdapter* > objects;
	ed::vector< uint32_t > objectinds;

	// Массив дорог (отсортирован по углам)
	//ed::vector< RoadAnchorAdapter* > roads;
	ed::vector< uint32_t > roadinds;
public:
	bool isDead()const{return false;}
	bool isStand() const { return false; }
	bool isOnRunway() const { return false; }
	bool isShelter() const { return false; }
	bool isForBigAirPlaines()const{ return false; }
};
/*/

/*/
struct ICheckRoadSinkAdapter
{

};

struct RoadNetworkAdapter
{
	void* Open(const char* filename) { return nullptr; }
	
	int getRoadCount() const { return 0; }
	RouteAdapter* getRoad(int index) { return nullptr; }
	
	int getCrossCount() { return 0; }
	CrossroadAdapter* getCross(int i) { return nullptr; }
	
	RouteAdapter* GetPath(
		CrossroadAdapter* start,	// начальная перекресток
		CrossroadAdapter* end,	// начальная перекресток
		float roffset,			// смещение от центра дороги
		// Выходные параметры
		PointOnPathAdapter& popStart,  // точка выезда на дорогу
		PointOnPathAdapter& popEnd,		// точка съезда с дороги
		ICheckRoadSinkAdapter* checkproc = 0	// позволяет фильтровать поиск
		)
	{
		return nullptr;
	}
};
/*/

}  // namespace edterrain



#else

#include "landscape3/lPathFile.h"

namespace path
{
	class PathNetwork;
	class PointOnPath;
}

namespace NM_ROAD
{
	struct Road;
	struct Crossroad;
	struct PointOnRoad;
	struct Segment;
	struct ICheckRoadSink;
	struct SceneObject;
}


namespace edterrain
{
	typedef path::PathNetwork RoadNetworkAdapter;
	typedef path::PointOnPath PointOnPathAdapter;
	typedef NM_ROAD::PointOnRoad PointOnRoadAdapter;
	typedef NM_ROAD::Road RouteAdapter;
	typedef NM_ROAD::Crossroad CrossroadAdapter;
	typedef NM_ROAD::Segment SegmentAdapter;
	typedef NM_ROAD::ICheckRoadSink ICheckRoadSinkAdapter;
	typedef NM_ROAD::SceneObject SceneObjectAdapter;
}

#endif // #ifdef USE_TERRAIN4

#endif //__IROADNETWORK_ADAPTER_H__
