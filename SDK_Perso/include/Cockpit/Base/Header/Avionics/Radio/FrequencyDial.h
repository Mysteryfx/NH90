#ifndef FREQUENCY_DIAL_H
#define FREQUENCY_DIAL_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Utilities/ccUtilities.h"

namespace Lua { class Config; };

namespace cockpit {

class FrequencyDial
/*!!!obsolete class. Used for avVHF_ARC_186 */
{
public:
	FrequencyDial() 
		: arg_(-1), tr_k_(1.0f), value_(0){}

	FrequencyDial(int arg, float tr_k) 
		: arg_(arg), tr_k_(tr_k), value_(0){}

	int		arg_;
	float	tr_k_; /*convert freq value to arg value*/
	char	value_;

	float arg_val() const { return  (float)value_/tr_k_; }
};

/************************************************************************/
/* Model depended frequency dial                                                                     */
/************************************************************************/
class FrequencyDialDigit
{
public:
	FrequencyDialDigit() 
		: arg_(-1), kD_(1.0f), dig_limit_(0,10), digit_(0), mult_(1) {}

	int		arg_;	     /*model argument*/
	float	kD_;		 /*(arg_val_ * kD_) convert model arg value to digit*/

	limits<short> dig_limit_;
	short	digit_;		 /*current digit*/
	int		mult_;		 /*convert digit to Hertz*/
	short	shift_;

	float get_arg() const { return  (float)(digit_ - shift_)/kD_; }
	int	  get_Hz() const { return digit_*mult_; }

	void  to_digit(float arg)  { 

		digit_ = dig_limit_.cycle_in_limit_s(shift_ + cockpit::round(arg * kD_));
	}
	short  to_digit(float arg) const { return dig_limit_.cycle_in_limit_s(shift_ + cockpit::round(arg * kD_)); }
};

class avDevice;

class COCKPITBASE_API FrequencyDial_Model
{
public:

	explicit FrequencyDial_Model(avDevice* parent, unsigned char dig_count);

	void l_read(Lua::Config& config, const char* name);

	void update_frequency(unsigned char dig);

	bool compare(unsigned char dig, float arg) const ;

	void set_frequency();
	void force_set_frequency(int frequency); 

	int get_frequency() const { return frequency_; }
	float get_arg(unsigned char dig) const;

	bool next_frequency_valid(unsigned char dig, int step) const;
	int get_indicated_value(unsigned char dig_start, unsigned char dig_end) const;

protected:
	ed::vector<FrequencyDialDigit> frequency_dial_;
	avDevice* parent_;
	int frequency_;

	limits_i limit_;
};

}
#endif