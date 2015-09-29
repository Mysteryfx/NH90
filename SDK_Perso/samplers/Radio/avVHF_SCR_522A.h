#pragma once

#include "ICommandsCockpitDevice.h"

#include "Avionics/Radio/avBaseRadio.h"
#include "FMElectricSystems/Items/Lamp.h"
#include "FMElectricSystems/Items/MultiThrowSwitch.h"
#include "FMElectricSystems/Items/Relay.h"

#include "WorldSound.h"

namespace cockpit
{

class COCKPITBASE_API avVHF_SCR_522A : public avBaseRadio
{
	enum SCR_RadioFailures {
		VHF_CRYSTAL = 0,
		VHF_SHORTED_CTL_BOX,
		VHF_VT_BURNED_OUT,
		VHF_VT207_DEFECTIVE,
		VHF_SQUELCH_RELAY
	};

public:
	enum SCR_522A_Lights
	{
		SCR_522_TRANS,
		SCR_522_A_Light,
		SCR_522_B_Light,
		SCR_522_C_Light,
		SCR_522_D_Light,
		SCR_522_Lights_MAX
	};

	class IIntercomDelegate
	{
	public:
		virtual void start_transmission(bool enable, bool micro) = 0;
	};

	template <class T> 
	class IntercomDelegate : public IIntercomDelegate
	{
	public:

		typedef void (T::*Delegate)(bool enable, bool micro);

		IntercomDelegate(T* device, Delegate method) :
			device_(device), method_(method)
		{
			assert(device_ && method_);
		}
		virtual void start_transmission(bool enable, bool micro)
		{
			(device_->*method_)(enable,micro);
		}
	private:
		T* device_;
		Delegate  method_;
	};
private:
	enum SCR_522A_Commands
	{
		C_SCR_522_OFF = iCommandCockpitDevice_Action_1,
		C_SCR_522_A,
		C_SCR_522_B,
		C_SCR_522_C,
		C_SCR_522_D,
		C_SCR_522_DIM,
		C_SCR_522_R_REM,
		C_SCR_522_T,
		C_SCR_522_OFF_EXT,
		C_SCR_522_A_EXT,
		C_SCR_522_B_EXT,
		C_SCR_522_C_EXT,
		C_SCR_522_D_EXT,
		C_SCR_522_DIM_EXT,
		C_SCR_522_VOLUME,
		C_SCR_522_VOLUME_EXT,
		C_SCR_522_LOCKING,
		C_SCR_522_LOCKING_EXT,
		C_SCR_522_VOLUME_AXIS,
		C_SCR_522_T_STATIC,
		C_SCR_522_R_REM_STATIC
	};

	enum enum_SCR_522A_Functional_Mode
	{
		/*Do not change sequence*/
		SCR_522A_R,
		SCR_522A_T,
		SCR_522A_REM
	};

public:
	avVHF_SCR_522A();
	~avVHF_SCR_522A();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
	virtual void release();
	virtual void SetCommand(int command, float value = 0);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//end of interface

	//avBreakable interface
	virtual bool	set_damage(unsigned Failure_ID, const bool can_be_rejected = false);
	virtual void	repair();
	//end of interface

	double	get_light(unsigned int light_numer) const;
	void	mic_button_pressed(bool pressed); 
	
	void set_intercom_delegate(IIntercomDelegate* delegate)
	{
		intercom_delegate_ = delegate;
	}

protected:

	void			connect_electric(EagleFM::Elec::Wire& power_wire);

private:
	
	virtual void	checkRadioDevices();
	virtual	void	perform_init_state();

	void			load_presets();
	void			initSound();

	void			onFunctionalModeChanging(int newMode);

	virtual float	get_set_frequency() const;
	void			setKnobsByFrequency(float frequency);
	void			setKnobsByChannel(int channel);

	void			update_communicator_state();

	bool			receiver_failure() const { return rcv_burned_out_; }

	virtual void		l_register_functions(lua_State *Lua_s,int index);
	static int			l_set_channel(lua_State *Lua_s);
	static int			l_get_channel(lua_State *Lua_s);
	static int			l_is_on(lua_State *Lua_s);
	virtual const char*	l_metatable_name() const { return "SCR_522A_meta";}

	ed::vector<EagleFM::Elec::Lamp> lights_;

	EagleFM::Elec::MultiThrowSwitch lights_switch_;
	EagleFM::Elec::Switch	receiving_switch_;
	EagleFM::Elec::Wire		lights_null_;
	EagleFM::Elec::Relay	lights_relay_;

	IIntercomDelegate*		intercom_delegate_;

	unsigned char functional_mode_;
	bool	mic_button_pressed_;
	bool	locked_;
		
	bool	ctl_box_shorted_;
	bool	rcv_burned_out_;

	//////////////////////////////////////////////////////////////////////////
	//button sound
	Sound::Source btn_sound_;
};

}