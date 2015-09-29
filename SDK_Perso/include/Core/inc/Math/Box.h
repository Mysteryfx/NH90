#ifndef _ED_Math_Box_h_
#define _ED_Math_Box_h_

#include "./_config.h"
#include "./_math.h"
#include "./Vector.h"

#include <limits>

namespace Math {


/**
 * N-dimensional Box.
 */
template<int N, class T>
class Box {
public:
	typedef Box<N,T> self_t;
	typedef Vector<N,T> vector_t;
	typedef T scalar_t;

	vector_t min, max;

	enum { Size = N };

		/**
		 * Create uninitialized box.
		 */
	Box() : min(std::numeric_limits<T>::max()), max(-std::numeric_limits<T>::max()) {}

		/**
		 * Create box with size s.
		 */
		explicit Box(T s) : min(-s/T(2)), max(s/T(2)) {}

		/**
		 * Create box [A, B]
		 */
		Box(const vector_t& A, const vector_t& B) : min(A), max(B) {} 

		/**
		 * Create Box<2,T>.
		 */
		Box(T a, T b, T c, T d) : min(a,b), max(c,d) {}

		/**
		 * Create Box<3,T>.
		 */
		Box(T a, T b, T c, T d, T e, T f) : min(a,b,c), max(d,e,f) {}

		/**
		 * Generic copy constructor.
		 */
		template<class F>
			Box(const Box<N, F>& u) : min(u.min), max(u.max) {}

	/**
	 * Set to box of size val.
	 */
	self_t& set(T val) { min.set(-val/T(2)); max.set(val/T(2)); return *this; }

	/**
	* Set box to [A, B]
	*/
	self_t& set(const vector_t& A, const vector_t& B) {min = A; max = B; return *this;} 

	/**
	 * Set Box<2,T> components.
	 */
	self_t& set(T x0, T y0, T x1, T y1) { min.set(x0,y0); max.set(x1,y1); return *this; }

	/**
	 * Set Box<3,T> components.
	 */
	self_t& set(T x0, T y0, T z0, T x1, T y1, T z1) { min.set(x0,y0,z0); max.set(x1,y1,z1); return *this; }

	/**
	 * Get number of components.
	 */
	static int size() { return N; }

	/**
	 * Get length of [min, max]
	 */
	T length() const {return Math::length(max-min);}

	/**
	 * Get radius of outer sphere.
	 */
	T radius() const {return std::max( Math::length(max), Math::length(min) );}

	/**
	 *Get radius of outer sphere for non-zero box center coordinates
	*/
	T radius_ex() const {return Math::length(max - min) * 0.5;}

	/**
	 * Get center point.
	 */
	vector_t center() const {return (max + min)/2;}

	/**
	 * Get delta vector.
	 */
	vector_t delta() const {return max-min;}

	/**
	 * Get delta axes.
	 */
	T dx() const {return max.x-min.x;}
	T dy() const {return max.y-min.y;}
	T dz() const {return max.z-min.z;}

	/**
	 * Get corner by number.
	 */
	vector_t corner(int n) const
	{
		vector_t res;
		for(int i=0, p=1; i<N; i++, p*=2)
			res[i] = (n & p) ? max[i] : min[i];
		return res;
	}

	/**
	 * Check validity (box contains any space).
	 */
	bool isvalid() const
	{
		for(int i=0; i<N; i++)
			if(max[i] < min[i]) return false;
		return true;
	}

	/**
	 * Reset to uninitialized state.
	 */
	void reset() {min.set( std::numeric_limits<T>::max() ); max.set( -std::numeric_limits<T>::max() ); }

	/**
	 * Insert point or box.
	 */
	void insert(const vector_t& u)
	{
		for(int i=0; i<N; i++)
		{
			if(max[i] < u[i]) max[i] = u[i];
			if(min[i] > u[i]) min[i] = u[i];
		}
	}

	void insert(const self_t& u)
	{insert(u.min); insert(u.max);}

	/**
	 * Check is point or box inside this.
	 */
	bool isinside(const vector_t& u) const
	{
		for(int i=0; i<N; i++)
			if((u[i] < min[i]) || (u[i] > max[i])) return false;
		return true;
	}

