#pragma once

#include <memory.h>
#include <math.h>

#include "GraphMath/vector.h"

#ifndef DISSERIALIZER
#include "Serializer\Serializer.h"
#endif
#include "MathExport.h"

//////////////////////////////////////////////////////////////////////////////////////////

static const double PI2 = M_PI * 2.0;


namespace	E2 
{
	// Vector
	class MATH_API Vector 
	{
		friend class	Operator;
	public:
		float	x;
		float	y;
 		Vector(void) {x = y = 0.0f;}
 		Vector(float c) {x=y=c;}
 		Vector(float cx,float cy) {
			x=cx; y=cy;
		}

		inline void set(float cx, float cy) {x=cx; y=cy;}
	// Copy ctr
		Vector(const Vector& v) {
			x=v.x; y=v.y;
		}

	// Copy 
 		Vector&  operator =	(const Vector& v)	{
			x=v.x; y=v.y;
			return *this;
		}
	//	Coordinate return inlines
 		inline float	getX() const { return x;}
 		inline float	getY() const { return y;}

	//	In-places
 		Vector&	operator +=	(const Vector& v){
			x+=v.x; y+=v.y;
			return *this;
		}
 		Vector&	operator -=	(const Vector& v){
			x-=v.x; y-=v.y;
			return *this;
		}
 		Vector&	operator *=	(float r) {
			x*=r;   y*=r;
			return *this;
		}
 		Vector&	operator /=	(float r) {
			x/=r;   y/=r;
			return *this;
		}
		
		// Relations
 		bool	operator ==	(const Vector& v) const	{ return (*this-v).norma() <= 0.0f; }
 		bool	operator !=	(const Vector& v) const	{ return (*this-v).norma() > 0.0f; }
 		bool	operator >	(const Vector& v) const	{ return x>v.x	&& y>v.y; }
 		bool	operator <	(const Vector& v) const	{ return x<v.x	&& y<v.y; }
 		bool	operator >=	(const Vector& v) const	{ return x>=v.x && y>=v.y; }
 		bool	operator <=	(const Vector& v) const	{ return x<=v.x && y<=v.y; }
		//

		// Unaries
 		Vector	operator -() const { return Vector(-x,-y); }

		// Binaries
		//...float
 		Vector	operator *	(float r) const	{ return Vector( (r*x),   (r*y)); }
MATH_API 		friend Vector operator * (float r, const Vector& v) { return Vector(r*v.x, r*v.y); }
 		Vector	operator /	(float r) const { return Vector( (x/r),   (y/r) ); }
		//...vector
 		Vector	operator +	(const Vector& v) const { return Vector( (x+v.x), (y+v.y) ); }
 		Vector	operator -	(const Vector& v) const { return Vector( (x-v.x), (y-v.y) ); }
 		Vector	Scale(const Vector& v) const { return Vector( (x*v.x), (y*v.y)); }

		// Dot product
 		float	operator *	(const Vector& v) const { return x*v.x+y*v.y; }

		// Cross product
 		float operator ^ (const Vector& v) const { return (x*v.y-y*v.x); }

		// Some linears
		float	length2() const { return (float)(x*x+y*y); }
 		float	length() const { return (float)sqrt(x*x+y*y); }
		float	norma() const { return (float)sqrt(x*x+y*y); }
 		float	normaAbs() const { return (float)(fabs(x)+fabs(y));}
 		Vector	ort() const {
				return Vector(*this/norma());
		}
		// Polar angle
 		double polar(void) const
		{
			double theta = atan2(y, x);
			if (theta < 0.0) 
				theta += M_PI * 2.0;
			return theta;
		}
		// Counter clockwise rotation
 		Vector rot(double ang)
		{
			float sinang = (float)sin(ang);
			float cosang = (float)cos(ang);
			return Vector(x * cosang - y * sinang, x * sinang + y * cosang);
		}
		// Counter clockwise PI/2 rotation
 		Vector rot(void)
		{
			return Vector(-y, x);
		}
		// Decompose this vector by vX, vY vectors (*this = vC.x * vX + vC.y * vY),
		// if det(vX, vY) != 0.0. Returns det.
 		float decompose(const Vector &vX, const Vector &vY, Vector &vC)
		{
			float det = vX.x * vY.y - vX.y * vY.x;
			if (det != 0.0f){
				vC.x = (x * vY.y - y * vY.x) / det;
				vC.y = (y * vX.x - x * vX.y) / det;
			}
			return det;
		}
		// Crossings
 		Vector	crossX(const Vector & towards) const {	// where do the line from (*this) to (towards) cross the X axis ?
			float t=y/(y-towards.y);
			return Vector(t*(towards.x-x)+x, 0.f);
		}
 		Vector	crossY(const Vector & towards) const {	// where do the line from (*this) to (towards) cross the Y axis ?
			float t=x/(x-towards.x);
			return Vector(0.f, t*(towards.y-y)+y);
		}
	};

	// Edge for right lines and segments operations
	enum {LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION};
	enum {COLLINEAR, PARALLEL, SKEW, SKEW_CROSS, SKEW_NO_CROSS};
	class MATH_API Edge
	{
	public:
		Vector org;
		Vector dest;
 		Edge(void) {}
 		Edge(const Vector &_org, const Vector &_dest) : org(_org), dest(_dest) {}
 		Edge(const Edge &e) : org(e.org), dest(e.dest) {}

 		Edge & operator = (const Edge &e) { org = e.org; dest = e.dest; return *this; }

 		bool operator == (const Edge &e) const { return (org == e.org) && (dest == e.dest); }
 		bool operator != (const Edge &e) const { return (org != e.org) || (dest != e.dest); }
	
 		Vector vector(void) const { return dest - org; }
 		Vector point(double t) const { return org + (dest - org) * (float)t; }

 		Edge flip(void) { return Edge(dest, org); }
 		Edge invert(void) { return Edge(org, org - vector()); }

 		Edge rot(double ang) const 	// CCW (rad)
		{ 
			Vector v = vector();
			float sinang = (float)sin(ang);
			float cosang = (float)cos(ang);
			return Edge(org, org +
				Vector(v.x * cosang - v.y * sinang,
				v.x * sinang + v.y * cosang)); 
		}
 		Edge rotNorm(void) const		// CCW (PI / 4)
		{
			Vector v = vector();
			return Edge(org, org + Vector(-v.y, v.x));
		}

