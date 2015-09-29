#pragma once

#include "Avionics/avDevice.h"
#include "ICommandsCockpitDevice.h"

namespace cockpit {

	enum enum_Accelerometer_ClkCommands
	{	
		Accelerometer_ClkCmd_Reset = iCommandCockpitDevice_Action_1
	};

	class COCKPITBASE_API avMechanicAccelerometer : public avDevice
	{
	public:
		avMechanicAccelerometer();
		~avMechanicAccelerometer();

		virtual void initialize(unsigned char, const ed::string&, const ed::string&);
		virtual void post_initialize();
		virtual void update();
		virtual void SetCommand(int command, float value /* = 0 */);
		//////////////////////////////////////////////////////////////////////////
		double get_gauge_acceleration() const { return current_acceleration_; }
		double get_min_achieved_acc() const { return min_achieved_acc_; }
		double get_max_achieved_acc() const { return max_achieved_acc_; }

	protected:
		virtual double get_sensor_G() const = 0; 
	private:

		double current_acceleration_;
		double min_achieved_acc_;
		double max_achieved_acc_;

		double acceleration_needle_min_nominal_;
		double acceleration_needle_max_nominal_;

		double acceleration_needle_min_;
		double acceleration_needle_max_;

		double acceleration_needle_J_;
		double acceleration_needle_D_;
		double acceleration_needle_C_;

		double accelerometer_reset_value_;

		double accel_cage_K_;
		double accel_return_K_;

		double accelerometer_dy_dt_;

		bool accel_reset_;
	};

}