#ifndef AV_UGR_4K_H
#define AV_UGR_4K_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "CockpitBase.h"
#include "Avionics/avHSI.h"
#include "ICommandsCockpitDevice.h"

#include "FMElectricSystems/Items/Consumer.h"

namespace cockpit {

class COCKPITBASE_API avUGR_4K : public avHSI
{
protected:
	enum UGR_4K_Commands
	{
		CMD_UGR_4K_COURSE_SET = iCommandCockpitDevice_Action_1,
		CMD_UGR_4K_COURSE_SET_EXT,
		CMD_UGR_4K_MAX
	};

public:
	avUGR_4K();
	~avUGR_4K();

	//avDevice
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void SetCommand(int, float value = 0);
	virtual void update();
	//avDevice

	float get_commanded_course() const { return commanded_course_; }
	float get_bearing() const { return bearing_.get_value_out(); }
	
	void connect_electric(EagleFM::Elec::ItemBase& power_wire);

protected:

	virtual float get_gmc_heading() const = 0;
	virtual float get_radio_bearing() const { return 0.0f; }

	float		commanded_course_;
	float		manual_course_;

	gauge_periodic		bearing_;
};

}

#endif