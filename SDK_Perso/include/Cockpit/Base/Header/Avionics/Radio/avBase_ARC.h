#ifndef AV_BASE_ARC_H
#define AV_BASE_ARC_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "Avionics/avBasicElectric.h"

#include "Avionics/Radio/avReceiver.h"
#include "Avionics/Radio/avADF.h"

#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"
#include "FMElectricSystems/Items/relay.h"

#include "FMMathematics/math.h"
#include "Utilities/ccTriggerSystem.h"

namespace cockpit {

class COCKPITBASE_API avBaseARC : public avDevice, 
								  public avBreakable
{
protected:

	enum ARC_MODE
	{
		ARC_OFF,
		ARC_ADF,
		ARC_ANTENNA,
		ARC_LOOP
	};

	enum ARC_failures
	{
		ARC_TOTAL_FAILURE,
		ARC_ADF_DAMAGE,
		ARC_ANT_DAMAGE
	};

	avBaseARC(ADF_Range adf_range);

public:
	virtual ~avBaseARC();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void update();
	virtual void release();
	//end of interface	

	//avBasicElectic interface
	virtual void	setElecPower(bool flag_in);
	virtual bool	getElecPower() const { return elec_consumer_DC.isPowered(); }
	//end of interface

	//avBreakable interface
	virtual bool	set_damage(unsigned Failure_ID, const bool can_be_rejected = false);
	virtual void	repair();
	//end of interface

	double get_signal_strength() const { return signal_strength_.get_value_out(); }
	double get_magnetic_bearing() const { return loop_ant_bearing_; }
	bool   is_bearing_valid() const;

	Sounder::Element & getSoundElement() { return main_regulator_; }

protected:

	virtual void set_mode(unsigned char modeIn);

	/* returns frequency which set on a control panel*/
	virtual int get_set_frequency() const = 0;

	/*set frequency by controls position, with change event call*/
	void		  update_frequency(); 

	/*returns ADF bearing*/
	virtual float get_adf_bearing() const { return adf_bearing_; }

	virtual float get_loop_signal_volume() const;

	/* frequency change event*/
	virtual void	on_frequency_changed();

	virtual void	simulate_loop_antenna(double dt){}
	virtual void	simulate_signal_strength(double dt){}

	void			set_volume(float gain);

	virtual void	connect_sound(); /*connects sound wires*/ 

	/* electric handlers */
	virtual void	on_power_on(const lwl::CSignalInfo* pInfo);
	virtual void	on_power_off(const lwl::CSignalInfo* pInfo);

	virtual void	electric_commutation();

	EagleFM::Elec::Switch	main_elec_sw;
	EagleFM::Elec::Relay	AC_relay_;
	EagleFM::Elec::Consumer elec_consumer_DC;
	EagleFM::Elec::Consumer elec_consumer_AC;

	/*ADF*/
	avReceiver		receiver_;
	avADF			ADF_;
	gauge_limited	signal_strength_;
	float			adf_bearing_;

	unsigned char	mode_;

	float				loop_ant_bearing_;
	Sounder::Regulator	loop_gain_;
	Sounder::Regulator	main_regulator_;

	Math::IIR_Filter	bearing_filter;

	/* failures */
	bool adf_damaged_;

	triggers::double_lambda_proxy trigger_adf_freq;
};

}

#endif
