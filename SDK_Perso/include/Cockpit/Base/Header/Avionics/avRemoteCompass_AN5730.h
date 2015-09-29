#pragma once

/************************************************************************/
/* Remote magnet compass indicator                          */
/************************************************************************/
#include "CockpitBase.h"
#include "Avionics/avRemoteMagnetCompass.h"
#include "ICommandsCockpitDevice.h"
#include "avMechanics.h"

namespace cockpit
{
	class COCKPITBASE_API avRemoteCompass_AN5730 : public avRemoteMagnetCompass
	{
		enum avRemoteCompass_AN5730_Commands
		{
			AN5730_Course = iCommandCockpitDevice_Action_1,
			AN5730_Course_Ext,
		};

	public:
		avRemoteCompass_AN5730();
		virtual ~avRemoteCompass_AN5730();

		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
		virtual bool checkAnimationConditions(int arg_num, int command, float& value);
		virtual void SetCommand(int command, float value /* = 0 */);
		virtual void update();
		//end of interface

		double get_heading() const { return heading.get_value_out(); }	
		double get_commanded_course() const { return commanded_course.get_value_out(); }	
		double getCourseKnobVal() const;

	protected:
	private:
		gauge_periodic   	commanded_course;
		gauge_periodic		heading;
		gauge_periodic		commandedCourseKnob;
		
		double              manual_commanded_course;
		double              knobK;
	};
}