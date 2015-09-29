#include "StdAfx.h"

#include "Avionics/Radio/avVHF_SCR_522A.h"
#include "Avionics/Radio/avReceiver.h"

#include "ccSound.h"

#include "IDispatcher.h"

#include "Utilities/ccUtilities.h"
#include "Utilities/ccTriggerSystem.h"

using namespace cockpit;

static const float CHANNEL_QTY = 4;

avVHF_SCR_522A::avVHF_SCR_522A() : avBaseRadio(CHANNEL),
	lights_(SCR_522_Lights_MAX, EagleFM::Elec::Lamp(28.0))
{
}

avVHF_SCR_522A::~avVHF_SCR_522A()
{

}

void avVHF_SCR_522A::initialize(unsigned char ID, const ed::string& name, const ed::string& script_name)
{
	avBaseRadio::initialize(ID,name,script_name);
	communicator->setModulation(MODULATION_AM);

	Lua::Config config(ensured_Lua());

	functional_mode_ = SCR_522A_R;
	intercom_delegate_ = NULL;
	ctl_box_shorted_ = false;
	rcv_burned_out_ = false;

	preset_channel_ = -1;
	presetChannels_.resize(CHANNEL_QTY);

	set_channel_frequency(105000000.f,0);
	set_channel_frequency(139000000.f,1);
	set_channel_frequency(124000000.f,2);
	set_channel_frequency(131000000.f,3);
		
	elec_consumer.setParamsFromState(config, "ElecConsumerParams");
	
	locked_ = false;
	mic_button_pressed_ = false;
	
	set_tuned_frequency(presetChannels_[0]);
	set_squelch(true);

	load_presets();
}

void avVHF_SCR_522A::perform_init_state()
{
	trigger_frequency = triggers::get_parameter_handle("VHF_AM_FREQ");

	initSound();

	performClickableAction(C_SCR_522_R_REM,1.0f);
	performClickableAction(C_SCR_522_DIM,0.0f);

	main_elec_sw >> lights_relay_ >> lights_switch_ >> lights_null_;
	lights_switch_ >> lights_[SCR_522_A_Light];
	lights_switch_ >> lights_[SCR_522_B_Light];
	lights_switch_ >> lights_[SCR_522_C_Light];
	lights_switch_ >> lights_[SCR_522_D_Light];

	lights_relay_ >> receiving_switch_ >> lights_[SCR_522_TRANS];
	
	lights_relay_.SetControlWire(&elec_consumer);

	if (aircraft().init_conditions.cpt_init_condition == CPT_INCON_COLD_GROUND)
	{
		performClickableAction(C_SCR_522_OFF,1.0); //Off station
	}
	else
	{
		performClickableAction(C_SCR_522_A,1.0);
	}

	performClickableAction(C_SCR_522_VOLUME,1.0f);
}

void avVHF_SCR_522A::initSound()
{
	if (!ccSound::instance().getHost("CPT_RADIO_ON_OFF").is_created())
	{
		Sound::HostParams h_params;
		if (ccSound::instance().getSoundPos("pnt_117", h_params))
			h_params.fields = SND_POSITION;

		ccSound::instance().createHost(Sound::COCKPIT_CONTEXT, "CPT_RADIO_ON_OFF", &h_params);
	}

	Sound::Host& host = ccSound::instance().getHost("CPT_RADIO_ON_OFF");
	btn_sound_.create(host,"Aircrafts/P-51D/Cockpit/RadioButton");
}

void avVHF_SCR_522A::release()
{
	btn_sound_.destroy();
	avBaseRadio::release();
}

