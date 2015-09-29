#ifndef _wTransiverStorage_
#define _wTransiverStorage_

#include "wRadio.h"
#include "Comm/IwTransiver.h"

class wTransiver;
class wRadioReceiver;

//Transiver stores several transivers and receivers within itself. Used by player-controlled units.
class WRADIO_API wTransiverStorage : public IwTransiver
{
public:
	wTransiverStorage();
	~wTransiverStorage();
	void addTransiver(wTransiver * transiver);
	void removeTransiver(wTransiver * transiver);
	void addReceiver(wRadioReceiver * pRadioReceiver);
	void removeReceiver(wRadioReceiver * pRadioReceiver);
	void setCurrentTransiver(wTransiver * transiver); //Sets current transiver
	wTransiver * getCurrentTransiver() const { return currentTransiver_; }
	bool resetCurrentTransiver(wTransiver * transiver);
	virtual void clear();

	virtual wRadioReceiver * getReceiver();
	virtual wRadioTransmitter * getTransmitter();
	const wRadioReceiver * getReceiver() const;
	const wRadioTransmitter * getTransmitter() const;
	virtual float getFrequency() const;
	virtual float getBandWidth() const;
	virtual wModulation getModulation() const;

	virtual void addCallback(wRadioDevice::Callback * pRadioDeviceCallbackIn);
	virtual void removeCallback(wRadioDevice::Callback * pRadioDeviceCallbackIn);
	virtual void setBody(viObject * pBodyIn);
	virtual void setPlace(wRadioDevice::Place * pOwnerIn);
	virtual void setPosition(const cPosition & posIn);
	virtual void setPosition(viObject * pOwnerIn);

	virtual void update();

	virtual void transmit(wRadioDevice::Action * pAction);
	virtual void startTransmition();
	virtual void finishTransmition();
	virtual bool isTransmitting() const;

	virtual void setTransmitterOnOff(bool onOff);
	virtual bool isTransmitterOn() const;
	virtual void setReceiverOnOff(bool onOff);
	virtual bool isReceiverOn() const;
	virtual double getSNR(const wRadioTransmitter * pRadioTransmitter, bool moment) const;
	double getSNRForSelectedTransiver(wRadioTransmitter * pRadioTransmitter, bool moment) const;

#ifdef DEBUG_TEXT
	virtual size_t print(char * buff, size_t size) const;
#endif
protected:
	typedef ed::list<wTransiver*> wTransivers;
	wTransivers transivers_; //All children transivers
	wTransiver * currentTransiver_; //Current transiver
	bool transmission_; //If transmission is active
	bool transmitterEnabled_; //If transmission is enabled
	typedef ed::list<wRadioReceiver*> wReceivers;
	wReceivers receivers_; //All children receivers
	typedef ed::list<wRadioDevice::Callback*>Callbacks;
	Callbacks callbacks; //Callbacks for radio devices
};

#endif _wTransiverStorage_
