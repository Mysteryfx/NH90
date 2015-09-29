#ifndef __IROADNETWORK_H__
#define __IROADNETWORK_H__

#include "ITerrain.h"
#include <math.h>
#include <osg/Vec3d>
#include "ed/vector.h"

class IRouteImpl4;
class ICrossroadImpl4;
class IRoadNetworkImpl4;
class IRoadNetworkImpl3;

namespace edterrain
{

class ICrossroad;

/**
 * —труктура, описывающа€ точку на дороге
 */
struct PointOnRoad
{
public:
	PointOnRoad(){road=-1; param=-1;}

	bool isValid(){return param>=0;}
// inhouse data
protected:
	//»ндекс дороги в дорожной сети
	int32_t road;
	//ѕараметр точки на дороге
	double param;

	PointOnRoad(int32_t road, double param){this->road=road; this->param=param;}

protected:
	friend class IRouteImpl4;
	friend class ICrossroadImpl4;
	friend class IRoadNetworkImpl4;
	friend class IRoadNetworkImpl3;
};

/*
 * ƒорога или маршрут.
 */
class IRoute
{
public:
	virtual ~IRoute() {}

	virtual void release() = 0;

	// им€
	virtual ed::string getName() { return ""; }

	// 
	virtual double getLength() = 0;

	// параметры по имени
	virtual ed::string getParam(const char* name) { return ""; }
	virtual ed::string getParam(const ed::string& name) { return ""; }

	virtual ICrossroad* getStartCrossroad()=0;
	virtual ICrossroad* getEndCrossroad()=0;

	virtual PointOnRoad getStartPoint()=0;
	virtual PointOnRoad getEndPoint()=0;

	// “очка
	virtual osg::Vec3d getPoint(const PointOnRoad& ponr)=0;
	// “ангент
	virtual osg::Vec3d getTangent(const PointOnRoad& ponr)=0;
	//  ривизна
	virtual double getCurvature(const PointOnRoad& start)=0;

	// –ассто€ние между точками
	virtual double getLength(const PointOnRoad& start, const PointOnRoad& end)=0;
	// —двинуть точку, вернет false если вышли за конец дороги
	virtual bool movePoint(PointOnRoad& ponr, double length)=0;

	// Ќайти ближайшую точку
	virtual PointOnRoad findClosestPoint(const osg::Vec3d& pt){return PointOnRoad();};

	// ѕостроить полилинию по роуту с максимальным допуском угла между сегментами
	virtual void buildPolyline(ed::vector<osg::Vec3d>& polyline, float angleTolerance = M_PI/6) {}
};

/*
 * ѕерекрЄсток: точка соединени€ дорог или окончание дороги.
 */
class ICrossroad
{
public:
	virtual ~ICrossroad() {}

	virtual void release() = 0;


	// им€
	virtual ed::string getName() { return ""; }


	// параметры по имени
	virtual ed::string getParam(const char* name) { return ""; }
	virtual ed::string getParam(const ed::string& name) { return ""; }
	// „исло дорог в перекрестке
	virtual int roadsCount() { return 0; }
	// точка на дороге
	virtual PointOnRoad getRoadInCross(int road) const=0;

	virtual osg::Vec3d getPos() { return osg::Vec3d(); }
};

/**
 * Sink дл€ определени€ возможности движени€ по дороге и перекрестку
 */
class ICheckRoadSink
{
public:
	virtual ~ICheckRoadSink() {}
	
	/**
	 * ћожно ли двигатьс€ по дороге
	 * @param road индекс дороги в дорожной сети
	 * @param startParam параметр на дороге, задающий точку, с которой начинаем движение
	 * @param endParam параметр на дороге, задающий точку, на которой заканчиваем движение
	 * @param return true если можно двигатьс€
	 */
	virtual bool canMoveAlongRoad(int32_t road, double startParam, double endParam){return true;};

	/**
	 * ћожно ли двигатьс€ по перекрестку
	 * @param crossroad индекс перекрестка в дорожной сети
	 * @param startRoad индекс дороги, с которой начинаем движение по перекрестку
	 * @param endRoad индекс дороги, на которой заканчиваем движение по перекрестку
	 */
	virtual bool canMoveAlongCrossroad(int32_t crossroad, int32_t startRoad, int32_t endRoad){return true;};

	/**
	 * ћожно ли брать точку на дороге
	 * @param road индекс дороги
	 */
	virtual bool canGetPointOnRoad(int32_t road){return true;};
};


class IRoadNetwork
{
public:
	virtual void release() = 0;

	virtual int roadsCount() { return 0; }
	virtual IRoute* getRoad(int i) { return nullptr; }

	virtual int crossroadsCount() { return 0; }
	virtual ICrossroad* getCrossroad(int i) { return nullptr; }

	// »менованые цепочки перекресток-дорога...дорога-перекресток. »спользуетс€ дл€ определени€ ¬ѕѕ
	virtual int chainCount() { return 0; }
	virtual ed::vector<int> getChain(int chain) { return ed::vector<int>(); }
	virtual ed::string getChainName(int chain, int i) { return ""; }

	// “очка в пространстве по точке на дороге
	virtual bool getPoint(const PointOnRoad& pointOnRoad, osg::Vec3d& point) {return false;}

	// Ѕлижайша€ точка
	virtual bool findClosestPoint(const osg::Vec3d& point, PointOnRoad& closestPoint,
		ICheckRoadSink* sink = NULL, double startDistance = 10000.0){return false;}

	// —оздать маршрут
	virtual IRoute* createRoute(const PointOnRoad& startPoint, const PointOnRoad& endPoint,
		ICheckRoadSink* sink = nullptr)=0;

protected:
	virtual ~IRoadNetwork() {}
};

}  // namespace edterrain

#endif //__IROADNETWORK_H__
