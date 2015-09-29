#pragma once

#include "Avionics/Sensors/avGyroPowerSource.h"

namespace cockpit {

class avBaseGyro
{
public:

	avBaseGyro(GyroPowerSource* power_source);

	virtual void initialize(Lua::Config& config);
	virtual void simulate(double dt, bool NoDynamicUpd = false);
	//////////////////////////////////////////////////////////////////////////

	double getRotRate() const {return RotRate;}
	double getNominalRotRate() const {return NominalRotRate;}
	double getThresholdRotRate() const {return ThresholdRotRate;}

	bool getIsGyroSpinning() const {return gyro_spinning_;}
	bool getSpinUp() const { return spin_up_; }

	GyroPowerSource* const get_power_source() const { return power_source_.get();}

	void set_power_source(GyroPowerSource* power_source)
	{
		power_source_.reset(power_source);
	}

protected:

	//Rotation rate, rad/sec
	double RotRate;

	// Nominal rotation rate, rad/sec
	double NominalRotRate;

	// Spinup rate, time constant
	double SpinUpT;

	// Slowdown rate, time constant
	double SlowDownT;

	// Threshold spinning velocity
	double ThresholdRotRate;

	bool gyro_spinning_; /*is gyro rotating or not*/
	bool spin_up_; /*gyro rate increasing*/

	GyroPowerSrcPtr power_source_;
};

}