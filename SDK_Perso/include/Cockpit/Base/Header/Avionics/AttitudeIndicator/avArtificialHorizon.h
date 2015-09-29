#pragma once

#include "Randomizer/RandomObject.h"
#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "Avionics/Sensors/avRollPitchGyro.h"

#include "Utilities/MathDefs.h"

namespace EagleFM
{
	class DynamicBody;
}

namespace cockpit { 

class GyroSound;

class COCKPITBASE_API avArtificialHorizon : public avDevice,
													 public avBreakable
{
protected:
	enum HorizonFailures
	{
		HORIZON_BAR_NOT_SETTLE = 0,
	};

	avArtificialHorizon(GyroPowerSource* pwr_src,GyroSound* gyro_sound);

public:
	virtual ~avArtificialHorizon();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void update();
	//end of interface

	//avBreakable interface
	virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
	virtual void repair();
	//end of interface

	virtual double get_pitch() const {return IndicatorPitch;}
	double get_pitch_delta() const {return IndicatorPitchDelta;}
	void   set_pitch_delta(double delta) {IndicatorPitchDelta = delta;}
	virtual double get_bank() const {return IndicatorBank;}

	void set_power(float pressure /*in. Hg*/);

protected:

	void initSound();
	virtual Sound::Host* initSoundHost(); 

    double IndicatorPitch;
	double ManualPitchDelta;    // pitch delta of A/C symbol
	bool NoDynamicUpd;

	EagleFM::DynamicBody* pDBody;
	avRollPitchGyro gyro;

	///////////////////////////////////////////////
	// Variables

	//servo frame bank
	double TFrameBank;
	//pitch and bank indicators
	double PitchTransducer, IndicatorPitchDelta, IndicatorBank;
	//gyro unit bank from cardan frame sensor
	double GyroBankOGFrame;

	// time constants
	double T_Pitch, T_Bank;

	GyroSound* gyro_sound_; 
};

} 