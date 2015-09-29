#include "stdafx.h"

#include "Avionics/Radio/avVHF_Radio.h"
#include "Avionics/Radio/avRadioGUI.h"
#include "Utilities/ccUtilities.h"
#include "Utilities/ccTriggerSystem.h"
#include "Lua/Config.h"
#include "GlobalConfig.h"
#include "ICommandDialogsPanel.h"

#include "ccSound.h"
#include "WorldSound.h"
#include "Sound/Sounder.h"

#include "Physic/wRadioReceiver.h"
#include "Physic/wRadioTransmitter.h"


using namespace cockpit;

avVHF_Radio::avVHF_Radio() : ElecConDevice(EagleFM::Elec::DC_26_VOLT)
{
	type_AMFM = VHF_RT_AM;

	functional_mode_ = 0;
	selectionDial = 0;

	presetChannel = 1;

	is_load_from_mission = false;

	trigger_vhf_freq = 0;
}

avVHF_Radio::~avVHF_Radio()
{
}

void avVHF_Radio::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avDevice::initialize(ID, Name, script_name);

	avBreakable::load_from_state(ensured_Lua());
	avBreakable::register_as_breakable();

	Lua::Config config(ensured_Lua());
	config.get("radio_type", &type_AMFM, (int)VHF_RT_AM);
	
	config.get("innerNoise_108_116_MHz_coeff", &noise_k);
	config.get("power", &power);
	config.get("innerNoise", &innerNoise);

	communicator.initialize(Name.c_str(), config);
	communicator.setModulation(isAMRadio() ? MODULATION_AM : MODULATION_FM);
	communicator.mute(false);

	make_default_activity();
}

void avVHF_Radio::post_initialize()
{
	communicator.postInitialize();
	
	sound.warningTone.create(&ccSound::instance().getHost("HEADPHONES"), "Aircrafts/Cockpits/VHF_WarningTone");
	communicator.addStaticSoundSource(sound.warningTone);

	if(isAMRadio())
		trigger_vhf_freq = triggers::get_parameter_handle("VHF_AM_FREQ");
	else
		trigger_vhf_freq = triggers::get_parameter_handle("VHF_FM_FREQ");
	
	load_settings();

	ElecConDevice.m_signalPowerOn +=
		lwl::delegate(this,&avVHF_Radio::OnVHF_PowerOn);

	ElecConDevice.m_signalPowerOff +=
		lwl::delegate(this,&avVHF_Radio::OnVHF_PowerOff);

	init_dialog_panel();
}

void avVHF_Radio::init_dialog_panel()
{
	Lua::Config config(ensured_Lua());
	if(config.open("GUI"))
	{
		ICommandDialogsPanel::Radio radio;
		radio.id = this->ID();
		loadRadio(config, radio);
		globalCommandDialogsPanel->loadRadio(radio);
		config.pop();
	}
}

void avVHF_Radio::connectElecPower(Elec::ItemBase& ElecInput)
{
	ElecInput >> Power_Switch_ >> ElecConDevice;
}

void avVHF_Radio::release()
{
	communicator.release();
}

void avVHF_Radio::update()
{
	communicator.update();
	if (!getElecPower() && sound.warningTone.getGain() > 0.9)
		sound.warningTone.update(0.0);
}

void avVHF_Radio::onFrequencyChanged()
{
	float freq = getFrequency();
	
	if((isAMRadio() && freq < VHF_BOUND_FREQ || !isAMRadio() && freq >= VHF_BOUND_FREQ) && sound.warningTone.getGain() < 0.1)
		sound.warningTone.update(1.);

	if((isAMRadio() && freq >= VHF_BOUND_FREQ || !isAMRadio() && freq < VHF_BOUND_FREQ) && sound.warningTone.getGain() > 0.9)
		sound.warningTone.update(0.);

	triggers::update_parameter(trigger_vhf_freq, freq / 1000000.); // MHz

	if(isAMRadio())
		freq = MAX(freq, MIN_VHF_AM_FREQ);
	else
		freq = MIN(freq, MAX_VHF_FM_FREQ);

	if(isAMRadio() && freq >= MIN_VHF_AM_FREQ && freq < MID_VHF_AM_FREQ)
	{
		if(communicator.getTransmitter())
		  communicator.getTransmitter()->setPower(0.0);
		if(communicator.getReceiver())
		  communicator.getReceiver()->setNoiseLevel(innerNoise * noise_k);
	}

	if(isAMRadio() && freq >= MID_VHF_AM_FREQ)
	{
		if(communicator.getTransmitter())
		   communicator.getTransmitter()->setPower(power);

		if(communicator.getReceiver())
		   communicator.getReceiver()->setNoiseLevel(innerNoise);
	}

	communicator.setFrequency(freq);
}

float avVHF_Radio::getPresetFrequency() const
{
	ed::map<int, float>::const_iterator it = presetChannels.find(presetChannel);
	if (it == presetChannels.end())
		return 1;

	return it->second;
}

void avVHF_Radio::onFunctionalModeChanging(int newMode)
{
	functional_mode_ = newMode;
}

void avVHF_Radio::repair()
{
	setflag_failure(false);
	ElecConDevice.setFailure(false);

	if (ElecConDevice.GetVoltage() > 0.0)
	{
		OnVHF_PowerOn(NULL);
	}
	avBreakable::repair();
}

