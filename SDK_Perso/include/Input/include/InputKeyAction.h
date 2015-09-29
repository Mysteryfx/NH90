#pragma once
#include "InputExports.h"
#include "assert.h"
union fictional_value
{
	float		 val;
	unsigned int composed;
};

//WARNING!!!!!!!!
/*
Compose routine is copy pasted from Projects\Cockpit\Base\Source\Utilities\ccUtilities.cpp,
they must be identical
*/

inline double make_cockpit_composed_value(unsigned int id,double value)
{
	/*
	static const unsigned CHAR_FLOAT_COMPOSING_MASK = 0x00FFFFFF;

	fictional_value tmp;
	value += 1.0;
	
	if (id > 255)
	{
		id = 255;
	}

	tmp.composed  = 0;
	tmp.composed  = (CHAR_FLOAT_COMPOSING_MASK &  id) << 24 ;
	tmp.composed |=  CHAR_FLOAT_COMPOSING_MASK & (unsigned int)(value * 1000000);

	return tmp.val;
	*/

	double normalized = (value + 2.0)/8.0;
	assert(normalized < 1.0 && normalized > 0);
	return (double)id + normalized;
}

class INPUT_API InputKeyAction
{
public:
	InputKeyAction(int down = 0, int pressed = 0, int up = 0)
	:	down_(down),
		pressed_(pressed),
		up_(up),
		hasValueDown_(false),
		valueDown_(0),
		hasValuePressed_(false),
		valuePressed_(0),
		hasValueUp_(false),
		valueUp_(0)
		{
		};

	int getDownAction() const			{ return down_; }
	int getPressedAction() const		{ return pressed_; }
	int getUpAction() const				{ return up_; }
	
	void setDownAction(int action)			{ down_ = action; }
	void setPressedAction(int action)		{ pressed_ = action; }
	void setUpAction(int action)			{ up_ = action; }
	
	bool hasValueForDown() const			{ return hasValueDown_; }
	bool hasValueForPressed() const			{ return hasValuePressed_; }
	bool hasValueForUp() const				{ return hasValueUp_; }
	
	double getValueForDown() const			{ return valueDown_; }
	double getValueForPressed() const		{ return valuePressed_; }
	double getValueForUp() const			{ return valueUp_; }

	void setValueForDown(double val)		{ hasValueDown_    = true; valueDown_ = val; }
	void setValueForPressed(double val)		{ hasValuePressed_ = true; valuePressed_ = val; }
	void setValueForUp(double val)			{ hasValueUp_      = true; valueUp_ = val; }

protected:
	int down_; 
	int pressed_;
	int up_;

	bool  hasValueDown_;
	double valueDown_;

	bool  hasValuePressed_;
	double valuePressed_;

	bool  hasValueUp_;
	double valueUp_;
};

class INPUT_API InputAxisAction
{
public:
	InputAxisAction(int action = 0, unsigned int device_id_ = -1/*invalid*/)
	{
		action_ = action;
		cockpitDeviceId_ = device_id_;
	}

	int getAction() const { return action_; }

	double getValue(double filtered_input_value) const
	{
		if (cockpitDeviceId_ < 256)
		{
			return make_cockpit_composed_value(cockpitDeviceId_, filtered_input_value);
		}

		return filtered_input_value;
	}

	void setAction(int action)				{ action_			 = action; }
	void setTargetDevice(int dev_id)		{ cockpitDeviceId_ = (unsigned)dev_id; }

protected:
	unsigned	cockpitDeviceId_;
	int			action_;
};

