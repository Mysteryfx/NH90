#include "stdafx.h"

#include "Avionics/Radio/avILS.h"
#include "ccModuleManager.h"
#include "Physic/wRadioTransmitter.h"
#include "Lua/Config.h"
#include "ccSound.h"

using namespace cockpit;

REGISTER_DYNAMIC_FACTORY(cockpit::avILS, avILS);

const double innerNoise = 3.0E-6 / (pow(10.0, 10.0 / 20.0) - 1.0); //V
const double localizerMinSignal = 40E-6; //V/m
const double glideslopeMinSignal = 1.5E-3; //V/m
const double markerMinSignal = 1.5E-3; //V/m

static const wRadioSignals markerSignals[ILS_MARKER_MAX] = { SIGNAL_OUTER_MARKER, SIGNAL_MIDDLE_MARKER, SIGNAL_INNER_MARKER };

void avILS::onLOCPowerOn(const lwl::CSignalInfo* pInfo)
{
	set_localizer_on_off(true);
}

void avILS::onLOCPowerOff(const lwl::CSignalInfo* pInfo)
{
	set_localizer_on_off(false);
}

void avILS::onBeaconPowerOn(const lwl::CSignalInfo* pInfo)
{
	set_marker_beacon_on_off(true);
}

void avILS::onBeaconPowerOff(const lwl::CSignalInfo* pInfo)
{
	set_marker_beacon_on_off(false);
}

avILS::avILS() : backAzimuthFunctor(avRadioMeasureEquipment::CheckSignal(SIGNAL_ILS_LOCALIZER,
									(localizerMinSignal + innerNoise) / innerNoise)),
				 backElevationFunctor(avRadioMeasureEquipment::CheckSignal(SIGNAL_ILS_GLIDESLOPE,
									(glideslopeMinSignal + innerNoise) / innerNoise)),
				 ElecConDevice(EagleFM::Elec::DC_26_VOLT), 
				 MarkerBeacon_Consumer_(EagleFM::Elec::DC_26_VOLT)
{
	
	frequencyMHz = 108;
	frequencyKHz = 100;

	markerFlags = 0;
	localizerValid = false;
	glideslopeValid = false;
	marker_ant_failure		= false;
	glideslope_ant_failure	= false;
	localizer_ant_failure	= false;
	const double markerSNR = (markerMinSignal + innerNoise) / innerNoise;
	for(int m = ILS_MARKER_OUTER; m < ILS_MARKER_MAX; m++)
		markerILSFunctor[m] = avRadioMeasureEquipment::CheckSignal(markerSignals[m], markerSNR);
}

avILS::~avILS()
{
}

void avILS::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avDevice::initialize(ID, Name, script_name);
	make_default_activity();

	avBreakable::load_from_state(ensured_Lua());
	avBreakable::register_as_breakable();

	//////////////////////////////////////////////////////////////////////////
	init_frequencies_map();

	wRadioReceiver & locRec = ILS_receiver.addReceiver(ILS_RECEIVER_LOCALIZER, ILS_MEASUREMENT_LOCALIZER, &backAzimuthFunctor).radioReceiver;
	locRec.setName("Base ILS localizer receiver");
	locRec.setBody(aircraft().human_ptr.Obj());
	locRec.setNoiseLevel(innerNoise);
	locRec.setBandWidth(300.0f);

	Lua::Config config(ensured_Lua());
	receiver.initialize("avILS", config, &locRec);
	receiver.setModulation(MODULATION_AM);
	receiver.setSquelchOnOff(true);

	wRadioReceiver & gsRec = ILS_receiver.addReceiver(ILS_RECEIVER_GLIDESLOPE, ILS_MEASUREMENT_GLIDESLOPE, &backElevationFunctor).radioReceiver;
	gsRec.setName("Base ILS glideslope receiver");
	gsRec.setBody(aircraft().human_ptr.Obj());
	gsRec.setNoiseLevel(innerNoise);
	gsRec.setBandWidth(300.0f);	

	wRadioReceiver & markerRec = ILS_receiver.addReceiver(ILS_RECEIVER_MARKER).radioReceiver;
	markerRec.setName("Base ILS marker receiver");
	markerRec.setMaxDistance(12000.0f);
	markerRec.setBody(aircraft().human_ptr.Obj());
	markerRec.setNoiseLevel(innerNoise);
	markerRec.setBandWidth(3000.0f);	
	markerRec.setFrequency(75000000.0f);

	for(int m = ILS_MARKER_OUTER; m < ILS_MARKER_MAX; m++)
		ILS_receiver.addMeasurement(ILS_MEASUREMENT_MARKER + m, ILS_RECEIVER_MARKER, &markerILSFunctor[m]);
}

