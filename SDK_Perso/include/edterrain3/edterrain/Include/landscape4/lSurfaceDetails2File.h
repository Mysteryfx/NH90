#ifndef LANDSCAPE4_lSurfaceDetails2File_H
#define LANDSCAPE4_lSurfaceDetails2File_H

#include "edterrain.h"
#include "landscape3/lMaterial.h"
#include "landscape4/lGeometry.h"
#include "landscape4/lDataFile.h"

#ifdef EDGE
	#include "Core/io/STLSerializer.h"
#else
	#include "io/STLSerializer.h"
#endif

namespace landscape4
{

class lSurfaceDetails2File : public lDataFile
{
public:

	// Геометрии.
	// Содержит обязательное поле INT1 type - тип инстансирумого объекта в списке types
	// Еще P, Xaxis, Zaxis
	struct Geometry : public geometry4::GeometrySource
	{
		// материал
		material3::Material material;
	};

	ed::vector<Geometry> geometries;

	virtual ed::string getType() { return "landscape4::lSurfaceDetails2File"; }
	virtual bool copyFrom(lDataFile* src){*this = *(lSurfaceDetails2File*)src;return true;};

public:
	EDTERRAIN_API lSurfaceDetails2File();
	EDTERRAIN_API virtual ~lSurfaceDetails2File();

	EDTERRAIN_API bool save(const char* filename);
	EDTERRAIN_API bool save(const ed::string& filename);
	EDTERRAIN_API bool load(io::MmfStream& stream);

private:

	EDTERRAIN_API bool serialize(io::MmfStream& stream);
};

};

template <class S> S& operator >>(S& stream, landscape4::lSurfaceDetails2File::Geometry& geometry)
{
	uint32_t version = 0;
	stream >> version;
	
	if (version >= 0)
	{
		stream >> (geometry4::GeometrySource&)geometry;
		stream >> geometry.material;
	}

	return stream;
}

#endif