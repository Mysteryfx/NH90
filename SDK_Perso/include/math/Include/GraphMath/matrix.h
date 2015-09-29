#pragma once

#include "../MathExport.h"
#include "vector.h"

#pragma pack(4)

// матрица 3x3
class MATH_API Matrix33
{
public:
	// Константы
	static const Matrix33 One;
	static const Matrix33 Zero;
	
	// вставлены служебные поля, для совместимости с аппаратными матрицами 4x4
public: Vector3 x; private: float dummy_x;
public: Vector3 y; private: float dummy_y;
public: Vector3 z; private: float dummy_z;

public:
	Matrix33();
	Matrix33(const Vector3& d);
	Matrix33(const Vector3& _x, const Vector3& _y, const Vector3& _z);

	// Создание матриц - преобразований
	static Matrix33 Rotation(const Vector3& from, const Vector3& to);
	static Matrix33 Rotation(const Vector3& axis, float rad);
	static Matrix33 Rotation(float ax, float ay, float az, float rad);
	static Matrix33 Scaling(const Vector3& op);
	static Matrix33 Scaling(float sx, float sy, float sz);

	// предпологается, что xyz - ортонормированный базис
	Matrix33 operator ~() const;
	void Transp();

	void Scale(float c);
	void Scale(const Vector3& op);
	void Scale(float sx, float sy, float sz);

	// повороты вокруг основных осей
	void RotateX(float rad);
	void RotateY(float rad);
	void RotateZ(float rad);

	// поворот вокруг произвольной оси (ось должна быть нормирована)
	void Rotate(const Vector3& axis, float rad);
	void Rotate(float ax, float ay, float az, float rad);

	Vector3 operator *(const Vector3& op) const;
	Matrix33 operator *(const Matrix33& op) const;
	Matrix33& operator *=(const Matrix33& op);
};

MATH_API Vector3 __fastcall operator *(const Vector3& op, const Matrix33& m);
MATH_API Vector3& __fastcall operator *=(Vector3& op, const Matrix33& m);

#pragma pack()
