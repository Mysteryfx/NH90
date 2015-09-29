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
 * ���������, ����������� ����� �� ������
 */
struct PointOnRoad
{
public:
	PointOnRoad(){road=-1; param=-1;}

	bool isValid(){return param>=0;}
// inhouse data
protected:
	//������ ������ � �������� ����
	int32_t road;
	//�������� ����� �� ������
	double param;

	PointOnRoad(int32_t road, double param){this->road=road; this->param=param;}

protected:
	friend class IRouteImpl4;
	friend class ICrossroadImpl4;
	friend class IRoadNetworkImpl4;
	friend class IRoadNetworkImpl3;
};

/*
 * ������ ��� �������.
 */
class IRoute
{
public:
	virtual ~IRoute() {}

	virtual void release() = 0;

	// ���
	virtual ed::string getName() { return ""; }

	// 
	virtual double getLength() = 0;

	// ��������� �� �����
	virtual ed::string getParam(const char* name) { return ""; }
	virtual ed::string getParam(const ed::string& name) { return ""; }

	virtual ICrossroad* getStartCrossroad()=0;
	virtual ICrossroad* getEndCrossroad()=0;

	virtual PointOnRoad getStartPoint()=0;
	virtual PointOnRoad getEndPoint()=0;

	// �����
	virtual osg::Vec3d getPoint(const PointOnRoad& ponr)=0;
	// �������
	virtual osg::Vec3d getTangent(const PointOnRoad& ponr)=0;
	// ��������
	virtual double getCurvature(const PointOnRoad& start)=0;

	// ���������� ����� �������
	virtual double getLength(const PointOnRoad& start, const PointOnRoad& end)=0;
	// �������� �����, ������ false ���� ����� �� ����� ������
	virtual bool movePoint(PointOnRoad& ponr, double length)=0;

	// ����� ��������� �����
	virtual PointOnRoad findClosestPoint(const osg::Vec3d& pt){return PointOnRoad();};

	// ��������� ��������� �� ����� � ������������ �������� ���� ����� ����������
	virtual void buildPolyline(ed::vector<osg::Vec3d>& polyline, float angleTolerance = M_PI/6) {}
};

/*
 * ����������: ����� ���������� ����� ��� ��������� ������.
 */
class ICrossroad
{
public:
	virtual ~ICrossroad() {}

	virtual void release() = 0;


	// ���
	virtual ed::string getName() { return ""; }


	// ��������� �� �����
	virtual ed::string getParam(const char* name) { return ""; }
	virtual ed::string getParam(const ed::string& name) { return ""; }
	// ����� ����� � �����������
	virtual int roadsCount() { return 0; }
	// ����� �� ������
	virtual PointOnRoad getRoadInCross(int road) const=0;

	virtual osg::Vec3d getPos() { return osg::Vec3d(); }
};

/**
 * Sink ��� ����������� ����������� �������� �� ������ � �����������
 */
class ICheckRoadSink
{
public:
	virtual ~ICheckRoadSink() {}
	
	/**
	 * ����� �� ��������� �� ������
	 * @param road ������ ������ � �������� ����
	 * @param startParam �������� �� ������, �������� �����, � ������� �������� ��������
	 * @param endParam �������� �� ������, �������� �����, �� ������� ����������� ��������
	 * @param return true ���� ����� ���������
	 */
	virtual bool canMoveAlongRoad(int32_t road, double startParam, double endParam){return true;};

	/**
	 * ����� �� ��������� �� �����������
	 * @param crossroad ������ ����������� � �������� ����
	 * @param startRoad ������ ������, � ������� �������� �������� �� �����������
	 * @param endRoad ������ ������, �� ������� ����������� �������� �� �����������
	 */
	virtual bool canMoveAlongCrossroad(int32_t crossroad, int32_t startRoad, int32_t endRoad){return true;};

	/**
	 * ����� �� ����� ����� �� ������
	 * @param road ������ ������
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

	// ���������� ������� �����������-������...������-�����������. ������������ ��� ����������� ���
	virtual int chainCount() { return 0; }
	virtual ed::vector<int> getChain(int chain) { return ed::vector<int>(); }
	virtual ed::string getChainName(int chain, int i) { return ""; }

	// ����� � ������������ �� ����� �� ������
	virtual bool getPoint(const PointOnRoad& pointOnRoad, osg::Vec3d& point) {return false;}

	// ��������� �����
	virtual bool findClosestPoint(const osg::Vec3d& point, PointOnRoad& closestPoint,
		ICheckRoadSink* sink = NULL, double startDistance = 10000.0){return false;}

	// ������� �������
	virtual IRoute* createRoute(const PointOnRoad& startPoint, const PointOnRoad& endPoint,
		ICheckRoadSink* sink = nullptr)=0;

protected:
	virtual ~IRoadNetwork() {}
};

}  // namespace edterrain

#endif //__IROADNETWORK_H__