 		int classify(const Vector &v) const
		{
			Vector a = vector();
			Vector b = v - org;
			double sa = a.x * b.y - b.x * a.y;
			if (sa > 0.0)
				return LEFT;
			if (sa < 0.0)
				return RIGHT;
			if ((a.x * b.x < 0.0) || (a.y * b.y < 0.0))
				return BEHIND;
			if (a * a < b * b)
				return BEYOND;
			if (org == v)
				return ORIGIN;
			if (dest == v)
				return DESTINATION;
			return BETWEEN;
		}
		// Intersection point of two right lines(if any):
		// Edge e1(...), e2(...);
		// double t;
		// Vector p;
		// int iClass = e1.intersect(e2, t);
		// if (iClass == SKEW)
		//		p = e1.point(t);
 		int intersect(const Edge &e, double &t) const
		{
			Vector v = e.vector();
			Vector n = Vector(-v.y, v.x);
			double denom = n * vector();
			if (denom == 0.0)
			{
				int aclass = e.classify(org);
				if ((aclass == LEFT) || (aclass == RIGHT))
					return PARALLEL;
				return COLLINEAR;
			}
			t = - (n * (org - e.org)) / denom;
			return SKEW;
		}
		// Cross of two edges (if any):
		// Edge e1(...), e2(...);
		// double t1, t2;
		// Vector p;
		// int iClass = e1.cross(e2, t1, t2);
		// if (iClass == SKEW_CROSS)
		//		p = e1.point(t1);
		// or
		//		p = e2.point(t2);
 		int cross(const Edge &e, double &t, double &te) const
		{
			t = 1e20;
			int crossType = e.intersect(*this, te);
			if ((crossType == COLLINEAR) || (crossType == PARALLEL))
				return crossType;
			if ((te < 0.0) || (te > 1.0))
				return SKEW_NO_CROSS;
			crossType = intersect(e, t);
			if ((0.0 <= t) && (t <= 1.0))
				return SKEW_CROSS;
			else
				return SKEW_NO_CROSS;
		}
	};
	
	// Operator
	class MATH_API Operator {
		float	a11;
		float	a21;

		float	a12;
		float	a22;
	public:
 		Operator(void) { }
 		Operator(float c) {
			a11=a22=c;
			a12=a21=0.;
		}
 		Operator(
			float	_a11,float	_a12,
			float	_a21,float	_a22) {
			a11=_a11;	a12=_a12;
			a21=_a21;	a22=_a22;
		}
	// Copy ctr
 		Operator(const Operator &A) {
			a11=A.a11;	a12=A.a12;
			a21=A.a21;	a22=A.a22;
		}
	// Copy 
 		Operator&  operator =	(const Operator& A)	{
			a11=A.a11;	a12=A.a12;
			a21=A.a21;	a22=A.a22;
			return *this;
		}
	//
 		Operator&	operator +=	(const Operator& A){
			a11+=A.a11; a12+=A.a12;
			a21+=A.a21; a22+=A.a22;
			return *this;
		}
 		Operator&	operator -=	(const Operator& A){
			a11-=A.a11; a12-=A.a12;
			a21-=A.a21; a22-=A.a22;
			return *this;
		}
 		Operator&	operator *=	(float r) {
			a11*=r;
			a21*=r;

			a12*=r;
			a22*=r;
			return *this;
		}
 		Operator&	operator /=	(float r) {
			a11/=r;
			a21/=r;

			a12/=r;
			a22/=r;
			return *this;
		}
		
		// Unaries
 		Operator	operator -() {
			return Operator(
			-a11,-a12,
			-a21,-a22
			);
		}

		// Binaries
		//...float
 		Operator	operator *	(float r)		{
			return Operator(
			r*a11,r*a12,
			r*a21,r*a22);
		}
 		Operator	operator /	(float r)		{
			return Operator(
			a11/r,a12/r,
			a21/r,a22/r);
		}
 		Operator	operator +	(const Operator& A)	{
			return Operator(
				a11+A.a11, a12+A.a12,
				a21+A.a21, a22+A.a22
				);
		}
 		Operator	operator -	(const Operator& A)	{
			return Operator(
				a11-A.a11, a12-A.a12,
				a21-A.a21, a22-A.a22
				);
		}

		// Transform vector
 		Vector	operator *	(const Vector& v)	{
			return Vector(
					a11*v.x + a12*v.y,
					a21*v.x + a22*v.y
				);
		}

		// Superposition
 		Operator operator * (const Operator &A) {
			return Operator(
					a11*A.a11 + a12*A.a21, // a11
					a11*A.a12 + a12*A.a22, // a12

					a21*A.a11 + a22*A.a21, // a21
					a21*A.a12 + a22*A.a22 // a22
				);
		}

 		Operator	T(){	// transpose
			return Operator(
			a11,a21,
			a12,a22
			);
		}

 		float	Det(){
			return a11*a22-a12*a21;
		}

 		Operator I(){
			float	d1=1.f/Det();
			return Operator(
				a22*d1, -a12*d1,
				-a21*d1, a11*d1 
				);
		}
	};
} // namespace E2


namespace E3 
{
	// Vector
	class MATH_API Vector 
	{
		friend class	Operator;
//	protected:
	public:
		float	x;
		float	y;
		float	z;

 		Vector(void) {x = y = z = 0.0f;}
 		Vector(float c) {x=y=z=c;}
 		Vector(float cx,float cy,float cz) {x=cx; y=cy; z=cz;}
 		Vector(const float* cvec) {x=cvec[0]; y=cvec[1]; z=cvec[2];}

		inline void set(float cx,float cy,float cz) {x=cx; y=cy; z=cz;}

