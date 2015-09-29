#pragma once
#include "Transport.h"
#include "CarsDebugGeometry.h"
#include "IdTypes.h"

#include "Registry/Registered.h"

namespace Graphics
{
	class RenderParser;
}

#include <ed/set.h>
#include <ed/vector.h>

class TRANSPORT_API CarsDebugDrawer
{
public:
	static CarsDebugDrawer* instance();
	void setColumnData(ObjectID columnId);
	void clear();
	void draw();

private:
	CarsDebugDrawer();
	~CarsDebugDrawer();

	typedef ed::set<NavMeshData::SquareIndex> SquaresList;
	typedef ed::vector<dbg_mesh_polygon*> PolygonsDrw;

	SquaresList getPathSquares(const NM_ROAD::Road& path) const;
	void addToMesh(const NavMeshData::SquareIndex& ind);

	PolygonsDrw drwMesh;
	RouteDrw drwRoute;
};
