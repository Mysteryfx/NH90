#ifndef AGB_3K_H 
#define AGB_3K_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "CockpitBase.h"

#include "Avionics/avBasicSAI.h"
#include "ICommandsCockpitDevice.h"
#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"

using namespace EagleFM;
using namespace EagleFM::Elec;

namespace cockpit { 

class COCKPITBASE_API avAGB_3K : public avBasicSAI
{
	typedef enum AGB_CageLeverPositions
	{
		AGR_LevP_NEUTRAL,
		AGR_LevP_CAGE
	};

public:
	avAGB_3K();
	virtual ~avAGB_3K();
	
	//avDevice interface
	virtual void post_initialize();
	virtual void update();
	virtual void release();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer) {}
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//end of interface

	virtual void setElecPower(bool flag_in) {ElecSwitchDevicePower.setOnOff(flag_in);}
	virtual bool getElecPower() const {return ElecConDevice.isPowered();}

	void	turn_off_correction(bool OnOff);

	void	connect_electric(EagleFM::Elec::ItemBase& wire);

protected:
	bool correction_off_;
};

} 

#endif