 		operator float*(){return &x;}
 		operator const float*()const{return &x;}
	// Copy ctr
 		Vector(const Vector &v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}
	//  Overload ctr
 		Vector(const Vector3 &v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

	// Copy 
 		Vector&  operator =	(const Vector& v)	{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}
	//	Coordinate return inlines
 		inline float	getX() const { return x;}
 		inline float	getY() const { return y;}
 		inline float	getZ() const { return z;}

	//  Projections
 		E2::Vector VectorXY(void) const { return E2::Vector(x, y); }
 		E2::Vector VectorXZ(void) const { return E2::Vector(x, z); }
 		E2::Vector VectorYZ(void) const { return E2::Vector(y, z); }
 		E2::Vector VectorYX(void) const { return E2::Vector(y, x); }
 		E2::Vector VectorZX(void) const { return E2::Vector(z, x); }
 		E2::Vector VectorZY(void) const { return E2::Vector(z, y); }

	//	In-places
 		Vector&	operator +=	(const Vector& v){
			x+=v.x; y+=v.y; z+=v.z;
			return *this;
		}
 		Vector&	operator -=	(const Vector& v){
			x-=v.x; y-=v.y; z-=v.z;
			return *this;
		}

 		Vector&	operator *=	(float r) {
			x*=r;   y*=r;   z*=r;
			return *this;
		}
 		Vector&	operator /=	(float r) {
			x/=r;   y/=r;   z/=r;
			return *this;
		}
		
		// Relations
 		bool	operator ==	(const Vector& v) const	{ return (*this-v).norma() <= 0.0f; }
 		bool	operator !=	(const Vector& v) const	{ return (*this-v).norma() > 0.0f; }
 		bool	operator >	(const Vector& v) const	{ return x>v.x	&& y>v.y && z>v.z; }
 		bool	operator <	(const Vector& v) const	{ return x<v.x	&& y<v.y && z<v.z; }
 		bool	operator >=	(const Vector& v) const	{ return x>=v.x && y>=v.y && z>=v.z; }
 		bool	operator <=	(const Vector& v) const	{ return x<=v.x && y<=v.y && z<=v.z; }
		//

		// Unaries
 		Vector	operator -() const { return Vector(-x,-y,-z); }

		// Binaries
		//...float
MATH_API 		friend Vector operator * (const float r, const Vector& v) { return Vector(r*v.x, r*v.y, r*v.z); }
 		Vector	operator *	(const float r) const	{ return Vector( (r*x),   (r*y),   (r*z)); }
 		Vector	operator /	(const float r) const { return Vector( (x/r),   (y/r),   (z/r) ); }
		//...vector
 		Vector	operator +	(const Vector& v) const	{ return Vector( (x+v.x), (y+v.y), (z+v.z) ); }
 		Vector	operator -	(const Vector& v) const	{ return Vector( (x-v.x), (y-v.y), (z-v.z) ); }
 		Vector	Scale(const Vector& v)	const { return Vector( (x*v.x), (y*v.y), (z*v.z) ); }

		// Dot product
 		float	operator *	(const Vector& v) const	{ return x*v.x+y*v.y+z*v.z; }
		// Vector product !!! USE a+(b^c), NOT a+b^c !!!
 		Vector operator ^ (const Vector& v)	const {
			return Vector( y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
		}

		// Some linears
 		float	norma() const { return (float)sqrt(x*x+y*y+z*z); }
 		float	length() const { return (float)sqrt(x*x+y*y+z*z); }
 		float	length2() const { return (float)(x*x+y*y+z*z); }
 		float   length2_ZX() const { return (float)(x*x+z*z); }
 		float	normaAbs() const { return (float)(fabs(x)+fabs(y)+fabs(z));}
 		Vector	ort() const {
				return Vector(*this/norma());
		}
 		void	norm(){
				*this=*this/norma();
		}
         void    normZX()
        {
            *this = *this/((float)sqrt(x*x+z*z));
        }
		// Rotations
 		Vector rotZ(double ang) const
		{
			float sinang = (float)sin(ang);
			float cosang = (float)cos(ang);
			return Vector(x * cosang - y * sinang, x * sinang + y * cosang, z);
		}
 		Vector rotX(double ang) const
		{
			float sinang = (float)sin(ang);
			float cosang = (float)cos(ang);
			return Vector(x, y * cosang - z * sinang, y * sinang + z * cosang);
		}
 		Vector rotY(double ang) const
		{
			float sinang = (float)sin(ang);
			float cosang = (float)cos(ang);
			return Vector(z * sinang + x * cosang, y, z * cosang - x * sinang);
		}
 		Vector rotXYZ(const Vector &axis, float ang) const
		{
			Vector ax = axis;
			ax.norm();
			double cosa = cos(ang);
			double sina = sin(ang);
			double versa = 1.0 - cosa;

			double xy = ax.x*ax.y;
			double yz = ax.y*ax.z;
			double zx = ax.x*ax.z;
			
			double sinx = ax.x*sina;
			double siny = ax.y*sina;
			double sinz = ax.z*sina;

			  /* подматрица поворота */
			double matr10 = ax.x*ax.x*versa + cosa;
			double matr11 = xy*versa + sinz;
			double matr12 = zx*versa - siny;
			double matr20 = xy*versa - sinz;
			double matr21 = ax.y*ax.y*versa + cosa;
			double matr22 = yz*versa + sinx;
			double matr30 = zx*versa + siny;
			double matr31 = yz*versa - sinx;
			double matr32 = ax.z*ax.z*versa + cosa;

			return Vector((float)(matr10*x + matr20*y + matr30*z),
						  (float)(matr11*x + matr21*y + matr31*z),
						  (float)(matr12*x + matr22*y + matr32*z));
		}

        // vector angle
         double polarZX() const
        {
			double theta = atan2(x, z);
			if (theta < 0.0) 
				theta += M_PI * 2.0;
			return theta;
        }

		// Crossings
 		Vector	crossXY(const Vector &towards) const {	// where do the line from (*this) to (towards) cross the XY plane ?
			float t=z/(z-towards.z);
			return Vector(t*(towards.x-x)+x, t*(towards.y-y)+y, 0);
		}
 		Vector	crossXZ(const Vector &towards) const {	// where do the line from (*this) to (towards) cross the XY plane ?
			float t=y/(y-towards.y);
			return Vector(t*(towards.x-x)+x, 0, t*(towards.z-z)+z);
		}
 		Vector	crossYZ(const Vector &towards) const {	// where do the line from (*this) to (towards) cross the XY plane ?
			float t=x/(x-towards.x);
			return Vector(0, t*(towards.y-y)+y, t*(towards.z-z)+z);
		}
	};

	
	// Linear operator

	class MATH_API Operator {
	public:
		union
		{
			struct
			{
				E3::Vector x;	// a*1
				E3::Vector y;	// a*2
				E3::Vector z;	// a*3
			};
			struct
			{
				float	a11;	// x.x
				float	a21;	// x.y
				float	a31;	// x.z

				float	a12;	// y.x
				float	a22;	// y.y
				float	a32;	// y.z

				float	a13;	// z.x
				float	a23;	// z.y
				float	a33;	// z.z
			};
			float a[9];
		};
 		Operator(void) { }
 		Operator(float c)
		{
			a11=a21=a31=0.0f;
			a12=a22=a32=0.0f;
			a13=a23=a33=0.0f;

			x.x = y.y = z.z = c;
		}
 		Operator(
			float	_a11,float	_a12,float	_a13,
			float	_a21,float	_a22,float	_a23,
			float	_a31,float	_a32,float	_a33) {
			x = E3::Vector(_a11, _a21, _a31);
			y = E3::Vector(_a12, _a22, _a32);
			z = E3::Vector(_a13, _a23, _a33);
		}
 		Operator(const E3::Vector &_x, const E3::Vector &_y, const E3::Vector &_z) :
		x(_x), y(_y), z(_z)
		{ }
	// Copy ctr
 		Operator(const Operator &A) : x(A.x), y(A.y), z(A.z) {
		}
	// Copy 
 		Operator&  operator =	(const Operator& A)
		{
			x = A.x;
			y = A.y;
			z = A.z;
			return *this;
		}
	//
 		Operator&	operator +=	(const Operator& A)
		{
			x += A.x;
			y += A.y;
			z += A.z;
			return *this;
		}
 		Operator&	operator -=	(const Operator& A)
		{
			x -= A.x;
			y -= A.y;
			z -= A.z;
			return *this;
		}
 		Operator&	operator *=	(float r) 
		{
			x *= r;
			y *= r;
			z *= r;
			return *this;
		}
 		Operator&	operator /=	(float r) 
		{
			x /= r;
			y /= r;
			z /= r;
			return *this;
		}
		
		// Unaries
 		Operator	operator -() 
		{
			return Operator(-x, -y, -z);
		}

		// Binaries
		//...float
 		Operator	operator *	(float r)		
		{
			return Operator(x * r, y * r, z * r);
		}
 		Operator	operator /	(float r)		
		{
			return Operator(x / r, y / r, z / r);
		}
 		Operator	operator +	(const Operator& A)	
		{
			return Operator(x + A.x, y + A.y, z + A.z);
		}
 		Operator	operator -	(const Operator& A)	
		{
			return Operator(x - A.x, y - A.y, z - A.z);
		}

		// Transform vector
 		Vector	operator *	(const Vector& v)	
		{
			return Vector(
					a11*v.x + a12*v.y + a13*v.z,
					a21*v.x + a22*v.y + a23*v.z,
					a31*v.x + a32*v.y + a33*v.z
				);
		}

		// Superposition
 		Operator operator * (const Operator &A) 
		{
			return Operator(
					a11*A.a11 + a12*A.a21 + a13*A.a31, // a11
					a11*A.a12 + a12*A.a22 + a13*A.a32, // a12
					a11*A.a13 + a12*A.a23 + a13*A.a33, // a13

					a21*A.a11 + a22*A.a21 + a23*A.a31, // a21
					a21*A.a12 + a22*A.a22 + a23*A.a32, // a22
					a21*A.a13 + a22*A.a23 + a23*A.a33, // a23

					a31*A.a11 + a32*A.a21 + a33*A.a31, // a31
					a31*A.a12 + a32*A.a22 + a33*A.a32, // a32
					a31*A.a13 + a32*A.a23 + a33*A.a33 // a33
				);
		}
 		Operator	T()
		{	// transpose
			return Operator(
			a11,a21,a31,
			a12,a22,a32,
			a13,a23,a33);
		}
 		float	Det()
		{
			return	a11*(a22*a33-a23*a32)+
				a12*(a23*a31-a21*a33)+
				a13*(a21*a32-a22*a31);
		}
 		Operator	I(){
			float	rDet=1.f/Det();
			return	Operator(
                    (a22*a33 - a23*a32)*rDet, ( a13*a32 - a12*a33)*rDet,   ( a12*a23 - a13*a22)*rDet, 
                    (a23*a31 - a21*a33)*rDet, ( a11*a33 - a13*a31)*rDet,   ( a13*a21 - a11*a23)*rDet, 
                    (a21*a32 - a22*a31)*rDet, ( a12*a31 - a11*a32)*rDet,   ( a11*a22 - a12*a21)*rDet
				);
		}
	};
}	// namespace E3


namespace E3D {
	// Vector
	class MATH_API Vector 
	{
		friend class	Operator;
//	protected:
	public:
		double	x;
		double	y;
		double	z;

