#ifndef LANDSCAPE4_LGEOMETRY4_H
#define LANDSCAPE4_LGEOMETRY4_H

#include "edterrain.h"
#include "../landscape3_config.h"

#include "landscape3/lmaterial.h"
#include "landscape3/lmaterial.inl"

#include <ed/string.h>
#include <ed/vector.h>
#include <osg/BoundingBox>

namespace geometry4
{

struct GeometryItem
{
	uint32_t startPrimitive;
	uint32_t primitiveCount;
};

struct GeometrySource
{
	// Возможные типы потоков
	enum STREAMTYPE_ENUM
	{
		FLOAT1 = 0,
		FLOAT2,
		FLOAT3,
		FLOAT4,

		// 32-битные инты
		INT1,
		INT2,
		INT3,
	};

	static int streamTypeStride(int type);
	static int streamTypeBytes(int type);

	// Возможные виды интерполяции данных в потоке. 
	enum STREAMINTERPOLATION_ENUM
	{
		PER_VERTEX = 0,
		PER_PRIMITIVE,
		PER_INSTANCE,
	};

	// Один поток геометрисорса
	struct Stream
	{
		// Строковое название потока - "P", "N", "UV0", ...
		ed::string name;

		// Тип потока - FLOAT1, FLOAT2, FLOAT3, ...
		uint32_t type;

		// PER_VERTEX, PER_PRIMITIVE, PER_INSTANCE, ...
		uint32_t interpolation;

		// Собственно данные
		io::mmfvector<uint8_t> data;

		Stream() {}
		Stream(const ed::string& name, uint32_t type, uint32_t interpolation) : name(name), type(type), interpolation(interpolation) {}

		int size() const
		{
			return data.size()/streamTypeBytes(type);
		}
		// upload data
		template <typename T> void update(const ed::vector<T>& input)
		{
			if( !input.size())
				return;
			int sizeInBytes = sizeof(T) * input.size();
			this->data.resize(sizeInBytes);
			memcpy(&this->data[0], &input[0], sizeInBytes);
		}
	};

	ed::vector<Stream> streams;
	int vertexCount;

	GeometrySource() { vertexCount = 0; }

	// stream by name
	const geometry4::GeometrySource::Stream* getStream(const ed::string& name)const;

	// dump
	void dump(FILE* output) const;
};

};

// stream by name
inline const geometry4::GeometrySource::Stream* geometry4::GeometrySource::getStream(const ed::string& name)const
{
	for( size_t i = 0; i < streams.size(); i++)
		if( streams[i].name == name)
			return &streams[i];
	return NULL;
}

// dump
inline void geometry4::GeometrySource::dump(FILE* output) const
{
	printf("verts %d:\n", vertexCount);
	for( int s=0; s<this->streams.size(); s++)
	{
		const Stream& stream = this->streams[s];
		printf("%s: type=%d, interpolation=%d\n", stream.name.c_str(), stream.type, stream.interpolation);
		int stride = this->streamTypeStride(stream.type);
		int vc = std::min( vertexCount, 10);
		for(int v=0; v<vc; v++)
		{
			if( v!=0) fprintf( output, ", ");
			switch( stream.type)
			{
			case FLOAT1:
				{
					const float* value = (const float*)&stream.data[v*stride*sizeof(float)];
					fprintf(output, "%f", *value);
					break;
				}
			case FLOAT2:
				{
					const float* value = (const float*)&stream.data[v*stride*sizeof(float)];
					fprintf(output, "{%f, %f}", value[0], value[1]);
					break;
				}
			case FLOAT3:
				{
					const float* value = (const float*)&stream.data[v*stride*sizeof(float)];
					fprintf(output, "{%f, %f, %f}", value[0], value[1], value[2]);
					break;
				}
				/*/
			FLOAT4,

			INT1,
			INT2,
			INT3,
				/*/
			}
		}
		if(vc!=vertexCount)
			printf("...");
		printf("\n");
	};
}

// Сериализация
template <class S> S& operator >>(S& stream, geometry4::GeometrySource::Stream& geometrySourceStream)
{
	uint32_t version = 0;
	stream >> version;
	
	if (version >= 0)
	{
		stream >> geometrySourceStream.name;
		stream >> geometrySourceStream.type;
		stream >> geometrySourceStream.interpolation;
		stream >> geometrySourceStream.data;
	}

	return stream;
}

template <class S> S& operator >>(S& stream, geometry4::GeometrySource& geometrySource)
{
	uint32_t version = 0;
	stream >> version;
	
	if (version >= 0)
	{
		stream >> geometrySource.vertexCount;
		stream >> geometrySource.streams;
	}

	return stream;
}

template <class S> S& operator >>(S& stream, geometry4::GeometryItem& geometryItem)
{
	uint32_t version = 0;
	stream >> version;
	
	if (version >= 0)
	{
		stream >> geometryItem.primitiveCount;
		stream >> geometryItem.startPrimitive;
	}

	return stream;
}

inline int geometry4::GeometrySource::streamTypeStride(int type)
{
	switch((geometry4::GeometrySource::STREAMTYPE_ENUM)type)
	{
		case geometry4::GeometrySource::FLOAT1: return 1;
		case geometry4::GeometrySource::FLOAT2: return 2;
		case geometry4::GeometrySource::FLOAT3: return 3;
		case geometry4::GeometrySource::FLOAT4: return 4;

		case geometry4::GeometrySource::INT1: return 1;
		case geometry4::GeometrySource::INT2: return 2;
		case geometry4::GeometrySource::INT3: return 3;
	}
	return 0;
}
inline int geometry4::GeometrySource::streamTypeBytes(int type)
{
	int stride = streamTypeStride(type);
	switch((geometry4::GeometrySource::STREAMTYPE_ENUM)type)
	{
		case geometry4::GeometrySource::FLOAT1:
		case geometry4::GeometrySource::FLOAT2:
		case geometry4::GeometrySource::FLOAT3:
		case geometry4::GeometrySource::FLOAT4:
			return stride*sizeof(float);
		case geometry4::GeometrySource::INT1: 
		case geometry4::GeometrySource::INT2: 
		case geometry4::GeometrySource::INT3: 
			return stride*sizeof(uint32_t);
	}
	return 0;
}

#endif


