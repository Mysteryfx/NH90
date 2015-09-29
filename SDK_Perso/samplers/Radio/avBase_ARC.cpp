#include "StdAfx.h"

#include "Avionics/Radio/avBase_ARC.h"
#include "Physic/wRadioReceiver.h"

using namespace cockpit;

avBaseARC::avBaseARC(ADF_Range adf_range)
	:  ADF_(adf_range),
	elec_consumer_DC(EagleFM::Elec::DC_26_VOLT),
	elec_consumer_AC(EagleFM::Elec::AC_26_VOLT),
	adf_damaged_(false)
{

}

avBaseARC::~avBaseARC()
{

}

void avBaseARC::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avDevice::initialize(ID, Name, script_name);

	avBreakable::load_from_state(ensured_Lua());
	avBreakable::register_as_breakable();

	Lua::Config config(ensured_Lua());

	receiver_.initialize(Name.c_str(), config);
	ADF_.initialize(receiver_.getReceiver(), config);

	if(config.open_raw("signal_strength"))
	{
		config.pop();
		signal_strength_.l_read(config, "signal_strength");
	}

	receiver_.setSquelchOnOff(false);	

	adf_bearing_ = 
	loop_ant_bearing_ = 0.0f;
	adf_damaged_  = false;

	make_default_activity();
}

void avBaseARC::post_initialize()
{
	avDevice::post_initialize();

	receiver_.postInitialize();

	connect_sound();
	electric_commutation();

	elec_consumer_DC.m_signalPowerOn += 
		lwl::delegate(this,&avBaseARC::on_power_on);
	elec_consumer_DC.m_signalPowerOff +=
		lwl::delegate(this,&avBaseARC::on_power_off);
}

void avBaseARC::electric_commutation()
{
	/************************************************************************/
	/* Base commutation
	
	DC => main_elec_sw => [elec_consumer_DC]
			||
		   \  /
		    \/
	AC => AC_relay_ => [elec_consumer_AC] (power for synchro excitation)
	
	*/
	/************************************************************************/

	main_elec_sw >> elec_consumer_DC;
	AC_relay_ >> elec_consumer_AC;
	AC_relay_.SetControlWire(&main_elec_sw);
}

void avBaseARC::update()
{
	if (getElecPower())
	{
		receiver_.update();

		if (mode_ == ARC_ADF || mode_ == ARC_LOOP)
		{
			double dt = p_device_timer->GetDeltaTime();

			if(!adf_damaged_) ADF_.update(dt);

			double trueBearing = ADF_.getBearing();

			adf_bearing_ = ADF_.isSignalReceived() ? adf_bearing_ = RAD_TO_2PI(trueBearing) : 0.0f;

			simulate_loop_antenna(dt);

			loop_gain_.setGain(get_loop_signal_volume());
		}
	}

	signal_strength_.simulate(receiver_.getSignalLF(),p_device_timer->GetDeltaTime());
}

void avBaseARC::release()
{
	ADF_.release();
	receiver_.release();
}

void avBaseARC::setElecPower(bool flag_in)
{
	main_elec_sw.setOnOff(flag_in);
}

void avBaseARC::set_mode(unsigned char modeIn) 
{
	mode_ = modeIn;

	main_elec_sw.setOnOff(mode_ > ARC_OFF);

	if(modeIn == ARC_ANTENNA || modeIn == ARC_OFF)
	{
		loop_ant_bearing_ =
		adf_bearing_ = 0.0f;
	}

	loop_gain_.setGain(get_loop_signal_volume());
}

void avBaseARC::update_frequency() 
{
	float freq = get_set_frequency();

	receiver_.setFrequency(freq);

	on_frequency_changed();
}

float avBaseARC::get_loop_signal_volume() const
{
	if(mode_ == ARC_ANTENNA)
		return 1.0f;

	return (1.0f - cos(Pi * sin(loop_ant_bearing_ - adf_bearing_)))/2.0f; 
}

void avBaseARC::on_frequency_changed()
{
	ED_DEBUG("ARC %s FREQUENCY = %f",receiver_.getReceiver()->getName(),get_set_frequency());
}

void avBaseARC::on_power_on(const lwl::CSignalInfo* pInfo)
{
	receiver_.setOnOff(true);
}

void avBaseARC::on_power_off(const lwl::CSignalInfo* pInfo)
{
	loop_ant_bearing_ = 
	adf_bearing_ = 0.0f;
	receiver_.setOnOff(false);
}

void avBaseARC::set_volume(float gain)
{
	main_regulator_.setGain(gain);
}

void avBaseARC::connect_sound()
{
	receiver_.getSoundElement() >> loop_gain_ >> main_regulator_; 
}

/////////////////////////////////////////////////////////////////////////
//Breakable
bool avBaseARC::set_damage(unsigned Failure_ID, const bool can_be_rejected )
{
	//no power - no break
	if(can_be_rejected && !getElecPower())
		return false;

	switch(Failure_ID)
	{
	case ARC_TOTAL_FAILURE:
		if(!getElecPower())
			return false;
		elec_consumer_DC.setFailure(true);
		break;
	case ARC_ADF_DAMAGE:
		if(!getElecPower())
			return false;
		adf_damaged_ = true;
		break;
	case ARC_ANT_DAMAGE:
		receiver_.damageAntenna(true);
		break;
	default:
		return false;
	}

	return avBreakable::set_damage(Failure_ID);
}

void avBaseARC::repair()
{
	elec_consumer_DC.setFailure(false);
	receiver_.damageAntenna(false);
	adf_damaged_ = false;

	avBreakable::repair();
}