 		Vector(void) {x = y = z = 0.0;}
 		Vector(double c) {x=y=z=c;}
 		Vector(double cx,double cy,double cz) {	x=cx; y=cy; z=cz;}
	// Copy ctr
 		Vector(const Vector &v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}
	Vector(const E3::Vector &v) 
{
			x = (double)v.x;
			y = (double)v.y;
			z = (double)v.z;
}	
	// Copy 
 		Vector&  operator =	(const Vector& v)	{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}
 		Vector&  operator =	(const E3::Vector& v)	{
			x = (double)v.x;
			y = (double)v.y;
			z = (double)v.z;
			return *this;
		}
	//	Coordinate return inlines
 		inline double	getX() const { return x;}
 		inline double	getY() const { return y;}
 		inline double	getZ() const { return z;}

		inline operator E3::Vector() const
		{return E3::Vector((float)x,(float)y,(float)z);}

 		operator double*(){return &x;}
 		operator const double*()const{return &x;}
	//  Projections
 		E2::Vector VectorXY(void) const { return E2::Vector((float)x, (float)y); }
 		E2::Vector VectorXZ(void) const { return E2::Vector((float)x, (float)z); }
 		E2::Vector VectorYZ(void) const { return E2::Vector((float)y, (float)z); }
 		E2::Vector VectorYX(void) const { return E2::Vector((float)y, (float)x); }
 		E2::Vector VectorZX(void) const { return E2::Vector((float)z, (float)x); }
 		E2::Vector VectorZY(void) const { return E2::Vector((float)z, (float)y); }

	//	In-places
 		Vector&	operator +=	(const Vector& v){
			x+=v.x; y+=v.y; z+=v.z;
			return *this;
		}
 		Vector&	operator -=	(const Vector& v){
			x-=v.x; y-=v.y; z-=v.z;
			return *this;
		}

 		Vector&	operator *=	(double r) {
			x*=r;   y*=r;   z*=r;
			return *this;
		}
 		Vector&	operator /=	(double r) {
			x/=r;   y/=r;   z/=r;
			return *this;
		}
		
		// Relations
 		bool	operator ==	(const Vector& v) const	{ return (*this-v).norma() <= 0.0; }
 		bool	operator !=	(const Vector& v) const	{ return (*this-v).norma() > 0.0; }
 		bool	operator >	(const Vector& v) const	{ return x>v.x	&& y>v.y && z>v.z; }
 		bool	operator <	(const Vector& v) const	{ return x<v.x	&& y<v.y && z<v.z; }
 		bool	operator >=	(const Vector& v) const	{ return x>=v.x && y>=v.y && z>=v.z; }
 		bool	operator <=	(const Vector& v) const	{ return x<=v.x && y<=v.y && z<=v.z; }
		//

		// Unaries
 		Vector	operator -() const { return Vector(-x,-y,-z); }

		// Binaries
		//...float
MATH_API 		friend Vector operator * (const double r, const Vector& v) { return Vector(r*v.x, r*v.y, r*v.z); }
 		Vector	operator *	(const double r) const { return Vector( (r*x),   (r*y),   (r*z)); }
 		Vector	operator /	(const double r) const { return Vector( (x/r),   (y/r),   (z/r) ); }
		//...vector
 		Vector	operator +	(const Vector& v) const	{ return Vector( (x+v.x), (y+v.y), (z+v.z) ); }
 		Vector	operator -	(const Vector& v) const	{ return Vector( (x-v.x), (y-v.y), (z-v.z) ); }
 		Vector	Scale(const Vector& v) const { return Vector( (x*v.x), (y*v.y), (z*v.z) ); }

