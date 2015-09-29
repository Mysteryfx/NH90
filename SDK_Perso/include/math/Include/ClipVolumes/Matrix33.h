#pragma once

#include "cLinear.h"
namespace oldmath
{
	#ifndef INTERSECTION_TYPE
	#define INTERSECTION_TYPE

	enum IntersectionType 
	{
		itInside,     // matrix frastrum is inside object
		itIntersects, // matrix frastrum intersects object
		itContains,   // matrix frastrum contains object
		itOutside     // matrix frastrum don't have common points with object
	};

	#endif // INTERSECTION_TYPE

	class Matrix33
	{
	  private:
		float M[9];
	  public:

		Matrix33(void);
		Matrix33(const cPoint& box);
		Matrix33(const cPoint &C1, const cPoint &C2, const cPoint &C3);
		Matrix33(cPoint *C); // array of 3 columns

		Matrix33 &operator =(const Matrix33 &Source);

		float &operator [](int Index);
		float operator [](int Index) const;

		float Det(void) const; // returns Determinant

		cVector GetOffset(void) const; // return translation component (x,z,0)

		Matrix33 operator *(const Matrix33 &Op) const;
		Matrix33 &operator *=(const Matrix33 &Op);

		void Transp(void); // flip matrix
		void Invert(void); // M*M.Invert() = I

		void Identity(void);
		void Zero(void);
		void Translate(float dx, float dy);
		void Rotate(float Angle); // Angle in degrees
    
		void Scale(float SX,float SY);
		void Scale(const cVector &S);

		operator float *(void);

		// frastrum operations
		void GetFrastrumBox(float &xMin, float &yMin, float &xMax, float &yMax) const; // возвращает описанный прямоугольник

		// intersection section
		// matrix is an operator converting points (0,0) (1,0) (0,1) (1,1) into frastrum corners	
		IntersectionType IntersectPoint(float x, float y) const; // must be called for INVERTED matrix
		IntersectionType IntersectBox(float x, float y) const; 
		IntersectionType IntersectBox(float x, float y, float X, float Y) const; 
		IntersectionType IntersectFrastrum(const Matrix33& obj) const;
		IntersectionType IntersectSphere(const cPoint& center, float r) const;
	};

	extern Matrix33 Matrix33I;

	cVector operator *(const Matrix33 &M, const cVector &V);
	cVector operator *(const cVector &V, const Matrix33 &M);
	cVector &operator *=(cVector &V, const Matrix33 &M);

	// itInside => box in circle
	IntersectionType BoxWithCircle(float x, float y, float X, float Y, const cVector& center, float r);
}

