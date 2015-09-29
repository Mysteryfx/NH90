#ifndef AIR_DRIVEN_TURN_IND_H 
#define AIR_DRIVEN_TURN_IND_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Avionics/avSimpleTurnSlipIndicator.h"

namespace cockpit
{
	class COCKPITBASE_API avAirDrivenTurnIndicator : 
									 public avSimpleTurnSlipIndicator
	{
	protected:
		enum TurnIndicatorFailures
		{
			NO_FAILURE  = -1,
			POINTER_FAILS_NO_VACUUM = 0,
			POINTER_FAILS_DEFECTIVE,
			INCORRECT_SENS_DEFECTIVE,
			INCORRECT_SENS_VAC_HIGH,
			INCORRECT_SENS_VAC_LOW,
			POINTER_NOT_SET_ZERO,
			POINTER_VIBRATES,
		};

	public:
		avAirDrivenTurnIndicator();
		virtual ~avAirDrivenTurnIndicator();

		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
		virtual void update();
		//end of interface

		//avBreakable interface
		virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
		virtual void repair();
		//end of interface

		void set_power(float pressure /*in. Hg*/);

	protected:

		void simulate_damage();

		int		failure_;
		float	failure_sens_K_;
		float	turn_needle_T2;
		float	failure_vacuum_;

	};
}

#endif