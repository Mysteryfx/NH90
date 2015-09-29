#pragma once

#include "Base.h"

#include "Sensor.h"

namespace EagleFM
{

class FMBASE_API SensorsSystem
{
public:
	virtual ~SensorsSystem() {}


	virtual double	getSensorOutput(int)	const	{ return 0; }

	virtual double	getVaneAoA_Value()		const	{ return 0.0; }
	virtual double	getVaneAoS_Value()		const	{ return 0.0; }

    virtual void    setAtmoPressureHg(double pressure_hg) = 0;
    virtual double  getAtmoPressureHg() const = 0;
};


}
