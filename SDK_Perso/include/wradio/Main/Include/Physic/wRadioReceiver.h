#ifndef __wRadioReceiver__
#define __wRadioReceiver__

#include "wRadio.h"

#include "wRadioDevice.h"
#include "Physic/wRadioSignal.h"
#include "Physic/wRadioAntenna.h"
#include "Utilities/wDatabase.h"
#include "Comm/wMessage.h"

class wRadioTransmitter;

//Descriptor of a radio receiver
struct WRADIO_API ReceiverDesc
{
	ReceiverDesc();
	ReceiverDesc(const ed::string & nameIn, double noiseLevelIn, float	frequencyAccuracyIn, const wRadioAntenna * radioAntennaIn = NULL);
	ed::string		name;
	double			noiseLevel;
	float			frequencyAccuracy;
	const wRadioAntenna * radioAntenna;
};

//Radio receiver determines what signals are received and what quality they have
class WRADIO_API wRadioReceiver : public wRadioDevice
{
public:
	typedef ReceiverDesc Desc;
	//template WRADIO_API wDescriptorDatabase<Desc>;	
	typedef wDescriptorDatabase<Desc> Database;
	enum Events
	{
		EVENT_CHANGE_NOISE_LEVEL = wRadioDevice::EVENT_LAST_COMMON + 1,
		EVENT_TRANSMITTER_ADDED,
		EVENT_TRANSMITTER_REMOVED
	};
	typedef ed::map<const wRadioTransmitter*, double> Transmitters; //Transmitters signals from which are received by the receiver and signal level, V/m
	//User signal filter determines will the signal be tested or rejected 
	class SignalFilter
	{
	public:
		virtual ~SignalFilter() {;}
		virtual bool operator()(const wMessagePtr & pMsg) const = 0;
	};
public:
	wRadioReceiver();
	~wRadioReceiver();
	void	init(const Desc & desc);
	//wRadioDevice interface begin
	Type	getType() const;
	virtual bool isActive() const;
	void	setSignalOfInterest(wRadioSignals signalsOfInterest); //Sets signal of interest - the specific filter of the signals
	void	setOnOff(bool onOffIn);
	void	setFrequency(float frequencyIn);
	void	setBandWidth(float bandWidthIn);
	void	setFilter(const SignalFilter * signalFilter); //Sets the abstract filter of the signals
	void	onEvent(wRadioDevice * pRadioDevice, Event event); //Processes events from transmitters
	//wRadioDevice interface end
	void	setNoiseLevel(double noiseLevel);
	void	setBlanked(bool blanked);
	inline bool	isBlanked() const { return blanked_; }
	inline double getNoiseLevel() const { return noiseLevel_; }
	double 	getSNR(const wRadioTransmitter * pRadioTransmitter, bool moment) const; //Returns SNR for the given transmitter
	double	getE(const wRadioTransmitter * pRadioTransmitter) const; //Returns the signal strength of the given transmitter, V/m
	inline const Transmitters & getTransmitters() const { return transmitters_; }
	inline double getSumE2() const { return sumE2_ + noiseLevel_ * noiseLevel_; } //Returns summary signal and inner noise strength in 2nd power, V/m
    bool    checkMessageFilter(const wMessagePtr & pMsg) const;
#ifdef DEBUG_TEXT
	virtual size_t	print(char * buff, size_t size) const; //Debug print
#endif
private:
	double	getE_(const wRadioTransmitter * pRadioTransmitter) const;
	void clearTransmitters_();
private:
	double			noiseLevel_; //Power of inner noise determines sensitivity of the receiver, V/m
	bool			blanked_; //Is receiver is temporary blanked
	Transmitters	transmitters_; //Transmitters which signals are received by th receiver
	double			sumE2_; //sumE^2 = E1^2+E2^2+...En^2
	wRadioSignals	signalsOfInterest_; //Signal of interest
	const SignalFilter * signalFilter_; //Signal filter
};

#endif