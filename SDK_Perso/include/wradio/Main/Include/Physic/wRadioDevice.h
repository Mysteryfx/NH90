#ifndef __wRadioDevice__
#define __wRadioDevice__

#include "wRadio.h"
#include "wTime.h"
#include "cPosition.h"
#include "Randomizer/RandomObject.h"

class viObject;
class wBroadcast;
class wRadioAntenna;
class wRadioDevice;

//Radio device: receiver or transmitter
class WRADIO_API wRadioDevice
{
public:
	enum Type
	{
		TYPE_VOID = -1,
		TYPE_TRANSMITTER,
		TYPE_RECEIVER
	};
	class Place
	{
	public:
		virtual ~Place() {;}
		virtual const cPosition & Position(wModelTime time) const = 0;
	};
	enum Event
	{
		EVENT_ON,
		EVENT_OFF,
		EVENT_DESTROY,
		EVENT_BEFORE_CHANGE_FREQUENCY,
		EVENT_AFTER_CHANGE_FREQUENCY,
		EVENT_BEFORE_CHANGE_BAND_WIDTH,
		EVENT_AFTER_CHANGE_BAND_WIDTH,
		EVENT_UPDATE,
		EVENT_LAST_COMMON
	};
	class Callback
	{
	public:
		virtual ~Callback() {;}
		virtual void operator()(wRadioDevice * pRadioDevice, Event event) = 0;
	};
	class Action
	{
	public:
		virtual ~Action() {;}
		virtual void operator()() = 0;
	};
public:
	wRadioDevice();
	virtual ~wRadioDevice();
	void			setName(const ed::string & name_) { name = name_; }
	void			setBody(viObject* pBodyIn); //Sets the object as the device body
	void			setPlace(Place* pPlaceIn); //Sets the abstract place as the device body
	void			setMaxDistance(float maxDistanceIn); //Sets the maximal receive/transmit distance
	void			resetMaxDistance();
	inline void		addCallback(Callback* pCallback) { callbacks.push_back(pCallback); }
	inline void		removeCallback(Callback* pCallback) { callbacks.remove(pCallback); }
	void			setAntenna(const wRadioAntenna * pAntennaIn);
	virtual void	setFrequency(float frequencyIn);
	virtual void	setFrequencyAccuracy(float frequencyAccuracyIn);
	virtual void	setBandWidth(float bandWidthIn);
	virtual void	setPosition(const cPosition & posInIn);
	virtual void	setPosition(viObject* pBodyIn);
	
	virtual Type	getType() const;
	inline const ed::string &	getName() const { return name; }
	inline viObject*getBody() const { return pBody; }
	inline Place*	getPlace() const { return pPlace; }
	bool			getMaxDistance(float & maxDistanceOut) const;
	inline bool		isOn() const { return onOff; }
	virtual bool	isActive() const; //Is the device is active: switched on and not blanked
	const wRadioAntenna * getAntenna() const { return pAntenna; }
	inline float	getFrequency() const { return frequency; }
	inline float	getFrequencyAccuracy() const { return frequencyAccuracy; }
	inline float	getBandWidth() const { return bandWidth; }
	const cPosition & getPosition() const { return pos; }

	virtual cPosition Position() const;
	virtual void	onEvent(wRadioDevice * pRadioDevice, Event event); //Receive and process the event from another device of another type.
	virtual void	update();
#ifdef DEBUG_TEXT
	virtual size_t	print(char * buff, size_t size) const; //Debug print
#endif
protected:
	virtual void	setOnOff(bool onOffIn);
	virtual void	generateEvent(Event event);
public:
	static wBroadcast*		pBroadcast;
	static RandomObject		randomEvenly;
protected:
	ed::string				name;
	viObject*				pBody;
	Place*					pPlace;
	bool					flagMaxDistance;
	float					maxDistance;
	typedef ed::list<Callback*>Callbacks;
	Callbacks				callbacks;
	cPosition				pos;
	const wRadioAntenna*	pAntenna;	
	bool					onOff;	
	float					frequency; //Current frequency, Hz
	float					frequencyAccuracy; //Accuracy of frequency tunning, Hz
	float					bandWidth; //Receiver/Signal band width, Hz
};

bool checkMaxDistance(const wRadioDevice * pRadioDevice1, const wRadioDevice * pRadioDevice2);

#endif
