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

#include "Utilities/ccUtilities.h"

namespace Sounder {
	class Element;
}

namespace cockpit {

class avCommunicator;
class avReceiver;

//Automatic-matching device (RUS: ASU) 
class avAMD_Timer : public avDevice_BasicTimer
{
public:
	avAMD_Timer(avDevice* p, wModelTime _dtime) 
		: avDevice_BasicTimer(p, _dtime),target_freq_(0.0){}

	virtual ~avAMD_Timer() {}

	virtual void Start(int target_freq, double start_delay = 0);
	virtual void NextEvent();

	int target_freq() const { return target_freq_; }
	double start_time() const { return start_time_; }

private:
	int target_freq_;
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
	
	struct FrequencyRanges;

	avBaseRadio(RadioGUI_Type gui_type, const FrequencyRanges& freq_range, unsigned char pres_chan_n = 0);

public:
	virtual ~avBaseRadio();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void update();
	virtual void release();
	virtual void OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params);
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
	const avCommunicator& getCommunicator() const;

	virtual Sounder::Element& getSoundElement();

	int			get_tuned_frequency() const;
	int			get_preset_channel() const { return preset_channel_; }

	int			get_preset_frequency() const;  /* returns current preset frequency */
	int			get_preset_frequency(int channel) const;
	
	/* returns frequency which set on a control panel*/
	virtual int	get_set_frequency() const = 0; 

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
	char			get_modulation() const;

	void			set_squelch(bool onOff);
	bool			get_squelch() const;
	void			set_transmitter_power(float power); /*set transmitter power, Watts*/
	void			set_receiver_noise(float noise);

	/*set communicator frequency*/
	void			set_tuned_frequency(int frequency);
	
	virtual void	set_preset_channel(unsigned int channel);

	/* end communicator */

	/* frequency */
	
	/* frequency change event*/
	virtual void	on_frequency_changed(){}

	/*set frequency by controls position, with change event call*/
	void			update_frequency(); 

	/*starts process of frequency switching */
	void			start_tuning();

	/*stops process of frequency switching */
	void			stop_tuning();

	/*initialize channel with frequency*/
	void			set_channel_frequency(int frequency, unsigned int channel);

	virtual void	tuning_finished();
	/* automatic tuning finished */

	bool			tuning_active() const;
	/* gets whether tuning is in process*/

	/* end frequency */

	//scripting
	virtual void	l_register_functions(lua_State *Lua_s,int index);
	static int		l_get_frequency(lua_State *Lua_s);
	static int		l_set_frequency(lua_State *Lua_s);
	static int		l_set_modulation(lua_State *Lua_s);
	static int		l_get_channel(lua_State *Lua_s);
	static int		l_set_channel(lua_State *Lua_s);
	static int		l_is_on(lua_State *Lua_s);

	virtual void	ext_set_frequency(int frequency);
	virtual void	ext_set_modulation(char modulation);
	virtual void	ext_set_channel(int channel);
	virtual bool	ext_is_on() const;
	//////////////////////////////////////////////////////////////////////////

	const ed::string	radio_path() const;
	void			load_settings();
	void			save_settings() const; // save settings to mission file

	void			save_preset_channels(int index) const;

	const char*		l_metatable_name() const {  ed::string name = Name(); name += "_meta"; return name.c_str(); }
	virtual void	load_dials(Lua::Config& config) = 0;
	virtual void	save_dials() const = 0;

	void			load_preset_channels(Lua::Config& config);
	
	//electric
	virtual void	checkRadioDevices(); /*called on power state changing*/
	void			OnPowerChanged(const lwl::CSignalInfo* pInfo);

	void			forceSetArgument(int command, float val);

	EagleFM::Elec::Switch	main_elec_sw;
	EagleFM::Elec::Consumer	elec_consumer;

	//preset channels
	ed::vector<int> presetChannels_;
	int preset_channel_;

	/*min and max frequencies*/

	typedef ed::vector<limits_i> RangeVector;
	struct FrequencyRanges
	{
		FrequencyRanges(const RangeVector& ranges, int step)
			: step_(step), min_(0.0f), max_(0.0f)
		{
			assert(step_ > 0.0);
			step_ = step_ > 0.0 ? step_ : 1.0;

			set_ranges(ranges);
		}

		void set_ranges(const RangeVector& ranges)
		{
			ranges_ = ranges;

			if(ranges_.empty()) return;

			min_ = ranges_[0].min;
			max_ = ranges_[0].max;

			for (size_t i = 1; i < ranges_.size(); ++i)
			{
				if(ranges_[i].max > max_)
					max_ = ranges_[i].max;

				if(ranges_[i].min < min_)
					min_ = ranges_[i].min;
			}
		}

		int min() const { return min_; }
		int max() const { return max_; }
		int step() const { return step_; }

		bool in_range(int frequency) const
		{
			for (size_t i = 0; i < ranges_.size(); ++i)
			{
				if (ranges_[i].in_limit_eq(frequency))
					return true;
			}
			return false;
		}

		int to_limit(int frequency) const
		{
			return __max(min_,__min(frequency,max_));
		}

		RangeVector ranges_;	//in Hz
		int step_;					//in Hz
		int min_;
		int max_;

	} freq_ranges_;
	
	void * trigger_frequency; 
	RadioGUI_Type radio_gui_type_;

	static RandomObject randomEvenly; // равномерное (0, 1)

	//automatic-matching device (RUS: ASU) 
private:
	avAMD_Timer* amd_timer_;

	float min_search_time_;
	float max_search_time_;
};

}

#endif