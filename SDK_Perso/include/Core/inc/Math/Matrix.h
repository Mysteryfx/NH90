/**
 * Based on
 * EasyGL library v0.1
 * (C)opyright 2002 by Dmitry S. Baikov
 *
 * @file Matrix.h
 * @author Dmitry S. Baikov
 */
#ifndef _ED_Math_Matrix_h_
#define _ED_Math_Matrix_h_


#include "./_config.h"
#include "Util/CTAssert.h"

#include "./Vector.h"


namespace Math {


template<int N, int M, typename T> class AsMatrix;
template<int N, typename T> class Matrix;

// --- matrix operations

// multiply matrix by matrix
template<int N, typename T>
ED_MATH_EXTERN void ED_MATH_API multiply(Matrix<N,T>& r, const Matrix<N,T>& m1, const Matrix<N,T>& m2);


// multiply matrix by vector
template<int N, int M, typename T, typename F>
void multiply(Vector<N,F>& r, const AsMatrix<N,M,T>& m, const Vector<N,F>& v)
{
	for(int i=0; i<N; ++i)
	{
		r[i] = v[0] * m[0][i];
		for(int j=1; j<N; ++j)
			r[i] += v[j] * m[j][i];
	}
}

template<int N, int M, typename T, typename F>
void multiply1(Vector<N,F>& r, const AsMatrix<N+1,M,T>& m, const Vector<N,F>& v)
{
	for(int i=0; i<N; ++i)
	{
		r[i] = m[N][i];
		for(int j=0; j<N; ++j)
			r[i] += m[j][i] * v[j];
	}
}

// invert matrix
template<int N, typename T>
ED_MATH_EXTERN void ED_MATH_API invert(Matrix<N,T>& r, const Matrix<N,T>& m);

// invert orthogonal matrix
template<int N, typename T>
ED_MATH_EXTERN void ED_MATH_API invertOrtho(Matrix<N,T>& r, const Matrix<N,T>& m);

// transpose matrix
template<int N, typename T>
ED_MATH_EXTERN void ED_MATH_API transpose(Matrix<N,T>& r, const Matrix<N,T>& m);


/**
 * Matrix minor. Used for multiplication.
 */
template<int N, int M, typename T>
class AsMatrix {
public:
	typedef Matrix<M,T> matrix_t;
	typedef T scalar_t;
	typedef Vector<N,T> vector_t;

		AsMatrix(const matrix_t& _m) : m(_m) { Util::CTAssert<N <= M>(); }

	const matrix_t& matrix() const { return m; }
	const vector_t& operator [] (int i) const { return reinterpret_cast<const vector_t&>(m[i]); }

	template <typename F>
	Vector<N,F> operator * (const Vector<N,F>& v) const
	{
		Vector<N,F> res;
		multiply(res, *this, v);
		return res;
	}

private:
	const matrix_t& m;
};

/**
 * construct minor from matrix.
 */
template<int K, int N, typename T> inline
AsMatrix<K, N, T> asMatrix(const Matrix<N,T>& m) { return AsMatrix<K, N, T>(m); }


template<typename T>
void _copy(T* out, int K, int N, const T* in);

/**
 * Column of matrix.
 */
template <int N, typename T>
class ColumnConst
{
public:
	typedef ColumnConst<N,T> self_t;
	typedef Matrix<N,T> matrix_t;
	typedef T scalar_t;
	typedef Vector<N,T> vector_t;

	ColumnConst(const matrix_t& _m, int _c) : m(_m), c(_c) {}

	/**
	 * Array-style component access.
	 */
	const T& operator [] (int i) const { return m[i][c]; }

	/**
	 * Vector conversion.
	 */
	operator vector_t() const
	{
		vector_t res;
		for(int i=0; i<N; i++)
			res[i] = m[i][c];
		return res;
	}

private:
	int c;
	const matrix_t& m;
};

template <int N, typename T>
class Column
{
public:
	typedef Column<N,T> self_t;
	typedef Matrix<N,T> matrix_t;
	typedef T scalar_t;
	typedef Vector<N,T> vector_t;

