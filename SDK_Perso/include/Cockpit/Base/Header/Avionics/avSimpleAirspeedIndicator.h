#pragma once

#include "CockpitBase.h"

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "Avionics/avMechanics.h"

namespace cockpit {
	
	class avFMProxyBase;

	class COCKPITBASE_API avSimpleAirspeedIndicator : public avDevice, public avBreakable
	{
		enum SimpleAirspeedIndicator_failures
		{
			AIRSPEED_INDICATOR_FAILURE,
		};

	public:
		avSimpleAirspeedIndicator();
		virtual ~avSimpleAirspeedIndicator();

		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
		virtual void update();
		//

		//avBreakable interface
		virtual bool	set_damage(unsigned Failure_ID, const bool can_be_rejected = false);
		virtual void	repair();
		//end of interface

		float get_IAS() const { return gauge_ias_.get_value_out(); }
	protected:
		avSimpleAirspeedIndicator(int sensor_id_);
		virtual float get_sensor_IAS() const;
	private:

		DECLARE_DEV_LINKPTR(avFMProxyBase,FM_Proxy);

		gauge_limited	gauge_ias_;

		float			error_k_;

		int				sensor_id_;
	};

}