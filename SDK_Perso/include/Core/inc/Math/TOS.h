#ifndef _ED_Math_TOS_h_
#define _ED_Math_TOS_h_

#include "../_config.h"

#ifndef ED_CORE_EXPORT
	#define ED_CORE_EXPORT EDSIM_DLL_IMPORT
#endif // ED_CORE_EXPORT

//Time-out-switcher

//One-way

class OWTOS
{
public:
	ED_CORE_EXPORT OWTOS(	bool state,
							double switchTime,
							double timeOutFalseToTrue);
	ED_CORE_EXPORT void reset(double switchTime);
	ED_CORE_EXPORT bool update(bool state, double time);
	inline bool getState() const { return state_ == STATE_TRUE; }
private:
	enum State
	{
		STATE_FALSE,
		STATE_FALSE_TIME_OUT,
		STATE_TRUE,
	};
	State state_;
	double switchTime_;
	double timeOutFalseToTrue_;
};

//Two-way

class TWTOS
{
public:
	ED_CORE_EXPORT TWTOS(	bool state,
							double switchTime,
							double timeOutTrueToFalse,
							double timeOutFalseToTrue);
	ED_CORE_EXPORT void reset(bool state, double switchTime);
	ED_CORE_EXPORT bool update(bool value, double time);
	inline bool getState() const { return state_ == STATE_TRUE || state_ == STATE_TRUE_TIMEOUT; }
private:
	enum State
	{
		STATE_FALSE,
		STATE_FALSE_TIME_OUT,
		STATE_TRUE,
		STATE_TRUE_TIMEOUT
	};
	State state_;
	double switchTime_;
	double timeOutTrueToFalse_;
	double timeOutFalseToTrue_;
};

#endif _ED_Math_TOS_h_