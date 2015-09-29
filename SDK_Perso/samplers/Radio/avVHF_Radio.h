#pragma once

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"

#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"

#include "Avionics/Radio/avReceiver.h"
#include "Sound/Sounder.h"

using namespace EagleFM;

namespace cockpit {

const double MIN_VHF_FM_FREQ = 30000000.0f;
const double MAX_VHF_FM_FREQ = 76000000.0f;

const double MIN_VHF_AM_FREQ =108000000.0f;
const double MID_VHF_AM_FREQ =116000000.0f;
const double MAX_VHF_AM_FREQ =151975000.0f;

const double VHF_BOUND_FREQ = 100000000.0f;

enum enum_VHF_radio_type
{
	VHF_RT_AM,
	VHF_RT_FM,
};

enum VHF_RADIO_FAILURES{
	VHF_RADIO_FAILURE_TOTAL,
};

class COCKPITBASE_API avVHF_Radio :  public avDevice,
									public avBreakable
{
protected:
	avVHF_Radio();
public:
	virtual ~avVHF_Radio();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void release();
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
	virtual void OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params);
	//end of interface

	//avBreakable interface
	virtual bool	set_damage(unsigned Failure_ID, const bool can_be_rejected = false);
	virtual void	repair();
	//end of interface
	
	void load_settings();
	void save_settings(); // save settings to mission file
	
	avCommunicator & getCommunicator() { return communicator; }
	virtual Sounder::Element & getSoundElement() { return communicator.getSoundElement(); }

	virtual bool getElecPower() const { return ElecConDevice.GetVoltage() > 0.0; }
	void connectElecPower(Elec::ItemBase& ElecInput);

	bool isAMRadio() { return type_AMFM == VHF_RT_AM; }
	bool isFMRadio() { return type_AMFM == VHF_RT_FM; }

	virtual float	getFrequency() const = 0;

	int getChannel() const { return presetChannel; }
	virtual void setChannel(int channel) { presetChannel = channel; onFrequencyChanged(); }
	
protected:
	virtual void onFunctionalModeChanging(int newMode);
	virtual void OnVHF_PowerOn(const lwl::CSignalInfo* pInfo);
	virtual void OnVHF_PowerOff(const lwl::CSignalInfo* pInfo);
	virtual void init_dialog_panel();

	void	switchOnCommunicator(bool switch_on);

	virtual void l_register_functions(lua_State *Lua_s,int index);
	static int	 l_set_frequency(lua_State *Lua_s);
	static int   l_get_frequency(lua_State *Lua_s);

	static int   l_is_on(lua_State *Lua_s);

	virtual	void	setSelectedFrequency(float frequency);
	virtual void	onFrequencyChanged();
	float			getPresetFrequency() const;

	Elec::Consumer		ElecConDevice;
	Elec::Switch		Power_Switch_;

	avCommunicator communicator;	

	ed::map<int, float> presetChannels;

	int presetChannel;
	int type_AMFM;

	mutable double selected_frequency_;

	double noise_k,
		   power,
		   innerNoise;

	bool is_load_from_mission;
	int functional_mode_;
	int selectionDial;

	void * trigger_vhf_freq; // VHF frequency [MHz]

	struct
	{
		Sounder::Source warningTone;
	} sound;

};

}
