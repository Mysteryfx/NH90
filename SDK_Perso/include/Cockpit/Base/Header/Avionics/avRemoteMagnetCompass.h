#pragma once

#include "CockpitBase.h"
#include "Avionics/avMagneticCompass.h"
#include "Avionics/avBasicElectric.h"
#include "FMElectricSystems/Items/Consumer.h"

namespace cockpit
{
class COCKPITBASE_API avRemoteMagnetCompass : public avMagneticCompass,
											  public avBasicElectric
{

protected:

	enum RemoteMagnetCompassFailures
	{
		NO_FAILURE = -1,
		NO_TORQUE = 0,
		ERRATIC_OPERATION,
		POINTER_PULLS,
		ERRATIC_INDIACATON,
	};

	avRemoteMagnetCompass();

public:
	virtual ~avRemoteMagnetCompass();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
	virtual void update();
	//end of interface

	//avBreakable interface
	virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
	virtual void repair();
	//end of interface

	void connect_electric_wire(EagleFM::Elec::ItemBase& wire);
protected:

	void simulate_damage(double dt);
	EagleFM::Elec::Consumer Consumer_;

	//damages
	short int failure_;
	double magnetic_heading_bframe_last_;
		
	unsigned int damage_data_index_;
	struct PullDamageAngles{
		float angle_1_;
		float angle_2_;
		float deviation_;
	};

	struct ErraticDamageAngles{
		float angle_1_;
		float angle_2_;
	};

	ed::vector<PullDamageAngles> pull_damage_data;
	ed::vector<ErraticDamageAngles> erratic_damage_data;

private:
};

}