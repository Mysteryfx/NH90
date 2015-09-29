#pragma once
//#include "Avionics/avDevice.h"
#include "Utilities/TransferFunctions.h"
#include "Utilities/avBasicTimer.h"
#include "Lua/Config.h"

namespace cockpit
{

class avActuator;

class COCKPITBASE_API avActuator_BasicTimer : public avBasicTimer<avActuator>
{ 
	avActuator_BasicTimer();
public:
	avActuator_BasicTimer(avActuator* _parent, wModelTime _dtime) : avBasicTimer<avActuator>(_parent,_dtime){};
	virtual void NextEvent();
};


class COCKPITBASE_API avActuator
{
public:
	avActuator(void);
	virtual ~avActuator(void);

	void		reset();

	void		initialize(const double min_position_, const double max_position_, const double default_position_,
							const double max_speed_, const double own_freq_, const double damper_coeff_,
							const bool use_own_activity_, const wModelTime actuator_dt_);
	const bool	initialize(Lua::Config& config, const char *actuator_table);

	void		setTargetPosition(const double position_);
	void		setTargetSpeed(const double speed_);
	void		cage();

	void		setMinMaxPosition(const double min_, const double max_);
	void		setMaxSpeed(const double max_);
	void		setDefaultPosition(const double default_position_){default_position = default_position_;};
	const double getDefaultPosition()const{return default_position;};

	bool		in_limit(double in_value) const { return (in_value < max_position) && (in_value > min_position);};

	void		setPosition(const double position_){position = position_;};
	void		setSpeed(const double speed_){speed = speed_;};

	const double	getPositionInterpolated()const;

	const double	getSpeed()const{return speed;};

	void		update(const double dt);

	const wModelTime	getUpdateTime()const{return update_time;};

	const double	getMinPosition()const{return min_position;};
	const double	getMaxPosition()const{return max_position;};
	const double	getMaxSpeed()const{return max_speed;};
private:
	bool		control_by_position;
	double		default_position;
	double		target_position;
	double		position;
	double		target_speed;
	double		speed;
	double		prev_position;
	double		prev_speed;

	double		max_speed;
	double		min_position;
	double		max_position;

	double		own_freq;
	double		damper_coeff;

	double		val1;

	TransferFunctionIntegral		Integrator1;
	TransferFunctionIntegralLimited	Integrator2;

	avActuator_BasicTimer*					p_timer;

	wModelTime	update_time;
};

}
