//	Plane.h *******************************************************************

#pragma once

#include "MathExport.h"
#include "Vector.h"

class	MATH_API Plane	// Всегда приводится к нормированному виду.============
{
public:	Vector3	n;
		float   d;

		Plane	(){}
inline	Plane	(const float*);
inline	Plane	(float A, float B, float C, float D);
		Plane	(const Vector3& p, const Vector3& n);
		Plane	(const Vector3& p1,const Vector3& p2,const Vector3& p3);

inline	Plane	operator - () const;
const	Plane&	operator   ()(const Vector3& p, const Vector3& n);
const	Plane&	operator   ()(const Vector3& p1,const Vector3& p2,const Vector3& p3);
const	Plane&	operator   ()(float A, float B, float C, float D);
inline	bool	operator ==	 (const Plane&) const;
inline	bool	operator !=	 (const Plane&) const;

inline	float		distanceToPoint	(const Vector3&) const;
inline	Plane&		normalize		(void);
};
#include "Plane.inl"

