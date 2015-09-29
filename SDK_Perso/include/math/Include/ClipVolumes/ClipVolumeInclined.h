#pragma once

#include "MathExport.h"
#include "position.h"
#include "box.h"
#include "Plane.h"

#include "ClipVolumeBox.h"

// наклонный бокс - для сбора объектов отбрасывающих тени
class MATH_API ClipVolumeInclined : public ClipVolumeBox
{
protected:
	Box box;
	Vector3 sun;
	float maxdist;

	Plane px, pz;
	float minpx, minpz;
	float maxpx, maxpz;

public:
	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell);

	ClipVolumeInclined();
	ClipVolumeInclined(const Box& box, const Vector3& sun, float maxdist);

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ);
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ);

	IntersectionType IntersectOrientedBox(const Position3& pos, const Box& box) const;
};

