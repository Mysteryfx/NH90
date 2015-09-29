#include "StdAfx.h"

#include "Avionics/Radio/avBaseRadio.h"
#include "Avionics/Radio/avReceiver.h"
#include "Avionics/Radio/avRadioGUI.h"

#include "ICommandDialogsPanel.h"

#include "Physic/wRadioTransmitter.h"

#include "Utilities/ccTriggerSystem.h"

#include "ccSound.h"

#include "ed_log.h"

using namespace cockpit;

//////////////////////////////////////////////////////////////////////////
void avCustomTumbSound::init(const cVector& position, const char* host_name, const char* sound_patch)
{
	if (!ccSound::instance().getHost(host_name).is_created())
	{
		Sound::HostParams h_params;

		Sound::set_vector(h_params.position,position);

		h_params.fields = SND_POSITION;

		ccSound::instance().createHost(Sound::COCKPIT_CONTEXT, host_name, &h_params);
	}

	Sound::Host& host = ccSound::instance().getHost(host_name);
	sound_.create(host,sound_patch);
}

void avCustomTumbSound::play_once()
{
	sound_.play_once();
}

//////////////////////////////////////////////////////////////////////////
avBaseRadio::avBaseRadio(RadioGUI_Type gui_type)
	: elec_consumer(EagleFM::Elec::DC_26_VOLT),
	preset_channel_(-1),
	trigger_frequency(nullptr),
	radio_gui_type_(gui_type),
	amd_timer_(nullptr),
	max_search_time_(0.0f)
{
	communicator = new avCommunicator();
	assert(communicator);
}

avBaseRadio::~avBaseRadio()
{
	delete communicator;
}

void avBaseRadio::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avDevice::initialize(ID, Name, script_name);

	avBreakable::load_from_state(ensured_Lua());
	avBreakable::register_as_breakable();

	Lua::Config config(ensured_Lua());
	communicator->initialize(Name.c_str(), config);
	avReceiver::extractors[this] = &extractReceiver;

	max_search_time_ = config.get("max_search_time",0.0f);

	if(max_search_time_ > 0.0f)
		amd_timer_ = new avAMD_Timer(this,0.0);

	setflag_ready(true);

	make_default_activity(0.2);
}

void avBaseRadio::post_initialize()
{
	communicator->postInitialize();

	main_elec_sw >> elec_consumer;

	elec_consumer.m_signalPowerOn +=
		lwl::delegate(this,&avBaseRadio::OnPowerChanged);
	elec_consumer.m_signalPowerOff +=
		lwl::delegate(this,&avBaseRadio::OnPowerChanged);

	perform_init_state();

	init_dialog_panel();
}

void avBaseRadio::release()
{
	communicator->release();

	if (amd_timer_)
	{
		amd_timer_->KillMe();
		amd_timer_ = nullptr;
	}
}

void avBaseRadio::init_dialog_panel()
{
	Lua::Config config(ensured_Lua());

	if(config.open("GUI"))
	{
		ICommandDialogsPanel::Radio radio;
		radio.id = this->ID();
		loadRadio(config, radio);

		if(radio_gui_type_ == CHANNEL)
		{
			radio.channels.resize(presetChannels_.size());
			std::copy(presetChannels_.begin(), presetChannels_.end(), radio.channels.begin());
		}

		globalCommandDialogsPanel->loadRadio(radio);
		config.pop();
	}
}
//////////////////////////////////////////////////////////////////////////
void avBaseRadio::update()
{
	if(getElecPower() && getflag_ready())
	{
		communicator->update();
	}
}

void avBaseRadio::setElecPower(bool flag_in)
{
	main_elec_sw.setOnOff(flag_in);
}

avCommunicator& avBaseRadio::getCommunicator() 
{
	return *communicator;
}

Sounder::Element& avBaseRadio::getSoundElement() 
{
	return communicator->getSoundElement();
}

void avBaseRadio::OnPowerChanged(const lwl::CSignalInfo* pInfo)
{
	checkRadioDevices();
}

void avBaseRadio::checkRadioDevices()
{
	communicator->setOnOff(getElecPower() && getflag_ready());
}

avReceiver * avBaseRadio::extractReceiver(lua_State * Lua_s)
{
	avBaseRadio * device = l_get_device_from_meta<avBaseRadio>(Lua_s);
	return device->communicator;
}

void avBaseRadio::set_volume(float volume)
{
	communicator->setVolume(volume);
}

float avBaseRadio::get_volume() const
{
	return communicator->getVolume();
}

float avBaseRadio::get_tuned_frequency() const
{
	return communicator->getFrequency();
}

void avBaseRadio::set_tuned_frequency(float frequency)
{
	communicator->setFrequency(frequency);
}

void avBaseRadio::update_frequency()
{
	float freq = get_set_frequency();

	triggers::update_parameter(trigger_frequency, freq / 1000000.); // MHz

	set_tuned_frequency(freq);
	on_frequency_changed();
}