void avVHF_SCR_522A::SetCommand(int command, float value /* = 0 */)
{
	switch(command)
	{
	case C_SCR_522_OFF_EXT:
		button_value_iterate(C_SCR_522_OFF);
		break;
	case C_SCR_522_OFF:
		setElecPower(value < 0.5);
		break;
	case C_SCR_522_A_EXT:
		button_value_iterate(C_SCR_522_A);
		break;
	case C_SCR_522_A:
		if (value > 0.5)
		{
			lights_switch_.setPos((preset_channel_ = 0) + 1);
			update_frequency();
		}
		else
		{
			lights_switch_.setPos((preset_channel_ = -1) + 1);
		}
		break;
	case C_SCR_522_B_EXT:
		button_value_iterate(C_SCR_522_B);
		break;
	case C_SCR_522_B:
		if (value > 0.5)
		{
			lights_switch_.setPos((preset_channel_ = 1) + 1);
			update_frequency();
		}
		else
		{
			lights_switch_.setPos((preset_channel_ = -1) + 1);
		}
		break;
	case C_SCR_522_C_EXT:
		button_value_iterate(C_SCR_522_C);
		break;
	case C_SCR_522_C:
		if (value > 0.5)
		{
			lights_switch_.setPos((preset_channel_ = 2) + 1);
			update_frequency();
		}
		else
		{
			lights_switch_.setPos((preset_channel_ = -1) + 1);
		}
		break;
	case C_SCR_522_D_EXT:
		button_value_iterate(C_SCR_522_D);
		break;
	case C_SCR_522_D:
		if (value > 0.5)
		{
			lights_switch_.setPos((preset_channel_ = 3) + 1);
			update_frequency();
		}
		else
		{
			lights_switch_.setPos((preset_channel_ = -1) + 1);
		}
		break;
	case C_SCR_522_DIM_EXT:
		axis_value_iterate(C_SCR_522_DIM,value > 0 ? 1 : -1);
		break;
	case C_SCR_522_DIM:
		break;
	case C_SCR_522_R_REM:
		if (value > 0.5)
		{
			onFunctionalModeChanging(SCR_522A_REM);
		}
		else
		{
			onFunctionalModeChanging(SCR_522A_R);
		}
		break;
	case C_SCR_522_T:
		if (value  < -0.5f)
		{
			onFunctionalModeChanging(SCR_522A_T);
		}
		else
		{
			onFunctionalModeChanging(SCR_522A_R);
		}
		break;
	case C_SCR_522_VOLUME_EXT:
		axis_value_iterate(C_SCR_522_VOLUME, value > 0.0 ? 1.0 : -1.0);
		break;
	case C_SCR_522_VOLUME:
		set_volume(value);
		break;
	case C_SCR_522_LOCKING_EXT:
		switcher_value_iterate(C_SCR_522_LOCKING);
		break;
	case C_SCR_522_LOCKING:
		break;
	case C_SCR_522_VOLUME_AXIS:
		performClickableAction(C_SCR_522_VOLUME,1.0f - (value + 1.0f) / 2.0f);
		break;
	default:
		break;
	}
}

bool avVHF_SCR_522A::checkAnimationConditions(int arg_num, int command, float& value)
{
	switch(command)
	{
	case C_SCR_522_A:
	case C_SCR_522_B:
	case C_SCR_522_C:
	case C_SCR_522_D:
	case C_SCR_522_OFF:
		if (value > 0.5)
		{
			if (preset_channel_ == (command - C_SCR_522_OFF - 1))
				return false;
			btn_sound_.play_once();
			performClickableAction(C_SCR_522_OFF + (preset_channel_ + 1),0.0f);
		}
		break;
	case C_SCR_522_T:
		if ((!locked_ && value > -0.5))
		{
			return false;
		}
		else if(functional_mode_ == SCR_522A_T && value < -0.5)
		{
			value = 0.0f;
		}
		return	true;
	case C_SCR_522_R_REM:
		return !(locked_  && value > 0.5) 
			&& !(functional_mode_ == SCR_522A_REM && value > 0.5) 
			&& !(functional_mode_ == SCR_522A_R && value <= 0.5);
	case C_SCR_522_LOCKING:
		locked_ = value > 0.5;
		if (value > 0.5f)
		{
			if (functional_mode_ == SCR_522A_REM)
			{
				locked_ = false;
				return false;
			}
			else if(functional_mode_ == SCR_522A_T)
			{
				performClickableAction(C_SCR_522_T,0.0f);
			}
		}
		break;
	case C_SCR_522_T_STATIC:
		performClickableAction(C_SCR_522_T,value);
		return false;
	case C_SCR_522_R_REM_STATIC:
		performClickableAction(C_SCR_522_R_REM,value);
		return false;
	default:
		break;
	}
	return true;
}

double avVHF_SCR_522A::get_light(unsigned int light_numer) const
{
	assert(light_numer >= 0 && light_numer <= SCR_522_Lights_MAX);
	if (light_numer >= 0 && light_numer <= SCR_522_Lights_MAX )
		return lights_[light_numer].getLightness();
	
	return 0.0;
}

float avVHF_SCR_522A::get_set_frequency() const
{
	return get_preset_frequency();
}

void avVHF_SCR_522A::onFunctionalModeChanging(int newMode)
{
	functional_mode_ = newMode;

	if (getElecPower())
		update_communicator_state();

	if (newMode == SCR_522A_T)
	{
		receiving_switch_.setOnOff(false);
		if(intercom_delegate_)
			intercom_delegate_->start_transmission(true,false);
	}
	else if(newMode == SCR_522A_R)
	{
		receiving_switch_.setOnOff(true);
		if(intercom_delegate_)
			intercom_delegate_->start_transmission(false,false);
	}
	else
	{
		receiving_switch_.setOnOff(!mic_button_pressed_);
		if(intercom_delegate_)
			intercom_delegate_->start_transmission(mic_button_pressed_,true);
	}

	//failure
	if(ctl_box_shorted_)
		receiving_switch_.setOnOff(true);
}