	bool isinside(const self_t& u) const
	{return isinside(u.min) && isinside(u.max);}

	/**
	 * Get distance to point or box.
	 */
	T distance(const vector_t& u) const
	{
		T res = T(0);
		for(int i=0; i<N; i++)
		{
			if(u[i] < min[i]) {T d = min[i]-u[i]; res += d*d;}
			else if(u[i] > max[i]) {T d = u[i]-max[i]; res += d*d;}
		}
		return T(sqrt(res));
	}

	T distance(const self_t& u) const
	{
		T res = T(0);
		for(int i=0; i<N; i++)
		{
			if(u.max[i] < min[i]) {T d = min[i]-u.max[i]; res += d*d;}
			else if(u.min[i] > max[i]) {T d = u.min[i]-max[i]; res += d*d;}
		}
		return T(sqrt(res));
	}

	/**
	 * Intersect boxes.
	 */
	void intersect(const self_t& u)
	{
		for(int i=0; i<N; i++)
		{
			if(u.max[i] < max[i]) max[i] = u.max[i];
			if(u.min[i] > min[i]) min[i] = u.min[i];
		}
	}

	/**
	 * Intersect segment.
	 */
private:
	bool intersectSide(const vector_t& a, const vector_t& d, scalar_t t, int side) const
	{
		for(int i=0; i<N; i++)
		{
			if(i == side) continue;
			scalar_t p = a[i] + d[i]*t;
			if((p < min[i]) || (p > max[i])) return false;
		}
		return true;
	}

public:
	bool intersects(const vector_t& a, const vector_t& b) const
	{
		vector_t d = b-a;
		for(int i=0; i<N; i++)
		{
			if((d[i] <= T(0.00001)) && (d[i] >= -T(0.00001))) continue;
			scalar_t k = T(1) / d[i];

			scalar_t t = (min[i] - a[i])*k;
			if((t >= 0) && (t <= 1) && intersectSide(a, d, t, i))
				return true;

			t = (max[i] - a[i])*k;
			if((t >= 0) && (t <= 1) && intersectSide(a, d, t, i))
				return true;
		}
		return false;
	}

	// binary operators
	self_t operator | (const self_t& u) const
	{
		self_t res = *this;
		res.insert(u);
		return res;
	}

	self_t operator & (const self_t& u) const
	{
		self_t res = *this;
		res.intersect(u);
		return res;
	}

	// vector & scalar operators
	inline self_t& operator += (const vector_t& u)
	{min += u; max += u; return *this;}

	inline self_t& operator -= (const vector_t& u)
	{min -= u; max -= u; return *this;}

	inline self_t& operator *= (const scalar_t& u)
	{min *= u; max *= u; return *this;}

	inline self_t& operator /= (const scalar_t& u)
	{min /= u; max /= u; return *this;}

	inline self_t operator + (const vector_t& u) const
	{return Box(min + u, max + u);}

	inline self_t operator - (const vector_t& u) const
	{return Box(min - u, max - u);}

	inline self_t operator * (const scalar_t& u) const
	{return Box(min * u, max * u);}

	inline self_t operator / (const scalar_t& u) const
	{return Box(min / u, max / u);}

}; // class Box<N, T>

template <int N, class T>
Box<N,T> operator + (const Vector<N,T>& u, const Box<N,T>& b)
{return Box<N,T>(b.min + u, b.max + u);}

template <int N, class T>
Box<N,T> operator * (T u, const Box<N,T>& b)
{return Box<N,T>(b.min * u, b.max * u);}


// common typedefs
typedef Box<2, int> Box2i;
typedef Box<3, int> Box3i;

typedef Box<2, float> Box2f;
typedef Box<3, float> Box3f;

typedef Box<2, double> Box2d;
typedef Box<3, double> Box3d;

}

/**
 * Write/Load components to/from a stream.
 */
template<int N, class T, class Stream> inline
Stream& operator >> (Stream& out, Math::Box<N, T>& v)
{
	out >> v.min >> v.max;
	return out;
}

#endif /* _ED_Math_Box_h_ */
