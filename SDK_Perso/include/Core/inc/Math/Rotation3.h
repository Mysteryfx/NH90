/**
 * Based on
 * EasyGL library v0.1
 * (C)opyright 2002 by Dmitry S. Baikov
 *
 * @file Rotation3.h
 * @author Dmitry S. Baikov
 */
#ifndef _ED_Math_Rotation3_h_
#define _ED_Math_Rotation3_h_


#include "./_config.h"
#include "./Vector.h"
#include "./Matrix.h"

namespace Math {

template<typename T> struct Rotation3;

/**
 * Do spherical linear interpolation of rotations.
 */
template<typename T> ED_MATH_EXTERN
Rotation3<T> ED_MATH_API
slerp(const Rotation3<T>& a, const Rotation3<T>& b, const T t);

/**
 * Order of 3d rotations (self-explanatory).
 */
enum RotationOrder {
	XYZ=0, 
	YZX, 
	ZXY, 
	XZY, 
	YXZ, 
	ZYX
};


/**
 * 3-dimensional rotation (unit quaternion).
 */
template<typename T>
struct Rotation3 {

	Vector<3,T> v;
	T w;

	typedef Rotation3<T> self_t;
	typedef T scalar_t;
	typedef Vector<3,T> vector_t;
	
		/**
		 * Create uninitialized rotation.
		 */
		Rotation3() {}

		/**
		 * Create unit rotation.
		 */
		explicit Rotation3(T w_) : v(0), w(w_) {}

		/**
		 * Create rotation from quaternion components.
		 */
		Rotation3(T x_, T y_, T z_, T w_)
			: v(x_, y_, z_), w(w_) {}

		/**
		 * Generic copy contructor.
		 */
		template<class F>
		Rotation3(const Rotation3<F>& r)
			: v(r.im()), w(r.re()) {}

		/**
		 * Create rotation around 'axis' by 'angle'.
		 * @param angle rotation angle in radians
		 * @param axis rotation axis
		 */
		Rotation3(scalar_t angle, const vector_t& axis)
			{ setAngleAxis(angle, axis); }

		/**
		 * Create rotation from one vector to another.
		 */
		Rotation3(const vector_t& from, const vector_t& to)
			{ setFromTo(from, to); }

		/**
		 * Create rotation from Euler anlges.
		 */
		Rotation3(RotationOrder order, const vector_t& angles)
			{ setEulerAngles(order, angles); }

		/**
		 * Create rotation from matrix.
		 */
		explicit Rotation3(const Matrix<3, T>& m);


	/**
	 * Get real quaternion part.
	 */
	scalar_t re() const { return w; }
	scalar_t& re() { return w; }

	/**
	 * Get imaginary quaternion part.
	 */
	const vector_t& im() const { return v; }
	vector_t& im() { return v; }
		
	/**
	 * Get basis vectors (rotation matrix).
	 */
	ED_MATH_EXTERN void ED_MATH_API getBasis(T* axis0, T* axis1, T* axis2) const;
	
	/**
	 * Set rotation by angle-axis pair.
	 */
	ED_MATH_EXTERN self_t& ED_MATH_API setAngleAxis(scalar_t angle, const vector_t& axis);

	/**
	 * Get rotation by angle-axis pair.
	 */
	ED_MATH_EXTERN void ED_MATH_API getAngleAxis(scalar_t& angle, vector_t& axis) const;

	/**
	 * Set rotation as euler angles.
	 */
	ED_MATH_EXTERN self_t& ED_MATH_API setEulerAngles(RotationOrder order, const vector_t& angles);

	/**
	 * Set rotation as yaw-pitch-roll.
	 */
	ED_MATH_EXTERN self_t& ED_MATH_API setYawPitchRoll(scalar_t yaw, scalar_t pitch, scalar_t roll);

	/**
	 * Get rotation as yaw-pitch-roll.
	 */
	ED_MATH_EXTERN void ED_MATH_API getYawPitchRoll(scalar_t& yaw, scalar_t& pitch, scalar_t& roll) const;
	
	/**
	 * Set rotation as transition from one vector to another.
	 */
	ED_MATH_EXTERN self_t& ED_MATH_API setFromTo(const vector_t& from, const vector_t& to);
	
	/**
	 * Concatenate rotations.
	 */
	/*ED_MATH_EXTERN*/ self_t /*ED_MATH_API*/ operator * (const self_t&) const;

	/**
	 * Concatenate rotations.
	 */
	self_t& operator *= (const self_t&);

