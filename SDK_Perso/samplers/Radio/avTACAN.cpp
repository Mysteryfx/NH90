#include "stdafx.h"

#include "Avionics/Radio/avTACAN.h"
#include "ccModuleManager.h"
#include "ccUnits.h"
#include "Physic/wRadioTransmitter.h"
#include "Lua/Config.h"
#include "IwoLA.h"
#include "MapCoords.h"
#include "MagVar/WMM_Interface.h"
#include "cAlgorithm.h"
#include "Utilities/ccUtilities.h"

class IwoLA;

using namespace cockpit;

avTACAN::avTACAN() : bearingFunctor(avRadioMeasureEquipment::CheckSignal(SIGNAL_TACAN_BEARING, 5.0)),
					 rangeFunctor(avRadioMeasureEquipment::CheckSignal(SIGNAL_TACAN_RANGE | SIGNAL_TACAN_X, 5.0)),
					 ElecConDeviceAC(EagleFM::Elec::AC_115_VOLT),
					 ElecConDeviceDC(EagleFM::Elec::DC_26_VOLT)
{
	magneticBearing = 0;
	range = 0;
	bearingValid = false;
	rangeValid = false;
	
	channelTens = 0;
	channelOnes = 0;
	channelXYMode = TACAN_X_CHANNEL;
	modeDial = TACAN_MD_OFF;
	
	bearingTrackTime = 3;
	rangeTrackTime = 15;
	bearingTrackStarted = 0;
	rangeTrackStarted = 0;
	is_test = false;
	test_light = false;

	transmitter_failure	= false;
	receiver_failure	= false;
}

avTACAN::~avTACAN()
{
}

void avTACAN::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avDevice::initialize(ID, Name, script_name);
	make_default_activity();

	avBreakable::load_from_state(ensured_Lua());
	avBreakable::register_as_breakable();
}

void avTACAN::post_initialize()
{
	avRadioMeasureEquipment::Receiver & rec = TACAN_receiver_.addReceiver(TACAN_RECEIVER);
	TACAN_receiver_.addMeasurement(TACAN_MEASUREMENT_BEARING, TACAN_RECEIVER,	&bearingFunctor);
	TACAN_receiver_.addMeasurement(TACAN_MEASUREMENT_DISTANCE, TACAN_RECEIVER,	&rangeFunctor);
	
	rec.radioReceiver.setName("Base TACAN receiver");
	rec.radioReceiver.setBody(aircraft().human_ptr.Obj());
	rec.radioReceiver.setNoiseLevel(3.5E-6);
	rec.radioReceiver.setBandWidth(5000.0f);

	Lua::Config config(ensured_Lua());

	receiver_.initialize("avTACAN", config, &rec.radioReceiver);

	receiver_.postInitialize();
	receiver_.setModulation(MODULATION_AM);
	receiver_.setSquelchOnOff(true);
}

void avTACAN::connectElecPower(Elec::ItemBase& ElecInputDC, Elec::ItemBase& ElecInputAC)
{
	ElecInputAC >> power_sw_AC_ >> ElecConDeviceAC;
	ElecInputDC >> power_sw_DC_ >> ElecConDeviceDC;

	ElecConDeviceAC.m_signalPowerOn += lwl::delegate(this,&avTACAN::onPowerOn);
	ElecConDeviceAC.m_signalPowerOff += lwl::delegate(this,&avTACAN::onPowerOff);

	ElecConDeviceDC.m_signalPowerOn += lwl::delegate(this,&avTACAN::onPowerOn);
	ElecConDeviceDC.m_signalPowerOff += lwl::delegate(this,&avTACAN::onPowerOff);
}

void avTACAN::release()
{
	receiver_.release();
}

