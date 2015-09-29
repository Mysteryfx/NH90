#pragma once

#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"

#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"
#include "Physic/avRadioMeasureEquipment.h"
#include "Avionics/Radio/avReceiver.h"

#include "Sound/Sounder.h"

using namespace EagleFM;

namespace cockpit {

enum enum_ILS_RECEIVERS
{
	ILS_RECEIVER_LOCALIZER,
	ILS_RECEIVER_GLIDESLOPE,
	ILS_RECEIVER_MARKER
};

enum enum_ILS_MEASUREMENTS
{
    ILS_MEASUREMENT_LOCALIZER = 0,
    ILS_MEASUREMENT_GLIDESLOPE,
	ILS_MEASUREMENT_DME,
	ILS_MEASUREMENT_MARKER,
};

enum enum_ILS_MARKERS
{
	ILS_MARKER_OUTER,
	ILS_MARKER_MIDDLE,
	ILS_MARKER_INNER,
	ILS_MARKER_MAX
};

enum enum_ILS_FAILURES{
	ILS_FAILURE_TOTAL,
	ILS_FAILURE_ANT_LOCALIZER,
	ILS_FAILURE_ANT_GLIDESLOPE,
	ILS_FAILURE_ANT_MARKER,
};

class COCKPITBASE_API avILS :  public avDevice,
								public avBreakable
{
public:
	static const int ILS_MARKER_FREQUENCY = 75000000;

	avILS();
	virtual ~avILS();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void release();
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
	virtual void OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params);
	//end of interface

	//avBreakable interface
	virtual bool	set_damage(unsigned Failure_ID, const bool can_be_rejected = false);
	virtual void	repair();
	//end of interface

	Sounder::Element& getLocalizerSoundElement() {return switcherLocalizer;}
	Sounder::Element& getMarkerBeaconsSoundElement() {return switcherMarkerBeacons;}

	virtual bool getElecPower() const {return ElecConDevice.isPowered();}
	virtual void connectElecPower(Elec::ItemBase& ElecInputILS, Elec::ItemBase& ElecMarkerBeacon);

	void setElecPower(bool onOff) { Power_Switch_.setOnOff(onOff); Beacon_Power_Switch_.setOnOff(onOff); }

	bool isLocalizerValid() { return localizerValid; }
	bool isGlideslopeValid() { return glideslopeValid; }
	double getLocalizerDeviation() { return localizerDeviation; }
	double getGlideslopeDeviation() { return glideslopeDeviation; }
	bool isMarkerBeacon() { return isOuterMarkerSignal() || isMiddleMarkerSignal() || isInnerMarkerSignal(); }
	bool isOuterMarkerSignal() const;
	bool isMiddleMarkerSignal() const;
	bool isInnerMarkerSignal() const;

	// ILS control panel
	void setFrequencyMHz(int freq) { if (frequencyMHz == freq) return; frequencyMHz = freq; onFrequencyChanged(); }
	void setFrequencyKHz(int freq) { if (frequencyKHz == freq) return; frequencyKHz = freq; onFrequencyChanged(); }

protected:

	void update_LOC();
	void update_Marker();

	Elec::Consumer		ElecConDevice;
	Elec::Consumer		MarkerBeacon_Consumer_;

	Elec::Switch		Power_Switch_;
	Elec::Switch		Beacon_Power_Switch_;

	Sounder::Switcher	switcherLocalizer;
	Sounder::Switcher	switcherMarkerBeacons;
	Sounder::Regulator	volumeLocalizer;
	Sounder::Regulator	volumeMarkerBeacons;
    Sounder::Source		source[ILS_MARKER_MAX];
	avReceiver			receiver;
	avReceiver			rcv_mark_beacon_;

	// localizer frequency parts
	int frequencyMHz;
	int frequencyKHz;

	double localizerDeviation;
	double glideslopeDeviation;
	bool localizerValid;
	bool glideslopeValid;
	int markerFlags;
	avRadioMeasureEquipment ILS_receiver;
	avRadioMeasureEquipment::BackAzimuth	backAzimuthFunctor;
	avRadioMeasureEquipment::BackElevation  backElevationFunctor;
	avRadioMeasureEquipment::Simple			markerILSFunctor[ILS_MARKER_MAX];

	ed::map<int, int> localizer_to_glideslope_map;

	void onFrequencyChanged();
	double getLocalizerFrequency();
	virtual double getGlideslopeFrequency(double localizerFreq);

private:

	virtual void onLOCPowerOn(const lwl::CSignalInfo* pInfo);
	virtual void onLOCPowerOff(const lwl::CSignalInfo* pInfo);

	virtual void onBeaconPowerOn(const lwl::CSignalInfo* pInfo);
	virtual void onBeaconPowerOff(const lwl::CSignalInfo* pInfo);

	void set_localizer_on_off(bool enable);
	void set_marker_beacon_on_off(bool enable);

	void init_frequencies_map();

	bool		marker_ant_failure;
	bool		glideslope_ant_failure;
	bool		localizer_ant_failure;
};

}
