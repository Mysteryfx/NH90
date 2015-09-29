#ifndef __wRadioTransmitter__
#define __wRadioTransmitter__

#include "wRadio.h"

#include "wRadioDevice.h"
#include "Physic/wRadioAntenna.h"
#include "Physic/wRadioSignal.h"
#include "Utilities/wDatabase.h"
#include "Comm/wMessage.h"

class wRadioReceiver;

//Descriptor of a radio transmitter
struct WRADIO_API TransmitterDesc
{
	TransmitterDesc();
	TransmitterDesc(const ed::string & nameIn, double PtIn, float frequencyAccuracyIn, const wRadioAntenna * radioAntennaIn = NULL);
	ed::string		name;
	double			Pt;
	float			frequencyAccuracy;
	const wRadioAntenna * radioAntenna;
};

//Radio transmitter generates radio signals and transmits them to the broadcast
class WRADIO_API wRadioTransmitter : public wRadioDevice
{
public:
	typedef TransmitterDesc Desc;
//	template WRADIO_API wDescriptorDatabase<Desc>;
	typedef wDescriptorDatabase<Desc> Database;
	typedef wRadioDevice::Action TransmitAction;
	enum Events
	{
		EVENT_CHANGE_POWER = wRadioDevice::EVENT_LAST_COMMON,
	};
public:
	wRadioTransmitter();
	~wRadioTransmitter();
	void	    init(const Desc & desc);
	//wRadioDevice interface begin
	Type	    getType() const;
	void	    onEvent(wRadioDevice * pRadioDevice, Event event);  //Processes events from receivers
	//wRadioDevice interface end
	void	    startTransmittion(wRadioSignals signals); //Start transmission of the signal
	void	    finishTransmittion(); //Finish transmission of the current signal
	void	    transmit(TransmitAction * pTransmitAction, wRadioSignals signals);
	void	    setPower(double Pt); //Sets power, Watts
	double	    getPower() const { return sqrPt_ * sqrPt_; }
	double	    getE(const wRadioReceiver * pRadioReceiver) const; //Return the current signal strength in the given receiver
	double	    getE(const cPoint & point) const; //Return the current signal strength at the given point of space
	inline      wRadioSignals getSignals() const { return signals_; }
#ifdef DEBUG_TEXT
	virtual size_t	print(char * buff, size_t size) const;
#endif
private:
	bool	isOn() const;
	double			sqrPt_; //square root of the power, sqr(Watts)
	wRadioSignals	signals_;
    wMessagePtr     pCurrMessage;
};

#endif