#include "stdafx.h"

#include "Avionics/Radio/avR_828.h"

#include "Utilities/ccUtilities.h"
#include "Comm/IwTransiver.h"

#include "Lua/Config.h"

#ifdef DEBUG_TEXT
#include "Sound/Sounder.h"
#endif

using namespace cockpit;

RandomObject avR_828::randomEvenly(0, 7493746);
static const float CHANNEL_QTY = 10;

avR_828::avR_828() : avBaseRadio(avBaseRadio::CHANNEL), 
	ElecCon_Search_Lamp(EagleFM::Elec::DC_26_VOLT)
{
}

avR_828::~avR_828()
{
}

void avR_828::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avBaseRadio::initialize(ID, Name, script_name);

	setSearchLampStatus(false);
	asu = false;
	
	set_modulation(MODULATION_FM);
	set_squelch(true);
	presetChannels_.resize(CHANNEL_QTY);
	preset_channel_ = 0;

#ifdef DEBUG_TEXT
	getSoundElement().name = "R_828";
#endif

	frequency_range_.min_ = 20E6;
	frequency_range_.max_ = 60E6;
}

void avR_828::perform_init_state()
{
	Lua::Config config(ensured_Lua());

	//сохранённое состояние Р-828
	bool noise_reductor_v = get_squelch();
	int   channel_v = 0;
	float volume_v = get_volume();

	lua_getglobal(ensured_Lua(),"Load_R_828_Status");
	if (lua_pcall(ensured_Lua(),0,0,0))
	{
		ED_ERROR("Cockpit: Load_R_828_Status %s", lua_tostring(ensured_Lua(), -1));
		lua_pop(ensured_Lua(), 1);
	}

	//восстанавливаем сохранённое состояние Р-800 
	config.get("noise_reductor",&noise_reductor_v);
	if(config.get("channel",&channel_v))
	{
		if(channel_v > 9 ||
			channel_v < 0)
		{
			channel_v = 0;
		}
	}
	if(config.get("volume",&volume_v))
	{
		if(volume_v > 1.0 ||
			volume_v < 0.0)
		{
			volume_v = 0.0;
		}		
	}

	//complete channel list by random frequencies
	for(int i = 0; i < presetChannels_.size(); i++) 
		presetChannels_[i] = 20000000.0f + randomEvenly.get() * (69750000.0f - 20000000.0f);

	if(config.open("r_828_channel"))
	{
		config.iterate_begin();
		while(config.iterate_next())
		{
			int    channel_num;
			double frequency;
			config.get_key(&channel_num);
			config.iterate_pop(&frequency);
			if(channel_num < CHANNEL_QTY)
			{					
				set_channel_frequency(frequency, channel_num);
			}
		}
		config.iterate_end();
		config.pop();
	}

	performClickableAction(CMD_R_828_NOISE_REDUCTOR_ON_OFF2 , noise_reductor_v);
	performClickableAction(CMD_R_828_VOLUME_CHANGE , volume_v);
	performClickableAction(CMD_R_828_CHANNEL_CHANGE , channel_v * 0.1);
	
	update_frequency();

	//рубильник УКВ-1
	if (aircraft().init_conditions.cpt_init_condition != CPT_INCON_COLD_GROUND)
	{
		performClickableAction(CMD_R_828_POWER_ON_OFF2 , 1.0f);
	}
	else
	{
		performClickableAction(CMD_R_828_POWER_ON_OFF2 , 0.0f);
	}
}

bool avR_828::checkAnimationConditions(int arg_num, int command, float& value)
{
	switch(command)
	{
		case iCommandPlane_R_828_volume_up:
			return get_volume() < 0.90f;
			break;
		case iCommandPlane_R_828_volume_down:
			return get_volume() > 0.1f;
			break;
	}
	return true;
}

void avR_828::SetCommand(int command, float value)
{
	switch(command)
	{
//--------------------------------------------------------------------
		case iCommandPlane_R_828_Channel_next:
            switcher_value_iterate(CMD_R_828_CHANNEL_CHANGE,1); return;
		case iCommandPlane_R_828_Channel_prevous:
			switcher_value_iterate(CMD_R_828_CHANNEL_CHANGE,-1); return;
		case CMD_R_828_CHANNEL_CHANGE:						
			setChannel_(floor(CHANNEL_QTY * value + 0.5));
		break;
//--------------------------------------------------------------------
		case iCommandPlane_R_828_volume_up:
			axis_value_iterate(CMD_R_828_VOLUME_CHANGE,1);return;
		case iCommandPlane_R_828_volume_down:
			axis_value_iterate(CMD_R_828_VOLUME_CHANGE,-1);return;
		case CMD_R_828_VOLUME_CHANGE:
			value = MIN_MAX(0.0, value, 1.0);
			set_volume(value);
		break;
//--------------------------------------------------------------------
		case iCommandPlane_R_828_Power_on_off:
			switcher_value_iterate(CMD_R_828_POWER_ON_OFF2); return;
		case CMD_R_828_POWER_ON_OFF2:			
			setElecPower(floor(value + 0.5f));
		break;
//--------------------------------------------------------------------
		case iCommandPlane_R_828_ASU:
			button_value_iterate(CMD_R_828_ASU); 
			return;
		case iCommandPlane_R_828_ASU_up:
			button_value_iterate(CMD_R_828_ASU);
			return; 
		case CMD_R_828_ASU:
		{
			switchASU(value > 0.5f);

			if(	checkASU() && 
				getElecPower() &&
				getflag_ready())
			{
				start_tuning();
				setSearchLampStatus(true);
			}
			else
			{
				setSearchLampStatus(false);
				stop_tuning();
			}			
		}
		break;
//-------------------------------------------------------------------------------
		case iCommandPlane_R_828_Noise_Reductor_on_off:
			switcher_value_iterate(CMD_R_828_NOISE_REDUCTOR_ON_OFF2); return;
		case CMD_R_828_NOISE_REDUCTOR_ON_OFF2:
			set_squelch(static_cast<boolean>(floor(value + 0.5f)));
		break;
	}
}

void avR_828::setChannel_(unsigned int channel_) 
{
	if(channel_ < presetChannels_.size())
		preset_channel_ = channel_;

	if(tuning_active())
		start_tuning();
}

float avR_828::get_set_frequency() const 
{
	return get_preset_frequency();
}

void avR_828::tuning_finished()
{
	avBaseRadio::tuning_finished();

	switchASU(false);
	setSearchLampStatus(false);
}

void avR_828::l_register_functions(lua_State *Lua_s,int index)
{
	avBaseRadio::l_register_functions(Lua_s, index);

	lua_pushstring(Lua_s, "set_channel");	lua_pushcfunction(Lua_s, l_set_channel);	lua_settable  (Lua_s, index);
}

int avR_828::l_set_channel(lua_State *Lua_s)
{
	avR_828  *device = l_get_device_from_meta<avR_828>(Lua_s);
	assert(lua_isnumber(Lua_s, 2));
	int channelNum = lua_tonumber(Lua_s, 2);
	device->performClickableAction(CMD_R_828_CHANNEL_CHANGE , channelNum * 0.1f);
	device->update_frequency();
	return 0;
}

void avR_828::connect_electric(EagleFM::Elec::ItemBase& wire)
{
	wire >> main_elec_sw;
	wire >> ElecSwitch_Search_Lamp >> ElecCon_Search_Lamp;
}