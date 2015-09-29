#pragma once

#include "Base.h"
#include "FMMathematics/math.h"

namespace EagleFM
{

class IPitotStatic;

class FMBASE_API BarometricAltimeter
{
public:
	BarometricAltimeter(const IPitotStatic* const);

	void		setPito(const IPitotStatic *pP) { if(pP)pPito = pP; }

	void		simulate(double dt);

	void		setBasicAtmoPressureHg(double PressureHg);

    
	double		getBarometricAltitude()		const { return Altitude; }
	double		getBasicAtmoPressureHg()	const; 

private:
	const IPitotStatic		*pPito;

	double		Altitude;
	double		BasicAtmoPressure;
};

}