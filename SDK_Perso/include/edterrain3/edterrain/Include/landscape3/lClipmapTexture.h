#ifndef LCLIPMAP_TEXTURE_H
#define LCLIPMAP_TEXTURE_H

#include "../landscape3_config.h"
#include "edterrain.h"

#include <osg/BoundingBox>
#include <osg/Vec2i>
#include <ed/vector.h>
#include "md5/md5.h"

#ifndef EDGE
#include "io/STLSerializer.h"
#endif

namespace surface3
{
	class EDTERRAIN_API lClipmapTexture
	{
	public:

		struct Square
		{
			uint64_t offset; // Смещение данных этого квадрата в байтах от начала файла клипмапа
		};

		struct Level
		{
			osg::Vec2i sw;                             // Индекс самого нижнего левого квадрата лода
			int32_t squarePixelSize;                       // Размер стороны квадрата в пикселях
			ed::vector<ed::vector<Square> > squares; // Квадраты данного лода
		};

		// Инфа об исходном файле
		ed::string sourceFileName;
//		uint64_t sourceFileTime;

		ed::string format;  // "rgba", "dxt5", ...
		bool zlib;           // Зажато ли zlib-ом
		int squarePixelSize; // Размер стороны квадрата в пикселях
		float pixelSize;     // Размер пикселя в метрах

		int8_t md5[HASHSIZE];

		ed::vector<Level> levels;

	public:

		lClipmapTexture();
		~lClipmapTexture();
				
		bool load(const ed::string& fullpath);
		bool save(const ed::string& fullpath);

		/**
		 * Получить центр данного клипмапа в Wold Space в метрах. y всегда 0.
		 */
		osg::Vec3d center() const;

		/**
		 * Получить баунд данного клипмапа в Wold Space в метрах. y всегда 0.
		 */
		osg::BoundingBoxd bound() const;
	};
}

template <class S> S& operator >> (S& s, surface3::lClipmapTexture::Square& square) 
{
	uint32_t version = 1;
	s >> version;
		
	if (version >= 1)
	{
		s >> square.offset;
	}

	return s;
}

template <class S> S& operator >> (S& s, surface3::lClipmapTexture::Level& level)
{
	uint32_t version = 1;
	s >> version;
	
	if (version >= 1)
	{
		s >> level.sw;
		s >> level.squarePixelSize;
		s >> level.squares;
	}

	return s;
}

template <class S> S& operator >> (S& s, surface3::lClipmapTexture& clipmapTexture) 
{
	uint32_t version = 3;
	s >> version;

	if (version >= 1)
	{
		s >> clipmapTexture.pixelSize;
		s >> clipmapTexture.squarePixelSize;
		s >> clipmapTexture.format;
		s >> clipmapTexture.zlib;
		s >> clipmapTexture.levels;
	}

	if (version >= 2)
	{
		uint64_t sourceFileTime=0;
		s >> clipmapTexture.sourceFileName;
		s >> sourceFileTime;
	}
	if (version >= 3)
	{
		s.stream(clipmapTexture.md5, sizeof(clipmapTexture.md5));
	}

	return s;
}

#endif