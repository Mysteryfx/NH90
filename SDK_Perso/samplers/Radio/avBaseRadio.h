#ifndef AV_BASE_RADIO_H
#define AV_BASE_RADIO_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "Avionics/avBasicElectric.h"

#include "FMElectricSystems/Items/Consumer.h"
#include "FMElectricSystems/Items/Switch.h"

#include "WorldSound.h"

namespace Sounder {
	class Element;
}

namespace cockpit {

class avCommunicator;
class avReceiver;

class COCKPITBASE_API avCustomTumbSound
{
public:
	void init(const cVector& position, const char* host_name, const char* sound_patch);
	void play_once();

private:
	Sound::Source sound_;
};

//Automatic-matching device (RUS: ASU) 
class avAMD_Timer : public avDevice_BasicTimer
{
public:
	avAMD_Timer(avDevice* p, wModelTime _dtime) 
		: avDevice_BasicTimer(p, _dtime),target_freq_(0.0){}

	virtual ~avAMD_Timer() {}

	virtual void Start(float target_freq, double start_delay = 0);
	virtual void NextEvent();

	float target_freq() const { return target_freq_; }
	double start_time() const { return start_time_; }

private:
	float target_freq_;
	double start_time_;
};

class COCKPITBASE_API avBaseRadio : public avDevice, 
									public avBreakable,
									public avBasicElectric
{
	friend class avAMD_Timer;
protected:		

	typedef enum RadioGUI_Type
	{
		CHANNEL = 0,
		RANGE
	};

	typedef enum avBaseRadio_failures
	{
		FAIL_BASERADIO_TRANSIVER = 0,
		FAIL_BASERADIO_ANTENNA,
		FAIL_BASERADIO_MAX
	};

	avBaseRadio(RadioGUI_Type gui_type);

public:
	virtual ~avBaseRadio();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void update();
	virtual void release();
	//end of interface	

	//avBreakable interface
	virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
	virtual void repair();
	//end of interface

	//avBasicElectic interface
	virtual void	setElecPower(bool flag_in);
	virtual bool	getElecPower() const { return elec_consumer.isPowered(); }
	//end of interface

	//radio
	avCommunicator& getCommunicator();
	Sounder::Element& getSoundElement();

	float			get_tuned_frequency() const;
	int				get_preset_channel() const { return preset_channel_; }

	float			get_preset_frequency() const;  /* returns current preset frequency */
	float			get_preset_frequency(int channel) const;
	
protected:

	//init
	virtual void	perform_init_state() {update_frequency();};
	void			init_dialog_panel();

	/* communicator */
	static avReceiver* extractReceiver(lua_State * Lua_s);
	avCommunicator*		communicator;
	
	virtual void	set_volume(float volume);
	virtual float	get_volume() const;

	void			set_modulation(char modulation);
	void			set_squelch(bool onOff);
	bool			get_squelch() const;
	void			set_transmitter_power(float power); /*set transmitter power, Watts*/
	/*set communicator frequency*/
	void			set_tuned_frequency(float frequency);

	/* end communicator */

	/* frequency */

	/* returns frequency which set on a control panel*/
	virtual float	get_set_frequency() const = 0; 
	
	/* frequency change event*/
	virtual void	on_frequency_changed(){}

	/*set frequency by controls position, with change event call*/
	void			update_frequency(); 

	/*starts process of frequency switching */
	void			start_tuning();

	/*stops process of frequency switching */
	void			stop_tuning();

	/*initialize channel with frequency*/
	void			set_channel_frequency(float frequency, unsigned int channel);

	virtual void	tuning_finished();
	/* automatic tuning finished */

	bool			tuning_active() const;
	/* gets whether tuning is in process*/

	/* end frequency */

	//scripting
	virtual void	l_register_functions(lua_State *Lua_s,int index);
	static int		l_get_frequency(lua_State *Lua_s);
	static int		l_is_on(lua_State *Lua_s);

	//electric
	virtual void	checkRadioDevices();
	void			OnPowerChanged(const lwl::CSignalInfo* pInfo);

	EagleFM::Elec::Switch	main_elec_sw;
	EagleFM::Elec::Consumer	elec_consumer;

	//preset channels
	ed::vector<float> presetChannels_;
	int preset_channel_;

	/*min and max frequencies*/
	struct FrequencyRange
	{
		FrequencyRange() : min_(0.0f), max_(1.0f){}
		float min_;
		float max_;
	} frequency_range_;

	void * trigger_frequency; 
	RadioGUI_Type radio_gui_type_;

	//automatic-matching device (RUS: ASU) 
private:
	avAMD_Timer* amd_timer_;
	float max_search_time_;
};

}

#endif