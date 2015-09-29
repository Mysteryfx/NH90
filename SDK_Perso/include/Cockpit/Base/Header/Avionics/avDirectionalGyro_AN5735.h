#pragma once

/************************************************************************/
/* Sperry Air Driven Directional Gyro        
Power: 4.5 to 5.2 in. Hg Vac. or Pressure
*/
/************************************************************************/
#include "CockpitBase.h"
#include "Avionics/avAirDrivenDirectionalGyro.h"
#include "avMechanics.h"

#include "ICommandsCockpitDevice.h"

namespace cockpit
{
	class COCKPITBASE_API avDirectionalGyro_AN5735 : public avAirDrivenDirectionalGyro
	{
		typedef enum AN5735_commands
		{
			AN5735_HEADING_ADJUST = iCommandCockpitDevice_Action_1,
			AN5735_HEADING_ADJUST_EXT,
			AN5735_CAGE,
			AN5735_CAGE_EXT
		};

	public:
		avDirectionalGyro_AN5735();
		virtual ~avDirectionalGyro_AN5735();

		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
		virtual void post_initialize();
		virtual void SetCommand(int command, float value /* = 0 */);
		virtual void update();
		//
		virtual double get_heading() const {return heading.get_value_out();}

	protected:
	private:
		gauge_periodic heading;
		double mag_var_;
	};
}