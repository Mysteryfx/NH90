#pragma once

#include <osg\Vec2d>
#include "LocalSpace.h"

#include "Mem\Shared.h"

#include <ed/set.h>
#include <ed/vector.h>
#include <hash_map>

class SimpleVehicle;

struct PointInfo
{
	PointInfo()
		:dist(-1./*f*/), index(-1), maxSpeed(DBL_MAX)
	{}

	PointInfo(double pDist)
		:dist(pDist), index(-1), maxSpeed(DBL_MAX)
	{}

	double dist;
	mutable unsigned int index;
	osg::Vec2d pos;
	mutable double maxSpeed;

	bool operator<(const PointInfo& rhs) const {return dist<rhs.dist;}
};

class AbstractCarPath
{
public:
    virtual PointInfo mapPointToPath (const osg::Vec2d& point, double& outside, double startDist = 0./*f*/, double endDist = DBL_MAX) const = 0;

	virtual PointInfo mapPathDistanceToPoint (double dist) const = 0;

    virtual double mapPointToPathDistance (const osg::Vec2d& point, double startDist = 0./*f*/, double endDist = DBL_MAX) const = 0;

	virtual osg::Vec2d tangentAt(const PointInfo& point) const = 0;
};

class Trail: public AbstractCarPath, public Mem::Shared
{
public:
	Trail();
	Trail(const double offset);
	~Trail();

	typedef ed::set<PointInfo> Points;
	const Points& points() const {return _points;}

	void appendPosition(const LocalSpace2d& pos, double maxSpeed);
	void reset();
	double distFromBegin() const;
	double endTrailPar() const;

	double offset() const {return _offset;}

	osg::Vec2d endPosition() const;
	osg::Vec2d endTangent() const;

	void reportTrailPar(const SimpleVehicle* veh, double pathPar) const;

	osg::Vec2d getOnTrailPos(double pathParam, osg::Vec2d* dir = 0) const;

	/*интерфейс для CarPah, пути по которому может следовать машинка*/
	PointInfo mapPointToPath (const osg::Vec2d& point, double& outside, double startDist, double endDist) const;
	double mapPointToPathDistance (const osg::Vec2d& point, double startDist, double endDist) const;
	PointInfo mapPathDistanceToPoint (double dist) const;
	osg::Vec2d tangentAt(const PointInfo& point) const;

	bool empty() const;
private:
	bool appendPoint(PointInfo& info, const osg::Vec2d& pointOffset);
	int findStartSegment(double dist, Points::const_iterator& firstPoint) const;

	void removeHistoryTail();
	void simlifyPath(Points::iterator last);

	void Trail::printPath(const char* str);

	double getAverangeMS(const PointInfo& first, const PointInfo& second, double dist) const;

	Points _points;
	ed::vector<osg::Vec2d> _normals;

	double _offset;//константа
	osg::Vec2d prevPointOffset;

	mutable stdext::hash_map<const SimpleVehicle*, double> _followerDist;
};


double pointToSegmentDistance(const osg::Vec2d& point, const PointInfo& a, const PointInfo& b,
							 const osg::Vec2d& norm, double& onSegmentDist, osg::Vec2d& onSegmentPoint);