void avILS::post_initialize()
{
	receiver.postInitialize();
	onFrequencyChanged();

	ElecConDevice.m_signalPowerOn += lwl::delegate(this,&avILS::onLOCPowerOn);
	ElecConDevice.m_signalPowerOff += lwl::delegate(this,&avILS::onLOCPowerOff);

	MarkerBeacon_Consumer_.m_signalPowerOn += lwl::delegate(this,&avILS::onBeaconPowerOn);
	MarkerBeacon_Consumer_.m_signalPowerOff += lwl::delegate(this,&avILS::onBeaconPowerOff);

	Sound::Host& headphonesHost = ccSound::instance().getHost("HEADPHONES");
	source[ILS_MARKER_OUTER].create(&headphonesHost, "Aircrafts/Cockpits/MarkerOuter");
	source[ILS_MARKER_MIDDLE].create(&headphonesHost, "Aircrafts/Cockpits/MarkerMiddle");
	source[ILS_MARKER_INNER].create(&headphonesHost, "Aircrafts/Cockpits/MarkerInner");

#ifdef DEBUG_TEXT
	source[ILS_MARKER_OUTER].name = "ILS OUTER MARKER";
	source[ILS_MARKER_MIDDLE].name = "ILS MIDDLE MARKER";
	source[ILS_MARKER_INNER].name = "ILS INNER MARKER";
#endif

	volumeLocalizer.setGain(1.0);
	volumeMarkerBeacons.setGain(1.0);
	switcherLocalizer.setOnOff(false);
	switcherMarkerBeacons.setOnOff(false);

	receiver.getSoundElement() >> volumeLocalizer >> switcherLocalizer;

	volumeMarkerBeacons >> switcherMarkerBeacons;
	source[ILS_MARKER_OUTER] >> volumeMarkerBeacons;
	source[ILS_MARKER_MIDDLE] >> volumeMarkerBeacons;
	source[ILS_MARKER_INNER] >> volumeMarkerBeacons;
}

void avILS::connectElecPower(Elec::ItemBase& ElecInputILS, Elec::ItemBase& ElecMarkerBeacon)
{
	ElecInputILS >> Power_Switch_ >> ElecConDevice;
	ElecMarkerBeacon >> Beacon_Power_Switch_ >> MarkerBeacon_Consumer_;
}

void avILS::release()
{
	receiver.release();
}

void avILS::update_LOC()
{
	if (!getElecPower())
	{
		localizerDeviation = 0;
		localizerValid = false;
		return;
	}

	receiver.update();

	Parameter locPar = ILS_receiver.getMeasurement(ILS_MEASUREMENT_LOCALIZER);
	if (!localizer_ant_failure)
	{
		if (localizerValid = locPar)
			localizerDeviation	= locPar;
	}
	else
	{
		localizerDeviation = 0;
		localizerValid = false;
	}
}

void avILS::update_Marker()
{
	glideslopeDeviation = 0;
	glideslopeValid = false;
	markerFlags = 0;

	if(!MarkerBeacon_Consumer_.isPowered())
		return;

	Parameter glsPar = ILS_receiver.getMeasurement(ILS_MEASUREMENT_GLIDESLOPE);
	if (!glideslope_ant_failure)
	{
		if (glideslopeValid = glsPar)
		{
			double plane_angle	= glsPar;
			glideslopeDeviation = plane_angle;
		}
	}
	else
	{
		glideslopeDeviation = 0;
		glideslopeValid = false;
	}

	for(int m = ILS_MARKER_OUTER; m < ILS_MARKER_MAX; m++)
	{
		Parameter mkrPar = ILS_receiver.getMeasurement(ILS_MEASUREMENT_MARKER + m);
		if(mkrPar)
		{
			markerFlags |= markerSignals[m];
		}
	}

	if (markerFlags != 0 && !marker_ant_failure)
	{	
		if (isOuterMarkerSignal())
			source[ILS_MARKER_OUTER].update(1.0);
		else if (isMiddleMarkerSignal())
			source[ILS_MARKER_MIDDLE].update(1.0);
		else if (isInnerMarkerSignal())
			source[ILS_MARKER_INNER].update(1.0);
	}
	else
	{
		source[ILS_MARKER_OUTER].update(0.0f);
		source[ILS_MARKER_MIDDLE].update(0.0f);
		source[ILS_MARKER_INNER].update(0.0f);
		markerFlags	= 0;
	}
}

void avILS::update()
{
	if (getflag_failure())
	{
		localizerDeviation = 0;
		glideslopeDeviation = 0;
		localizerValid = false;
		glideslopeValid = false;

		markerFlags = 0;
		return;
	}

	ILS_receiver.update();
	update_LOC();
	update_Marker();
}


inline bool avILS::isOuterMarkerSignal() const { return markerFlags & SIGNAL_OUTER_MARKER; }

inline bool avILS::isMiddleMarkerSignal() const { return markerFlags & SIGNAL_MIDDLE_MARKER; }

inline bool avILS::isInnerMarkerSignal() const { return markerFlags & SIGNAL_INNER_MARKER; }


