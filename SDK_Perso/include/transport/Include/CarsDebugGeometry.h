#pragma once

#include "Renderer/CustomGeometry.h"
#include "Renderer/DbgGraph.h"
#include "lTerraBasic.h"

namespace Graphics
{
	class RenderObject;
	class RenderParser;
};
namespace NavMeshData
{
	class MeshPolygon;
}

namespace roads
{
	struct Road;
}
namespace roads3
{
	struct Road;
}

class Obstacle;
class wcRoute;

class dbg_mesh_polygon
{
public:
	dbg_mesh_polygon();
	dbg_mesh_polygon(const NavMeshData::MeshPolygon& p);
	~dbg_mesh_polygon();
	void  set_polygon(const NavMeshData::MeshPolygon& p);
	void draw();

private:
	void initObjMaterial(const Vector3& defColor, float opacity, char *matName, 
		bool colorFromParams, bool zBuf, Graphics::PrimitiveType primType, 
		Graphics::RenderObject*& obj);
	Graphics::RenderObject *dbg_pouring;
	Graphics::RenderObject *dbg_wire;
};

class ObstacleDrw: public dbg_obj
{
public:
	explicit ObstacleDrw(const Obstacle& ob);
};

class RouteDrw: public dbg_obj
{
public:
	RouteDrw();
	void set(const NM_ROAD::Road& path);
	void clear();
};
