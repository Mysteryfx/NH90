#pragma once

# include <memory.h>
# include <wtypes.h>
#include "MathExport.h"
#include "cLinear.h"
#include "cPosition.h"
#include "GraphMath/trigonometry.h"
#include "GraphMath/position.h"


// Класс dPosition соответствует матрице OpenGL. Это
// означает, что после чтения матрицы M из базы данных
// позиция pos устанавливается следующим образом:
// pos.x = dVector(M[0], M[1], M[2]);
// pos.y = dVector(M[4], M[5], M[6]);
// pos.z = dVector(M[8], M[9], M[10]);
// pos.p = dPoint(M[12], M[13], M[14]);
// Элементы M[3], M[7], M[11] и M[15] игнорируются. При
// копировании из позиции в матрицу эти элементы полагаются
// равными 0.0, 0.0, 0.0 и 1.0 соответственно.
class MATH_API dPosition 
{
public:
 	static		dPosition	POS_ONE;
	dVector		x;
	dVector		y;
	dVector		z;
	dVector		p;

 		dPosition(void);
 		dPosition(const dPosition &pos) : x(pos.x), y(pos.y), z(pos.z), p(pos.p) {}; 
		dPosition(const cPosition &pos) : x(pos.x), y(pos.y), z(pos.z), p(pos.p) {}; 
 		dPosition(const dVector&, const dVector&, const dVector&, const dPoint&);
 		dPosition(const double * const m);
 		dPosition(const E3::Operator &, const E3::Vector &);

 	dPosition	&operator = (const dPosition& pos);

			// OPERATORS WITH POSITION3
			dPosition	&operator = (const Position3& pos)
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
				x = *((dVector *)&pos.x);
				y = *((dVector *)&pos.y);
				z = *((dVector *)&pos.z);
				p = *((dVector *)&pos.p);
*/
				return *this;
			}

			operator Position3() const
			{
				Position3 res;
				res.x.x = (float)x.x;
				res.x.y = (float)x.y;
				res.x.z = (float)x.z;
				res.y.x = (float)y.x;
				res.y.y = (float)y.y;
				res.y.z = (float)y.z;
				res.z.x = (float)z.x;
				res.z.y = (float)z.y;
				res.z.z = (float)z.z;
				res.p.x = (float)p.x;
				res.p.y = (float)p.y;
				res.p.z = (float)p.z;
				return res;
			}

			// OPERATORS WITH CPOSITION
			dPosition	&operator = (const cPosition& pos)
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
				x = *((dVector *)&pos.x);
				y = *((dVector *)&pos.y);
				z = *((dVector *)&pos.z);
				p = *((dVector *)&pos.p);
*/
				return *this;
			}

			operator cPosition() const
			{
				cPosition res;
				res.x.x = (float)x.x;
				res.x.y = (float)x.y;
				res.x.z = (float)x.z;
				res.y.x = (float)y.x;
				res.y.y = (float)y.y;
				res.y.z = (float)y.z;
				res.z.x = (float)z.x;
				res.z.y = (float)z.y;
				res.z.z = (float)z.z;
				res.p.x = (float)p.x;
				res.p.y = (float)p.y;
				res.p.z = (float)p.z;
				return res;
			}

 	const bool		operator ==	(const dPosition& pos)	{ return x==pos.x && y==pos.y && z==pos.z && p==pos.p; }
 	const bool		operator !=	(const dPosition& pos)	{ return x!=pos.x || y!=pos.y || z!=pos.z || p!=pos.p; }

 dPosition	operator +	(dPosition& pos)	{ return dPosition( (x+pos.x), (y+pos.y), (z+pos.z), (p+pos.p) ); }
 dPosition	operator -	(dPosition& pos)	{ return dPosition( (x-pos.x), (y-pos.y), (z-pos.z), (p-pos.p) ); }
 dPosition	operator *	(double r)			{ return dPosition( (r*x),	   (r*y),     (r*z),     (r*p)	   ); }
 dPosition	operator /	(double r)			{ return dPosition( (x/r),     (y/r),     (z/r),     (p/r)     ); }

 	void	mult(const dPosition &by, dPosition &to) const;
 	void	invert(dPosition &to) const;

     void    transp(dPosition&);
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

 	void	rot_X(double Degrees) { rotX(Grad_to_Rad*Degrees); }
 	void	rot_Y(double Degrees) { rotY(Grad_to_Rad*Degrees); }
 	void	rot_Z(double Degrees) { rotZ(Grad_to_Rad*Degrees); }
 	void	rotX(double Radians);
 	void	rotY(double Radians);
 	void	rotZ(double Radians);