		// Dot product
 		double	operator *	(const Vector& v) const	{ return x*v.x+y*v.y+z*v.z; }
		// Vector product !!! USE a+(b^c), NOT a+b^c !!!
 		Vector operator ^ (const Vector& v)	const {
			return Vector( y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
		}

		// Some linears
 		double	norma() const { return sqrt(x*x+y*y+z*z); }
 		double	length() const { return sqrt(x*x+y*y+z*z); }
 		double	length2() const { return x*x+y*y+z*z; }
 		double   length2_ZX() const { return x*x+z*z; }
 		double	normaAbs() const { return fabs(x)+fabs(y)+fabs(z);}
 		Vector	ort() const {
				return Vector(*this/norma());
		}
 		void	norm(){
				*this=*this/norma();
		}
         void    normZX()
        {
            *this = *this/(sqrt(x*x+z*z));
        }
		// Rotations
 		Vector rotZ(double ang)
		{
			double sinang = sin(ang);
			double cosang = cos(ang);
			return Vector(x * cosang - y * sinang, x * sinang + y * cosang, z);
		}
 		Vector rotX(double ang)
		{
			double sinang = sin(ang);
			double cosang = cos(ang);
			return Vector(x, y * cosang - z * sinang, y * sinang + z * cosang);
		}
 		Vector rotY(double ang)
		{
			double sinang = sin(ang);
			double cosang = cos(ang);
			return Vector(z * sinang + x * cosang, y, z * cosang - x * sinang);
		}
 		Vector rotXYZ(const Vector &axis, double ang)
		{
			Vector ax = axis;
			ax.norm();
			double cosa = cos(ang);
			double sina = sin(ang);
			double versa = 1.0 - cosa;

			double xy = ax.x*ax.y;
			double yz = ax.y*ax.z;
			double zx = ax.x*ax.z;
			
			double sinx = ax.x*sina;
			double siny = ax.y*sina;
			double sinz = ax.z*sina;

			  /* подматрица поворота */
			double matr10 = ax.x*ax.x*versa + cosa;
			double matr11 = xy*versa + sinz;
			double matr12 = zx*versa - siny;
			double matr20 = xy*versa - sinz;
			double matr21 = ax.y*ax.y*versa + cosa;
			double matr22 = yz*versa + sinx;
			double matr30 = zx*versa + siny;
			double matr31 = yz*versa - sinx;
			double matr32 = ax.z*ax.z*versa + cosa;

			return Vector(matr10*x + matr20*y + matr30*z,
						  matr11*x + matr21*y + matr31*z,
						  matr12*x + matr22*y + matr32*z);
		}

        // vector angle
         double polarZX() const
        {
			double theta = atan2(x, z);
			if (theta < 0.0) 
				theta += M_PI * 2.0;
			return theta;
        }

		// Crossings
 		Vector	crossXY(const Vector &towards) const {	// where do the line from (*this) to (towards) cross the XY plane ?
			double t=z/(z-towards.z);
			return Vector(t*(towards.x-x)+x, t*(towards.y-y)+y, 0);
		}
 		Vector	crossXZ(const Vector &towards) const {	// where do the line from (*this) to (towards) cross the XY plane ?
			double t=y/(y-towards.y);
			return Vector(t*(towards.x-x)+x, 0, t*(towards.z-z)+z);
		}
 		Vector	crossYZ(const Vector &towards) const {	// where do the line from (*this) to (towards) cross the XY plane ?
			double t=x/(x-towards.x);
			return Vector(0, t*(towards.y-y)+y, t*(towards.z-z)+z);
		}
	};

	
	
	// Linear operator

	class MATH_API Operator {
	public:
		union
		{
			struct
			{
				E3D::Vector x;	// a*1
				E3D::Vector y;	// a*2
				E3D::Vector z;	// a*3
			};
			struct
			{
				double	a11;	// x.x
				double	a21;	// x.y
				double	a31;	// x.z

				double	a12;	// y.x
				double	a22;	// y.y
				double	a32;	// y.z

				double	a13;	// z.x
				double	a23;	// z.y
				double	a33;	// z.z
			};
			double a[9];
		};
 		Operator(void) { }
 		Operator(float c)
		{
			a11=a21=a31=0.0;
			a12=a22=a32=0.0;
			a13=a23=a33=0.0;
			
			x.x = y.y = z.z = c;
		}
 		Operator(
			double	_a11,double	_a12,double	_a13,
			double	_a21,double	_a22,double	_a23,
			double	_a31,double	_a32,double	_a33) {
			x = E3D::Vector(_a11, _a21, _a31);
			y = E3D::Vector(_a12, _a22, _a32);
			z = E3D::Vector(_a13, _a23, _a33);
		}
 		Operator(const E3D::Vector &_x, const E3D::Vector &_y, const E3D::Vector &_z) :
		x(_x), y(_y), z(_z)
		{ }
	// Copy ctr
 		Operator(const Operator &A) : x(A.x), y(A.y), z(A.z) {
		}
	// Copy 
 		Operator&  operator =	(const Operator& A)
		{
			x = A.x;
			y = A.y;
			z = A.z;
			return *this;
		}
	//
 		Operator&	operator +=	(const Operator& A)
		{
			x += A.x;
			y += A.y;
			z += A.z;
			return *this;
		}
 		Operator&	operator -=	(const Operator& A)
		{
			x -= A.x;
			y -= A.y;
			z -= A.z;
			return *this;
		}
 		Operator&	operator *=	(float r) 
		{
			x *= r;
			y *= r;
			z *= r;
			return *this;
		}
 		Operator&	operator /=	(double r) 
		{
			x /= r;
			y /= r;
			z /= r;
			return *this;
		}
		
		// Unaries
 		Operator	operator -() 
		{
			return Operator(-x, -y, -z);
		}

		// Binaries
		//...float
 		Operator	operator *	(double r)		
		{
			return Operator(x * r, y * r, z * r);
		}
 		Operator	operator /	(double r)		
		{
			return Operator(x / r, y / r, z / r);
		}
 		Operator	operator +	(const Operator& A)	
		{
			return Operator(x + A.x, y + A.y, z + A.z);
		}
 		Operator	operator -	(const Operator& A)	
		{
			return Operator(x - A.x, y - A.y, z - A.z);
		}

		// Transform vector
 		Vector	operator *	(const Vector& v)	
		{
			return Vector(
					a11*v.x + a12*v.y + a13*v.z,
					a21*v.x + a22*v.y + a23*v.z,
					a31*v.x + a32*v.y + a33*v.z
				);
		}

		// Superposition
 		Operator operator * (const Operator &A) 
		{
			return Operator(
					a11*A.a11 + a12*A.a21 + a13*A.a31, // a11
					a11*A.a12 + a12*A.a22 + a13*A.a32, // a12
					a11*A.a13 + a12*A.a23 + a13*A.a33, // a13

					a21*A.a11 + a22*A.a21 + a23*A.a31, // a21
					a21*A.a12 + a22*A.a22 + a23*A.a32, // a22
					a21*A.a13 + a22*A.a23 + a23*A.a33, // a23

					a31*A.a11 + a32*A.a21 + a33*A.a31, // a31
					a31*A.a12 + a32*A.a22 + a33*A.a32, // a32
					a31*A.a13 + a32*A.a23 + a33*A.a33 // a33
				);
		}
 		Operator	T()
		{	// transpose
			return Operator(
			a11,a21,a31,
			a12,a22,a32,
			a13,a23,a33);
		}
 		double	Det()
		{
			return	a11*(a22*a33-a23*a32)+
				a12*(a23*a31-a21*a33)+
				a13*(a21*a32-a22*a31);
		}
 		Operator	I(){
			double	rDet=1.f/Det();
			return	Operator(
                    (a22*a33 - a23*a32)*rDet, ( a13*a32 - a12*a33)*rDet,   ( a12*a23 - a13*a22)*rDet, 
                    (a23*a31 - a21*a33)*rDet, ( a11*a33 - a13*a31)*rDet,   ( a13*a21 - a11*a23)*rDet, 
                    (a21*a32 - a22*a31)*rDet, ( a12*a31 - a11*a32)*rDet,   ( a11*a22 - a12*a21)*rDet
				);
		}
	};
} // namespace E3D

#ifndef DISSERIALIZER
inline Serializer& operator << (Serializer& serializer, E3::Vector& data)
			{	return serializer << data.x << data.y << data.z;}

inline Serializer& operator << (Serializer& serializer, E3D::Vector& data)
			{	return serializer << data.x << data.y << data.z;}		
#endif

namespace E4 {
	// Vector
	class MATH_API Vector {
		friend class	Operator;
	public:
		float	x;
		float	y;
		float	z;
		float	w;
 		Vector(void) {x = y = z = 0.0; w =  1.0f;}
 		Vector(float c) {x=y=z=c; w=1.f;}
 		Vector(float cx,float cy,float cz, float cw) {
			x=cx; y=cy; z=cz; w=cw;
		}
	// Copy ctr
 		Vector(const Vector &v) {
			x=v.x; y=v.y; z=v.z; w=v.w;
		}
	// Copy 
 		Vector&  operator =	(const Vector& v)	{
			x=v.x; y=v.y; z=v.z; w=v.w;
			return *this;
		}
	//	Coordinate return inlines
 		inline float	getX() { return x;}
 		inline float	getY() { return y;}
 		inline float	getZ() { return z;}
 		inline float	getW() { return w;}

