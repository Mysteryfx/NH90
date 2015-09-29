#pragma once

#include "Wire.h"


namespace EagleFM { namespace Elec {

class FMBASE_API VoltageRegulator : public Wire
{
public:
	VoltageRegulator(double _voltageIn, double _voltageOut, double _gain);
	virtual ~VoltageRegulator();

private:
	virtual double convertVoltage(double voltage);

	double voltageIn,
		voltageOut,
		gain;
};

} }
