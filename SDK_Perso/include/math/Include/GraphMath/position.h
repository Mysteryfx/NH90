//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------

#include "../MathExport.h"
#include "matrix.h"

// Позиция в пространстве:
// точка и 3 вектора направления (желательно отронормированных)
// x - вперед
// y - вверх
// z - вправо

#pragma pack(4)

class MATH_API Position3 : public Matrix33
{
public:
	// Константы
	static const Position3 One;
	static const Position3 Zero;

	// служебное поле - для совместимости с аппаратными матрицами 4x4
public: Vector3 p; private: float dummy_p;

public:
	Position3();
	Position3(const Vector3& _p);
	Position3(const Matrix33& m);
	Position3(const Vector3& _x, const Vector3& _y, const Vector3& _z, const Vector3& _p);

	void Norm();

	void SetTransform(const Matrix33& m);

	// предпологается, что xyz - ортонормированный базис
	Position3 operator ~() const;
	void Invert();
	
	// полноценная инвертация матрицы
	Position3 TrueInverted() const;
	void TrueInvert();

	// нормализация, в порядке букв
	void NormXYZ();
	void NormXZY();
	void NormYXZ();
	void NormYZX();
	void NormZXY();
	void NormZYX();
	void YZX();

	void Move(const Vector3& op);
	void Move(float dx, float dy, float dz);

	void Scale(float c);
	void Scale(const Vector3& op);
	void Scale(float sx, float sy, float sz);

	Position3 operator +(const Vector3& op) const;
	Position3 operator -(const Vector3& op) const;

	Position3& operator +=(const Vector3& op);
	Position3& operator -=(const Vector3& op);

	Vector3 operator *(const Vector3& op) const
	{
		return Vector3(	(x.x*op.x + y.x*op.y + z.x*op.z + p.x),
						(x.y*op.x + y.y*op.y + z.y*op.z + p.y),
						(x.z*op.x + y.z*op.y + z.z*op.z + p.z));
	}
	Position3 operator *(const Position3& op) const;
	Position3 operator /(const Position3& op) const;

	Position3& operator *=(const Position3& op);
	Position3& operator /=(const Position3& op);
};

MATH_API Vector3 __fastcall operator *(const Vector3& op, const Position3& pos);
MATH_API Vector3& __fastcall operator *=(Vector3& op, const Position3& pos);

template <typename T>
inline Position3 toPos3(const T& arg)
{
	return Position3( toVec3(arg.x), toVec3(arg.y), toVec3(arg.z), toVec3(arg.p));
}

#pragma pack()


