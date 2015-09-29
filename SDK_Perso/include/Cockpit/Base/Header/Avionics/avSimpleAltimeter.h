#pragma once

/************************************************************************/
/* Barometric altimeter                                                 */
/************************************************************************/

#include "CockpitBase.h"

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "Avionics/avMechanics.h"

#include "ICommandsCockpitDevice.h"

namespace cockpit 
{
	class avFMProxyBase;

	class COCKPITBASE_API avSimpleAltimeter : public avDevice, public avBreakable
	{
		enum Altimeter_Commands
		{
			CMD_ADJUST_PRESSURE =  iCommandCockpitDevice_Action_1,
			CMD_ADJUST_PRESSURE_EXT
		};

	public:
		avSimpleAltimeter();

		virtual ~avSimpleAltimeter();

		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
		virtual void post_initialize();
		virtual void update();
		virtual void SetCommand(int command, float value /* = 0 */);
		virtual bool checkAnimationConditions(int arg_num, int command, float& value);
		//

		double get_altitude() const { return gauge_altitude_.get_value_out(); }
		double get_altitude_MSL() const { return alt_MSL_; }

		virtual double	get_base_atmo_pressure_hg() const; /*mm mercury*/
	
	protected:
	
		avSimpleAltimeter(int Sensor_Id, unsigned char Altimeter_Id);

		DECLARE_DEV_LINKPTR(avFMProxyBase,FM_Proxy);

		virtual double	get_sensor_altitude() const;
		virtual void	set_base_atmo_pressure_hg(double pressure);
		virtual void	set_field_pressure();
	private:

		gauge_limited	gauge_altitude_;
		float			alt_MSL_;

		float	error_k_;
		float	min_pressure_;
		float	max_pressure_;

		int		sensor_id_;
		unsigned char altimeter_id_;
	};
	
}