	Column(matrix_t& _m, int _c) : m(_m), c(_c) {}

	/**
	 * Array-style component access.
	 */
	T& operator [] (int i) { return m[i][c]; }
	const T& operator [] (int i) const { return m[i][c]; }

	/**
	 * Vector conversion.
	 */
	operator vector_t() const
	{
		vector_t res;
		for(int i=0; i<N; i++)
			res[i] = m[i][c];
		return res;
	}

	/**
	 * Set components.
	 */
	self_t& set(const vector_t& v) {for(int i=0; i<N; i++) m[i][c] = v[i]; return *this;}
	self_t& set(T v0, T v1) { m[0][c] = v0; m[1][c] = v1; return *this; }
	self_t& set(T v0, T v1, T v2) { m[0][c] = v0; m[1][c] = v1; m[2][c] = v2; return *this; }
	self_t& set(T v0, T v1, T v2, T v3) { m[0][c] = v0; m[1][c] = v1; m[2][c] = v2; m[3][c] = v3; return *this; }

	/**
	 * Assignment.
	 */
	self_t& operator =(const vector_t& v)
	{return set(v);}

	self_t& operator +=(const vector_t& v)
	{for(int i=0; i<N; i++) m[i][c] += v[i]; return *this;}

	self_t& operator -=(const vector_t& v)
	{for(int i=0; i<N; i++) m[i][c] -= v[i]; return *this;}

private:
	int c;
	matrix_t& m;
};

/**
 * Square matrix of size NxN.
 */
template<int N, typename T>
class Matrix {
public:
	typedef Matrix<N,T> self_t;
	typedef T scalar_t;
	typedef Vector<N,T> vector_t;

	// static identity matrix
	static ED_MATH_EXTERN self_t id;

	vector_t m[N];

		Matrix() {}
	explicit Matrix(T scale) { set(scale); }
	template<int K>	Matrix(const AsMatrix<N,K,T>& a) { _copy(data(), N, K, a.matrix().data()); }

	// generic copy constructor
	template<class F>
	Matrix(const Matrix<N,F>& op) {for(int i=0; i<N; ++i) m[i] = op[i];}

	explicit Matrix(const T array[N*N]) { set(array); }

	ED_MATH_EXTERN self_t& set(T val);
	self_t& identity() { return set(1); }

	self_t& set(const T array[N*N]) { for(int i=0; i<N*N; ++i) data()[i] = array[i]; return *this; }
	
	// array operations
	T* data() { return m[0].data(); }
	const T* data() const { return m[0].data(); }

	vector_t& operator[](int i) { return m[i]; }
	const vector_t& operator[](int i) const { return m[i]; }

	vector_t& x() {return m[0];}
	vector_t& y() {return m[1];}
	vector_t& z() {return m[2];}
	Column<N,T> p() {return col(3);}

	const vector_t& x() const {return m[0];}
	const vector_t& y() const {return m[1];}
	const vector_t& z() const {return m[2];}
	ColumnConst<N,T> p() const {return col(3);}

	self_t operator * (const self_t& m) const
	{
		self_t res;
		multiply(res, *this, m);
		return res;
	}

	self_t& operator *= (const self_t& m)
	{
		self_t res;
		multiply(res, *this, m);
		return (*this = res);
	}
	
	self_t& invert()
	{
		self_t res;
		Math::invert(res, *this);
		return (*this = res);
	}

	self_t& invertOrtho()
	{
		self_t res;
		Math::invertOrtho(res, *this);
		return (*this = res);
	}

	ED_MATH_EXTERN self_t& transpose();

	self_t inverted() const
	{
		self_t res;
		Math::invert(res, *this);
		return res;
	}

	self_t invertedOrtho() const
	{
		self_t res;
		Math::invertOrtho(res, *this);
		return res;
	}

	self_t transposed() const
	{
		self_t res;
		Math::transpose(res, *this);
		return res;
	}

	vector_t& row(int i)
	{return m[i];}

	const vector_t& row(int i) const
	{return m[i];}

