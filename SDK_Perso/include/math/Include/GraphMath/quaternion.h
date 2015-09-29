#pragma once

#include "MathExport.h"
#include "matrix.h"

// при всех вычисления поддерживается единичная норма кватерниона
class MATH_API Quaternion
{
public:
	static Quaternion One;

	float w;
	Vector3 v;

	Quaternion();
	Quaternion(float w, const Vector3& v);
	Quaternion(float w, float x, float y, float z);
	Quaternion(const Matrix33& m);

	// нормирование
	void norm();
	Quaternion normed() const;

	// инвертирование (предпологает, что кватернион единичный)
	void invert();
	Quaternion operator ~() const;

	// единичный кватернион
	void unit();

	// Угол поворота
	void set_angle(float rad);
	float get_angle() const;

	// Ось вращения
	void set_axis(const Vector3& axis);
	Vector3 get_axis() const;

	// Задать кватернион осью и углом
	void set_axis_angle(const Vector3& axis, float rad);
	// Задать кватернион двумя векторами
	void set_2_vectors(const Vector3& from, const Vector3& to, bool bNeedNormalize=true);

	// Углы Эйлера
	void GetEulerAngles(float &yaw, float &pitch, float &roll) const;
	void SetEulerAngles(float yaw, float pitch, float roll);

	// операции между кватернионами
	inline	bool	operator == (const Quaternion& op)	const;
	Quaternion operator *(const Quaternion& op) const;
	Quaternion operator /(const Quaternion& op) const;
	Quaternion operator +(const Quaternion& op) const;
	Quaternion operator -(const Quaternion& op) const;
	Quaternion& operator *=(const Quaternion& op);
	Quaternion& operator /=(const Quaternion& op);
	Quaternion& operator +=(const Quaternion& op);
	Quaternion& operator -=(const Quaternion& op);

	// умножение на скаляр
	Quaternion operator *(float c) const;
	Quaternion& operator *=(float c);
	Quaternion operator /(float c) const;
	Quaternion& operator /=(float c);

	// приведение к матрице
	operator Matrix33() const;
};

MATH_API Quaternion operator *(float c, const Quaternion& op);
MATH_API void Slerp(Quaternion *res, const Quaternion *q, const Quaternion *p, float t);

