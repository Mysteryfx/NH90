#ifndef EDTG_IGEOMETRY_H
#define EDTG_IGEOMETRY_H

#include "edtg.h"
#include "renderer/IRenderAPI.h"
#include "rwrappers/renderUserAPI.h"

namespace offshore
{
	struct Scene;
};
namespace geometry4
{
	struct GeometrySource;
};

namespace edtg
{

// Вертексный буфер и поток в нем
typedef render::VertexStream Stream;


// Держит vb и ib буфера
// отдает потоки, параметры и куски буферов
struct IGeometry
{
	// бокс
	virtual osg::BoundingBoxf getBox(const char* geometryString)
	{
		return osg::BoundingBoxf();
	}
	
	// Поток
	bool getStream(
	    const char* geometryString,
	    const char* streamName,
		edtg::Stream& stream
		){return this->getStream(geometryString, streamName, stream.vb, stream.stream);}
	virtual bool getStream(
	    const char* geometryString,
	    const char* streamName,
	    render::VertexBuffer& buffer,
	    render::stream_handle_t& stream
	) = 0;

	// Параметр
	virtual ed::string getParameter(
	    const char* geometryString,
	    const char* paramName
	) = 0;

	// Primitives
	virtual bool getPrimitives(
	    const char* geometryString,
	    render::IndexBuffer& buffer,
	    render::PRIMTYPE_ENUM& primType,
	    int& startPrim,
	    int& faceCount
	) = 0;

	virtual bool getStructured(
	    const char* geometryString,
	    render::StructuredBuffer& buffer
		){return false;};


	// Загружена?
	virtual bool isLoaded() const
	{
		return bLoaded;
	}

	// Сбросить геометрию в файл
	virtual void toDumpScene(
	    const char* geometryString,
	    offshore::Scene* scene,
	    int parent
	) {}

	// обязателен в дочерних классах
	virtual ~IGeometry() {}

	// хак! доступ к исходным данным
	virtual const geometry4::GeometrySource* getGeometrySource()const{return 0;};
public:
	IGeometry();
	void addRef();
	void release();
	int getRefCount() const;
protected:
	int refcount;
	bool bLoaded;
};

// врапер для IGeometry
// пока не используется
class Geometry
{
	IGeometry* pResource;
public:
	Geometry();
	inline Geometry(edtg::IGeometry* arg);
	inline Geometry(const Geometry& arg);
	inline Geometry& operator=(const Geometry& arg);
	inline bool operator !()const;
	IGeometry* get(){return pResource;};
	~Geometry();
public:
	// бокс
	inline osg::BoundingBoxf getBox(
	    const char* geometrystring
	);
	// Поток
	inline bool getStream(
	    const char* geometrystring,
	    const char* streamname,
	    edtg::Stream& stream
		);
	inline bool getStream(
	    const char* geometrystring,
	    const char* streamname,
	    render::VertexBuffer& buffer,
	    render::stream_handle_t& stream
		);
	// Параметр
	inline ed::string getParameter(
	    const char* geometrystring,
	    const char* paramname
	);
	// Primitives
	inline bool getPrimitives(
	    const char* geometrystring,
	    render::IndexBuffer& buffer,
	    render::PRIMTYPE_ENUM& primtype,
	    int& startprim,
	    int& facecount
	);
	inline bool getStructured(
	    const char* geometryString,
	    render::StructuredBuffer& buffer
		);	
	inline bool isLoaded(
	);
	// Сбросить геометрию в файл
	inline void toDumpScene(
	    const char* geometrystring,
	    offshore::Scene* scene,
	    int parent
	);
	// хак! доступ к исходным данным
	inline const geometry4::GeometrySource* getGeometrySource()const;
};
}

inline edtg::IGeometry::IGeometry()
{
	refcount = 0;
	bLoaded = false;
}
inline void edtg::IGeometry::addRef()
{
	refcount++;
}
inline void edtg::IGeometry::release()
{
	refcount--;
	if(refcount<=0)
		delete this;
}

inline int edtg::IGeometry::getRefCount() const
{
	return refcount;
}

inline edtg::Geometry::Geometry()
{
	pResource = NULL;
}
inline edtg::Geometry::Geometry(edtg::IGeometry* arg)
{
	pResource = arg;
	if(pResource)
		pResource->addRef();
}

inline edtg::Geometry::Geometry(const edtg::Geometry& arg)
{
	pResource = NULL;
	pResource = arg.pResource;
	if(pResource)
		pResource->addRef();
}
inline edtg::Geometry& edtg::Geometry::operator=(const edtg::Geometry& arg)
{
	if(pResource)
		pResource->release();
	pResource = NULL;

	pResource = arg.pResource;
	if(pResource)
		pResource->addRef();

	return *this;
}
inline edtg::Geometry::~Geometry()
{
	if(pResource)
		pResource->release();
	pResource = NULL;
}
inline bool edtg::Geometry::operator !()const
{
	return !pResource;
}

// бокс
inline osg::BoundingBoxf edtg::Geometry::getBox(
    const char* geometrystring
)
{
	if(!pResource)
		return osg::BoundingBoxf();
	return pResource->getBox(geometrystring);
}

// Поток
inline bool edtg::Geometry::getStream(
    const char* geometrystring,
    const char* streamname,
    edtg::Stream& stream
	)
{
	if(!pResource)
		return false;
	if(!pResource->getStream(
	            geometrystring,
	            streamname,
	            stream.vb,
	            stream.stream
	        ))
		return false;
	if(stream.stream==render::INVALID_STREAM_HANDLE)
		return false;
	return true;
}
inline bool edtg::Geometry::getStream(
    const char* geometrystring,
    const char* streamname,
    render::VertexBuffer& buffer,
    render::stream_handle_t& stream
)
{
	if(!pResource)
		return false;
	if(!pResource->getStream(
	            geometrystring,
	            streamname,
	            buffer,
	            stream
	        ))
		return false;
	if(stream==render::INVALID_STREAM_HANDLE)
		return false;
	return true;
}
// Параметр
inline ed::string edtg::Geometry::getParameter(
    const char* geometrystring,
    const char* paramname
)
{
	if(!pResource)
		return "";
	return pResource->getParameter(
	           geometrystring, paramname);
}
// Primitives
inline bool edtg::Geometry::getPrimitives(
    const char* geometrystring,
    render::IndexBuffer& buffer,
    render::PRIMTYPE_ENUM& primtype,
    int& startprim,
    int& facecount
)
{
	if(!pResource)
		return "";
	return pResource->getPrimitives(
	           geometrystring,
	           buffer,
	           primtype,
	           startprim,
	           facecount
	       );
}
inline bool edtg::Geometry::getStructured(
	const char* geometryString,
	render::StructuredBuffer& buffer
	)
{
	if(!pResource)
		return false;
	return pResource->getStructured( geometryString, buffer);
}

// Загружена и можно использовать
inline bool edtg::Geometry::isLoaded(
)
{
	if(!pResource)
		return false;
	return pResource->isLoaded();
}
// Сбросить геометрию в файл
inline void edtg::Geometry::toDumpScene(
    const char* geometrystring,
    offshore::Scene* scene,
    int parent
)
{
	if(!pResource)
		return;
	return pResource->toDumpScene(geometrystring, scene, parent);
}
// хак! доступ к исходным данным
const geometry4::GeometrySource* edtg::Geometry::getGeometrySource() const 
{
	if(!pResource)
		return 0;
	return pResource->getGeometrySource();
}

#endif
