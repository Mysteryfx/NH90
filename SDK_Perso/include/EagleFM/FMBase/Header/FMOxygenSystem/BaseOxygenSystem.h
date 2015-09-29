#ifndef BASE_OXY_SYS_H 
#define BASE_OXY_SYS_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Base.h"
#include "Optimizable.h"

//#include "FMSensorsSystem/Sensor.h"

namespace EagleFM {

class FMBASE_API BaseOxygenSystem :  
	public Optimizable
{

public:
	virtual ~BaseOxygenSystem();

	virtual void initAirStart() {}
	virtual void initHotStart() {}
	virtual void initColdStart() {}

	virtual void doSimulate() = 0; 

private:
	virtual float getOxygenConsumption(float altitude) const = 0;

};

}

#endif