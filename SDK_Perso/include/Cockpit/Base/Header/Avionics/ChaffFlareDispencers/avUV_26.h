#ifndef _avUV_26_h
#define _avUV_26_h

#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "ICommandsCockpitDevice.h"

#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"
#include "FMElectricSystems/Items/Lamp.h"

using namespace EagleFM;

namespace cockpit {

class COCKPITBASE_API avUV_26:  public avDevice,
				public avBreakable, 
				public avBasicElectric
{
public:
	typedef enum UV_26_modes_of_indication
	{
		FLARE_COUNT,
		PROGRAM
	};

	typedef enum UV_26_board
	{
		LEFT,
		BOTH,
		RIGHT
	};

protected:
	typedef enum UV_26_clickable_commands
	{
		CMD_UV26_ChangeBoard_toleft = iCommandCockpitDevice_Action_1,
		CMD_UV26_ChangeBoard_toright,
		CMD_UV26_ShowAmountOrProgram,
		CMD_UV26_ChangeRippleQuantity,
		CMD_UV26_ChangeBurstAmount,
		CMD_UV26_ChangeRippleInterval,
		CMD_UV26_Dispence,
		CMD_UV26_CancelCurrentProgram,
		CMD_UV26_DispenceStop,
		CMD_UV26_Power,
		CMD_UV26_BIT,
		CMD_UV26_MAX
	};

	enum enum_avUV_26_failures
	{
		left_dispenser_failure,
		right_dispenser_failure,
	};

public:
	avUV_26();
	virtual ~avUV_26();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void release();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer){}
	virtual void update();
	//end of interface

	//avBreakable interface
	virtual bool set_damage(unsigned FailureID, const bool can_be_rejected = false);
	virtual void setflag_failure(bool flag_in) {avBreakable::setflag_failure(flag_in); left_board_dispenser_failure = flag_in; right_board_dispenser_failure = flag_in;}
	virtual bool getflag_failure() const {return left_board_dispenser_failure || right_board_dispenser_failure;}
	void		 repair();

	UV_26_modes_of_indication get_mode_of_indication() const {return mode_of_indication;}
	int get_program() const;
	UV_26_board get_current_board() const {return current_board;}

	virtual bool getElecPower() const {return ElecConDevice.isPowered();}
	bool getBITPower() const {return ElecConBIT.isPowered(); }

	float getLeftLampStatus() const {return ElecConLampLeft.getLightness(); }
	float getRightLampStatus() const {return ElecConLampRight.getLightness(); }

	void reset_activity(bool flag_in);

protected:

	void connect_electric(EagleFM::Elec::ItemBase& wire);

	void complete_reset();
	void dispenser_reset();
	void stopDispence();
	void cancel_program();
	void set_ripple_quantity();
	void set_num_in_burst();
	void set_ripple_interval();
	void change_lamps_status();
    int  get_nFlare(unsigned board) const;
	
	void onPwrOn(const lwl::CSignalInfo* pInfo);
	void onPwrOff(const lwl::CSignalInfo* pInfo);
	
	Elec::Switch ElecSwitchDevicePower;
	Elec::Consumer ElecConDevice;
	
	Elec::Switch ElecSwitchLampLeft;
	Elec::Lamp ElecConLampLeft;

	Elec::Switch ElecSwitchLampRight;
	Elec::Lamp ElecConLampRight;

	Elec::Switch ElecSwitchBITPower;
	Elec::Consumer ElecConBIT;

	typedef ed::map<int, int> type_program_int_int;
	typedef ed::map<int, double> type_program_int_double;

	bool program_activated;
	UV_26_modes_of_indication mode_of_indication;
	UV_26_board current_board;

	int init_ripple_quantity_program,
		ripple_quantity_program,
		ripple_quantity,
		init_num_in_burst_program,
		num_in_burst_program,
		num_in_burst,
		init_ripple_interval_program,
		ripple_interval_program;
	wModelTime ripple_interval;

	type_program_int_int ripple_quantities_programs;
	type_program_int_int num_in_burst_programs;
	type_program_int_double ripple_intervals_programs;

	int bursts_dispensed, quantity_dispensed;
	int current_flare_count;
	int current_burst_count;
	wModelTime release_timer;

    bool        left_board_dispenser_failure;
    bool        right_board_dispenser_failure;
};

} 

#endif // _avUV_26_h
