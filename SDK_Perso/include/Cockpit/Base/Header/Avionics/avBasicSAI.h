#pragma once

#include "Randomizer/RandomObject.h"
#include "Avionics/avADI.h"
#include "Avionics/Sensors/avRollPitchGyro.h"
#include "ICommandsCockpitDevice.h"
#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"


#include "Utilities/MathDefs.h"
#include "WorldSound.h"

namespace EagleFM
{
	class DynamicBody;
}

namespace cockpit { 

class GyroPowerSource;


// Basic class for Standby Attitude Indicator (SAI)
class COCKPITBASE_API avBasicSAI : public avADI
{
protected:
	typedef enum SAI_clk_commands
	{
		CMD_SAI_TEST = iCommandCockpitDevice_Action_1,
		CMD_SAI_CAGE,
		CMD_SAI_PITCH_ZERO_SHIFT,
		CMD_SAI_POWER,
		CMD_SAI_PITCH_EXT,
		CMD_SAI_POWER_EXT,
	};

	typedef enum SAI_TestCageLeverPositions
	{
		SAI_LevP_NEUTRAL,
		SAI_LEVP_TEST,
		SAI_LevP_CAGE
	};

public:
	
	virtual ~avBasicSAI();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
	//end of interface

	// тангаж с датчика угла карданной рамы
	virtual double get_pitch() const {return IndicatorPitch;}
	double get_pitch_delta() const {return IndicatorPitchDelta;}
	void   set_pitch_delta(double delta) {IndicatorPitchDelta = delta;}
	virtual double get_bank() const {return IndicatorBank;}
	double getAttFailure_flag() const {return AttFailure_flag.get_value_out();}

	virtual bool getElecPower() const {return ElecConDevice.isPowered();}

	double getLongitudinalDeviationBarPos() const {return LongitudinalDeviation.get_value_out();}
	double getLateralDeviationBarPos() const {return LateralDeviation.get_value_out();}
	
protected:

	avBasicSAI(GyroPowerSource* power_source);
	void initSound();
	virtual Sound::Host* initSoundHost(); 

    double IndicatorPitch;
	double ManualPitchDelta;    // pitch delta of A/C symbol
	bool NoDynamicUpd;

	EagleFM::DynamicBody* pDBody;
	avRollPitchGyro gyro;

	gauge_linear AttFailure_flag;

	EagleFM::Elec::Switch	ElecSwitchDevicePower;
	EagleFM::Elec::Consumer	ElecConDevice;

	///////////////////////////////////////////////
	// Variables
	wModelTime PrevUpdateTime;

	// Test is in progress
	bool IsTest;

	// Крен следящей рамы в с.с.к
	double TFrameBank;
	// Указатели тангажа и крена
	double PitchTransducer, IndicatorPitchDelta, IndicatorBank;
	// Крен гироузла с датчика карданной рамы
	double GyroBankOGFrame;

	// Постоянные времени эд. картушки и следящей рамы крена
	double T_Pitch, T_Bank;

	gauge_limited LongitudinalDeviation;
	gauge_limited LateralDeviation;

	SAI_TestCageLeverPositions TestCageLeverPos;
	SAI_TestCageLeverPositions TestCageLeverCmdPos; // Which mouse button was pressed
	wModelTime TestCageLeverPushPullTime;
	static const float SAI_CageLeverRotMaxPhase; // constant
	float SAI_CageLeverRotPhase;
	bool IsProgrammaticLeverRot;

	struct
	{ 
		Sound::Source stby_att_ind;
		float stby_att_ind_freq;
	} sound;
};

} 