#ifndef GeometryCache_H
#define GeometryCache_H

#include "Renderer\CustomGeometry.h"

namespace Graphics
{

class Geometry;

// Кэш для исключения дублирования данных геометрии в CMD
class GeometryCache
{
public:
	Geometry *FindGeometry(PrimitiveType ptype,
		const CustomGeometry::VertexList& vertices,
		const CustomGeometry::IndexList& indexes,
		const CustomGeometry::VertexList& normals,
		const CustomGeometry::VertexList& texcoords);

	void AddRef(Geometry *geom);
	void Release(Geometry *geom);

protected:
	struct GeometryDesc
	{
		Geometry *geom;
		mutable int refcount;

		PrimitiveType ptype;
		const CustomGeometry::VertexList* vertices;
		const CustomGeometry::IndexList* indexes;
		const CustomGeometry::VertexList* normals;
		const CustomGeometry::VertexList* texcoords;

		GeometryDesc();
		GeometryDesc(Geometry *_geom);
		GeometryDesc(PrimitiveType ptype,
			const CustomGeometry::VertexList& vertices,
			const CustomGeometry::IndexList& indexes,
			const CustomGeometry::VertexList& normals,
			const CustomGeometry::VertexList& texcoords);

		bool operator <(const GeometryDesc& desc) const;
	};

	typedef ed::set<GeometryDesc> descList;
	typedef ed::map<Geometry*, descList::iterator> geomPointerList;
	
	descList geoms;
	geomPointerList pointers;
};

};

extern Graphics::GeometryCache geomCache;

#endif // GeometryCache_H