	//	In-places
 		Vector&	operator +=	(const Vector& v){
			x+=v.x; y+=v.y; z+=v.z; w+=v.w;
			return *this;
		}
 		Vector&	operator -=	(const Vector& v){
			x-=v.x; y-=v.y; z-=v.z; w-=v.w;
			return *this;
		}
 		Vector&	operator *=	(float r) {
			x*=r;   y*=r;   z*=r; w*=r;
			return *this;
		}
 		Vector&	operator /=	(float r) {
			x/=r;   y/=r;   z/=r; w/=r;
			return *this;
		}
		
		// Relations
 		bool	operator ==	(const Vector& v)	{ return (*this-v).norma() <= 0.0f; }
 		bool	operator !=	(const Vector& v)	{ return (*this-v).norma() > 0.0f; }
 		bool	operator >	(const Vector& v)	{ return x>v.x	&& y>v.y && z>v.z && w>v.w; }
 		bool	operator <	(const Vector& v)	{ return x<v.x	&& y<v.y && z<v.z && w<v.w; }
 		bool	operator >=	(const Vector& v)	{ return x>=v.x && y>=v.y && z>=v.z && w>=v.w; }
 		bool	operator <=	(const Vector& v)	{ return x<=v.x && y<=v.y && z<=v.z && w<=v.w; }
		//

		// Unaries
 		Vector	operator -() { return Vector(-x,-y,-z,-w); }

		// Binaries
		//...float
 		Vector	operator *	(float r)		{ return Vector( (r*x),   (r*y),   (r*z),	(r*w)); }
MATH_API 		friend Vector operator * (float r, Vector &v) { return Vector(r*v.x, r*v.y, r*v.z, r*v.w); }
 		Vector	operator /	(float r) { return Vector( (x/r),   (y/r),   (z/r),	(w/r)); }
		//...vector
 		Vector	operator +	(const Vector& v)	{ return Vector( (x+v.x), (y+v.y), (z+v.z), w+v.w ); }
 		Vector	operator -	(const Vector& v)	{ return Vector( (x-v.x), (y-v.y), (z-v.z), w-v.w); }
 		Vector	Scale(const Vector& v)	{ return Vector( (x*v.x), (y*v.y), (z*v.z), (w*v.w) ); }

		// Dot product
 		float	operator *	(const Vector& v)	{ return x*v.x+y*v.y+z*v.z+w*v.w; }

		// Some linears
 		float	norma(){ return (float)sqrt(x*x+y*y+z*z+w*w); }
 		float	normaAbs(){ return (float)(fabs(x)+fabs(y)+fabs(z)+fabs(w)); }
 		Vector	ort(float r){
				return Vector(x/=r, y/=r, z/=r, w/=r);
		}
		
		// Crossings
 		Vector	crossXYW(const Vector &towards){	// where do the line from (*this) to (towards) cross the XY plane ?
			float t=z/(z-towards.z);
			return Vector(t*(towards.x-x)+x, t*(towards.y-y)+y, 0, t*(towards.w-w)+w);
		}
 		Vector	crossXZW(const Vector &towards){	// where do the line from (*this) to (towards) cross the XY plane ?
			float t=y/(y-towards.y);
			return Vector(t*(towards.x-x)+x, 0, t*(towards.z-z)+z, t*(towards.w-w)+w);
		}
 		Vector	crossYZW(const Vector &towards){	// where do the line from (*this) to (towards) cross the XY plane ?
			float t=x/(x-towards.x);
			return Vector(0, t*(towards.y-y)+y, t*(towards.z-z)+z, t*(towards.w-w)+w);
		}
 		Vector	crossXYZ(const Vector &towards){	// where do the line from (*this) to (towards) cross the XY plane ?
			float t=w/(w-towards.w);
			return Vector(t*(towards.x-x)+x, t*(towards.y-y)+y, t*(towards.z-z)+z, 0);
		}
		// Conversions
 		E3::Vector	BackHomogenius(){
			return( E3::Vector(x/w,y/w,z/w) );
		}
	};
	
	// Linear operator

	class MATH_API Operator {
		float	a11;
		float	a21;
		float	a31;
		float	a41;

		float	a12;
		float	a22;
		float	a32;
		float	a42;

		float	a13;
		float	a23;
		float	a33;
		float	a43;

		float	a14;
		float	a24;
		float	a34;
		float	a44;

