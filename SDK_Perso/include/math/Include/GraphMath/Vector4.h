//	Vector4.h *****************************************************************

#pragma once

class Vector4
{
public:
float x, y, z, w;
inline Vector4		(void):x(0.0f), y(0.0f), z(0.0f), w(0.0f){};
inline	Vector4		(const float*);
inline	Vector4		(float,float,float,float);

inline				operator		float* ();
inline				operator const	float* ()					const;

inline	float  &	operator []		(int)						const;
inline	Vector4&	operator ()		(float,float,float,float)	const;
inline	bool		operator ==		(const Vector4&)			const;
inline	bool		operator !=		(const Vector4&)			const;
inline	Vector4&	operator +=		(const Vector4&);
inline	Vector4&	operator -=		(const Vector4&);
inline	Vector4&	operator *=		(float);
inline	Vector4&	operator /=		(float);
inline	float		operator *		(const Vector4&)			const;
inline	Vector4		operator +		(const Vector4&)			const;
inline	Vector4		operator -		(const Vector4&)			const;
inline	Vector4		operator *		(float)						const;
inline	Vector4		operator /		(float)						const;
inline	Vector4		operator +		()							const;
inline	Vector4		operator -		()							const;

friend	Vector4		operator *		(float,const class Vector4&);

inline	Vector4&	add				(const Vector4&,const Vector4&);
inline	float		dot				(const Vector4&);
inline	float		length			(void);
inline	float		lengthSq		(void);
inline	Vector4&	lerp			(const Vector4&,const Vector4&,float);
inline	Vector4&	maximize		(const Vector4&,const Vector4&);
inline	Vector4&	minimize		(const Vector4&,const Vector4&);
inline	Vector4&	normalize		(void);
inline	Vector4&	scale			(const Vector4&,float);
inline	Vector4&	subtract		(const Vector4&,const Vector4&);
};

#include "Vector4.inl"

