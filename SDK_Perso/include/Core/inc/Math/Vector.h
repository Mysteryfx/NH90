/**
 * Based on
 * EasyGL library v0.1
 * (C)opyright 2002 by Dmitry S. Baikov
 *
 * @file Vector.h
 * @author Dmitry S. Baikov
 *
 * @todo expression templates
 * @todo ET + SSE
 */
#ifndef _ED_Math_Vector_h_
#define _ED_Math_Vector_h_

#include "./_config.h"
#include "./_VectorBase.h"
#include "./_math.h"
#include "Util/CTAssert.h"


namespace Math {


/**
 * N-dimensional Vector.
 * \note typedef naming convention: \n
 *	typedef Vector<N,T> VecNt; \n
 *	where t is selected by the following rules: \n
 *	char -> b \n
 *	short -> s \n
 *	int -> i \n
 *	unsigned XXX -> uX \n
 *	\n
 *	float -> f \n
 *	double -> d \n
 *	\n
 *	typedefs should be sorted by type (integer types first) and then by size.
 */
template<int N, class T>
class Vector : public _VectorBase<N,T> {
public:
	typedef Vector<N,T> self_t;
	typedef T scalar_t;
	enum { Size = N };

		/**
		 * Create uninitialized vector.
		 */
		Vector() {}

		// Copying constructor
		Vector(const self_t& in) {*this = in;}

		/**
		 * Create vector with all components set to s.
		 */
		explicit Vector(T s) { set(s); }

		/**
		 * Create Vector<2,T>.
		 */
		Vector(T a, T b) { Util::CTAssert<N==2>(); v[0]=a; v[1]=b; }

		/**
		 * Create Vector<3,T>.
		 */
		Vector(T a, T b, T c) { Util::CTAssert<N==3>(); v[0]=a; v[1]=b; v[2]=c; }

		/**
		 * Create Vector<4,T>.
		 */
		Vector(T a, T b, T c, T d) { Util::CTAssert<N==4>(); v[0]=a; v[1]=b; v[2]=c; v[3]=d; }

		/**
		 * Generic copy constructor.
		 */
		template<class F>
		Vector(const Vector<N, F>& u) { for(int i=0; i<N; ++i) v[i]=T(u[i]); }

		template<class F>
		explicit Vector(const Vector<N-1, F>& u, const T f) { for(int i=0; i<N-1; ++i) v[i]=T(u[i]); v[N-1]=f; }

		template<class F>
		explicit Vector(const Vector<N+1, F>& u) { for(int i=0; i<N; ++i) v[i]=T(u[i]); }

	/**
	 * Set all components to specified value.
	 */
	self_t& set(T val) { for(int i=0; i<N; ++i) v[i] = val; return *this; }

	/**
	 * Set Vector<2,T> components.
	 */
	self_t& set(T v0, T v1) { Util::CTAssert<N==2>(); x = v0; y = v1; return *this; }

	/**
	 * Set Vector<3,T> components.
	 */
	self_t& set(T v0, T v1, T v2) { Util::CTAssert<N==3>(); x=v0; y=v1; z=v2; return *this; }

	/**
	 * Set Vector<4,T> components.
	 */
	self_t& set(T v0, T v1, T v2, T v3) { Util::CTAssert<N==4>(); x=v0; y=v1; z=v2; w=v3; return *this; }

	/**
	 * Get components array.
	 */
	T* data() { return v; }

	/**
	 * Get components array.
	 */
	const T* data() const { return v; }

	/**
	 * Get number of components.
	 */
	static int size() { return N; }

	/**
	 * Array-style component access.
	 */
	T& operator [] (int i) { return v[i]; }

	/**
	 * Array-style component access.
	 */
	const T& operator [] (int i) const { return v[i]; }

	/**
	 * Get negative copy.
	 */
	self_t operator - () const
	{
		self_t res;
		for(int i=0; i<N; ++i)
			res[i] = -v[i];
		return res;
	}