	public:
 		Operator(void) { }
 		Operator(float c) {
			    a12=a13=a14=0.0f;
			a21=    a23=a24=0.0f;
			a31=a32=    a34=0.0f;
			a41=a42=a43=    0.0f;

			a11=a22=a33=a44=c;
		}
 		Operator(
			float	_a11,float	_a12,float	_a13,float	_a14,
			float	_a21,float	_a22,float	_a23,float	_a24,
			float	_a31,float	_a32,float	_a33,float	_a34,
			float	_a41,float	_a42,float	_a43,float	_a44) {
			a11=_a11;	a12=_a12;	a13=_a13;	a14=_a14;
			a21=_a21;	a22=_a22;	a23=_a23;	a24=_a24;
			a31=_a31;	a32=_a32;	a33=_a33;	a34=_a34;
			a41=_a41;	a42=_a42;	a43=_a43;	a44=_a44;
		}
	// Copy ctr
 		Operator(const Operator &A) {
			a11=A.a11;	a12=A.a12;	a13=A.a13;	a14=A.a14;
			a21=A.a21;	a22=A.a22;	a23=A.a23;	a24=A.a24;
			a31=A.a31;	a32=A.a32;	a33=A.a33;	a34=A.a34;
			a41=A.a41;	a42=A.a42;	a43=A.a43;	a44=A.a44;
		}
	// Copy 
 		Operator&  operator =	(const Operator& A)	{
			a11=A.a11;	a12=A.a12;	a13=A.a13;	a14=A.a14;
			a21=A.a21;	a22=A.a22;	a23=A.a23;	a24=A.a24;
			a31=A.a31;	a32=A.a32;	a33=A.a33;	a34=A.a34;
			a41=A.a41;	a42=A.a42;	a43=A.a43;	a44=A.a44;
			return *this;
		}
	//
 		Operator&	operator +=	(const Operator& A){
			a11+=A.a11; a12+=A.a12; a13+=A.a13; a14+=A.a14;
			a21+=A.a21; a22+=A.a22; a23+=A.a23; a24+=A.a24;
			a31+=A.a31; a32+=A.a32; a33+=A.a33; a34+=A.a34;
			a41+=A.a41; a42+=A.a42; a43+=A.a43; a44+=A.a44;
			return *this;
		}
 		Operator&	operator -=	(const Operator& A){
			a11-=A.a11; a12-=A.a12; a13-=A.a13; a14-=A.a14;
			a21-=A.a21; a22-=A.a22; a23-=A.a23; a24-=A.a24;
			a31-=A.a31; a32-=A.a32; a33-=A.a33; a34-=A.a34;
			a41-=A.a41; a42-=A.a42; a43-=A.a43; a44-=A.a44;
			return *this;
		}
 		Operator&	operator *=	(float r) {
			a11*=r;
			a21*=r;
			a31*=r;
			a41*=r;

			a12*=r;
			a22*=r;
			a32*=r;
			a42*=r;

			a13*=r;
			a23*=r;
			a33*=r;
			a43*=r;

			a14*=r;
			a24*=r;
			a34*=r;
			a44*=r;
			return *this;
		}
 		Operator&	operator /=	(float r) {
			a11/=r;
			a21/=r;
			a31/=r;
			a41/=r;

			a12/=r;
			a22/=r;
			a32/=r;
			a42/=r;

			a13/=r;
			a23/=r;
			a33/=r;
			a43/=r;

			a14/=r;
			a24/=r;
			a34/=r;
			a44/=r;
			return *this;
		}
		
		// Unaries
 		Operator	operator -() {
			return Operator(
			-a11,-a12,-a13,-a14,
			-a21,-a22,-a23,-a24,
			-a31,-a32,-a33,-a34,
			-a41,-a42,-a43,-a44	);
		}

		// Binaries
		//...float
 		Operator	operator *	(float r)		{
			return Operator(
			r*a11,r*a12,r*a13,r*a14,
			r*a21,r*a22,r*a23,r*a24,
			r*a31,r*a32,r*a33,r*a34,
			r*a41,r*a42,r*a43,r*a44	);
		}
 		Operator	operator /	(float r)		{
			return Operator(
			a11/r,a12/r,a13/r,a14/r,
			a21/r,a22/r,a23/r,a24/r,
			a31/r,a32/r,a33/r,a34/r,
			a41/r,a42/r,a43/r,a44/r	);
		}
 		Operator	operator +	(const Operator& A)	{
			return Operator(
				a11+A.a11, a12+A.a12, a13+A.a13, a14+A.a14,
				a21+A.a21, a22+A.a22, a23+A.a23, a24+A.a24,
				a31+A.a31, a32+A.a32, a33+A.a33, a34+A.a34,
				a41+A.a41, a42+A.a42, a43+A.a43, a44+A.a44
				);
		}
 		Operator	operator -	(const Operator& A)	{
			return Operator(
				a11-A.a11, a12-A.a12, a13-A.a13, a14-A.a14,
				a21-A.a21, a22-A.a22, a23-A.a23, a24-A.a24,
				a31-A.a31, a32-A.a32, a33-A.a33, a34-A.a34,
				a41-A.a41, a42-A.a42, a43-A.a43, a44-A.a44
				);
		}

		// Transform vector
 		Vector	operator *	(const Vector& v)	{
			return Vector(
					a11*v.x + a12*v.y + a13*v.z + a14*v.w,
					a21*v.x + a22*v.y + a23*v.z + a24*v.w,
					a31*v.x + a32*v.y + a33*v.z + a34*v.w,
					a41*v.x + a42*v.y + a43*v.z + a44*v.w
				);
		}

		// Superposition
 		Operator operator * (const Operator &A) {
			return Operator(
					a11*A.a11 + a12*A.a21 + a13*A.a31 + a14*A.a41, // a11
					a11*A.a12 + a12*A.a22 + a13*A.a32 + a14*A.a42, // a12
					a11*A.a13 + a12*A.a23 + a13*A.a33 + a14*A.a43, // a13
					a11*A.a14 + a12*A.a24 + a13*A.a34 + a14*A.a44, // a14

					a21*A.a11 + a22*A.a21 + a23*A.a31 + a24*A.a41, // a21
					a21*A.a12 + a22*A.a22 + a23*A.a32 + a24*A.a42, // a22
					a21*A.a13 + a22*A.a23 + a23*A.a33 + a24*A.a43, // a23
					a21*A.a14 + a22*A.a24 + a23*A.a34 + a24*A.a44, // a24

					a31*A.a11 + a32*A.a21 + a33*A.a31 + a34*A.a41, // a31
					a31*A.a12 + a32*A.a22 + a33*A.a32 + a34*A.a42, // a32
					a31*A.a13 + a32*A.a23 + a33*A.a33 + a34*A.a43, // a33
					a31*A.a14 + a32*A.a24 + a33*A.a34 + a34*A.a44, // a34

					a41*A.a11 + a42*A.a21 + a43*A.a31 + a44*A.a41,  // a41
					a41*A.a12 + a42*A.a22 + a43*A.a32 + a44*A.a42,  // a42
					a41*A.a13 + a42*A.a23 + a43*A.a33 + a44*A.a43,  // a43
					a41*A.a14 + a42*A.a24 + a43*A.a34 + a44*A.a44  // a44
				);
		}

 		Operator	T(){	// transpose
			return Operator(
			a11,a21,a31,a41,
			a12,a22,a32,a42,
			a13,a23,a33,a43,
			a14,a24,a34,a44	);
		}

