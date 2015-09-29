#pragma once

#include "CockpitBase.h"

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "Avionics/avMechanics.h"

namespace cockpit {
	
	class avFMProxyBase;

	class COCKPITBASE_API avSimpleMachIndicator : public avDevice, public avBreakable
	{
		enum SimpleMachIndicator_failures
		{
			MACH_INDICATOR_FAILURE,
		};

	public:
		avSimpleMachIndicator();
		virtual ~avSimpleMachIndicator();

		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
		virtual void update();
		//

		//avBreakable interface
		virtual bool	set_damage(unsigned Failure_ID, const bool can_be_rejected = false);
		virtual void	repair();
		//end of interface

		float get_Mach() const { return gauge_mach_.get_value_out(); }
	protected:
		avSimpleMachIndicator(int sensor_id_);
		virtual double get_sensor_Mach() const;
	private:

		DECLARE_DEV_LINKPTR(avFMProxyBase,FM_Proxy);

		gauge_limited	gauge_mach_;

		float			error_k_;

		int				sensor_id_;
	};

}