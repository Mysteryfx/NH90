#pragma once

#include "Consumer.h"

namespace EagleFM { namespace Elec {

class FMBASE_API LampParams
{
public:
	LampParams();
	LampParams(double brightnessMinValIn);

	void loadFromState(Lua::Config& config, const char* params);

	double brightnessMinVal;
};


class FMBASE_API Lamp : public Consumer
{
public:
	Lamp(double nominalVol);
	Lamp(double nominalVol, double impedance);
	Lamp(double nominalVol, const char* name);
	Lamp(double nominalVol, double impedance, const char* name);
	virtual ~Lamp();

	virtual void updateOwnVoltage(double valIn = -1.0);

	void setBrightnessRegulator(double valIn);
	double getBrightnessRegulatorValue() const;
	void setBrightnessMinVal(double valIn);
	
	double getLightness() const;
	
	void setLampParams(double brightnessMinValIn);
	void setLampParams(const LampParams& params);
	void setLampParamsFromState(Lua::Config& config, const char* params);

protected:
	void updateLightness();
	void init();

private:
	double brightnessRegulator;
	double brightnessMinVal;
	double brightnessMultiplier;
	
	double lightness;
};

} }
