#pragma once

#include "MathExport.h"
#include "vector.h"

// бокс в пространстве
class MATH_API Box
{
public:
	Vector3 min, max;

	inline Box();
	inline Box(const Vector3& _min, const Vector3& _max);
	inline Box(float x, float y, float z, float X, float Y, float Z);
	inline Box(const Vector3& center, float size); // Куб

	inline bool operator ==(const Box& arg) const;
	inline bool operator !=(const Box& arg) const;

	// расстояние между min и max
	inline float diameter() const;

	// радиус описанной сферы
	inline float radius() const;

	// центр
	inline Vector3 center() const	{ return (max+min)/2; }

	// габариты
	inline float DX() const	{ return max.x - min.x; }
	inline float DY() const	{ return max.y - min.y; }
	inline float DZ() const	{ return max.z - min.z; }

	// углы
	inline Vector3 xyz() const	{ return Vector3(min.x, min.y, min.z); }
	inline Vector3 xyZ() const	{ return Vector3(min.x, min.y, max.z); }
	inline Vector3 xYz() const	{ return Vector3(min.x, max.y, min.z); }
	inline Vector3 xYZ() const	{ return Vector3(min.x, max.y, max.z); }
	inline Vector3 Xyz() const	{ return Vector3(max.x, min.y, min.z); }
	inline Vector3 XyZ() const	{ return Vector3(max.x, min.y, max.z); }
	inline Vector3 XYz() const	{ return Vector3(max.x, max.y, min.z); }
	inline Vector3 XYZ() const	{ return Vector3(max.x, max.y, max.z); }

	// угол по номеру (от 0 до 7)
	inline Vector3 corner(int i) const;

	// нормален ли бокс (max >= min)
	inline bool isValid() const { return !max.less_any(min);}

	// восстановить начальное состояние
	inline void reset();

	// объединение
	void insertBox(const Box& arg);
	void InsertPoint(const Vector3& pt, bool bFirst=false);

	// Пересечение
	inline	bool intersectionXZ	 (const Box& arg) const;
	bool intersectionXZ	 (const Box& arg, Box& res) const;
	bool intersectSegment(const Vector3& a, const Vector3& b, float *t = 0) const;
	bool intersectSegment2(const Vector3& a, const Vector3& b, float & t1, float & t2) const;

	// Тесты на включение
	inline bool isInside(const Vector3& p)	const { return !p.less_any(min) && !p.more_any(max); }
	bool isInsideXZ(const Vector3& p) const;

	// расстояние до точки
	float distanceTo(const Vector3& p) const;
};

#include "box.inl"
