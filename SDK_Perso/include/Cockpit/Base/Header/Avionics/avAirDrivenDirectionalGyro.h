#pragma once

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "Avionics/Sensors/avYawGyro.h"


namespace EagleFM
{
	class DynamicBody;
}

namespace cockpit
{
	class AirDrivenGyroSound;

	class COCKPITBASE_API avAirDrivenDirectionalGyro : public avDevice, 
									   public avBreakable
	{
		enum DirectionalGyroFailures
		{
			DEFECTIVE_INSTRUMENT = 0,
			DEFECTIVE_MECHANISM
		};

	public:
		avAirDrivenDirectionalGyro();
		virtual ~avAirDrivenDirectionalGyro();

		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
		virtual void post_initialize();
		virtual void update();
		//end of interface

		//avBreakable interface
		virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
		virtual void repair();
		//end of interface

		virtual double get_heading() const {return TrueHeading_;}
		void set_power(float pressure /*in. Hg*/);

	protected:

		void		 initSound();
		virtual Sound::Host* initSoundHost(); 
		void		 update_sound();

		EagleFM::DynamicBody* pDBody;
		avYawGyro gyro;

		bool caged_;
		double TrueHeading_;
		double TrueHeadingLast_;

		float relative_drift_;
		float fail_relative_drift_;
		int   fail_spin_direction_;
		float slight_drift_; // 2 degr in 15-20 min (take 18)

		float max_falure_drift_;

		AirDrivenGyroSound* sound_;

	private:
	};
}