void avILS::onFrequencyChanged()
{
	double loc_freq = getLocalizerFrequency();
	double gs_freq = getGlideslopeFrequency(loc_freq);
	ILS_receiver.getReceivers()[ILS_RECEIVER_LOCALIZER].radioReceiver.setFrequency(loc_freq);
	ILS_receiver.getReceivers()[ILS_RECEIVER_GLIDESLOPE].radioReceiver.setFrequency(gs_freq);
}

double avILS::getLocalizerFrequency()
{
	return frequencyMHz * 1000000 + frequencyKHz * 1000;
}

double avILS::getGlideslopeFrequency(double localizerFreq)
{
	double glideslope = 1;
	int loc_freq = (int)localizerFreq;
	if (localizer_to_glideslope_map.find(loc_freq) != localizer_to_glideslope_map.end())
		glideslope = localizer_to_glideslope_map[loc_freq];
	return glideslope;
}

void avILS::init_frequencies_map()
{
	localizer_to_glideslope_map[108100000] = 334700000;
	localizer_to_glideslope_map[108150000] = 334550000;
	localizer_to_glideslope_map[108300000] = 334100000;
	localizer_to_glideslope_map[108350000] = 333950000;
	localizer_to_glideslope_map[108500000] = 329900000;
	localizer_to_glideslope_map[108550000] = 329750000;
	localizer_to_glideslope_map[108700000] = 330500000;
	localizer_to_glideslope_map[108750000] = 330350000;
	localizer_to_glideslope_map[108900000] = 329300000;
	localizer_to_glideslope_map[108950000] = 329150000;
	localizer_to_glideslope_map[109100000] = 331400000;
	localizer_to_glideslope_map[109150000] = 331250000;
	localizer_to_glideslope_map[109300000] = 332000000;
	localizer_to_glideslope_map[109350000] = 331850000;
	localizer_to_glideslope_map[109500000] = 332600000;
	localizer_to_glideslope_map[109550000] = 332450000;
	localizer_to_glideslope_map[109700000] = 333200000;
	localizer_to_glideslope_map[109750000] = 333050000;
	localizer_to_glideslope_map[109900000] = 333800000;
	localizer_to_glideslope_map[109950000] = 333650000;
	localizer_to_glideslope_map[110100000] = 334400000;
	localizer_to_glideslope_map[110150000] = 334250000;
	localizer_to_glideslope_map[110300000] = 335000000;
	localizer_to_glideslope_map[110350000] = 334850000;
	localizer_to_glideslope_map[110500000] = 329600000;
	localizer_to_glideslope_map[110550000] = 329450000;
	localizer_to_glideslope_map[110700000] = 330200000;
	localizer_to_glideslope_map[110750000] = 330050000;
	localizer_to_glideslope_map[110900000] = 330800000;
	localizer_to_glideslope_map[110950000] = 330650000;
	localizer_to_glideslope_map[111100000] = 331700000;
	localizer_to_glideslope_map[111150000] = 331550000;
	localizer_to_glideslope_map[111300000] = 332300000;
	localizer_to_glideslope_map[111350000] = 332150000;
	localizer_to_glideslope_map[111500000] = 332900000;
	localizer_to_glideslope_map[111550000] = 332750000;
	localizer_to_glideslope_map[111700000] = 333500000;
	localizer_to_glideslope_map[111750000] = 333350000;
	localizer_to_glideslope_map[111900000] = 331100000;
	localizer_to_glideslope_map[111950000] = 330950000;
}

bool avILS::set_damage(unsigned Failure_ID, const bool can_be_rejected)
{
	//Если нет электричества, то не ломаемся
	if (can_be_rejected && !getElecPower())
		return false;

	switch (Failure_ID)
	{
	case ILS_FAILURE_TOTAL:
		setflag_failure(true);
		ElecConDevice.setFailure(true);
		break;
	case ILS_FAILURE_ANT_MARKER:
		marker_ant_failure	= true;
		break;
	case ILS_FAILURE_ANT_GLIDESLOPE:
		glideslope_ant_failure	= true;
		break;
	case ILS_FAILURE_ANT_LOCALIZER:
		localizer_ant_failure	= true;
		break;
	}

	return true;
}

void avILS::OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params)
{ 
}

void avILS::repair()
{
	setflag_failure(false);
	ElecConDevice.setFailure(false);
	marker_ant_failure		= false;
	glideslope_ant_failure	= false;
	localizer_ant_failure	= false;
}

void avILS::set_localizer_on_off(bool enable)
{
	switcherLocalizer.setOnOff(enable);
	receiver.setOnOff(enable);
	ILS_receiver.getReceivers()[ILS_RECEIVER_LOCALIZER].radioReceiver.setOnOff(enable);
}

void avILS::set_marker_beacon_on_off(bool enable)
{
	switcherMarkerBeacons.setOnOff(enable);
	ILS_receiver.getReceivers()[ILS_RECEIVER_MARKER].radioReceiver.setOnOff(enable);
	ILS_receiver.getReceivers()[ILS_RECEIVER_GLIDESLOPE].radioReceiver.setOnOff(enable);
}