// поворот относительно вектора V, проходящего через центр,
// на угол arad (V - должен быть единичным)
 	void	rotVec(double Vx, double Vy, double Vz, double Radians);

 	void	scale(const dVector &);
 	void	scale(double);

 	void	move(const dVector &);
 	void	putToMatrix(double *m) const;
 	void	fromMatrix(const double *m);

 	bool	checkOrtNorm(void);

	//virtual float Distance2(cVector& Vec) { return (Vec - p).length2();}

MATH_API 	friend void		mult(dVector&,const dPosition&,const dVector&);
MATH_API 	friend void		mult_transp(dVector&,const dPosition&,const dVector&);
MATH_API	friend void		mult_move(dVector&,const dPosition&, const dVector&);
MATH_API   friend double	Bearing(const dVector&,const dVector&);
MATH_API 	friend double	Pitch(const dVector&,const dVector&);
MATH_API   friend double    Hor_Distance(const dVector&,const dVector&);
MATH_API   friend double    Hor_Distance2(const dVector& a,const dVector& b); 
MATH_API   friend void     Direction(dVector&,const dVector&,const dVector&);
MATH_API   friend double    Distance(const dVector& a,const dVector& b);
MATH_API   friend double    Distance2(const dVector& a,const dVector& b); 
/*
MATH_API 	friend float    Vel_of_Sound(float h_); //Возвращает скорость звука (м/c) на высоте h_ м 
MATH_API 	friend float    Air_Density (float h_);//Возвращает плотность воздуха на высоте h_ м 
MATH_API 	friend void		satm (float H, float *VelSound, float *Dens,float *T = 0);//Вычисляет параметры атмосферы на высоте h_ м 
MATH_API 	friend bool		SideBySide(cVector& a,  cVector& b,  float r); // Функция возвращает "true", если точки "a" и "b" лежат внутри куба с длиной стороны "r"
MATH_API 	friend float    splah (int l,float *x, float *y, float u,int iextr); // Сплайн
*/
#ifndef DISSERIALIZER
MATH_API	friend Serializer& operator << (Serializer& serializer, dPosition& data)
			{	return serializer << data.x << data.y << data.z << data.p;}
#endif
};
/*
// позиция камеры МФД
# define MFD_UNDEFINED_MODE	0
# define MFD_RADAR_MODE		1
# define MFD_TV_MODE		2
# define MFD_IR_MODE		3

const float InitHalfViewAngleMFD = float(Pi * 0.0625);

class MATH_API cPositionMFD : public cPosition {
public:
	float	HalfViewAngle; // половина угола раствора камеры (рад.)
	int		Mode;	// режим работы МФД.
	bool	isIIR;	// infrared		
 	cPositionMFD(void) : HalfViewAngle(InitHalfViewAngleMFD), Mode(MFD_UNDEFINED_MODE), isIIR(false) {}
 	cPositionMFD(cPosition &p, float a = 0.f) : 
						 cPosition(p), HalfViewAngle(a), Mode(MFD_UNDEFINED_MODE), isIIR(false) {}
 	cPositionMFD &operator=(const cPositionMFD& Pos) { 
		p = Pos.p; x = Pos.x; y = Pos.y; z = Pos.z;  HalfViewAngle = Pos.HalfViewAngle;
		return *this; 
	}

 	cPositionMFD &operator=(const cPosition& Pos) { p = Pos.p; x = Pos.x; y = Pos.y; z = Pos.z; return *this; }
};
*/
