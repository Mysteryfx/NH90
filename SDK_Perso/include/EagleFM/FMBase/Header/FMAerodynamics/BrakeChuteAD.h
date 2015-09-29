#ifndef __BrakeChuteAD_h__
#define __BrakeChuteAD_h__

#include "Base.h"
#include "FMAerodynamics/AerodynamicBody.h"

namespace EagleFM
{

class FMBASE_API BrakeChuteAD : public AerodynamicBody
{
public:
	BrakeChuteAD(double area = 50);
	virtual void calcAerodynamics(double* args, double speedVim, double mach, double aoa, double aos, Vec3 rotSpeed_l, Vec3* adforce_l, Vec3* adforce_pos_l, Vec3* ownADDamperMoment_l, double* shakeAmpl, double* shakeFreq);
};

}


#endif