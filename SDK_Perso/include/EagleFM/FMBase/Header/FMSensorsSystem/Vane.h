#pragma once

#include "Base.h"
#include "FMMathematics/math.h"

namespace EagleFM
{

class DynamicBody;

class FMBASE_API Vane
{
public:
	Vane(DynamicBody*);

	void			setRoll(float Roll);

	void			setIF(double _IF) { IF = __max(0.0, __min(_IF, 1.0)); }
	void			setVind_l(Vec3 Vind) { Vind_l = Vind; }
	void			setDeicerRate(double Rate) { DeicerRate = __max(0.0, Rate); }
	
	void			simulate(double dt);

	double			getIF()				const { return IF; }
	double			getAngle()			const { return Angle; } //Знак определяется по тангажу флюгарки в своей СК: концы вниз - угол положительный (но угол атаки отр.!)
	double			getRelativeAngle()	const { return 	Angle > 0.0 ? Angle/MaxAngle : -Angle/MinAngle; } 


	Vec3			Pos_l;			

	double			MaxAngle;
	double			MinAngle;

	Vec3			SurfacePoint_w;		//точка на поверхности под ЛА (для коррекции ветра)
	Vec3			SurfaceNormal_w;	//нормаль к точке на поверхности


private:
	DynamicBody		*pDBody;

	double			Angle;
	double			Omega;

	Vec3			Vind_l;
	double			IF;		

	float			SinRoll;
	float			CosRoll;

	//лед
	double			IceLayerThickness;	//толщина льда на диске
	double			DeicerRate;			//скорость таянья льда от искуственного подогрева
};


}