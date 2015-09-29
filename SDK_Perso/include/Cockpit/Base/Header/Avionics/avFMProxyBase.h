#ifndef AV_FM_PROXYBASE_H 
#define AV_FM_PROXYBASE_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "CockpitBase.h"
#include "Avionics/avDevice.h"

namespace EagleFM
{
	class SensorsSystem;
	class ControlSystem;
}

class IwHumanSensor;

namespace cockpit {

	struct COCKPITBASE_API pilot_controls
	{ 
		double stick_Roll;
		double stick_Pitch;
		double rudder_Pedals;
	};

	class COCKPITBASE_API avFMProxyBase : public avDevice
	{
	protected:
		avFMProxyBase();
	public:
		virtual ~avFMProxyBase();

		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
		virtual void post_initialize();
		//

		double get_sensor_output(int sensor_id) const;
		double get_barometric_altitude(int sensor_id = -1) const;
		double get_vertical_speed(int sensor_id = -1) const;
		double get_IAS(int sensor_id = -1) const;
		double get_Mach(int sensor_id = -1) const;

		double* get_data_ptr(const ed::string& name);

		virtual double get_base_atmo_pressure_hg(unsigned char alt_ID = 0) const;
		virtual void   set_atmo_pressure_hg(double pressure, unsigned char alt_ID = 0);

		
		virtual const pilot_controls* get_controls() = 0;
		virtual double get_angular_rate_Y() const = 0;
		virtual dVector	get_G() const;
		virtual float get_atmo_pressure() const; /*In P*/

		double       get_wheelbrake(int what) const;

	protected:

		virtual double get_Barometric_Altitude_() const;
		virtual double get_Vertical_Speed_() const;
		virtual double get_IAS_() const;
		virtual double get_Mach_() const;

		bool native_FM;
		EagleFM::SensorsSystem*  sensor_system;
		EagleFM::ControlSystem*  control_system;
		IwHumanSensor * s_;
	};
}

#endif