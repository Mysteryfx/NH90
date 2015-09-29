//	Vector.h ******************************************************************

#pragma once

#ifndef NOOLDMAP
#include "Serializer\Serializer.h"
#endif

// Трехмерный вектор
class	Vector3
{
public:
float	x,y,z;

inline			Vector3		(void);
inline			Vector3		(const float*);
inline			Vector3		(float,float,float);
inline			Vector3		(const Vector3&);
// casting
operator		float*		();
operator const	float*		()					const;
// assignment operators
inline	Vector3&operator += (const Vector3&);
inline	Vector3&operator -= (const Vector3&);
inline	Vector3&operator *= (float);
inline	Vector3&operator /= (float);
inline	Vector3&operator ^=	(const Vector3&);
// unary operators
inline	Vector3 operator +	()					const;
inline	Vector3 operator -	()					const;
// binary operators
inline	Vector3 operator +	(const Vector3&)	const;
inline	Vector3 operator -	(const Vector3&)	const;
inline	float	operator *	(const Vector3&)	const;
inline	Vector3 operator *	(float)				const;
inline	Vector3 operator /	(float)				const;
inline	Vector3 operator ^	(const Vector3&)	const;
inline	bool	operator == (const Vector3&)	const;
inline	bool	operator != (const Vector3&)	const;

inline	friend Vector3 operator * (float, const class Vector3&);

inline	float	length		(void)				const;
inline	float	lengthSq	(void)				const;
inline	Vector3&scale		(float);
inline	void	set_length	(float);
inline	void	norm		(void);
inline	Vector3 normed		(void)				const;
inline	void	set			(float,float,float);
inline	void	null		(void);

// возвращает true если все координаты меньше соответствующих в v.

inline	bool	less_all	(const Vector3&v)	const;
inline	bool	more_all	(const Vector3&v)	const;

// возвращает true если хоть одна координата меньше соответствующей в v.

inline	bool	less_any	(const Vector3&v)	const;
inline	bool	more_any	(const Vector3&v)	const;
};

// Константная ссылка
typedef const Vector3& CRVector3;

template <typename T>
inline Vector3 toVec3(const T& arg);

#include "vector.inl"

