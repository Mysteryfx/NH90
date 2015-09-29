#pragma once

#include <osg\Vec2d>
#include <ed/vector.h>

#include "Tools.h"
#include "Mem\Shared.h"

class viObjectShape;
#ifndef _OPENSTEER
	namespace NavMeshData {class MeshPolygon;}
#endif

namespace Utils
{
	struct Edge
	{
		Edge(){}
		Edge(const osg::Vec2d& a, const osg::Vec2d& b, const osg::Vec2d& norm, int ind)
			:p1(a), p2(b), normal(norm), index(ind), checked(false) {}
		Edge(const osg::Vec2d& a, const osg::Vec2d& b, bool isRightNorm);
		Edge(const osg::Vec2d& a, const osg::Vec2d& b)
			:p1(a), p2(b), index(-1), checked(false){}

		osg::Vec2d p1, p2;
		osg::Vec2d normal;
		int index;
		bool checked; //это поле используется только в findIntersectionWithObstacle
	};
	class Edges: public ed::vector<Edge>{};
}

class /*TRANSPORT_API */Obstacle: public Mem::Shared
{
public:
	enum Type
	{
		TypeNotDef = 0,
		SurfaceObstacle,
		StaticObject
	};

	enum AvoidDir
	{
		DirNotDef= 0,
		CW,
		CCW,
		EveryTimeDefinded
	};

	//Предполагается что Contour ориентирован против часовой
	typedef ed::vector<osg::Vec2d> Contour;

	explicit Obstacle (const Contour& cont);
#ifndef _OPENSTEER
	explicit Obstacle (const viObjectShape* s);
	explicit Obstacle (const NavMeshData::MeshPolygon* p);
#endif

	~Obstacle(){}
	

	Contour::size_type vertsCount() const {return _verts.size();};
	const osg::Vec2d* vert(int i) const {return (i>-1 && i<static_cast<int>(_verts.size())) ? &_verts[i] : 0;};
	const osg::Vec2d& com() const {return _com;};
	const Contour& verts() const {return _verts;};
	const Utils::Edges& edges() const {return _edges;};

	Type type() const {return _type;};
	AvoidDir avoidDir() const {return _avoidDir;};
	void setAvoidDir(AvoidDir dir) {_avoidDir = dir;};
	double avoidDirEndPar() const {return _avoidDirEndPar;}
	void setAvoidDirEndPar(double par) {_avoidDirEndPar = par;};

	bool operator == (const Obstacle& rhs) const;

	bool inObstacle(const osg::Vec2d& point) const;

private:
	Contour _verts;
	Utils::Edges _edges;
	osg::Vec2d _com; //center of mass
	Type _type;
	AvoidDir _avoidDir;
	//путевой параметр машинки до которого определено направление объезда препятвия
	double _avoidDirEndPar;
	const void* _worldObjPtr;
};

//считает центр масс по периметру (масса сосредоточена в рёбрах многоугольника)
//объезд препятствия осуществяется со стороны где нет центра масс
//что в обзем случае может быть неверно
osg::Vec2d getCOM(const Obstacle::Contour& cont);

#ifndef _OPENSTEER
void getContour(const NavMeshData::MeshPolygon* p, Obstacle::Contour& cont);
void getContour(const viObjectShape* s, Obstacle::Contour& cont);
#endif

void getEdges(const Obstacle::Contour& cont, Utils::Edges& edges);

template<class Iterator>
bool isInConvexArea(Iterator s, Iterator e, const osg::Vec2d& p)
{
	if (std::distance(s, e) < 2)
		return false;

	--e;
	osg::Vec2d v1, v2;
	Iterator t1, t2;
	for (t1 = s; t1 != e; ++t1)
	{
		t2 = t1 + 1;

		v1 = p - *t1;
		v2 = *t2 - *t1;

		const double res = v1.y()*v2.x() - v1.x()*v2.y();
		if (res<0)
			return false;
	}
	return true;
}

//typedef ed::vector<Mem::Ptr<Obstacle> > ObstacleGroup;
class ObstacleGroup: public ed::vector<Mem::Ptr<Obstacle> >
{
};

class RideOnObstacles: public ed::vector<const viObjectShape*>
{
};