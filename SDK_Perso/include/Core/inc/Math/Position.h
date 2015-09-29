#ifndef _ED_Math_Position_h_
#define _ED_Math_Position_h_

#include "./_config.h"
#include "./Vector.h"
#include "./Matrix.h"

namespace Math {


/**
 * N-dimensional Position.
 * Stores (rotation + scale) matrix plus translation vector
 * R - rotation components
 * T - translation components
 */
template<int N, class R, class T>
class Position {
public:
	typedef Position<N,R,T> self_t;
	typedef Vector<N,T> vector_t;
	typedef Vector<N,R> rvector_t;
	typedef Matrix<N,R> matrix_t;
	typedef R rotate_t;
	typedef T translate_t;

	// static identity position
	static ED_MATH_EXTERN self_t id;

	enum { Size = N };

	matrix_t r;
	vector_t p;

		/**
		 * Create uninitialized position.
		 */
		Position() {}

		/**
		 * Create position with all translation components set to s.
		 */
		explicit Position(T s) : p(s) { r.identity(); }

		/**
		 * Create position at (x,y).
		 */
		Position(T x, T y) : p(x,y) { r.identity(); }

		/**
		 * Create position at (x,y,z).
		 */
		Position(T x, T y, T z) : p(x,y,z) { r.identity(); }

		/**
		 * Create position at point v.
		 */
		explicit Position(const vector_t& v) : p(v) { r.identity(); }

		/**
		 * Create position with orientation m.
		 */
		explicit Position(const matrix_t& m) : r(m), p(0) {}

		/**
		 * Create position from matrix.
		 */
		template <class U>
		Position(const Matrix<N+1, U>& m)
		{
			for(int i=0; i<N; i++)
				r[i] = Vector<N,R>(m[i]);
			p = Vector<N,T>(m[N]);
		}

		/**
		 * Create position by rotation and translation.
		 */
		Position(const matrix_t& m, const vector_t& v) : r(m), p(v) {}

		/**
		 * Generic copy constructor.
		 */
		template<class RU, class TU>
		Position(const Position<N, RU, TU>& u) : r(u.r), p(u.p) {}

	/**
	 * Get number of components.
	 */
	static int size() { return N; }

	// Component access
	rvector_t& x() {return r[0];}
	rvector_t& y() {return r[1];}
	rvector_t& z() {return r[2];}

	const rvector_t& x() const {return r[0];}
	const rvector_t& y() const {return r[1];}
	const rvector_t& z() const {return r[2];}

	/**
	 * Conversion.
	 */
	template <class U>
	operator Matrix<N+1, U>() const
	{
		Matrix<N+1, U> res;
		for(int i=0; i<N; i++)
			res[i] = Vector<N+1, U>(r[i], 0);
		res[N] = Vector<N+1, U>(p, 1);
		return res;
	};

	/**
	 * Translation.
	 */
	self_t& move(const vector_t& d) {p += d; return *this;}

	/**
	 * Rotation.
	 */
	self_t& rot(const rvector_t& axis, rotate_t angle)
	{
		Util::CTAssert<N==3>();

		R s = R( sin(angle) );
		R c = R( cos(angle) );

		matrix_t op;
		op[0] = axis * (axis.x*(R(1)-c)) + vector_t(c, axis.z*s, -axis.y*s);
		op[1] = axis * (axis.y*(R(1)-c)) + vector_t(-axis.z*s, c, axis.x*s);
		op[2] = axis * (axis.z*(R(1)-c)) + vector_t(axis.y*s, -axis.x*s, c);

		return (*this *= op);
	}

	self_t& rot(rotate_t angle)
	{
		Util::CTAssert<N==2>();

		R s = R( sin(angle) );
		R c = R( cos(angle) );

		matrix_t op;
		op[0].set(c, -s);
		op[1].set(s, c);

		return (*this *= op);
	}

	/**
	 * Scale.
	 */
	self_t& scale(const vector_t& s) {for(int i=0; i<N; ++i) r[i] *= s[i]; p *= s; return *this;}
	self_t& scale(const rotate_t& s) {for(int i=0; i<N; ++i) r[i] *= s; p *= s; return *this;}

	/**
	 * Invert.
	 */
	self_t& invert()
	{
		r.transpose();
		for(int i=0; i<N; i++)
			r[i] /= length2(r[i]);
		p = -(r * p);
		return *this;
	}

	self_t inverted() const
	{
		self_t res = *this;
		res.invert();
		return res;
	}

	// assignment operators
	self_t& operator *= (const matrix_t& op) {r *= op; p *= op; return *this;}
	self_t& operator *= (const self_t& op) {r *= op.r; p = op.r*p + op.p; return *this;}

	// binary operators
	self_t operator *(const matrix_t& op) const {return self_t(r * op, op * p);}
	self_t operator *(const self_t& op) const {return self_t(r*op.r, op.r*p + op.p);}

	// vector operators
	vector_t operator *(const vector_t& op) const {return r*op + p;}

}; // class Position<N, R, T>


// common typedefs
typedef Position<2, float, int> Pos2i;
typedef Position<3, float, int> Pos3i;

typedef Position<2, float, float> Pos2f;
typedef Position<3, float, float> Pos3f;

typedef Position<2, float, double> Pos2d;
typedef Position<3, float, double> Pos3d;

typedef Position<2, double, double> Pos2dd;
typedef Position<3, double, double> Pos3dd;
}

// non-member operators
template <int N, class R, class T>
Math::Position<N,R,T> operator *(const Math::Matrix<N,R>& op, const Math::Position<N,R,T>& pos)
{return pos * op;}

template <int N, class R, class T>
Math::Vector<N,T> operator *(const Math::Vector<N,T>& op, const Math::Position<N,R,T>& pos)
{return pos * op;}

/**
 * Write/Load components to/from a stream.
 */
template<int N, class R, class T, class Stream> inline
Stream& operator >> (Stream& out, Math::Position<N, R, T>& p)
{
	out >> p.r >> p.t;
	return out;
}

#endif /* _ED_Math_Position_h_ */