	/**
	 * Add quaternions
	 */
	/*ED_MATH_EXTERN*/ self_t /*ED_MATH_API*/ operator + (const self_t&) const;
	self_t& operator += (const self_t&);

	/**
	 * Multiply by value
	 */
	self_t operator * (T v) const;
	
	
	/**
	 * Reset rotation.
	 */
	self_t& identity();

	/**
	 * Normalize quaternion (in-place).
	 */
	ED_MATH_EXTERN self_t& ED_MATH_API normalize();

	/**
	 * Get normalized copy of quaternion.
	 */
	ED_MATH_EXTERN self_t ED_MATH_API normalized() const;

	/**
	 * Multiply by vector
	 */
	Vector<3, T> operator * (const Vector<3, T>& vec) const
	{
		// from nVidia SDK implementation
		// По идее быстрее чем преобразовывать в матрицу, а потом умножать
		Vector<3, T> uv, uuv;
		uv = cross(v, vec);
		uuv = cross(v, uv);
		uv *= (w+w);
		return vec + uv + uuv + uuv;
	}


	/**
	 * Matrix operator.
	 */
	operator Matrix<3,T>() const
	{
		Matrix<3,T> m;
		getBasis(m[0].data(), m[1].data(), m[2].data());
		return m;
	}

	ED_MATH_EXTERN void ED_MATH_API fromMatrix(const Matrix<3, T>& m);

