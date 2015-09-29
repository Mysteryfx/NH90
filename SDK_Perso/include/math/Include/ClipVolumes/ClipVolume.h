#pragma once

#include "MathExport.h"
#include "IntersectionType.h"

// базовый класс описывает фигуру в пространстве
// для использования при поиске (выборке) по объему
class MATH_API ClipVolume
{
friend class MapObjectsStorage;
friend class MapTreeNode;

public:
	// границы фигуры (должны быть проинициализированны до поиска)
	float x,y,z;
	float X,Y,Z; 

	// подготовка фигуры к поиску
	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell) = 0;

	virtual ~ClipVolume() {};
	
	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ) = 0;
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ) = 0;
	virtual IntersectionType IntersectShape(ClipVolume *shape); // можно заложиться на особое взаимодействие определенных фигур
};
