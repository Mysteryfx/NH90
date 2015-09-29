#pragma once

#include "MathExport.h"
#include "cPosition.h"
#include "position.h"
#include "ClipVolumePlanes.h"

// объем камеры (пирамидка)
class MATH_API ClipVolumeCamera : public ClipVolumePlanes
{
protected:
	Position3 cam;
	float len, angle, aspect; // cone length, horizontal angle, width/height
	float pnear;	// optional - near clipping plane
public:
	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell);
	
	ClipVolumeCamera();

	ClipVolumeCamera(const cPosition &_cam, float _len, float _angle, float _aspect = float(4.f/3.f));
	ClipVolumeCamera(const Position3 &_cam, float _len, float _angle, float _aspect = float(4.f/3.f));
	ClipVolumeCamera(const Position3 &_cam, float _pnear, float _pfar, float _angle, float _aspect);

	void set(const cPosition &_cam, float _len, float _angle, float _aspect = float(4.f/3.f));
	void set(const Position3 &_cam, float _len, float _angle, float _aspect = float(4.f/3.f));
	void set(const Position3 &_cam, float _pnear, float _pfar, float _angle, float _aspect);

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ);
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ);
};
