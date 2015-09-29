#ifndef LANDSCAPE4_LSURFACE_DETAILS_FILE_H
#define LANDSCAPE4_LSURFACE_DETAILS_FILE_H

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

class lSurfaceDetailsFile : public lDataFile
{
public:

	// Геометрии	
	struct Geometry : public geometry4::GeometrySource
	{
		ed::string name;

		// материал
		material3::Material material;
	};

	ed::vector<Geometry> geometries;

	virtual ed::string getType() { return "landscape4::lSurfaceDetailsFile"; }
	virtual bool copyFrom(lDataFile* src){*this = *(lSurfaceDetailsFile*)src;return true;};

public:
	EDTERRAIN_API virtual ~lSurfaceDetailsFile();

	EDTERRAIN_API bool save(const char* filename);
	EDTERRAIN_API bool save(const ed::string& filename);
	EDTERRAIN_API bool load(io::MmfStream& stream);

private:

	EDTERRAIN_API bool serialize(io::MmfStream& stream);
};

};

template <class S> S& operator >>(S& stream, landscape4::lSurfaceDetailsFile::Geometry& geometry)
{
	uint32_t version = 0;
	stream >> version;
	
	if (version >= 0)
	{
		stream >> (geometry4::GeometrySource&)geometry;
		stream >> geometry.name;
		stream >> geometry.material;
	}

	return stream;
}

#endif