bool avVHF_Radio::set_damage(unsigned Failure_ID, const bool can_be_rejected)
{
	//Если нет электричества, то не ломаемся
	if (can_be_rejected && !getElecPower())
		return false;

	switch (Failure_ID)
	{
	case VHF_RADIO_FAILURE_TOTAL:
		setflag_failure(true);
		communicator.setOnOff(false);
		ElecConDevice.setFailure(true);
		break;
	}

	return true;
}

void avVHF_Radio::save_settings()
{
	if(L)
	{
		lua_newtable(L);
		int index = lua_gettop(L);

		lua_pushstring(L,"dials");
		lua_newtable(L);
		{
			int sub = lua_gettop(L);
			
			lua_pushstring(L,"mode_dial");
			lua_pushnumber(L,functional_mode_);
			lua_settable(L, sub);

			lua_pushstring(L,"channel_dial");
			lua_pushnumber(L,presetChannel);
			lua_settable(L, sub);

			lua_pushstring(L,"selection_dial");
			lua_pushnumber(L,selectionDial);
			lua_settable(L, sub);

			lua_pushstring(L,"manual_frequency");
			lua_pushnumber(L, selected_frequency_);
			lua_settable(L, sub);
			
			lua_settable(L,index);
		}


		lua_pushstring(L,"presets");
		lua_newtable(L);
		{
			ed::map<int, float>::iterator it;

			int k = 0;
			int sub = lua_gettop(L);

			for(it = presetChannels.begin(); presetChannels.end() != it; ++it)
			{
				k++;
				lua_pushnumber(L,k);
				lua_pushnumber(L,it->second);
				lua_settable(L,sub);
			}

			lua_settable(L,index);
		}

		lua_setglobal(L,"settings");
		lua_getglobal(L,"SaveSettings");
		if(isAMRadio())
			lua_pushstring(L,"VHF_AM/");
		else
			lua_pushstring(L,"VHF_FM/");
		if (lua_pcall(L,1,0,0))
		{
			//ED_ERROR("Cockpit:  SaveSettings %s", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
}

void avVHF_Radio::load_settings()
{
	Lua::Config config(L);

	int res = 0;
    
	if(isAMRadio())
	  config.push("VHF_AM/");
	else
      config.push("VHF_FM/");

	if(config.call("LoadSettings",1, 1))
		config.pop(&res);

	if(!res)
		return;

	if(config.open("settings"))
	{
		if (config.open("presets"))
		{

			config.iterate_begin();
			while (config.iterate_next())
			{
				int Index = 0;
				config.get_key(&Index);

				if (Index < 1 ||
					Index > 20)			
					continue;

				float fq;
				if (config.iterate_pop(&fq))
					presetChannels[Index] = fq;
			}
			config.iterate_end();
			config.pop();
		}

		if (config.open("dials"))
		{
			float frequency;
			config.get("mode_dial",     &functional_mode_);
			config.get("selection_dial",     &selectionDial);
			config.get("channel_dial",  &presetChannel);
			config.get("manual_frequency",  &frequency);
			config.pop();
			setSelectedFrequency(frequency);

			is_load_from_mission = true;
		}

	}
}

void avVHF_Radio::setSelectedFrequency(float frequency)
{
	selected_frequency_ = frequency;
}

void avVHF_Radio::OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params) 
{
	if(event == "cockpit_release") {
		// save data to mission file
		Lua::Config conf(globalConfig);

		ed::string mission_mode;
		conf.get("mission_mode", &mission_mode);
		bool prepare = mission_mode == "prepare";

		if (prepare)
			save_settings();
	}
}

void avVHF_Radio::OnVHF_PowerOn(const lwl::CSignalInfo* pInfo)
{	
	if (!isAMRadio())
	{
		onFrequencyChanged();
	}
	switchOnCommunicator(true);
}

void avVHF_Radio::OnVHF_PowerOff(const lwl::CSignalInfo* pInfo)
{	
	switchOnCommunicator(false);
}

void avVHF_Radio::switchOnCommunicator(bool switch_on)
{
	if (switch_on)
	{
		communicator.setOnOff(!getflag_failure());
		return;
	}
	communicator.setOnOff(false);
}

void avVHF_Radio::l_register_functions(lua_State *Lua_s,int index)
{
    avDevice::l_register_functions(Lua_s,index);
	lua_pushstring(Lua_s, "set_frequency");		lua_pushcfunction(Lua_s, l_set_frequency);	lua_settable  (Lua_s, index);
	lua_pushstring(Lua_s, "get_frequency");		lua_pushcfunction(Lua_s, l_get_frequency);	lua_settable  (Lua_s, index);
	lua_pushstring(Lua_s, "is_on");				lua_pushcfunction(Lua_s, l_is_on);			lua_settable  (Lua_s, index);
}

int avVHF_Radio::l_set_frequency(lua_State *Lua_s)
{
	avVHF_Radio  *device = l_get_device_from_meta<avVHF_Radio>(Lua_s);
	assert(lua_isnumber(Lua_s, 2));
	float frequency = lua_tonumber(Lua_s, 2);
	device->setSelectedFrequency(frequency);
	return 0;
}

int avVHF_Radio::l_get_frequency(lua_State *Lua_s)
{
	avVHF_Radio  *device = l_get_device_from_meta<avVHF_Radio>(Lua_s);
	lua_pushnumber(Lua_s, device->getCommunicator().getFrequency());
	return 1;
}

int avVHF_Radio::l_is_on(lua_State *Lua_s)
{
	avVHF_Radio  *device = l_get_device_from_meta<avVHF_Radio>(Lua_s);
	lua_pushboolean(Lua_s, device->getCommunicator().isOn());
	return 1;
}