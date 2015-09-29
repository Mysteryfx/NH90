#pragma once

#include "Physic/avRadioMeasureEquipment.h"
#include "Avionics/Radio/avReceiver.h"

namespace cockpit
{
	class COCKPITBASE_API avVOR 
	{
	public:

		avVOR(double SNR);

		void initialize(const char * name_, Lua::Config& config);
		void release();

		void set_frequency(float frequency);
		void set_obs(float obs_bearing);
		void set_on_off(bool onOff);

		bool  get_TO_flag() const { return TO_flag_; }
		bool  get_FROM_flag() const { return FROM_flag_; }
		float get_bearing() const { return bearing_; }
		bool  get_bearing_valid() const {return bearing_valid_; }
		float get_deviation() const { return deviation_; }
		Sounder::Element & getSoundElement() { return receiver_.getSoundElement(); }

		void simulate();

		void set_antenna_damage(bool damage) { receiver_.damageAntenna(damage); }
	protected:

		float obs_bearing_;
		float bearing_;
		float deviation_;
		bool  bearing_valid_;

		bool TO_flag_;
		bool FROM_flag_;

		double bearingTrackStarted_;
		float bearingTrackTime_;

		avReceiver				receiver_;
		avRadioMeasureEquipment VOR_receiver_;
		avRadioMeasureEquipment::MagneticBearing bearing_functor_;
	};
}