#pragma once

#include "avMechanicClock.h"
#include "ICommandsCockpitDevice.h"

namespace cockpit
{

class COCKPITBASE_API avA11Clock : public avMechanicClock
{
	typedef enum CLOCK_commands
	{
		CLOCK_lev_up = iCommandCockpitDevice_Action_1,
		CLOCK_lev_rotate,
		CLOCK_lev_up_Ext,
		CLOCK_lev_rotate_Ext
	};

	enum A11_Clock_Failures
	{
		A11_CLOCK_FAILURE = 0,
	};

public:
	avA11Clock();
	virtual ~avA11Clock();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
    virtual void post_initialize();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer){};
	virtual void update();
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//end of interface

	//avBreakable
	virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
	virtual void repair();
	//avBreakable

	double get_currtime_seconds() const {return currtime_seconds;};

private:

	static const wModelTime MaxOperateTime;

	wModelTime prev_time;
	wModelTime currtime_seconds;

	bool lev_pulled, lev_reject_release;

	double status;
	bool rewind_delay;
	wModelTime rewind_delay_left;
};

}
