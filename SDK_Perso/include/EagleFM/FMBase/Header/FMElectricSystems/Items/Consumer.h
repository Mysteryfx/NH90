#pragma once

#include "Wire.h"

namespace Lua
{ 
    class Config;
}

namespace EagleFM { namespace Elec {

static const double maxLoadR = 9999999999.9;

class FMBASE_API ConsumerParams
{
public:
	ConsumerParams();
	ConsumerParams(double powerOrCurrentIn, bool toSetPowerIn = true,
		double thresholdOffIn = -1, double thresholdOnIn = -1, 
		double nominalVoltageIn = -1);

	void loadFromState(Lua::Config& config, const char* params);

	double powerOrCurrent;
	bool   toSetPower;
	double thresholdOff;
	double thresholdOn;
	double nominalVoltage;
};

class FMBASE_API Consumer : public Wire 
{
public:
	Consumer(double nominalVol);
	Consumer(double nominalVol, double impedance);
	Consumer(double nominalVol, const char* name);
	Consumer(double nominalVol, double impedance, const char* name);
	virtual ~Consumer();

	virtual void updateOwnVoltage(double valIn = -1.0);
	
	virtual void setLoadR(double load);
	virtual void setLoadsR(double load);
	void setLoadsR_ByConsumedCurrent(double valIn);
	void setLoadsR_ByConsumedPwr(double valIn);

	void setParams(double powerOrCurrent,
		bool toSetPower = true, double thresholdOff = -1.0, double thresholdOn = -1.0,
		double nominalVoltage = -1.0);

	void setParams(const ConsumerParams& params);
	void setParamsFromState(Lua::Config& config, const char* params);

	double getNominalPower() const {return mNominalPower;}

protected:
	void initWithNominalVoltage(double nominalVol);
	void initWithNominalVoltageAndName(double nominalVol, const char* name);

private:
	double mNominalPower;
};

} }