void avBaseRadio::start_tuning()
{
	if(amd_timer_)
	{
		assert((frequency_range_.max_ - frequency_range_.min_) > 0.0);

		float search_time = 0.0f;
		if(amd_timer_->IsActive())
		{
			double cur_frequency = get_tuned_frequency() + 
				(amd_timer_->target_freq() - get_tuned_frequency()) * 
				(wTime::GetModelTime() - amd_timer_->start_time()) / 
				(amd_timer_->wakeUpTime - amd_timer_->start_time());

			search_time = fabs(get_set_frequency() - cur_frequency) / 
				(frequency_range_.max_ - frequency_range_.min_);
		}
		else
		{
			search_time = max_search_time_ * 
				fabs(get_set_frequency() - get_tuned_frequency()) / 
				(frequency_range_.max_ - frequency_range_.min_);
		}

		ED_DEBUG("Tuning started. Cur Time: %f Search time: %f",wTime::GetModelTime(), search_time);
		ED_DEBUG("Tuning started. Cur Freq: %f, Target Freq: %f",amd_timer_->target_freq(), get_set_frequency());

		amd_timer_->Start(get_set_frequency(), search_time);
	}
	else
	{
		update_frequency();
	}
}

void avBaseRadio::stop_tuning()
{
	if(amd_timer_ && amd_timer_->IsActive())
	{
		amd_timer_->DoStop();

		double tuned_freq = get_tuned_frequency() + 
			(amd_timer_->target_freq() - get_tuned_frequency()) * 
			(wTime::GetModelTime() - amd_timer_->start_time()) / 
			(amd_timer_->wakeUpTime - amd_timer_->start_time());

		set_tuned_frequency(tuned_freq);

		ED_DEBUG("Tuning stopped. Tuned Freq: %f, Target Freq: %f",tuned_freq, amd_timer_->target_freq());
	}
}

void avBaseRadio::set_modulation(char modulation)
{
	communicator->setModulation((wModulation)modulation);
}

void avBaseRadio::set_squelch(bool onOff)
{
	communicator->setSquelchOnOff(onOff);
}

bool avBaseRadio::get_squelch() const
{
	return communicator->getSquelchIsOn();
}

void avBaseRadio::set_transmitter_power(float power)
{
	wRadioTransmitter * pRadioTransmitter = communicator->getTransmitter();
	if (pRadioTransmitter)
		pRadioTransmitter->setPower(power);
}

float avBaseRadio::get_preset_frequency() const
{
	return (preset_channel_ >=0 && preset_channel_ < presetChannels_.size())
		? presetChannels_[preset_channel_] : 0.0f;
}

float avBaseRadio::get_preset_frequency(int channel_) const
{
	return (channel_ >=0 && channel_ < presetChannels_.size()) 
		? presetChannels_[channel_] : 0.0f;
}

void avBaseRadio::set_channel_frequency(float frequency, unsigned int channel)
{
	if(channel >=0 && channel < presetChannels_.size())
	{
		presetChannels_[channel] = frequency;
	}
}

void avBaseRadio::tuning_finished()
{
	ED_DEBUG("Tuning complete: Time: %f", wTime::GetModelTime());
	update_frequency();
}

bool avBaseRadio::tuning_active() const
{
	assert(amd_timer_);
	return amd_timer_? amd_timer_->IsActive() : false;
}

//////////////////////////////////////////////////////////////////////////
void avBaseRadio::l_register_functions(lua_State *Lua_s,int index)
{
	avDevice::l_register_functions(Lua_s, index);

	lua_pushstring(Lua_s, "get_frequency");	lua_pushcfunction(Lua_s, l_get_frequency);	lua_settable  (Lua_s, index);
	lua_pushstring(Lua_s, "is_on");			lua_pushcfunction(Lua_s, l_is_on);			lua_settable  (Lua_s, index);
}

int avBaseRadio::l_get_frequency(lua_State *Lua_s)
{
	avBaseRadio  *device = l_get_device_from_meta<avBaseRadio>(Lua_s);
	lua_pushnumber(Lua_s, device->getCommunicator().getFrequency());
	return 1;
}

int avBaseRadio::l_is_on(lua_State *Lua_s)
{
	avBaseRadio  *device = l_get_device_from_meta<avBaseRadio>(Lua_s);
	lua_pushboolean(Lua_s, device->getCommunicator().isOn());
	return 1;
}
//////////////////////////////////////////////////////////////////////////
//Breakable
bool avBaseRadio::set_damage(unsigned Failure_ID, const bool can_be_rejected )
{
	//no power - no break
	if(can_be_rejected && !getElecPower())
		return false;

	switch(Failure_ID)
	{
	case FAIL_BASERADIO_TRANSIVER:
		elec_consumer.setFailure(true);
		break;
	case FAIL_BASERADIO_ANTENNA:
		communicator->damageAntenna(true);
		break;
	default:
		return false;
	}

	return avBreakable::set_damage(Failure_ID);
}

void avBaseRadio::repair()
{
	elec_consumer.setFailure(false);
	communicator->damageAntenna(false);
	avBreakable::repair();
}

//////////////////////////////////////////////////////////////////////////
//avAMD_Timer
void avAMD_Timer::Start(float target_freq, double start_delay /* = 0 */)
{
	avDevice_BasicTimer::Start(start_delay);
	start_time_ = wTime::GetModelTime();
	target_freq_ = target_freq;
}

void avAMD_Timer::NextEvent()
{
	avBaseRadio* radio = static_cast<avBaseRadio*>(parent);
	if (!radio || to_cancel)
	{
		Cancel();
		return;
	}

	radio->tuning_finished();
}