	// operators
#define _GEN_ASSIGN_OPERATOR( op ) \
	inline \
	self_t& operator op##= (const self_t& u) \
	{ for(int i=0; i<N; ++i) v[i] op##= u[i]; return *this; } \
	\
	inline \
	self_t& operator op##= (scalar_t s) \
	{ for(int i=0; i<size(); ++i) v[i] op##= s; return *this; }

	_GEN_ASSIGN_OPERATOR( + )
	_GEN_ASSIGN_OPERATOR( - )
	_GEN_ASSIGN_OPERATOR( * )
	_GEN_ASSIGN_OPERATOR( / )

#undef _GEN_ASSIGN_OPERATOR


#define _GEN_BINARY_OPERATOR( op ) \
	inline \
	self_t operator op (const self_t& u) const \
	{ self_t r; for(int i=0; i<N; ++i) r[i] = v[i] op u[i]; return r; } \
	\
	inline \
	self_t operator op (scalar_t s) const \
	{ self_t r; for(int i=0; i<N; ++i) r[i] = v[i] op s; return r; }

	_GEN_BINARY_OPERATOR( + )
	_GEN_BINARY_OPERATOR( - )
	_GEN_BINARY_OPERATOR( * )
	_GEN_BINARY_OPERATOR( / )

#undef _GEN_BINARY_OPERATOR


#define _GEN_SCALAR_OPERATOR( op ) \
	friend inline \
	self_t operator op (scalar_t s, const self_t& v) \
	{ self_t r; for(int i=0; i<N; ++i) r[i] = s op v[i]; return r; }

	_GEN_SCALAR_OPERATOR( + )
	_GEN_SCALAR_OPERATOR( - )
	_GEN_SCALAR_OPERATOR( * )
	_GEN_SCALAR_OPERATOR( / )

#undef _GEN_SCALAR_OPERATOR



}; // class Vector<N, T>

// Specializations of operators for N == 3
#define _GEN_BINARY_OPERATOR( op ) \
	template<> \
	inline \
	Vector<3, double> Vector<3, double>::operator op (const Vector<3, double>& u) const \
	{ self_t r; r.x = x op u.x; r.y = y op u.y; r.z = z op u.z; return r; } \
	\
	template <> \
	inline \
	Vector<3, double> Vector<3, double>::operator op (double s) const \
	{ self_t r; r.x = x op s; r.y = y op s; r.z = z op s; return r; }

_GEN_BINARY_OPERATOR( + )
_GEN_BINARY_OPERATOR( - )
_GEN_BINARY_OPERATOR( * )
_GEN_BINARY_OPERATOR( / )

#undef _GEN_BINARY_OPERATOR

/**
 * Get squared euclidian length (sum of squared components).
 */
template<int N, class T>
inline
T length2(const Vector<N, T>& v)
{
	T res = v[0] * v[0];
	for( int i=1; i<N; ++i )
		res += v[i] * v[i];
	return res;
}

/**
 * Get euclidian length sqrt(sum of squared components).
 */
template<int N, class T>
inline
T length(const Vector<N, T>& v)
{
	return T(sqrt( length2(v) ));
}

/**
 * Get normalized copy.
 */
template<int N, class T>
inline
Vector<N, T> normalize(const Vector<N, T>& v)
{
	return v / length(v);
}

/**
 * Calculate dot product.
 */
template<int N, class T>
inline
T dot(const Vector<N, T>& u, const Vector<N, T>& v)
{
	T res = u[0] * v[0];
	for(int i=1; i<N; ++i)
		res += u[i] * v[i];
	return res;
}

/**
 * Calculate cross product.
 */
template<class T>
inline
Vector<3, T> cross(const Vector<3, T>& a, const Vector<3, T>& b)
{
	Vector<3, T> res;
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
	return res;
}

/**
 * Get homogenized copy.
 */
template<int N, class T>
Vector<N-1, T> homogenize(const Vector<N, T>& v)
{
	Vector<N-1, T> res;
	for(int i=0; i<N-1; ++i)
		res[i] = v[i]/v[N-1];
	return res;
}


/**
 * Linear interpolation.
 */
template<int N, class T> inline
Vector<N, T> lerp(const Vector<N,T>& a, const Vector<N,T>& b, T factor)
{
//	return a*(1-factor) + b*factor;
	Vector<N,T> res;
	for(int i=0; i<N; ++i)
		res[i] = a[i]*(1-factor) + b[i]*factor;
	return res;
}




// common typedefs
typedef Vector<4, char> Vec4b;

typedef Vector<2, int> Vec2i;
typedef Vector<3, int> Vec3i;

typedef Vector<2, float> Vec2f;
typedef Vector<3, float> Vec3f;
typedef Vector<4, float> Vec4f;

typedef Vector<2, double> Vec2d;
typedef Vector<3, double> Vec3d;
typedef Vector<4, double> Vec4d;

template<int N, class T>
inline Vector<N, T> min(const Vector<N, T> & left, const Vector<N, T> & right)
{
	Vector<N, T> res;
	for(int i = 0; i < N; i++)
		res[i] = left[i] < right[i] ? left[i] : right[i];
	return res;
}

template<int N, class T>
inline Vector<N, T> max(const Vector<N, T> & left, const Vector<N, T> & right)
{
	Vector<N, T> res;
	for(int i = 0; i < N; i++)
		res[i] = left[i] > right[i] ? left[i] : right[i];
	return res;
}

}

/**
 * Write/Load components to/from a stream.
 */
template<int N, class T, class Stream> inline
Stream& operator >> (Stream& out, Math::Vector<N, T>& v)
{
	for( int i=0; i<N; ++i )
		out >> v[i];
	return out;
}

#endif /* _ED_Math_Vector_h_ */
