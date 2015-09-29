#ifndef __wTransiver__
#define __wTransiver__

#include "wRadio.h"
#include "Comm/IwTransiver.h"

struct TransmitterDesc;
struct ReceiverDesc;

//Transiver has reference to radio transmitter and radio receiver
class WRADIO_API wTransiver : public IwTransiver
{
public:
	wTransiver(	wRadioTransmitter & radioTransmitterIn,
				wRadioReceiver & radioReceiverIn,
				bool fullDuplexIn = false);
	void setName(const char * name);
	virtual void addCallback(wRadioDevice::Callback * pRadioDeviceCallbackIn);
	virtual void removeCallback(wRadioDevice::Callback * pRadioDeviceCallbackIn);
	virtual void setBody(viObject * pBodyIn);
	virtual void setPlace(wRadioDevice::Place * pPlaceIn);
	virtual void setPosition(const cPosition & posIn);
	virtual void setPosition(viObject * pOwnerIn);
	virtual void setAntenna(const wRadioAntenna * radioAntenna);

	virtual wRadioReceiver * getReceiver();
	virtual wRadioTransmitter * getTransmitter();
	virtual const wRadioReceiver * getReceiver() const;
	virtual const wRadioTransmitter * getTransmitter() const;
	virtual float getFrequency() const;
	virtual float getBandWidth() const;
	virtual wModulation getModulation() const;

	virtual void setDevices(const TransmitterDesc & transmitterDesc, const ReceiverDesc & recevierDesc);
	virtual void setDevices(double innerNoiseIn, double powerIn);
	virtual void setFrequency(float frequencyIn);
	virtual void setBandWidth(float bandWidthIn);
	virtual void setModulation(wModulation modulation);

	virtual void update();

	void select(bool transmission);
	void unselect();

	virtual void transmit(wRadioDevice::Action * pAction);
	virtual void startTransmition();
	virtual void finishTransmition();
	virtual bool isTransmitting() const;

	virtual void setTransmitterOnOff(bool onOff);
	virtual bool isTransmitterOn() const;
	virtual void setReceiverOnOff(bool onOff);
	virtual bool isReceiverOn() const;
	virtual double getSNR(const wRadioTransmitter * pRadioTransmitter, bool moment) const;

#ifdef DEBUG_TEXT
	virtual size_t print(char * buff, size_t size) const;
#endif
protected:
	ed::string			name_;
	bool				transmitterEnabled_;
	bool				transmission_;
	wRadioTransmitter &	radioTransmitter_;
	wRadioReceiver &	radioReceiver_;
	const bool			fullDuplex_;
	wModulation			modulation_;
};

#endif __wTransiver__