#ifndef CustomGeometry_H
#define CustomGeometry_H

#include "vector.h"
#include "Color.h"

#include <ed/vector.h>


namespace Graphics
{

// тип примитивов в создаваемой геометрии
enum PrimitiveType
{
	ptPoints,		// точки
	ptLines,		// отрезки
	ptTriangles,	// треугольники
};

// Интерфейс для элемента геометрии, который можно программно формировать
class CustomGeometry
{
public:
	typedef ed::vector<Vector3>        VertexList;
	typedef ed::vector<unsigned short> IndexList;
	typedef ed::vector<Color>          ColorList;

	virtual ~CustomGeometry(){}

	// установка параметров
	virtual void SetPrimitiveType(PrimitiveType ptype) = 0;
	virtual void SetScreenSpace(bool isScreenSpace) = 0;

	virtual void SetVertices(const VertexList& vertices) = 0;
	virtual void SetIndices(const IndexList& indices) = 0;
	virtual void SetNormals(const VertexList& normals) = 0;
	virtual void SetTexCoords(const VertexList& texCoords,const int index = 0) = 0;
	virtual void SetColors(const ColorList& colors) = 0;
	
	virtual void SetVertices(VertexList&& vertices) = 0;
	virtual void SetIndices(IndexList&& indices) = 0;
	virtual void SetNormals(VertexList&& normals) = 0;
	virtual void SetTexCoords(VertexList&& texCoords,const int index = 0) = 0;
	virtual void SetColors(ColorList&& colors) = 0;

	// получение параметров
	virtual PrimitiveType GetPrimitiveType() = 0;
	virtual bool IsScreenSpace() = 0;
	virtual const VertexList& GetVertices() = 0;
	virtual const IndexList& GetIndices() = 0;
	virtual const VertexList& GetNormals() = 0;
	virtual const VertexList& GetTexCoords(const int index = 0) = 0;
	virtual const ColorList& GetColors() = 0;

	virtual VertexList&& MoveVertices() = 0;
	virtual IndexList&& MoveIndices() = 0;
	virtual VertexList&& MoveNormals() = 0;
	virtual VertexList&& MoveTexCoords(const int index = 0) = 0;
	virtual ColorList&& MoveColors() = 0;
    
    virtual bool  HasAlpha() = 0;
};

}

#endif // CustomGeometry_H
