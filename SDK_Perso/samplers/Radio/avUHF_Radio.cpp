#include "stdafx.h"

#include "Avionics/Radio/avUHF_Radio.h"
#include "Avionics/Radio/avRadioGUI.h"
#include "ccModuleManager.h"
#include "Physic/wRadioTransmitter.h"
#include "ICommandDialogsPanel.h"
#include "Comm/wHumanCommunicator.h"
#include "Utilities/ccTriggerSystem.h"
#include "Lua/Config.h"
#include "GlobalConfig.h"
#include "IwoLa.h"
#include "IWing.h"
// #include "MapCoords.h"
// #include "MagVar/WMM_Interface.h"

#include "Comm/wMsgEnum.h"
#include "Comm/wMessage.h"

using namespace cockpit;

REGISTER_DYNAMIC_FACTORY(cockpit::avUHF_Radio, avUHF_Radio);

avUHF_Radio::avUHF_Radio() : ADF(ADF_RANGE_HF),
	MIN_AVAILABLE_FQ(225000000.0f), MAX_AVAILABLE_FQ(399975000.0f),
	GUARD_FQ(243000000.0f), df_transmitting_(false),
	ElecConDevice(EagleFM::Elec::DC_26_VOLT),
	manual_selected_frequency_(0.0f)
{
	presetChannel = 1;
	
	freqModeDial = UHF_FMD_MNL;
	functionDial = UHF_FD_OFF;
	
	alternateFrequencyStatus = 0;

	// presets
	presetChannels[1]   = 225000000.0f;
	presetChannels[2]   = 235000000.0f;
	presetChannels[3]   = 245000000.0f;
	presetChannels[4]   = 255000000.0f;
	presetChannels[5]   = 265000000.0f;
	presetChannels[6]   = 275000000.0f;
	presetChannels[7]   = 285000000.0f;
	presetChannels[8]   = 295000000.0f;
	presetChannels[9]   = 305000000.0f;
	presetChannels[10]  = 315000000.0f;
	presetChannels[11]  = 325000000.0f;
	presetChannels[12]  = 335000000.0f;
	presetChannels[13]  = 345000000.0f;
	presetChannels[14]  = 355000000.0f;
	presetChannels[15]  = 365000000.0f;
	presetChannels[16]  = 375000000.0f;
	presetChannels[17]  = 385000000.0f;
	presetChannels[18]  = 395000000.0f;
	presetChannels[19]  = 396000000.0f;
	presetChannels[20]  = 397000000.0f;

	trigger_uhf_freq = 0;

	is_load_from_mission = false;
}

avUHF_Radio::~avUHF_Radio()
{
}

void avUHF_Radio::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avDevice::initialize(ID, Name, script_name);

	avBreakable::load_from_state(ensured_Lua());
	avBreakable::register_as_breakable();

	Lua::Config config(ensured_Lua());
	communicator.initialize(Name.c_str(), config);
	communicator.setModulation(MODULATION_AM);

	ADF.initialize(communicator.getReceiver(), config);

	make_default_activity();
}