 		float	Det(){
			float	a33a44=a33*a44;
			float	a34a43=a34*a43;
			float	a33a44_a34a43=a33a44 - a34a43;

			float	a34a42=a34*a42;
			float	a32a44=a32*a44;
			float	a34a42_a32a44=a34a42 - a32a44;

			float	a32a43=a32*a43;
			float	a33a42=a33*a42;
			float	a32a43_a33a42=a32a43 - a33a42;
			
			
			float	a34a41=a34*a41;
			float	a31a44=a31*a44;
			float	a34a41_a31a44=a34a41 - a31a44;

			float	a31a43=a31*a43;
			float	a33a41=a33*a41;
			float	a31a43_a33a41=a31a43 - a33a41;

			float	a31a42=a31*a42;
			float	a32a41=a32*a41;
			float	a31a42_a32a41=a31a42 - a32a41;

			return
				  a11*(a22*(a33a44_a34a43)	+	a23*(a34a42_a32a44)	+	a24*(a32a43_a33a42))
				- a12*(a21*(a33a44_a34a43)	+	a23*(a34a41_a31a44)	+	a24*(a31a43_a33a41))
				+ a13*(						+	a22*(a34a41_a31a44)	+	a24*(a31a42_a32a41)	- a21*(a34a42_a32a44))
				- a14*(a21*(a32a43_a33a42)	-	a22*(a31a42_a32a41)	+	a23*(a31a42_a32a41));
		}
 		Operator	I(){
			Operator	B;
			// entirely from CGT - great product of Nijny Novgorod
		   float det,  invDet;
		   float md4,  md5,  md6,  md7;
		   float im11, im12, im13, im14;
		   float d12,  d13,  d23,  d24, d34, d41;
		   float tmp0, tmp1, tmp2, tmp3, tmp5, tmp6, tmp7, tmp10;

		   if( a41 == 0.f && a42 == 0.f && a43 == 0.f && a44 == 1.f ) 
			  {
			  /* Inverse = adjoint / det. (See linear algebra texts.)*/
			  tmp0= a22 * a33 - a23 * a32;
			  tmp1= a23 * a31 -  a21 * a33;
			  tmp2= a21 * a32  - a22 * a31;

			  /* Compute determinant as early as possible using these cofactors. */
			  det= a11 * tmp0 + a12 * tmp1 + a13 * tmp2;

			  /* Run singularity test. */
			  if (det == 0.f){
					B=Operator(
						1.f, 0.f, 0.f, 0.f,
						0.f, 1.f, 0.f, 0.f,
						0.f, 0.f, 1.f, 0.f,
						0.f, 0.f, 0.f, 1.f
						);
			  } 
			  else {
				 det= 1.f / det;     

				 /* Compute rest of inverse. */
				 im11= a11 * det;     im12= a12  * det;
				 im13= a13 * det;     im14= a14 * det;

				 B.a11 = tmp0 * det;
				 tmp1  *= det; 
				 tmp2  *= det;
				 B.a41 = 0.f;

				 B.a12 = im13 * a32  - im12 * a33;
				 tmp5   = im11 * a33 - im13 * a31;
				 tmp6   = im12 * a31  - im11 * a32;
				 B.a42 = 0.f;

				 /* Pre-compute 2x2 dets for first two rows when computing */
				 /* cofactors of last two rows. */
				 d12 = im11 * a22  - a21 * im12;
				 d13 = im11 * a23  - a21 * im13;
				 d23 = im12 * a23  - a22 * im13;
				 d24 = im12 * a24 - a22 * im14;
				 d34 = im13 * a24 - a23 * im14;
				 d41 = im14 * a21  - a24* im11;

				 B.a13  =  d23;
				 B.a23  = -d13;
				 tmp10   = d12;
				 B.a43 = 0.f;

				 B.a41 = -(a32 * d34 - a33 * d24 + a34 * d23);
				 B.a42 =  (a31 * d34 + a33 * d41 + a34 * d13);
				 B.a43 = -(a31 * d24 + a32  * d41 + a34 * d12);
				 B.a44 =  1.f;

				 B.a21 = tmp1;   B.a31  = tmp2;   B.a22 = tmp5;
				 B.a32 = tmp6;   B.a33 = tmp10;
			  }
		   } 
		   else {
			  /* pre-compute 2x2 dets for last two rows when computing */
			  /* cofactors of first two rows. */
			  d12 =  (a31  * a42  - a41  * a32);
			  d13 =  (a31  * a43 - a41  * a33);
			  d23 =  (a32  * a43 - a42  * a33);
			  d24 =  (a32  * a44 - a42  * a34);
			  d34 =  (a33 * a44 - a43 * a34);
			  d41 =  (a34 * a41  - a44 * a31);

			  tmp0 =  (a22 * d34 - a23 * d24 + a24 * d23);
			  tmp1 = -(a21 * d34 + a23 * d41 + a24 * d13);
			  tmp2 =  (a21 * d24 + a22 * d41 + a24 * d12);
			  tmp3 =  -a21 * d23 + a22 * d13 - a23 * d12;

			  /* Compute determinant as early as possible using these cofactors */
			  det = a11 * tmp0 + a12 * tmp1 + a13 * tmp2 + a14 * tmp3;

			  /* Run singularity test. */
			  if (det == 0.f) 
					B=Operator(
						1.f, 0.f, 0.f, 0.f,
						0.f, 1.f, 0.f, 0.f,
						0.f, 0.f, 1.f, 0.f,
						0.f, 0.f, 0.f, 1.f
						);
			  else
				 {
				 invDet = 1.f / det;
				 /* Compute rest of inverse. */
				 md4 =  (a13 * d24 - a12 * d34 - a14 * d23);
				 md5 =  (a11 * d34 + a13 * d41 + a14 * d13);
				 md6 = -(a11 * d24 + a12 * d41 + a14 * d12);
				 md7 =  (a11 * d23 - a12 * d13 + a13  * d12);

				 d12 = a11 * a22  - a21 * a12;
				 d13 = a11 * a23  - a21 * a13;
				 d23 = a12 * a23  - a22 * a13;
				 d24 = a12 * a24 - a22 * a14;
				 d34 = a13 * a24 - a23 * a14;
				 d41 = a14* a21  - a24* a11;

				 B.a11  = tmp0 * invDet;   B.a21  = tmp1 * invDet;
				 tmp2   *= invDet;          tmp3   *= invDet;

				 B.a12 = md4 * invDet;     B.a22  = md5 * invDet;
				 tmp6   = md6 * invDet;     tmp7   = md7 * invDet;

				 /* Pre-compute 2x2 dets for first two rows when computing */
				 /* cofactors of last two rows. */

				 B.a13  =  (a42  * d34 - a43 * d24 + a44 * d23) * invDet;
				 B.a23  = -(a41  * d34 + a43 * d41 + a44 * d13) * invDet;
				 tmp10   =  (a41  * d24 + a42  * d41 + a44 * d12) * invDet;
				 B.a43 =  (a42  * d13 - a41  * d23 - a43 * d12) * invDet;
				 B.a41 =  (a33 * d24 - a32  * d34 - a34 * d23) * invDet;
				 B.a42 =  (a31  * d34 + a33 * d41 + a34 * d13) * invDet;
				 B.a43 = -(a31  * d24 + a32  * d41 + a34 * d12) * invDet;
				 B.a44 =  (a31  * d23 - a32  * d13 + a33 * d12) * invDet;

				 B.a31  = tmp2;  B.a41  = tmp3;  B.a32  = tmp6;  
				 B.a42  = tmp7;  B.a33 = tmp10;
			  }
		   }
		   return B;
		}
	};
}	// namespace E4

typedef E3::Vector cVector;
typedef cVector cPoint;
typedef cVector fVector;
typedef cPoint fPoint;
typedef E3D::Vector dVector;
typedef dVector dPoint;