void avVHF_SCR_522A::update_communicator_state()
{
	if (ctl_box_shorted_)
	{
		communicator->setReceiverOnOff(!receiver_failure());
		return;
	}

	switch(functional_mode_)
	{
	case SCR_522A_T:
		communicator->setReceiverOnOff(false);
		communicator->setTransmitterOnOff(true);
		break;
	case SCR_522A_R:
		communicator->setReceiverOnOff(!receiver_failure());
		communicator->setTransmitterOnOff(false);
		break;
	case SCR_522A_REM:
		communicator->setReceiverOnOff(!receiver_failure());
		communicator->setTransmitterOnOff(true);//mic_button_pressed_);
		break;
	default:
		break;
	}
}

void avVHF_SCR_522A::connect_electric(EagleFM::Elec::Wire& power_wire)
{
	power_wire >> main_elec_sw;
}

void avVHF_SCR_522A::checkRadioDevices()
{
	avBaseRadio::checkRadioDevices();
	if(communicator->isOn())
		update_communicator_state();
}

void avVHF_SCR_522A::mic_button_pressed(bool pressed)
{
	mic_button_pressed_ = pressed;

	if (functional_mode_ == SCR_522A_REM)
	{
		//communicator.setTransmitterOnOff(mic_button_pressed_);
		receiving_switch_.setOnOff(!pressed);
		if (intercom_delegate_)
			intercom_delegate_->start_transmission(pressed,true);
	}
}

void avVHF_SCR_522A::load_presets()
{
	if (!aircraft().human_ptr)
		return;

	Lua::Config config(globalDispatcher->mission());

	if (config.open("db.human.SCR522"))
	{
		float frequency = 100000000.0f;
		config.get("ButtonA", &frequency);
		presetChannels_[0] = frequency * 1000000.f;
		config.get("ButtonD", &frequency);
		presetChannels_[3] = frequency * 1000000.f;
		config.get("ButtonB", &frequency);
		presetChannels_[1] = frequency * 1000000.f;
		config.get("ButtonC", &frequency);
		presetChannels_[2] = frequency * 1000000.f;
		config.pop();
	}	
}

void avVHF_SCR_522A::setKnobsByFrequency(float frequency)
{
	for (int i = 0; i < presetChannels_.size(); i++)
	{
		if ((int)presetChannels_[i]/1000000 == (int)frequency/1000000)
			performClickableAction(C_SCR_522_A + (i - 1),1.0f);
	}
}

void avVHF_SCR_522A::setKnobsByChannel(int channel)
{
	performClickableAction(C_SCR_522_OFF + channel,1.0f);
}

void avVHF_SCR_522A::l_register_functions(lua_State *Lua_s,int index)
{
	avBaseRadio::l_register_functions(Lua_s,index);
	lua_pushstring(Lua_s, "set_channel");		lua_pushcfunction(Lua_s, l_set_channel);	lua_settable  (Lua_s, index);
	lua_pushstring(Lua_s, "get_channel");		lua_pushcfunction(Lua_s, l_get_channel);	lua_settable  (Lua_s, index);
	lua_pushstring(Lua_s, "is_on");				lua_pushcfunction(Lua_s, l_is_on);			lua_settable  (Lua_s, index);
}

int avVHF_SCR_522A::l_set_channel(lua_State *Lua_s)
{
	avVHF_SCR_522A  *device = l_get_device_from_meta<avVHF_SCR_522A>(Lua_s);
	assert(lua_isnumber(Lua_s, 2));

	int channel = lua_tonumber(Lua_s, 2);
	device->setKnobsByChannel(channel + 1);
	return 0;
}


int avVHF_SCR_522A::l_get_channel(lua_State *Lua_s)
{
	avVHF_SCR_522A  *device = l_get_device_from_meta<avVHF_SCR_522A>(Lua_s);
	lua_pushnumber(Lua_s, device->get_preset_channel());
	return 1;
}

int avVHF_SCR_522A::l_is_on(lua_State *Lua_s)
{
	avVHF_SCR_522A  *device = l_get_device_from_meta<avVHF_SCR_522A>(Lua_s);
	lua_pushboolean(Lua_s, device->getCommunicator().isTransmitterOn());
	return 1;
}

bool avVHF_SCR_522A::set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */)
{
	if (can_be_rejected && !getElecPower())
		return false;

	switch (Failure_ID)
	{
	case VHF_CRYSTAL:
		communicator->damageAntenna(true);
		break;
	case VHF_SHORTED_CTL_BOX:
		ctl_box_shorted_ = true;
		communicator->setReceiverOnOff(true);
		communicator->setTransmitterOnOff(false);
		break;
	case VHF_VT_BURNED_OUT:
		rcv_burned_out_ = true;
		communicator->setReceiverOnOff(false);
		break;
	case VHF_VT207_DEFECTIVE:
	case VHF_SQUELCH_RELAY:
		set_squelch(false);
		break;
	default: 
		return false;
	}

	setflag_failure(true);
	return true;
}

void avVHF_SCR_522A::repair()
{
	setflag_failure(false);
	communicator->damageAntenna(false);
	ctl_box_shorted_ = false;
	rcv_burned_out_ = false;
	set_squelch(true);
}