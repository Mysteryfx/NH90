#pragma once

#include "WorldGeneral.h"

#include "cPosition.h"
#include "GraphMath/Box.h"
#include "Registry/Registered.h"

class viObject;
class viSearch;

class WORLDGENERAL_API FindLOSObstacles
{
public:
	FindLOSObstacles(cPoint from, cPoint to, ObjectID selfID) : from_(from), to_(to), selfID_(selfID)
	{
	}
	bool operator()(viObject *item, viSearch *search);

	static bool func(viObject *item, viSearch *search, void *data)
	{
		return (*static_cast<FindLOSObstacles*>(data))(item, search);
	}
private:
	cPoint from_;
	cPoint to_;
	ObjectID selfID_;
};

extern WORLDGENERAL_API bool check_trees_LOS(const cPosition & obj_pos, const Box & box_, const cPoint & point1, const cPoint & point2);