	Column<N,T> col(int i)
	{return Column<N,T>(*this, i);}

	ColumnConst<N,T> col(int i) const
	{return ColumnConst<N,T>(*this, i);}

	template <typename F>
	Vector<N,F> operator * (const Vector<N,F>& v) const
	{
		Vector<N,F> res;
		Math::multiply(res, AsMatrix<N,N,T>(*this), v);
		return res;
	}

	template <typename F>
	Vector<N-1,F> operator * (const Vector<N-1,F>& v) const
	{
		Vector<N-1,F> res;
		Math::multiply1<N-1,N,T,F>(res, AsMatrix<N,N,T>(*this), v);
		return res;
	}
};

#include "./_Matrix.inl.h"


// --- useful typedefs

typedef Matrix<3, float> Matrix3f;
typedef Matrix<3, double> Matrix3d;

typedef Matrix<4, float> Matrix4f;
typedef Matrix<4, double> Matrix4d;

typedef AsMatrix<3, 4, float> Matrix4As3f;

// useful functions
template <class T> 
Matrix<3, T> rotMatrixAxisAngle(const Vector<3, T>& a, T cos_fi, T sin_fi)
{
	Matrix<3, T> res;
	T inv_c = 1 - cos_fi;

	T xy = a.x * a.y * inv_c;
	T xz = a.x * a.z * inv_c;
	T yz = a.y * a.z * inv_c;
	T x = sin_fi * a.x;
	T y = sin_fi * a.y;
	T z = sin_fi * a.z;

	res.m[0][0] = cos_fi + inv_c*a.x*a.x;
	res.m[0][1] = xy + z;
	res.m[0][2] = xz - y;

	res.m[1][0] = xy - z;
	res.m[1][1] = cos_fi + inv_c*a.y*a.y;
	res.m[1][2] = yz + x;

	res.m[2][0] = xz + y;
	res.m[2][1] = yz - x;
	res.m[2][2] = cos_fi + inv_c*a.z*a.z;

	return res;
}

template <class T> 
Matrix<3, T> rotMatrixAxisAngle(const Vector<3, T>& axis, T ang)
{
	return rotMatrixAxisAngle(axis, cos(ang), sin(ang));
}

template <class T>
Matrix<3, T> rotMatrixFromTo(const Vector<3, T>& a, const Vector<3, T>& b)
{
	Vector<3, T> axis = cross(a, b);
	T sin_fi = length(axis);
    if (sin_fi < 1E-8)
        return Matrix<3, T>(1.0);
	T cos_fi = dot(a, b);
	axis /= sin_fi;

	return rotMatrixAxisAngle(axis, cos_fi, sin_fi);
}

}

template<int N, int M, typename T, typename F>
Math::Vector<N,F>& operator *= (Math::Vector<N,F>& v, const Math::AsMatrix<N,M,T>& m)
{return v = m * v;}

template <int N, typename T, typename F>
Math::Vector<N,F>& operator *= (Math::Vector<N,F>& v, const Math::Matrix<N,T>& m)
{return v = m * v;}

template <int N, typename T, typename F>
Math::Vector<N-1,F>& operator *= (Math::Vector<N-1,F>& v, const Math::Matrix<N,T>& m)
{return v = m * v;}


template<int N, int M, typename T, typename F>
Math::Vector<N,F> operator * (const Math::Vector<N,F>& v, const Math::AsMatrix<N,M,T>& m)
{return m * v;}

template <int N, typename T, typename F>
Math::Vector<N,F> operator * (const Math::Vector<N,F>& v, const Math::Matrix<N,T>& m)
{return m * v;}

template <int N, typename T, typename F>
Math::Vector<N-1,F> operator * (const Math::Vector<N-1,F>& v, const Math::Matrix<N,T>& m)
{return m * v;}


/**
 * Write/Load components to/from a stream.
 */
template<int N, class T, class Stream> inline
Stream& operator >> (Stream& out, Math::Matrix<N, T>& v)
{
	for( int i=0; i<N; ++i )
		out >> v[i];
	return out;
}

#endif /* _ED_Math_Matrix_h_ */
