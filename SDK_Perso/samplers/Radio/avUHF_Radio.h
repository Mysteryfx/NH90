#pragma once

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"

#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"

#include "Physic/avRadioMeasureEquipment.h"

#include "Avionics/Radio/avReceiver.h"
#include "Avionics/Radio/avADF.h"

using namespace EagleFM;

namespace cockpit {


enum enum_avUHF_Function_Dial
{
	UHF_FD_OFF,
	UHF_FD_MAIN,
	UHF_FD_BOTH,
	UHF_FD_ADF
};

enum enum_avUHF_Freq_Mode_Dial
{
	UHF_FMD_MNL,
	UHF_FMD_PRESET,
	UHF_FMD_GRD
};

enum UHF_RADIO_FAILURES{
	UHF_RADIO_FAILURE_TOTAL,
};

class COCKPITBASE_API avUHF_Radio :  public avDevice,
									public avBreakable
{
public:
	avUHF_Radio();
	virtual ~avUHF_Radio();

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

	//TO DO: to power as required!
	virtual bool getElecPower() const { return ElecConDevice.GetVoltage() > 0.0; }

	bool indicationOff() const {  return !getElecPower(); }
	void connectElecPower(Elec::ItemBase& ElecInput);

	int		getPresetChannel() { return presetChannel; }

	float	getFrequency() const;

	virtual void setExternalFrequency(double frequency);

	bool	isAlternateFreqStatus() { return freqModeDial == UHF_FMD_GRD || alternateFrequencyStatus + 5 > wTime::GetModelTime(); }
	int		getAlternateFrequency();
	float	getPresetChannelFrequency(int channel) const;
	bool	isPresetChannelMode() { return freqModeDial == UHF_FMD_PRESET; }
	double	getBearing() const;

	virtual void OnUHF_PowerOn(const lwl::CSignalInfo* pInfo);
	virtual void OnUHF_PowerOff(const lwl::CSignalInfo* pInfo);

protected:

	void	switchOnCommunicator(bool switch_on);

	virtual void onFunctionDialChanging(int newFunction);
	virtual	void setManualSelectedFrequency(double frequency);

	void	setPresetChannel(int channel);
	void	onFrequencyChanged();
	float	getPresetFrequency() const;
	void	transmit_DF_tone(bool transmit);

	void		 l_register_functions(lua_State *Lua_s,int index);
	static int   l_set_frequency(lua_State *Lua_s);
	static int   l_get_frequency(lua_State *Lua_s);
	static int   l_is_on(lua_State *Lua_s);
protected:
	Elec::Consumer		ElecConDevice;
	Elec::Switch		PowerSwitch_;

	avCommunicator		communicator;	

	ed::map<int, float> presetChannels;
	avADF				ADF;

	int		presetChannel;
	int		functionDial;
	int		freqModeDial;
	
	double alternateFrequencyStatus;
	mutable float	manual_selected_frequency_;

	const float MIN_AVAILABLE_FQ; // minimum frequency = 225 Mhz
	const float MAX_AVAILABLE_FQ; // minimum frequency = 399.975 Mhz
	const float GUARD_FQ;

	bool is_load_from_mission;

	void * trigger_uhf_freq; // trigger param UHF_FREQ [MHz]\

	bool df_transmitting_;

};

}
