#pragma once

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"

#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"
#include "Physic/avRadioMeasureEquipment.h"

#include "Avionics/Radio/avReceiver.h"

using namespace EagleFM;

namespace cockpit {

enum enum_TACAN_CHANNEL_XY_MODE
{
	TACAN_X_CHANNEL = 0,
	TACAN_Y_CHANNEL
};

enum enum_TACAN_MODE_DIAL
{
	TACAN_MD_OFF = 0,
	TACAN_MD_REC,
	TACAN_MD_TR,
	TACAN_MD_AAREC,
	TACAN_MD_AATR
};

enum enum_TACAN_RECEIVERS
{
	TACAN_RECEIVER = 0
};

enum enum_TACAN_MEASUREMENTS
{
    TACAN_MEASUREMENT_BEARING = 0,
    TACAN_MEASUREMENT_DISTANCE,
};

enum enum_TACAN_FAILURES{
	TACAN_FAILURE_TOTAL,
	TACAN_FAILURE_TRANSMITTER,
	TACAN_FAILURE_RECEIVER,
};

class COCKPITBASE_API avTACAN :  public avDevice,
								public avBreakable
{
public:
	avTACAN();
	virtual ~avTACAN();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void release();
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
	//end of interface

	//avBreakable interface
	virtual bool	set_damage(unsigned Failure_ID, const bool can_be_rejected = false);
	virtual void	repair();
	//end of interface

	virtual void OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params);	

	Sounder::Element & getSoundElement() { return receiver_.getSoundElement(); }

	virtual bool getElecPower() const {return (ElecConDeviceDC.isPowered()) 
		&& (ElecConDeviceAC.isPowered()) 
		&& modeDial != TACAN_MD_OFF; }

	void connectElecPower(Elec::ItemBase& ElecInputDC, Elec::ItemBase& ElecInputAC);

	bool isBearingValid();
	double getMagneticBearing() {return magneticBearing;}
	bool isRangeValid();
	double getRange() { return range; }
	
	bool isTest() const {return is_test;}

protected:
	void setChannelTens(int channel) { if (channelTens == channel) return; channelTens = channel; onChannelChanged(); }
	void setChannelOnes(int channel) { if (channelOnes == channel) return; channelOnes = channel; onChannelChanged(); }
	void onChannelChanged();
	double getReceiverFrequency();
	double getTransmitterFrequency();
	double getStowedBearing();

protected:

	virtual void onPowerOn(const lwl::CSignalInfo* pInfo);
	virtual void onPowerOff(const lwl::CSignalInfo* pInfo);

	void switch_power(bool enable);
	void set_receivers_state(bool enable);

	Elec::Switch		power_sw_AC_;
	Elec::Switch		power_sw_DC_;
	Elec::Consumer		ElecConDeviceDC;
	Elec::Consumer		ElecConDeviceAC;

	avReceiver			receiver_;

	double magneticBearing;
	double range;
	bool bearingValid;
	bool rangeValid;
	double bearingTrackStarted;
	double rangeTrackStarted;
	double bearingTrackTime;
	double rangeTrackTime;
	avRadioMeasureEquipment TACAN_receiver_;
	avRadioMeasureEquipment::MagneticBearing bearingFunctor;
	avRadioMeasureEquipment::Range rangeFunctor;
	int channelTens;
	int channelOnes;
	int channelXYMode;
	
	int modeDial;
	bool is_test;
	bool test_light;
	wModelTime start_test;

	bool		transmitter_failure;
	bool		receiver_failure;
};

}
