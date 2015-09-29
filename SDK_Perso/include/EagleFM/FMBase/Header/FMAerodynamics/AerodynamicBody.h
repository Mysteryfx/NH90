#ifndef __AerodynamicsBody_h__
#define __AerodynamicsBody_h__

#include "Base.h"
#include "FMMathematics/math.h"

namespace EagleFM
{

//Абстрактный класс, описывающий аэродинамическое тело. 
class FMBASE_API AerodynamicBody
{
public:

	AerodynamicBody();
	virtual ~AerodynamicBody();
	
	void setArea(double S) { Area = S; }

	virtual void calcAerodynamics(double *Arguments, double SpeedVim, double Mach, double AoA, double AoS,
		Vec3 RotateSpeed_l_l, Vec3 *ADForce_l_l, Vec3 *ADForce_pos_l_l, Vec3 *OwnADDamperMoment_l_l, 
		double *ShakeAmplitude, double *ShakeFrequency);

	double getArea() const { return Area; }
protected:
	double Area;
};

}

#endif