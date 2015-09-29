#pragma once

/************************************************************************/
/* Sperry Air Driven Artificial Horizon
Power: 4.5 to 5.2 in. Hg Vac. or Pressure
*/
/************************************************************************/

#include "CockpitBase.h"
#include "Avionics/AttitudeIndicator/avArtificialHorizon.h"

#include "ICommandsCockpitDevice.h"
#include "Avionics/avMechanics.h"

namespace cockpit
{
	class COCKPITBASE_API avArtificialHorizont_AN5736 : public avArtificialHorizon
	{
		typedef enum AN5736_commands
		{
			AN5736_Cage = iCommandCockpitDevice_Action_1,
			AN5736_Pitch_zero_shift,
			AN5736_Cage_Rotate,
			AN5736_Pitch_zero_shift_Ext,
			AN5736_Cage_Ext,
			AN5736_Cage_Rotate_Ext,

#ifdef _ED_HIDDEN_DEBUG 
			AN5736_POWER_EXT
#endif
		};
	public:
		avArtificialHorizont_AN5736();
		virtual ~avArtificialHorizont_AN5736();

		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
		virtual void post_initialize();
		virtual void SetCommand(int command, float value /* = 0 */);
		virtual bool checkAnimationConditions(int arg_num, int command, float& value);
		virtual void update();
		//

		float get_pitch_shift() { return pitch_shift_; }
		float getAttFailure_flag() const {return AttFailure_flag.get_value_out();}

	protected:
	private:

		gauge_linear AttFailure_flag;

		float pitch_shift_;
		bool caged_;
		bool fixed_;
	};
}
