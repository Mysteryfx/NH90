// пересечение бокса с плоскостью
inline IntersectionType IntersectPlane(const Plane& plane, const Box& box)
{
	return IntersectPlane(plane, box.min.x, box.min.y, box.min.z, box.max.x, box.max.y, box.max.z);
}

inline IntersectionType IntersectPlane(const Plane& plane, float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
	float bz, bZ;
	PlaneDistances(plane, minx, miny, minz, maxx, maxy, maxz, bz, bZ);
	if(bz > 0) return itOutside;
	if(bZ < 0) return itContains;
	return itIntersects;
}

inline IntersectionType IntersectPlane(const Plane& plane, const Position3& boxpos, const Box& box)
{
	int outside = 0;

	if(plane.distanceToPoint(box.xyz() * boxpos) > 0) outside++;
	if(plane.distanceToPoint(box.xyZ() * boxpos) > 0) outside++;
	if(plane.distanceToPoint(box.xYz() * boxpos) > 0) outside++;
	if(plane.distanceToPoint(box.xYZ() * boxpos) > 0) outside++;
	if(plane.distanceToPoint(box.Xyz() * boxpos) > 0) outside++;
	if(plane.distanceToPoint(box.XyZ() * boxpos) > 0) outside++;
	if(plane.distanceToPoint(box.XYz() * boxpos) > 0) outside++;
	if(plane.distanceToPoint(box.XYZ() * boxpos) > 0) outside++;

	if(outside == 0) return itContains;
	if(outside == 8) return itOutside;
	return itIntersects;
}

inline void PlaneDistances(const Plane& plane, const Box& box, float& mindist, float& maxdist)
{
	PlaneDistances(plane, box.min.x, box.min.y, box.min.z, box.max.x, box.max.y, box.max.z, mindist, maxdist);
}

inline void PlaneDistances(const Plane& plane, float minx, float miny, float minz, float maxx, float maxy, float maxz, float& bz, float& bZ)
{
	bz = plane.d;
	bZ = plane.d;
	const Vector3& dir = plane.n;
	
	if(dir.x>0)
	{
		bz += minx*dir.x;
		bZ += maxx*dir.x;
	}
	else
	{
		bz += maxx*dir.x;
		bZ += minx*dir.x;
	}

	if(dir.y>0)
	{
		bz += miny*dir.y;
		bZ += maxy*dir.y;
	}
	else
	{
		bz += maxy*dir.y;
		bZ += miny*dir.y;
	}

	if(dir.z>0)
	{
		bz += minz*dir.z;
		bZ += maxz*dir.z;
	}
	else
	{
		bz += maxz*dir.z;
		bZ += minz*dir.z;
	}
}

inline void PlaneDistCheckPoint(const Plane& plane, const Vector3& p, float &bz, float &bZ)
{
	float d = plane.distanceToPoint(p);
	if(d < bz) bz = d;
	if(d > bZ) bZ = d;
}

inline void PlaneDistances(const Plane& plane, const Position3& boxpos, const Box& box, float& mindist, float& maxdist)
{
	mindist = 1e10f;
	maxdist = -1e10f;

	PlaneDistCheckPoint(plane, box.xyz() * boxpos, mindist, maxdist);
	PlaneDistCheckPoint(plane, box.xyZ() * boxpos, mindist, maxdist);
	PlaneDistCheckPoint(plane, box.xYz() * boxpos, mindist, maxdist);
	PlaneDistCheckPoint(plane, box.xYZ() * boxpos, mindist, maxdist);
	PlaneDistCheckPoint(plane, box.Xyz() * boxpos, mindist, maxdist);
	PlaneDistCheckPoint(plane, box.XyZ() * boxpos, mindist, maxdist);
	PlaneDistCheckPoint(plane, box.XYz() * boxpos, mindist, maxdist);
	PlaneDistCheckPoint(plane, box.XYZ() * boxpos, mindist, maxdist);
}