void avUHF_Radio::post_initialize()
{
	trigger_uhf_freq = triggers::get_parameter_handle("UHF_FREQ");

	communicator.postInitialize();
	communicator.setAsTransiverForTunningFromOutside();

	ElecConDevice.m_signalPowerOn +=
		lwl::delegate(this,&avUHF_Radio::OnUHF_PowerOn);

	ElecConDevice.m_signalPowerOff +=
		lwl::delegate(this,&avUHF_Radio::OnUHF_PowerOff);

	if (aircraft().init_conditions.cpt_init_condition != CPT_INCON_COLD_GROUND)
	{	
		float flightFrequency = cockpit::c_LA()->getWing()->get_frequency();
		if (flightFrequency >= MIN_AVAILABLE_FQ && flightFrequency <= MAX_AVAILABLE_FQ)
		{
			presetChannels[1] = flightFrequency;
		}
		onFrequencyChanged();
	}

	load_settings();

	if(is_load_from_mission)
	{
		float frequency;
		if(freqModeDial == UHF_FMD_PRESET)
			frequency = presetChannels[presetChannel];		
		setManualSelectedFrequency(frequency);
	}

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

void avUHF_Radio::connectElecPower(Elec::ItemBase& ElecInput)
{
	ElecInput >> PowerSwitch_ >> ElecConDevice;
}

void avUHF_Radio::release()
{
	c_get_communicator()->getTransiverStorage().setTransiverForTunningFromOutside(NULL);
	ADF.release();
	communicator.release();
}

void avUHF_Radio::update()
{
	if (!getElecPower())
		return;

	communicator.update();
	if (functionDial == UHF_FD_ADF)
	{
		const double dt = p_device_timer->GetDeltaTime();
		ADF.update(dt);
	}
}

void avUHF_Radio::setPresetChannel(int channel)
{
	presetChannel = channel;
}

void avUHF_Radio::switchOnCommunicator(bool switch_on)
{
	communicator.setOnOff(!getflag_failure() && functionDial != UHF_FD_OFF && switch_on);
	
	if ((functionDial == UHF_FD_ADF) && switch_on)
		communicator.setTransmitterOnOff(false);
}

void avUHF_Radio::onFunctionDialChanging(int newFunction)
{
	functionDial = newFunction;
	PowerSwitch_.setOnOff(functionDial != UHF_FD_OFF);
	switchOnCommunicator(getElecPower());
}

void avUHF_Radio::onFrequencyChanged()
{
	float freq = getFrequency();
	triggers::update_parameter(trigger_uhf_freq, freq / 1000000.0); // MHz
	if (freq >= MIN_AVAILABLE_FQ && freq <= MAX_AVAILABLE_FQ)
	{
		communicator.setFrequency(freq);
		switchOnCommunicator(true);
	}
	else
	{
		switchOnCommunicator(false);	
	}
}

float avUHF_Radio::getPresetChannelFrequency(int channel) const
{
	ed::map<int, float>::const_iterator it = presetChannels.find(channel);
	if (it == presetChannels.end())
		return 1;

	return it->second;
}

float avUHF_Radio::getPresetFrequency() const 
{
	return getPresetChannelFrequency(presetChannel);
}

int avUHF_Radio::getAlternateFrequency()
{
	return cockpit::round(getFrequency() / 1000.0f);
}

double avUHF_Radio::getBearing() const
{
	if(ADF.isSignalReceived())
	{
		double trueBearing = ADF.getBearing();
//		const dVector& pt = get_context()->human_ptr->GetPoint();
// 		double lat, lon;
// 		MapCoordsSpace::MapCoords::LatLon(pt.x, pt.z, lat, lon);
		return RAD_TO_2PI(trueBearing/* - magvar::get_mag_decl(lat, lon)*/);
	}
	return -1.0;
}

void avUHF_Radio::save_settings()
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
			lua_pushnumber(L,functionDial);
			lua_settable(L, sub);

			lua_pushstring(L,"channel_dial");
			lua_pushnumber(L,presetChannel);
			lua_settable(L, sub);

			lua_pushstring(L,"selection_dial");
			lua_pushnumber(L,freqModeDial);
			lua_settable(L, sub);

			lua_pushstring(L,"manual_frequency");
			lua_pushnumber(L, getFrequency());
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
		lua_pushstring(L,"UHF/");
		if (lua_pcall(L,1,0,0))
		{
			//ED_ERROR("Cockpit:  SaveSettings %s", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
}

void avUHF_Radio::load_settings()
{
	Lua::Config config(L);

	int res = 0;

	config.push("UHF/");
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
			int new_func_dial; 

			config.get("mode_dial",     &new_func_dial);
			config.get("selection_dial",     &freqModeDial);
			config.get("channel_dial",  &presetChannel);
			config.get("manual_frequency",  &frequency);
			config.pop();
			setManualSelectedFrequency(frequency);
			onFunctionDialChanging(new_func_dial);

			is_load_from_mission = true;
		}

	}
}

