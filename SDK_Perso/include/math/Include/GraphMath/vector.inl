//	vector.inl ****************************************************************

#pragma once
#include   <math.h>

inline	Vector3::Vector3(void)
{
x=y=z=0.f;
}
inline	Vector3::Vector3(const float* v)
{
x=v[0];
y=v[1];
z=v[2];
}

inline	Vector3::Vector3(float X,float Y,float Z)
{
x=X;
y=Y;
z=Z;
}

inline	Vector3::Vector3(const Vector3&v)
{
x=v.x;
y=v.y;
z=v.z;
}

inline
Vector3::operator float* ()
{
return (float *)&x;
}

inline
Vector3::operator const float* () const
{
return (const float *)&x;
}

inline	Vector3 &Vector3::operator +=(const Vector3 &v)
{
x+=v.x;
y+=v.y;
z+=v.z;	return *this;
}

inline	Vector3 &Vector3::operator -=(const Vector3 &v)
{
x-=v.x;
y-=v.y;
z-=v.z;	return *this;
}

inline	Vector3 &Vector3::operator *=(float f)
{
x*=f;
y*=f;
z*=f;	return *this;
}

inline	Vector3 &Vector3::operator /=(float f)
{
float	fInc = 1.f/f;

x*=fInc;
y*=fInc;
z*=fInc;	return *this;
}

inline	Vector3 &Vector3::operator ^=(const Vector3 &v)
{
*this = *this ^ v;	return *this;
}

inline	Vector3 Vector3::operator +(void) const
{
return *this;
}

inline	Vector3 Vector3::operator -(void) const
{
return Vector3(-x,-y,-z);
}

inline	Vector3 Vector3::operator +(const Vector3 &v) const
{
return Vector3(x+v.x,y+v.y,z+v.z);
}

inline	Vector3 Vector3::operator -(const Vector3 &v) const
{
return Vector3(x-v.x,y-v.y,z-v.z);
}

inline	float Vector3::operator *(const Vector3 &v) const
{
return x*v.x+y*v.y+z*v.z;
}

inline	Vector3 Vector3::operator *(float f) const
{
return Vector3(x*f,y*f,z*f);
}

inline	Vector3 Vector3::operator /(float f) const
{
float fInv = 1.f/f;

return Vector3(x*fInv,y*fInv,z*fInv);
}

inline	Vector3 Vector3::operator ^(const Vector3 &v) const
{
float X = y*v.z - z*v.y;
float Y = z*v.x - x*v.z;
float Z = x*v.y - y*v.x;	return Vector3(X,Y,Z);
}

inline	bool Vector3::operator ==(const Vector3& v) const
{
return (x==v.x)&&(y==v.y)&&(z==v.z);
}

inline	bool Vector3::operator !=(const Vector3& v) const
{
return (x!=v.x)||(y!=v.y)||(z!=v.z);
}

inline	Vector3	operator * (float f, const class Vector3& v)
{
return Vector3(f*v.x,f*v.y,f*v.z);
}

inline	float Vector3::length(void) const
{
return sqrtf(x*x+y*y+z*z);
}

inline	float Vector3::lengthSq(void) const
{
return x*x+y*y+z*z;
}

inline	Vector3 &Vector3::scale(float s)
{
x*= s; y*= s; z*= s; return *this;
}

inline	void Vector3::set_length(float l)
{
float cur = length();	if	(cur > 0.00001f) (*this) *= l/cur;
}

inline	void Vector3::norm(void)
{
set_length(1.f);
}

inline	Vector3 Vector3::normed(void) const
{
float cur = length();

if	(cur > 0.00001f) return *this/cur;
else				 return *this;
}

inline	void Vector3::set(float X, float Y, float Z)
{
x=X; y=Y; z=Z;
}

inline	void Vector3::null(void)
{
x=y=z=0.f;
}

inline	bool Vector3::less_all(const Vector3& v) const
{
return (x<v.x)&&(y<v.y)&&(z<v.z);
}

inline	bool Vector3::more_all(const Vector3& v) const
{
return (x>v.x)&&(y>v.y)&&(z>v.z);
}

inline	bool Vector3::less_any(const Vector3& v) const
{
return (x<v.x)||(y<v.y)||(z<v.z);
}

inline	bool Vector3::more_any(const Vector3& v) const
{
return (x>v.x)||(y>v.y)||(z>v.z);
}

template <typename T>
inline Vector3 toVec3(const T& arg)
{
	return Vector3(arg.x, arg.y, arg.z);
}
