#pragma once

#include "MathExport.h"
#include "Vector.h"
#include "Position.h"
#include "box.h"
#include "Plane.h"

#include <math.h>

inline float fsqrt(float v) {
float t;
t = sqrt(v);
/*
__asm
{
    fld v
    fsqrt
    fstp t
}
*/
return t;
}

void MATH_API __fastcall rotate(void* fulcrum, void* lever, float angle);
void MATH_API __fastcall rotate2(void* fulcrum, void* lever, float angsin,float angcos);

class MATH_API Plane3D
{
public:
    
    Vector3 n;
    float   d;

    Plane3D() : n(1,0,0), d(0) {}
    Plane3D( const Vector3 &_n, const float _d )
    { n = _n; d = _d; Norm(); }

    void  Transform( const Position3 & pos );
    Vector3 Intersect( const Vector3 & vec );
    float DistanceFromPoint( const Vector3 & vec );

    void  Norm()
    { float f = 1.0f / n.length(); d *= f; n *= f; }

    void  ClipLe( float angle );
    void  ClipRi( float angle );
    void  ClipUp( float angle );
    void  ClipDo( float angle );
    void  ClipFa( float farClip  );
    void  ClipNe( float nearClip );
};

// пересекаются ли два бокса
// pos - позиция второго бокса относительно первого
// боксы считаются центрированными, их радиусы - a и b соответственно
bool MATH_API __fastcall BoxBoxIntersect(
	const Position3& pos,
	const Vector3& a, const Vector3& b);

bool MATH_API __fastcall BoxBoxIntersect(
	const ::Box& box1, const Position3& pos1,
	const ::Box& box2, const Position3& pos2);

// расстояние от точки p до отрезка [a,b]
float MATH_API __fastcall PointSegmentDistance(const Vector3& a, const Vector3& b, const Vector3& p);
