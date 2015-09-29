#ifndef SIMPLE_TURN_SLIP_IND_H 
#define SIMPLE_TURN_SLIP_IND_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "avMechanics.h"
#include "Avionics/avSlipBall.h"
#include "Avionics/Sensors/avGyroPowerSource.h"

namespace cockpit
{
	
class GyroPowerSource;
class avFMProxyBase;

class COCKPITBASE_API avSimpleTurnSlipIndicator : public avDevice, 
	public avBreakable
{

protected:
	avSimpleTurnSlipIndicator(GyroPowerSrcPtr power_source);

public:
	virtual ~avSimpleTurnSlipIndicator();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void update();
	//end of interface

	//avBreakable interface
	virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
	virtual void repair();
	//end of interface

	virtual double get_turn()	  const { return turn_needle.get_value_out(); }
	virtual double get_sideslip() const { return SlipBall.get_sideslip(); }

protected:

	DECLARE_DEV_LINKPTR(avFMProxyBase, FM_Proxy);

	GyroPowerSrcPtr power_source_;

	avSlipBall SlipBall;
	gauge_limited turn_needle;
};

}

#endif