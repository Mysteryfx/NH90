#ifndef __GearPostAD_h__
#define __GearPostAD_h__

#include "Base.h"
#include "FMAerodynamics/AerodynamicBody.h"

namespace EagleFM
{

class FMBASE_API GearPostAD : public AerodynamicBody
{
public:
	GearPostAD();
	GearPostAD(double area, double cx = 0.2);
	virtual void calcAerodynamics(double *Arguments, double SpeedVim, double Mach, double AngleOfAttack, double AngleOfSlip, Vec3 RotateSpeed_l, Vec3 *ADForce_l, Vec3 *ADForce_pos_l, Vec3 *OwnADDamperMoment_l, double *ShakeAmplitude, double *ShakeFrequency);

	void	setCxa(double cx) { cxa = cx; }
	double	getCxa() const { return cxa; }

protected:
	double cxa;
};

}


#endif