	friend ED_MATH_EXTERN Rotation3<T> ED_MATH_API
	slerp<>(const Rotation3<T>& a, const Rotation3<T>& b, const T t);
	
}; // class Rotation3<T>


// --- implementation
template <class T> inline
Rotation3<T>::Rotation3(const Matrix<3, T>& m)
{
	fromMatrix(m);
}

template<class T> inline
Rotation3<T> invert(const Rotation3<T>& r)
{
	return Rotation3<T>(-r.v.x, -r.v.y, -r.v.z, r.w);
}

template<class T> inline
Rotation3<T>& Rotation3<T>::identity()
{
	w = 1;
	v[2] = v[1] = v[0] = 0;
	return *this;
}

template<class T> inline
Rotation3<T>& Rotation3<T>::operator *= (const Rotation3<T>& op)
{
	Rotation3<T> res = *this * op;
	return *this = res;
}

template <class T> inline
Rotation3<T>& Rotation3<T>::operator += (const Rotation3<T>& op)
{
	Rotation3<T> res = *this + op;
	return *this = res;
}

template<class T> inline
Rotation3<T> Rotation3<T>::operator * (const Rotation3<T>& q2) const
{
	T A, B, C, D, E, F, G, H;

	A = (w + v.x) * (q2.w + q2.v.x);
	B = (v.z - v.y) * (q2.v.y - q2.v.z);
	C = (v.x - w) * (q2.v.y + q2.v.z);
	D = (v.y + v.z) * (q2.v.x - q2.w);
	E = (v.x + v.z) * (q2.v.x + q2.v.y);
	F = (v.x - v.z) * (q2.v.x - q2.v.y);
	G = (w + v.y) * (q2.w - q2.v.z);
	H = (w - v.y) * (q2.w + q2.v.z);

	Rotation3<T> res;

	res.w = B + (-E - F + G + H) * (T)0.5;
	res.v.x = A - ( E + F + G + H) * (T)0.5; 
	res.v.y =-C + ( E - F + G - H) * (T)0.5;
	res.v.z =-D + ( E - F - G + H) * (T)0.5;

	return res;
}

template<class T> inline
Rotation3<T> Rotation3<T>::operator + (const Rotation3<T>& op) const
{
	Rotation3<T> res;
	res.w = w + op.w;
	res.v.x = v.x + op.v.x;
	res.v.y = v.y + op.v.y;
	res.v.z = v.z + op.v.z;
	return res;
}

template<class T>
inline
Rotation3<T> Rotation3<T>::operator * (T op) const
{
	Rotation3<T> res;

	res.w = w*op;
	res.v = v*op;

	return res;
}

#ifdef ED_MATH_IMPLEMENT_ROTATION3

template<class T> ED_MATH_EXTERN
Rotation3<T> ED_MATH_API Rotation3<T>::normalized() const
{
	T scale = sqrt(re()*re() + length2(im()));
	if( scale > 0 ) {
		return Rotation3<T>(w/scale, v/scale);
	} else {
		return Rotation3<T>(1);
	}
}

template<class T> ED_MATH_EXTERN
Rotation3<T>& ED_MATH_API Rotation3<T>::normalize()
{
	T scale = sqrt(w*w + v.x*v.x + v.y*v.y + v.z*v.z);
	if( scale > 0 ) {
		w/=scale;
		v/=scale;
	} else {
		w = 1;
		v.set(0);
	}

	return *this;
}

template<class T> ED_MATH_EXTERN
void ED_MATH_API Rotation3<T>::getBasis(T* axis0, T* axis1, T* axis2) const
{
	T wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	x2 = v.x + v.x;
	y2 = v.y + v.y;
	z2 = v.z + v.z;
	xx = v.x * x2;   xy = v.x * y2;   xz = v.x * z2;
	yy = v.y * y2;   yz = v.y * z2;   zz = v.z * z2;
	wx = w * x2;   wy = w * y2;   wz = w * z2;

	if( axis0 ) {
		axis0[0] = (T)1 - (yy + zz);
		axis0[1] = xy+wz;
		axis0[2] = xz-wy;
	}
	
	if( axis1 ) {
		axis1[0] = xy-wz;
		axis1[1] = (T)1 - (xx + zz);
		axis1[2] = yz + wx;
	}
	
	if( axis2 ) {
		axis2[0] = xz+wy;
		axis2[1] = yz-wx;
		axis2[2] = (T)1 - (xx+yy);
	}
}

template<class T> ED_MATH_EXTERN
Rotation3<T>& ED_MATH_API Rotation3<T>::setAngleAxis(T angle, const Vector<3,T>& axis)
{
	w = cos(angle/2);
	v = Math::normalize(axis) * sin(angle/2);
	return *this;
}

template<class T> ED_MATH_EXTERN
void ED_MATH_API Rotation3<T>::getAngleAxis(T& angle, Vector<3,T>& axis) const
{
	angle = 2*acos( w );
	axis = Math::normalize( v );
}
	
template<class T> ED_MATH_EXTERN
Rotation3<T>& ED_MATH_API Rotation3<T>::setEulerAngles(RotationOrder order, const Vector<3,T>& angle)
{
	static int axes[6][3] = {
		{0,1,2}, //XYZ
		{1,2,0}, //YZX
		{2,0,1}, //ZXY
		{0,2,1}, //XZY
		{1,0,2}, //YXZ
		{2,1,0}  //ZYX
	};
	identity();
	for(int i=0; i<3; ++i)
	{
		Rotation3<T> rot(0,0,0,cos(angle[i]/2));
		rot.v[ axes[order][i] ] = sin(angle[i]/2);
		*this *= rot;
	}
	return *this;
}

template<class T> ED_MATH_EXTERN
void ED_MATH_API Rotation3<T>::getYawPitchRoll(T &yaw, T &pitch, T &roll) const
{
#if 0
	T x = v[0]*v[0] - v[1]*v[1] - v[2]*v[2] + w*w;
	yaw = atan(2.f * (v[0]*v[2] + v[1]*w) / x );
	if(x < 0) yaw += (T)M_PI;
	//yaw = M_PI/2 - yaw;
	if(yaw < 0) yaw += 2*(T)M_PI;

	x = -v[0]*v[0] + v[1]*v[1] - v[2]*v[2] + w*w;
	roll = atan(2.f * (v[1]*v[2] + v[0]*w) / x );
	if(x < 0) roll += (T)M_PI;
	if(roll > (T)M_PI) roll -= 2*(T)M_PI;

	pitch = /*-*/asin(-2.f * (v[0]*v[1] - v[2]*w) );
	roll = /*-*/roll; //!!!
#else

	T sin_pitch = 2*(v[0]*v[1] + v[2]*w); 
	if (sin_pitch > 1.0) sin_pitch = 1.0;
	else if (sin_pitch < -1.0) sin_pitch = -1.0;

	T cos_pitch = sqrt(1 - sin_pitch*sin_pitch);

	pitch = asin(sin_pitch);
	if (cos_pitch > 1E-6)
	{
		yaw   = -atan2(2*(v[2]*v[0] - v[1]*w), 1 - 2*(v[1]*v[1] + v[2]*v[2]));
		if (yaw < -1E-6) yaw += 2*(T)M_PI;
		double vv = (1 - 2*(v[2]*v[2] + v[0]*v[0]))/cos_pitch;
		if (vv > 1.0) vv = 1.0;
		else if (vv < -1.0) vv = -1.0;
		roll  = (T)acos(vv);
		if (2 * (v[1]*v[2] - v[0]*w) > 1E-6)
			roll = -roll;
	}
	else
		yaw = roll = 0;
#endif
}

template<class T> ED_MATH_EXTERN
Rotation3<T>& ED_MATH_API Rotation3<T>::setYawPitchRoll(T yaw, T pitch, T roll)
{
	/*yaw = M_PI/2 - yaw; 
	pitch = -pitch;*/
/*	pitch = -pitch;
	roll = -roll;*/

	Rotation3<T> qyaw  (0, sin(yaw/2), 0, cos(yaw/2));
	Rotation3<T> qpitch(0, 0, sin(pitch/2), cos(pitch/2));
	Rotation3<T> qroll (sin(roll/2), 0, 0, cos(roll/2));
	return *this = /*qroll * qpitch * qyaw*/qyaw*qpitch*qroll;
}

template<class T> ED_MATH_EXTERN
Rotation3<T>& ED_MATH_API Rotation3<T>::setFromTo(const Vector<3,T>& from, const Vector<3,T>& to)
{
	vector_t axis = cross(from, to);
	
	T cos2Alfa = dot(from, to);

	if (cos2Alfa > 1)
		cos2Alfa = 1;
	else if (cos2Alfa < -1)
		cos2Alfa = -1;
	T sin2Alfa = sqrt(1 - cos2Alfa * cos2Alfa);
	
	T sinAlfa = sqrt( (1-cos2Alfa) / 2 );
	T cosAlfa = sqrt( (1+cos2Alfa) / 2 );
	
	if (sin2Alfa != 0 )
		axis *= sinAlfa / sin2Alfa;
	
	*this = Rotation3<T>(/*-*/axis[0], /*-*/axis[1], /*-*/axis[2], cosAlfa);
	return *this;
}


template <class T> ED_MATH_EXTERN
void ED_MATH_API Rotation3<T>::fromMatrix(const Matrix<3, T>& m)
{
	scalar_t tr = m[0][0] + m[1][1] + m[2][2];

	if(tr >= 0)
	{
		scalar_t s = (scalar_t)sqrt(tr + 1);
		w = scalar_t(0.5) * s;
		s = scalar_t(0.5) / s;
		v[0] = (m[1][2] - m[2][1]) * s;
		v[1] = (m[2][0] - m[0][2]) * s;
		v[2] = (m[0][1] - m[1][0]) * s;
	}
	else
	{
		int i=0;
		if(m[1][1] > m[0][0]) i=1;
		if((m[2][2] > m[1][1]) && (m[2][2] > m[0][0])) i=2;

		int i1 = (i+1)%3;
		int i2 = (i+2)%3;

		scalar_t s = (scalar_t)sqrt((m[i][i] - (m[i1][i1] + m[i2][i2])) + 1);
		v[i] = scalar_t(0.5) * s;
		s = scalar_t(0.5) / s;
		v[i1] = (m[i1][i] + m[i][i1]) * s;
		v[i2] = (m[i][i2] + m[i2][i]) * s;
		w = (m[i1][i2] - m[i2][i1]) * s;
	}
}


template<class T> ED_MATH_EXTERN
Rotation3<T> ED_MATH_API slerp(const Rotation3<T>& from_, const Rotation3<T>& to, const T t)
{
	// compute dot product, aka cos(theta):
	T cosom = from_.w * to.w + dot(from_.v, to.v);

	Rotation3<T> from;
	if(cosom < 0) {
		// flip start quaternion
		cosom = -cosom;
		from.w = -from_.w;
		from.v = -from_.v;
	} else {
		from.w = from_.w;
		from.v = from_.v;
	}
	// calculate coefficients
	T scale0, scale1;
	if ( (1 - cosom) > T(0.05f) ) {
		// standard case (slerp)
		T omega = acos(cosom);
		T sinom = sin(omega);
		scale0 = sin((1 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	} else {       
		// close case (lerp)
		scale0 = 1 - t;
		scale1 = t;
	}
	// calculate final values
	return Rotation3<T>(
		/*x*/scale0 * from.v[0] + scale1 * to.v[0],
		/*y*/scale0 * from.v[1] + scale1 * to.v[1],
		/*z*/scale0 * from.v[2] + scale1 * to.v[2],
		/*w*/scale0 * from.w + scale1 * to.w
		);
}
#endif /* ED_MATH_IMPLEMENT_ROTATION3 */


typedef Rotation3<float> Rot3f;
typedef Rotation3<double> Rot3d;

} // namespace Math

/**
 * Write/Load components to/from a stream.
 */
template<class T, class Stream> inline
Stream& operator >> (Stream& out, Math::Rotation3<T>& v)
{
	out >> v.im() >> v.re();
	return out;
}

#endif /* _ED_Math_Rotation3_h_ */
