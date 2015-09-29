#pragma once

#include "MathExport.h"
#include "cPosition.h"

#include "ClipVolume.h"

// фигура - конус неопределенности (плоский)
class MATH_API ClipVolumeCone : public ClipVolume
{
private:
	cPosition pos;
	float len, angle;

	// запоминаем как три границы - прямые в виде Ax + Bz + C = 0
	// cVector(A,-C,B)
	cVector side[3];

protected:
	void CreateLine(cVector &line, const cPoint& a, const cPoint& b);

	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell);

public:
	// позиция, расстояние, половинный угол раствора
	ClipVolumeCone(const cPosition& _pos, float _len, float _angle);

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ);
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ);
};

