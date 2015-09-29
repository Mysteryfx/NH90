#ifndef EDTG_IGeometryStorage_H
#define EDTG_IGeometryStorage_H

namespace geometry3
{
	struct GeometrySource;
}

namespace edtg
{
struct IGeometry;

// Интерфейс для сторонних хранилищ геометрии
struct IGeometryStorage
{
// IGeometryStorage
public:
	virtual edtg::IGeometry* findGeometry( geometry3::GeometrySource* source)=0;
	virtual void addGeometry( geometry3::GeometrySource* source, edtg::IGeometry* geometry)=0;
};

}

#endif
