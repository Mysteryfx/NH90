#pragma once

#include "MathExport.h"
#include <ed/list.h>

#include "ClipVolume.h"

// Составные фигуры (объединение, пересечение, вычитание)
class ClipVolumeCompound : public ClipVolume
{
protected:
	// части составного объекта
	typedef ed::list<ClipVolume *> partsList;
	partsList parts;

public:
	MATH_API virtual void AddPart(ClipVolume *part);
};

// объединение
class ClipVolumeUnion : public ClipVolumeCompound
{
public:
	// подготовка фигуры к поиску
	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell);

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ);
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ);
};

// пересечение
class ClipVolumeIntersection : public ClipVolumeCompound
{
public:
	// подготовка фигуры к поиску
	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell);

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ);
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ);
};

// вычитание (из первой - все остальные)
class ClipVolumeSubstraction : public ClipVolumeCompound
{
public:
	// подготовка фигуры к поиску
	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell);

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ);
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ);
};
