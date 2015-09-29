#pragma once

#include "MathExport.h"
#include "ClipVolume.h"
#include "plane.h"
#include "position.h"
#include "box.h"

// точка пересечения трех плоскостей
MATH_API Vector3 IntersectPlanes(const Plane& p1, const Plane& p2, const Plane& p3);

// пересечение бокса с плоскостью
inline IntersectionType IntersectPlane(const Plane& plane, const Box& box);
inline IntersectionType IntersectPlane(const Plane& plane, float x, float y, float z, float X, float Y, float Z);
inline IntersectionType IntersectPlane(const Plane& plane, const Position3& boxpos, const Box& box);

inline void PlaneDistances(const Plane& plane, const Box& box, float& mindist, float& maxdist);
inline void PlaneDistances(const Plane& plane, float x, float y, float z, float X, float Y, float Z, float& mindist, float& maxdist);
inline void PlaneDistances(const Plane& plane, const Position3& boxpos, const Box& box, float& mindist, float& maxdist);

// максимальное количество плоскостей в объеме
#define CLIP_VOLUME_PLANES_MAX 10

// Выпуклый объем, ограниченный плоскостями
class MATH_API ClipVolumePlanes : public ClipVolume
{
protected:
	Plane planes[CLIP_VOLUME_PLANES_MAX];
    int count;

public:
    ClipVolumePlanes();

	void AddPlane(const Plane& plane);
	inline const Plane& GetPlane(int i) {return planes[i];}
	inline int GetPlaneCount() {return count;}

	/// Removes all planes.
	void clear();

	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell);

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ);
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ);

	IntersectionType IntersectOrientedBox(const Position3& pos, const Box& box) const;
};

#include "ClipVolumePlanes.inl"