void avTACAN::update()
{
	if (!getElecPower())
	{
		bearingValid = false;
		rangeValid = false;
		magneticBearing = getStowedBearing();
		range = 0;
		test_light = false;
		return;
	}

	if(is_test)
	{
		if (wTime::GetModelTime() - start_test < 1.0)
			test_light = true;
		else
		{
			if (receiver_failure)
				test_light = true;
			else if (transmitter_failure &&
				(modeDial == TACAN_MD_TR || modeDial == TACAN_MD_AATR))
				test_light = true;
			else
				test_light = false;
		}

		if (wTime::GetModelTime() - start_test < 7.0)
		{
			magneticBearing = Pi * 1.5;
			bearingValid = false;
			range = 0.0;
			rangeValid = false;
			return;
		}

		if ((wTime::GetModelTime() - start_test >= 7.0) && (wTime::GetModelTime() - start_test < 15.0))
		{
			magneticBearing = Pi;
			bearingValid = true;
			range = 0.0;
			rangeValid = true;
			return;
		}

		if (wTime::GetModelTime() - start_test >= 15.0)
		{
			static double undefined_bearing = 180.0;
			undefined_bearing += 30 * p_device_timer->GetDeltaTime();
			magneticBearing = fmod(undefined_bearing, 360.0) / 57.3;

			bearingValid = false;
			range = 0.0;
			rangeValid = false;
			return;
		}

		return;
	}

	TACAN_receiver_.update();
	receiver_.update();

	// Magnetic bearing
	if (!receiver_failure)
	{
		Parameter brgPar = TACAN_receiver_.getMeasurement(TACAN_MEASUREMENT_BEARING);
		if (bool(brgPar) != bearingValid)
		{
			// start bearing tracking on signal lost for bearingTrackTime seconds
			if (!brgPar)
			{
				if (bearingTrackStarted + bearingTrackTime < wTime::GetModelTime())
				{
					bearingTrackStarted = wTime::GetModelTime();
				}
				else
				{
					bearingValid = false;
					magneticBearing = 0;
				}
			}
		}

		if (brgPar)
		{
			double brn = brgPar;
			bearingValid = true;
			magneticBearing = RAD_TO_2PI(brn - Pi);
		}
		else
		{
			static double undefined_bearing = 0.0;
			undefined_bearing += 30 * p_device_timer->GetDeltaTime();
			magneticBearing = fmod(undefined_bearing, 360.0) / 57.3;
		}
	}
	else
	{
		bearingValid = false;
		magneticBearing = 0;
	}
	
	// Range
	if (!receiver_failure && !transmitter_failure)
	{
		Parameter rngPar;
		if(modeDial == TACAN_MD_TR || modeDial == TACAN_MD_AATR)
			rngPar = TACAN_receiver_.getMeasurement(TACAN_MEASUREMENT_DISTANCE);
		else
		{
			rangeValid = false;
			range = 0;
			return;
		}

		if (bool(rngPar) != rangeValid)
		{
			// start range tracking on signal lost for rangeTrackTime seconds
			if (!rngPar)
			{
				if (rangeTrackStarted + rangeTrackTime < wTime::GetModelTime())
					rangeTrackStarted = wTime::GetModelTime();
				else
				{
					rangeValid = false;
					range = 0;
				}
			}
		}

		if (rngPar)
		{
			double rng = rngPar;
			rangeValid = true;
			range = ccUConverter::instance().MetersToNM(rng);
		}
	}
	else
	{
		rangeValid = false;
		range = 0;
	}
}

bool avTACAN::isBearingValid()
{
	return bearingValid && (modeDial != TACAN_MD_OFF);
}

bool avTACAN::isRangeValid()
{
	if(is_test)
		return rangeValid;
	else
	    return rangeValid && (modeDial == TACAN_MD_TR || modeDial == TACAN_MD_AATR);
}

