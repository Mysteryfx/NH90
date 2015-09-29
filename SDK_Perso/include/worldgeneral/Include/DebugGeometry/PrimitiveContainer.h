#pragma once
#include <ed/list.h>
#include <ed/string.h>
#include <osg/Vec3d>
#include "WorldGeneral.h"

template <class T> 
class PrimitiveList
{
public:
	PrimitiveList();
	~PrimitiveList();
	PrimitiveList(const PrimitiveList&);
	PrimitiveList& operator = (const PrimitiveList&);

	T &getNext();
	void reset();

private:
	typedef ed::list<T> ObjList;

	ObjList objs;
	typename ObjList::iterator lastActiveIt;

	friend class GlobalPrimitiveCache;
};

class PrimitiveContainer
{
public:
	WORLDGENERAL_API PrimitiveContainer();
	WORLDGENERAL_API ~PrimitiveContainer();

	WORLDGENERAL_API void	reset();

	WORLDGENERAL_API void	annotateLine(const osg::Vec3d& p1, const osg::Vec3d& p2, const osg::Vec3d& color);
	WORLDGENERAL_API void	annotateCircle(const osg::Vec3d& pos, double radius, const osg::Vec3d& color, int segments);
	WORLDGENERAL_API void	annotateText(const char *str, const osg::Vec3d& pos, const osg::Vec3d& color);

	struct LableDef
	{
		ed::string text;
		osg::Vec3d pos;
		osg::Vec3d color;
	};

	struct LineDef
	{
		osg::Vec3d p1;
		osg::Vec3d p2;
		osg::Vec3d color;
	};

	struct CircleDef
	{
		osg::Vec3d pos;
		osg::Vec3d color;
		double radius;
		int segments;
	};

	typedef PrimitiveList<LineDef>		LinesDefList;
	typedef PrimitiveList<CircleDef>	CircleDefList;
	typedef PrimitiveList<LableDef>		LabelDefList;

private:
	LinesDefList	_lines;
	CircleDefList	_circles;
	LabelDefList	_labels;

	friend class GlobalPrimitiveCache;
};

template <class T>
PrimitiveList<T>::PrimitiveList()
{
	lastActiveIt = objs.begin();
}

template <class T>
PrimitiveList<T>::~PrimitiveList()
{
}

template <class T>
T& PrimitiveList<T>::getNext()
{
	T* result;
	if (lastActiveIt == objs.end())
	{
		objs.push_back(T());
		result = &objs.back();
		lastActiveIt = objs.end();
	}
	else
	{
		result = &(*lastActiveIt);
		++lastActiveIt;
	}
	return *result;
}

template <class T>
void PrimitiveList<T>::reset()
{
	lastActiveIt = objs.begin();
}

template <class T>
PrimitiveList<T>::PrimitiveList(const PrimitiveList& pl)
{
	assert(pl.lastActiveIt==pl.objs.begin());
	objs = pl.objs;
	reset();
}

template <class T>
PrimitiveList<T>& PrimitiveList<T>::operator = (const PrimitiveList& rhs)
{
	//Копирование не имеет смысла для занятых списков
	//В принципе для непустых кэшей копирование вообще не имеет смысла 
	assert(rhs.lastActiveIt==rhs.objs.begin());
	objs = rhs.objs;
	reset();
	return *this;
}

namespace
{
	const osg::Vec3d gBlack   (0, 0, 0);
	const osg::Vec3d gWhite   (1, 1, 1);
	const osg::Vec3d gRed     (1, 0, 0);
	const osg::Vec3d gYellow  (1, 1, 0);
	const osg::Vec3d gGreen   (0, 1, 0);
	const osg::Vec3d gCyan    (0, 1, 1);
	const osg::Vec3d gBlue    (0, 0, 1);
	const osg::Vec3d gMagenta (1, 0, 1);
}