float avUHF_Radio::getFrequency() const
{
	float frequency = 0;
	switch (freqModeDial)
	{
	case UHF_FMD_MNL :
		frequency = manual_selected_frequency_;
		break;

	case UHF_FMD_PRESET :
		frequency = getPresetFrequency();
		break;

	case UHF_FMD_GRD :
		frequency = GUARD_FQ;
		break;
	}
	return frequency;
}

void avUHF_Radio::setExternalFrequency(double frequency)
{
	setManualSelectedFrequency(frequency);
}

void avUHF_Radio::setManualSelectedFrequency(double frequency)
{
	manual_selected_frequency_ = frequency;
	if (freqModeDial == UHF_FMD_MNL)
	{
		onFrequencyChanged();
	}
}

void avUHF_Radio::OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params) 
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

void avUHF_Radio::repair()
{
	setflag_failure(false);
	ElecConDevice.setFailure(false);
	communicator.setOnOff(functionDial != UHF_FD_OFF);

	avBreakable::repair();
}

bool avUHF_Radio::set_damage(unsigned Failure_ID, const bool can_be_rejected)
{
	//Если нет электричества, то не ломаемся
	if (can_be_rejected && !getElecPower())
		return false;

	switch (Failure_ID)
	{
	case UHF_RADIO_FAILURE_TOTAL:
		setflag_failure(true);
		communicator.setOnOff(false);
		ElecConDevice.setFailure(true);
		break;
	}

	return true;
}

void avUHF_Radio::OnUHF_PowerOn(const lwl::CSignalInfo* pInfo)
{	
	switchOnCommunicator(true);
}

void avUHF_Radio::OnUHF_PowerOff(const lwl::CSignalInfo* pInfo)
{	
	switchOnCommunicator(false);
}

void avUHF_Radio::transmit_DF_tone(bool transmit)
{
	if (transmit && !df_transmitting_)
	{
		wMessagePtr pMessage = new wMessage(wMessage::Sender(wMessage::Sender::DEFAULT_CALLSIGN, aircraft().human_ptr.Obj(),""),
											wMsgBeacon,
											"Sounds/Effects/Aircrafts/Cockpits/1020Hz_DF_tone",
											false,
											"",
											wMessage::PLAY_MODE_LOOPED);
		pMessage->pRadioTransmitter = communicator.getTransmitter();
		aircraft().human_ptr->getCommunicator()->transmitMessageNow(pMessage);
		
		df_transmitting_ = true; 
	}
	else if(df_transmitting_)
	{
		aircraft().human_ptr->getCommunicator()->finishTransmition(false);
		df_transmitting_ = false; 
	}
}

void avUHF_Radio::l_register_functions(lua_State *Lua_s,int index)
{
    avDevice::l_register_functions(Lua_s,index);
	lua_pushstring(Lua_s, "set_frequency");		lua_pushcfunction(Lua_s, l_set_frequency);	lua_settable  (Lua_s, index);
	lua_pushstring(Lua_s, "get_frequency");		lua_pushcfunction(Lua_s, l_get_frequency);	lua_settable  (Lua_s, index);
	lua_pushstring(Lua_s, "is_on");				lua_pushcfunction(Lua_s, l_is_on);			lua_settable  (Lua_s, index);
}

int avUHF_Radio::l_set_frequency(lua_State *Lua_s)
{
	avUHF_Radio  *device = l_get_device_from_meta<avUHF_Radio>(Lua_s);
	assert(lua_isnumber(Lua_s, 2));
	float frequency = lua_tonumber(Lua_s, 2);
	device->setExternalFrequency(frequency);
	return 0;
}

int avUHF_Radio::l_get_frequency(lua_State *Lua_s)
{
	avUHF_Radio  *device = l_get_device_from_meta<avUHF_Radio>(Lua_s);
	lua_pushnumber(Lua_s, device->getCommunicator().getFrequency());
	return 1;
}

int avUHF_Radio::l_is_on(lua_State *Lua_s)
{
	avUHF_Radio  *device = l_get_device_from_meta<avUHF_Radio>(Lua_s);
	lua_pushboolean(Lua_s, device->getCommunicator().isOn());
	return 1;
}