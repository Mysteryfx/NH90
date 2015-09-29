#include "StdAfx.h"

#include "Avionics/Radio/avVOR.h"
#include "Lua/Config.h"
#include "ccCockpitContext.h"
#include "cAlgorithm.h"

using namespace cockpit;

#define  VOR_MEASUREMENT_BEARING 0 
#define  VOR_RECEIVER 0 
#define  COURSE_DEVIATION_MAX TO_RAD(10.0f)

avVOR::avVOR(double SNR)
	: bearing_functor_(avRadioMeasureEquipment::CheckSignal(SIGNAL_VOICE_AM | SIGNAL_VOR,SNR))
{
}

void avVOR::initialize(const char * name_, Lua::Config& config)
{
	bearing_valid_ = 
		TO_flag_ = 
		FROM_flag_ = false;

	bearing_ = 
		obs_bearing_ = 0.0f;

	bearingTrackStarted_ = 0.0;
	bearingTrackTime_ = 3.0;

	avRadioMeasureEquipment::Receiver& rec = VOR_receiver_.addReceiver(VOR_RECEIVER);
	VOR_receiver_.addMeasurement(VOR_MEASUREMENT_BEARING, VOR_RECEIVER, &bearing_functor_);
	rec.radioReceiver.setName("Base VOR receiver");
	rec.radioReceiver.setBody(aircraft().human_ptr.Obj());
	rec.radioReceiver.setNoiseLevel(3.5E-6);
	rec.radioReceiver.setBandWidth(34E3);

	receiver_.initialize(name_, config, &rec.radioReceiver);

	receiver_.postInitialize();
	receiver_.setModulation(MODULATION_AM);
	receiver_.setSquelchOnOff(true);
	receiver_.setVolume(1.0);

	receiver_.setOnOff(false);
}

void avVOR::release()
{
	receiver_.release();
}

void avVOR::set_frequency(float frequency)
{
	receiver_.setFrequency(frequency);
}

void avVOR::set_obs(float obs_bearing)
{
	obs_bearing_ = obs_bearing;
}

void avVOR::set_on_off(bool onOff)
{
	receiver_.setOnOff(onOff);

	bearing_ = 
	deviation_ = 0.0f;

	bearing_valid_ = 
	FROM_flag_ = 
	TO_flag_ = false;
}

void avVOR::simulate()
{
	FROM_flag_ = TO_flag_ = false;

	VOR_receiver_.update();
	receiver_.update();
	
	Parameter brgPar = VOR_receiver_.getMeasurement(VOR_MEASUREMENT_BEARING);

	if (!(bool)brgPar)
	{
		if (bearingTrackStarted_ == 0.0)
		{
			bearingTrackStarted_ = wTime::GetModelTime();
		}
		else if ((bearingTrackStarted_ + bearingTrackTime_) < wTime::GetModelTime())
		{
			bearing_valid_ = false;
		}
	}
	else
	{
		bearingTrackStarted_ = 0.0;
		bearing_valid_ = true;

		bearing_ = RAD_TO_2PI(float(brgPar) - Pi);
	}

	if (bearing_valid_)
	{
		deviation_= RAD_TO_2PI((bearing_ + Pi) - obs_bearing_);

		if (deviation_ > Pi)
			deviation_ = deviation_ - Pi_2;

		if (fabs(deviation_) > Pi_05)
		{
			deviation_ = (deviation_ > 0 ? 1 : -1 )*Pi - deviation_;
			TO_flag_ = deviation_ <= TO_RAD(85.0f);
		}
		else
		{
			FROM_flag_ = fabs(deviation_) <= TO_RAD(85.0f);
		}

		deviation_ = __min(__max(deviation_/COURSE_DEVIATION_MAX, -1.0f),1.0f);
	}
	else
	{
		deviation_ = 
		bearing_ = 0.0f;
	}
}
