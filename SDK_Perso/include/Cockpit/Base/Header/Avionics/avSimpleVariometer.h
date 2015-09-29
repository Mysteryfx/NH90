#ifndef AV_SIMPLE_VARIOMETER_H 
#define AV_SIMPLE_VARIOMETER_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

/************************************************************************/
/* Variometer                                                           */
/************************************************************************/
#include "CockpitBase.h"

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "Avionics/avMechanics.h"

#include "ICommandsCockpitDevice.h"

namespace cockpit
{
class avFMProxyBase;

class COCKPITBASE_API avSimpleVariometer : public avDevice, public avBreakable
{
public:
	avSimpleVariometer();
	virtual ~avSimpleVariometer();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
	virtual void update();
	//

	double get_vertical_speed() const { return variometer_.get_value_out(); }
protected:

	virtual double	get_sensor_speed() const;
	avSimpleVariometer(int Sensor_Id);

	DECLARE_DEV_LINKPTR(avFMProxyBase,FM_Proxy);

	gauge_limited variometer_;
	int sensor_id_;
};


class avAdjustableVariometer
	: public avSimpleVariometer
{
public:
	enum Variometer_Commands
	{
		CMD_ADJUST_NEEDLE = iCommandCockpitDevice_Action_1,
		CMD_ADJUST_NEEDLE_EXT
	};

	avAdjustableVariometer();

	//avDevice interface
	virtual void SetCommand(int command, float value /* = 0 */);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//

protected:
	virtual double	get_sensor_speed() const;
	float adjustment_;
};

}

#endif

