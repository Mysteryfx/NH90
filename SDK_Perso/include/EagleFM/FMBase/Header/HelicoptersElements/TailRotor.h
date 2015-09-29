#ifndef __TailRotor_h__
#define __TailRotor_h__

#include "Base.h"
#include "FMMathematics/math.h"
#include "Optimizable.h"

namespace EagleFM
{

class DynamicBody;
class AerodynamicBody;
class RigidBody;

//        !!!!!!!!!!!! UNDER CONSTRUCTION !!!!!!!!!!
class FMBASE_API TailRotor : public Optimizable
{
	class Blade
	{
	public:
		void calcForce(double omegd) {}
	};

// ----------------------------- МЕТОДЫ ---------------------------------------
public:
	// Создание и инициализация
	TailRotor(DynamicBody*, AerodynamicBody*, RigidBody*);
	virtual ~TailRotor();

	void			createBlades(int num);


	double			getBetaX() const { return betaX; }
	double			getBetaZ() const { return betaZ; }


protected:
	virtual void	doSimulate();
public:

	double			omega_l;

	double			pitch;

protected:

	double			betaX;
	double			betaZ;

	double			dbetaX;
	double			dbetaZ;

	Vec3			bladeForce_l[3];
	Vec3			bladeMoment_l[3];
	double			elementR[5];


	Vec3			force_l;
	Vec3			moment_l;

	DynamicBody		*pDBody;
	AerodynamicBody	*pADBody;
	RigidBody		*pRBody;
};

}

#endif