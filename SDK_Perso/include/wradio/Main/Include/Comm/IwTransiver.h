#ifndef __IwTransiver__
#define __IwTransiver__

#include "Physic/wRadioDevice.h"
#include "Physic/wRadioSignal.h"

class wRadioReceiver;
class wRadioTransmitter;

//Modulation is a property of transiver
enum wModulation 
{
	MODULATION_VOID =-1,
	MODULATION_AM,
	MODULATION_FM,
	MODULATION_AM_AND_FM
};

//Modulation is converted into radio signal
inline wRadioSignals getSignalByModulation(wModulation modulation)
{
	switch(modulation)
	{
		case MODULATION_AM:
			return SIGNAL_VOICE_AM;
		case MODULATION_FM:
			return SIGNAL_VOICE_FM;
		case MODULATION_AM_AND_FM:
			return SIGNAL_VOICE_AM | SIGNAL_VOICE_FM;
		default:
			return SIGNAL_NULL;
	};
}

//Interface of transiver. Transiver is a facade for pair: radio receiver and radio transmitter
class IwTransiver
{
public:
	virtual ~IwTransiver() {;}
	virtual void addCallback(wRadioDevice::Callback * pRadioDeviceCallbackIn) = 0;
	virtual void removeCallback(wRadioDevice::Callback * pRadioDeviceCallbackIn) = 0;
	virtual void setBody(viObject * pBodyIn) = 0;
	virtual void setPlace(wRadioDevice::Place * pPlaceIn) = 0;
	virtual void setPosition(const cPosition & posIn) = 0;
	virtual void setPosition(viObject * pOwnerIn) = 0;
	virtual void setAntenna(const wRadioAntenna * radioAntenna) = 0;

	virtual wRadioReceiver * getReceiver() = 0;
	virtual wRadioTransmitter * getTransmitter() = 0;
	virtual const wRadioReceiver * getReceiver() const = 0;
	virtual const wRadioTransmitter * getTransmitter() const = 0;
	virtual float getFrequency() const = 0;
	virtual float getBandWidth() const = 0;
	virtual wModulation getModulation() const = 0;

    virtual void setDevices(double innerNoiseIn, double powerIn) = 0;
	virtual void setFrequency(float frequencyIn) = 0;
	virtual void setBandWidth(float bandWidthIn) = 0;
	virtual void setModulation(wModulation modulation) = 0;

	virtual void update() = 0;

	virtual void transmit(wRadioDevice::Action * pAction) = 0;
	virtual void startTransmition() = 0;
	virtual void finishTransmition() = 0;
	virtual bool isTransmitting() const = 0;

	virtual void setTransmitterOnOff(bool onOff) = 0;
	virtual bool isTransmitterOn() const = 0;
	virtual void setReceiverOnOff(bool onOff) = 0;
	virtual bool isReceiverOn() const = 0;
	virtual double getSNR(const wRadioTransmitter * pRadioTransmitter, bool moment) const = 0;

#ifdef DEBUG_TEXT
	virtual size_t print(char * buff, size_t size) const = 0;
#endif
};

#endif __IwTransiver__