#pragma once

#include "avMechanicClock.h"
#include "ICommandsCockpitDevice.h"

namespace cockpit
{

class ccPanelGauge;

typedef enum CLOCK_clickable_commands
{
	Command_CLOCK_left_lev_down = iCommandCockpitDevice_Action_1,
	Command_CLOCK_left_lev_up,
	Command_CLOCK_left_lev_rotate,
	Command_CLOCK_right_lev_down,
	Command_CLOCK_right_lev_rotate
};

// OBSOLETE
typedef enum CLOCK_arguments
{
	Argument_CLOCK_left_lev_rotate = 511,
	Argument_CLOCK_right_lev_rotate = 512
};

class COCKPITBASE_API avAChS_1 : public avMechanicClock
{
public:
	avAChS_1();
	virtual ~avAChS_1();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
    virtual void post_initialize();
	virtual void release();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer){};
	virtual void update();
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//end of interface

	double get_currtime_seconds() const {return currtime_seconds;};

	double get_flighttime() const {return flighttime;};
	double get_seconds_meter_time() const {return seconds_meter_time;};

	bool get_flight_time_meter_started() const {return flight_time_meter_started;};
	bool get_flight_time_meter_stopped() const {return flight_time_meter_stopped;};
	
	double get_status() const {return status;};

private:
	void change_timer_state(bool&, bool&);

	virtual	void read_gauges(Lua::Config & config);


	static const wModelTime MaxOperateTime;

	wModelTime prev_time;
	wModelTime currtime_seconds;

	wModelTime flighttime_start,
		flighttime,
		flighttime_delta_time;

	wModelTime seconds_meter_time_start,
		seconds_meter_time,
		seconds_meter_time_delta_time;

	bool seconds_meter_started, seconds_meter_stopped,
		flight_time_meter_started, flight_time_meter_stopped;

	bool left_lev_pulled, left_lev_pushed, left_lev_reject_release, right_lev_pushed;

	double status;
	bool rewind_delay;
	wModelTime rewind_delay_left;

	ed::vector<ccPanelGauge*> gauges; //optional gauges creation
};

}
