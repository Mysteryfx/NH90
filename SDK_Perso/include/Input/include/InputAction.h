#pragma once

#include "InputExports.h"

// действие, привязанное к клавиатурной/осевой комбинациии
class INPUT_API InputAction
{
public:
	InputAction() 
	: action_(0), 
		value_(0), 
		hasValue_(false), 
		absolute_(false), 
		time_(0),
		device_(-1)
	{
	}

	InputAction(long action, unsigned long time, unsigned long device)
	: action_(action), 
		value_(0),
		hasValue_(false),
		absolute_(false),
		time_(time),
		device_(device)
	{
	}

	InputAction(long action, double value, bool absolute, unsigned long time, unsigned long device)
	: action_(action), 
		value_(value),
		hasValue_(true),
		absolute_(absolute),
		time_(time),
		device_(device)
	{
	}

	void set(long action, unsigned long time, unsigned long device)
	{
		action_ = action;
		value_ = 0;
		hasValue_ = false;
		absolute_ = false;
		time_ = time;
		device_ = device;
	}

	void set(long action, double value, bool absolute, unsigned long time, unsigned long device)
	{
		action_ = action;
		value_ = value;
		hasValue_ = true;
		absolute_ = absolute;
		time_ = time;
		device_ = device;
	}

	// назначенное пользователем действие
	long getAction() const { return action_; }
	void setAction(long action) { action_ = action; }
	bool hasValue() const { return hasValue_; }

	// состояние устройства
	double getValue() const { return value_; }
	void setValue(double value) { value_ = value; }
	bool isAbsolute() const { return absolute_; }

	// время возникновения комбинации (из DirectInput)
	unsigned long getTime() const { return time_; }
	unsigned long getDevice() const { return device_; }

	bool operator<(const InputAction& other) const
	{
		return time_ < other.time_; 
	}

private:
	long action_;
	double value_;
	bool hasValue_;
	bool absolute_;
	unsigned long time_;
	unsigned long device_;
};