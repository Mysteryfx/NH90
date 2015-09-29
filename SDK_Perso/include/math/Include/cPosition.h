#pragma once

# include <memory.h>
# include <wtypes.h>
#include "MathExport.h"
#include "cLinear.h"
#include "GraphMath/trigonometry.h"
#include "GraphMath/position.h"

//const  double Pi = 3.141592653589793;
const  double Pi_2= Pi*2.0;
const  double Pi_05 = Pi*0.5;
const  double Grad_to_Rad = Pi/180.0;
const  double Rad_to_Grad = 180.0/Pi;
const  float g = 9.8f; // ускорение свободного падения.

// Класс cPosition соответствует матрице OpenGL. Это
// означает, что после чтения матрицы M из базы данных
// позиция pos устанавливается следующим образом:
// pos.x = cVector(M[0], M[1], M[2]);
// pos.y = cVector(M[4], M[5], M[6]);
// pos.z = cVector(M[8], M[9], M[10]);
// pos.p = cPoint(M[12], M[13], M[14]);
// Элементы M[3], M[7], M[11] и M[15] игнорируются. При
// копировании из позиции в матрицу эти элементы полагаются
// равными 0.0f, 0.0f, 0.0f и 1.0f соответственно.
class MATH_API cPosition 
{
public:
 	static		cPosition	POS_ONE;
	cVector		x;
	cVector		y;
	cVector		z;
	cPoint		p;

 		cPosition(void);
 		cPosition(const cPosition &pos) : x(pos.x), y(pos.y), z(pos.z), p(pos.p) {}; 
 		cPosition(const cVector&, const cVector&, const cVector&, const cPoint&);
 		cPosition(const double * const m);
 		cPosition(const E3::Operator &, const E3::Vector &);

 	cPosition	&operator = (const cPosition& pos);

			// OPERATORS WITH POSITION3
			cPosition	&operator = (const Position3& pos)
			{
				x.x = pos.x.x;
				x.y = pos.x.y;
				x.z = pos.x.z;
				y.x = pos.y.x;
				y.y = pos.y.y;
				y.z = pos.y.z;
				z.x = pos.z.x;
				z.y = pos.z.y;
				z.z = pos.z.z;
				p.x = pos.p.x;
				p.y = pos.p.y;
				p.z = pos.p.z;
/*
				x = *((cVector *)&pos.x);
				y = *((cVector *)&pos.y);
				z = *((cVector *)&pos.z);
				p = *((cVector *)&pos.p);
*/
				return *this;
			}

			operator Position3() const
			{
				Position3 res;
				res.x.x = x.x;
				res.x.y = x.y;
				res.x.z = x.z;
				res.y.x = y.x;
				res.y.y = y.y;
				res.y.z = y.z;
				res.z.x = z.x;
				res.z.y = z.y;
				res.z.z = z.z;
				res.p.x = p.x;
				res.p.y = p.y;
				res.p.z = p.z;
/*
				res.x = *((Vector3 *)&x);
				res.y = *((Vector3 *)&y);
				res.z = *((Vector3 *)&z);
				res.p = *((Vector3 *)&p);
*/
				return res;
			}


 	const bool		operator ==	(const cPosition& pos) const { return x==pos.x && y==pos.y && z==pos.z && p==pos.p; }
	const bool		operator !=	(const cPosition& pos) const { return x!=pos.x || y!=pos.y || z!=pos.z || p!=pos.p; }

 cPosition	operator +	(const cPosition& pos) const { return cPosition( (x+pos.x), (y+pos.y), (z+pos.z), (p+pos.p) ); }
 cPosition	operator -	(const cPosition& pos) const { return cPosition( (x-pos.x), (y-pos.y), (z-pos.z), (p-pos.p) ); }
 cPosition	operator *	(float r) const		{ return cPosition( (r*x),	   (r*y),     (r*z),     (r*p)	   ); }
 cPosition	operator /	(float r) const		{ return cPosition( (x/r),     (y/r),     (z/r),     (p/r)     ); }

 	void	mult(const cPosition &by, cPosition &to) const;
 	void	invert(cPosition &to) const;

     void    transp(cPosition&);
	// Функции ортогонализации для двух произвольных начальных векторов
 	void	norm_X_Y(void);
 	void	norm_X_Y_Z(void);
 	void	norm_Y_Z_X(void);
 	void	norm_Z_X_Y(void);
	// Для двух заведомо единичных начальных векторов
 	void	X_Y_Z(void);
 	void	Y_Z_X(void);
 	void	Z_X_Y(void);

 	void	ONE(void);
 	void	ONEBASIS(void);

 	void	rot_X(float Degrees) { rotX(float(Grad_to_Rad*Degrees)); }
 	void	rot_Y(float Degrees) { rotY(float(Grad_to_Rad*Degrees)); }
 	void	rot_Z(float Degrees) { rotZ(float(Grad_to_Rad*Degrees)); }
 	void	rotX(float Radians);
 	void	rotY(float Radians);
 	void	rotZ(float Radians);

// поворот относительно вектора V, проходящего через центр,
// на угол arad (V - должен быть единичным)
 	void	rotVec(float Vx, float Vy, float Vz, float Radians);

 	void	scale(const cVector &);
 	void	scale(float);

 	void	move(const cVector &);
 	void	putToMatrix(double *m) const;
 	void	fromMatrix(const double *m);

 	bool	checkOrtNorm(void);

	//virtual float Distance2(cVector& Vec) { return (Vec - p).length2();}

MATH_API 	friend void		mult(cVector&,const cPosition&,const cVector&);
MATH_API 	friend void		mult_transp(cVector&,const cPosition&,const cVector&);
MATH_API	friend void		mult_move(cVector&,const cPosition&,const cVector&);
MATH_API   friend float	Bearing(const cVector&,const cVector&);
MATH_API 	friend float	Pitch(const cVector&,const cVector&);
MATH_API   friend float    Hor_Distance(const cVector&,const cVector&);
MATH_API   friend float    Hor_Distance2(const cVector& a,const cVector& b); 
MATH_API   friend void     Direction(cVector&,const cVector&,const cVector&);
MATH_API   friend float    Distance(const cVector& a,const cVector& b);
MATH_API   friend float    Distance2(const cVector& a,const cVector& b); 
MATH_API 	friend float    Vel_of_Sound(float h_); //Возвращает скорость звука (м/c) на высоте h_ м 
MATH_API 	friend float    Air_Density (float h_);//Возвращает плотность воздуха на высоте h_ м 
MATH_API 	friend void		satm (float H, float *VelSound = NULL, float *Dens = NULL,float *T = NULL);//Вычисляет параметры атмосферы на высоте h_ м 
MATH_API 	friend bool		SideBySide(const cVector& a,const cVector& b,  float r); // Функция возвращает "true", если точки "a" и "b" лежат внутри куба с длиной стороны "r"
MATH_API 	friend float    splah (int l,float *x, float *y, float u,int iextr); // Сплайн
#ifndef DISSERIALIZER
MATH_API	friend Serializer& operator << (Serializer& serializer, cPosition& data)
			{	return serializer << data.x << data.y << data.z << data.p;}
#endif
};
