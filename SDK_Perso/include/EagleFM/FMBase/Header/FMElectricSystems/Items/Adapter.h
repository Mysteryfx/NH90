#pragma once

#include "FMElectricSystems/Items/Wire.h"

namespace EagleFM { namespace Elec {

// Adapter (transformer) converts voltage in the same electric circuit
class FMBASE_API Adapter : public Wire
{
public:
	Adapter(double inputV, double outputV);
	virtual ~Adapter();

	virtual double convertVoltage(double voltage);
	
	void setConvertionK(double valIn) {convertionK = valIn;}
	double getConvertionK() const {return convertionK;}

protected:
	double m_outputVoltage;
	double m_inputVoltage;

	double convertionK;
};

} }
