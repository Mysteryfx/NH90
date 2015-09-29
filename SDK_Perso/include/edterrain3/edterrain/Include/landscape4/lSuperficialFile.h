#ifndef LANDSCAPE4_lSuperficialFile_H
#define LANDSCAPE4_lSuperficialFile_H

#include "edterrain.h"
#include "landscape3/lMaterial.h"
#include "landscape4/lGeometry.h"
#include "landscape4/lDataFile.h"
#include <osg/Matrixd>
#include "landscape4/SpatialTree.h"

#ifdef EDGE
	#include "Core/io/STLSerializer.h"
#else
	#include "io/STLSerializer.h"
#endif

namespace landscape4
{

// Для тестов, если погадобится - сделаем спатиал
class lSuperficialFile : public lDataFile
{
public:
	osg::BoundingBoxf box;

	struct Fragment
	{
		osg::Matrixd pos;
		osg::BoundingBoxf box;
		material3::Material material;
		geometry4::GeometrySource geometry;
	};

	ed::vector<Fragment> fragments;

	struct Geometry : public geometry4::GeometrySource
	{
		material3::Material material;
	};

	struct Square
	{
		osg::Matrixf transform;

		osg::BoundingBoxf box;

		ed::vector<Geometry> fragments;
	};

	ed::vector<Square> squares;

	landscape4::SpatialTree<uint32_t> spatialTree;

	virtual ed::string getType() { return "landscape4::lSuperficialFile"; }
	virtual bool copyFrom(lDataFile* src){*this = *(lSuperficialFile*)src;return true;};

public:
	EDTERRAIN_API lSuperficialFile();
	EDTERRAIN_API virtual ~lSuperficialFile();

	EDTERRAIN_API bool save(const char* filename);
	EDTERRAIN_API bool save(const ed::string& filename);
	EDTERRAIN_API bool load(io::MmfStream& stream);

private:
	EDTERRAIN_API bool serialize(io::MmfStream& stream);
};
}

template <class S> S& operator >>(S& stream, landscape4::lSuperficialFile::Fragment& fragment)
{
	uint32_t version = 0;
	stream >> version;
	
	if (version >= 0)
	{
		stream >> fragment.pos;
		stream >> fragment.box;
		stream >> fragment.material;
		stream >> fragment.geometry;
	}

	return stream;
}

template <class Stream> Stream& operator >> (Stream& stream, landscape4::lSuperficialFile::Geometry& geometry)
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

template <class Stream> Stream& operator >> (Stream& stream, landscape4::lSuperficialFile::Square& square)
{
	uint32_t version = 0;
	stream >> version;

	if (version >= 0)
	{
		stream >> square.transform;
		stream >> square.box;
		stream >> square.fragments;
	}

	return stream;
}

#endif