void avTACAN::onChannelChanged()
{
	double freq = getReceiverFrequency();
	receiver_.setFrequency(freq);
	bearingValid = false;
	magneticBearing = getStowedBearing();
	rangeValid = false;
	range = 0;

	switch(modeDial)
	{
	  case TACAN_MD_REC:
	  case TACAN_MD_TR:
		  {		
			 avRadioMeasureEquipment::Range r_(avRadioMeasureEquipment::CheckSignal(SIGNAL_TACAN_RANGE | 
				(channelXYMode == TACAN_Y_CHANNEL? SIGNAL_TACAN_Y : SIGNAL_TACAN_X), 5.0));
		     rangeFunctor = r_;
		  }
		  break;
	  case TACAN_MD_AAREC:
	  case TACAN_MD_AATR: 
		  {
			  avRadioMeasureEquipment::Range r_(avRadioMeasureEquipment::CheckSignal(SIGNAL_TACAN_RANGE_AA | 
				  (channelXYMode == TACAN_Y_CHANNEL? SIGNAL_TACAN_Y : SIGNAL_TACAN_X), 5.0));
			  rangeFunctor = r_;
		  }
		  break;
	}
	IwoLA *la = c_LA();
	if (la)
	{
		wRadioSignals signals = SIGNAL_TACAN_RANGE_AA | (channelXYMode == TACAN_Y_CHANNEL ? SIGNAL_TACAN_Y : SIGNAL_TACAN_X);
		la->send_tacan_status_message(modeDial == TACAN_MD_AATR, signals, getTransmitterFrequency());
	}
}

double avTACAN::getReceiverFrequency()
{
	int channel = channelTens * 10 + channelOnes;
	
	double freq = 0.000001; // in MHz
	if (channel == 0)
		return freq;

	if (modeDial == TACAN_MD_AAREC || modeDial == TACAN_MD_AATR || channelXYMode == TACAN_Y_CHANNEL)
	{
		if (channel < 64)
			freq = 1088 + channel - 1;
		else
			freq = 1025 + channel - 64;
	}
	else
	{
		if (channel < 64)
			freq = 962 + channel - 1;
		else
			freq = 1151 + channel - 64;
	}

	return freq * 1000000.0; // return in Hz
}

double avTACAN::getTransmitterFrequency()
{
	int channel = channelTens * 10 + channelOnes;

	double freq = 0.000001; // in MHz
	if (channel == 0)
		return freq;

	if(modeDial == TACAN_MD_AAREC || modeDial == TACAN_MD_AATR)
		return (
		        channel < 64 
		        ?(1025 + (channel + 63) - 64)
				:(1088 + (channel - 63) - 1)
	            ) * 1000000.0;
	else
		return getReceiverFrequency();
}

double avTACAN::getStowedBearing()
{
	return Pi / 2;
}

bool avTACAN::set_damage(unsigned Failure_ID, const bool can_be_rejected)
{
	//Если нет электричества, то не ломаемся
	if (can_be_rejected && !getElecPower())
		return false;

	switch (Failure_ID)
	{
	case TACAN_FAILURE_TOTAL:
		setflag_failure(true);
		ElecConDeviceDC.setFailure(true);
		ElecConDeviceAC.setFailure(true);
		break;
	case TACAN_FAILURE_TRANSMITTER:
		transmitter_failure	= true;
		break;
	case TACAN_FAILURE_RECEIVER:
		receiver_failure	= true;
		transmitter_failure	= true;
		break;
	}

	return true;
}

void avTACAN::OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params)
{ 
}

void avTACAN::repair()
{
	setflag_failure(false);
	ElecConDeviceDC.setFailure(false);
	ElecConDeviceAC.setFailure(false);
	receiver_failure	= false;
	transmitter_failure	= false;

	avBreakable::repair();
}

void avTACAN::onPowerOn(const lwl::CSignalInfo* pInfo)
{
	set_receivers_state(true);
}

void avTACAN::onPowerOff(const lwl::CSignalInfo* pInfo)
{
	set_receivers_state(false);
}

void avTACAN::switch_power(bool enable)
{
	power_sw_AC_.setOnOff(enable);
	power_sw_DC_.setOnOff(enable);
}

void avTACAN::set_receivers_state(bool enable)
{
	receiver_.setOnOff(enable);
	TACAN_receiver_.setOnOff(enable); 
}