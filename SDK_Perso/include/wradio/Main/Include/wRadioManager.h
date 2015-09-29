#ifndef __wRadioManager__
#define __wRadioManager__

#include "WRadio.h"

#include <ed/set.h>
#include "cPosition.h"
#include "wTime.h"
#include "IwRadioBeacon.h"

#include "Comm/wMessage.h"
#include "Comm/wCommNetFlags.h"
#include "Physic/wRadioDevice.h"

class wCommunicator;
class wCommNet;
enum RadioMessageEvent;

wCommNetFlags WRADIO_API getMessageCommNetFlags(const wMessagePtr & pMessage);

class wCommNetActivity : public wTime
{
public:
    wCommNetActivity() { };
    void            setCommNet(wCommNet * pCN);
	virtual void    NextEvent();
private:
	wCommNet * pCommNet;
};
//The broadcast - the mediator between wCommunicator objects. Responsible for message transmission.
class wCommNet
{
public:
	typedef ed::set<wCommunicator*> wReceivers;
	typedef ed::map<wMessagePtr, wCommNetFlags> wMessages;
	wCommNet();
	void addReceiver(wCommunicator * pReceiver);
	void removeReceiver(wCommunicator * pReceiver);
	void WRADIO_API sendMessage(const wMessagePtr & pMessage, wCommNetFlags senderFlags, RadioMessageEvent event); //Sends the message event to the broadcast
	void updateSenderFlag(const wMessagePtr & pMessage, wCommNetFlags newSenderFlags);
	void updateReceiverFlag(wCommunicator * receiver, wCommNetFlags newReceiverFlags);
	inline const wMessages & getMessages() const { return messages_; }
	inline const wReceivers & getCommunicators() const { return receivers_; }
	void updateMessages();
private:
	wMessages	        messages_; //All the messages in the broadcast now
	wReceivers	        receivers_; //All the receivers (wCommunicator) in the broadcast now
    wCommNetActivity    m_activity; // степ для апдейта сообщений
};

//The radio broadcast - the mediator between wRadioTransmitter objects and radio wRadioReceiver objects. Responsible for signal transmission.
class wBroadcast
{
private:
	typedef std::multimap<float, wRadioDevice*> wRadioDeviceMap;
	typedef ed::map<wRadioDevice*, wRadioDeviceMap::iterator> wRadioDeviceAuxMap;
public:
	wBroadcast();
	void release();
	void onEvent(wRadioDevice * pRadioDevice, wRadioDevice::Event event); //Processes the event and sends it to devices of another type in the same frequency band
	bool getDevices(wRadioDevice::Type type, float freqMin, float freqMax, ed::list<wRadioDevice*> & list) const;
private:
	void notifyDevices_(wRadioDevice::Type type, wRadioDevice * pRadioDevice, wRadioDevice::Event event); 
private:
	wRadioDeviceMap		radioDeviceMap_[2]; //Devices which are ordered by their frequency
	wRadioDeviceAuxMap	radioDeviceAuxMap_[2]; //Position of each devices in the map above
};

class wRadioBeaconsBuilder;

class WRADIO_API wRadioManager :	public wCommNet,
									public wBroadcast
{
public:
	wRadioManager();
	~wRadioManager();
	void	loadBeacons();
	void	activateBeacons();
	void	release();
	IwRadioBeacon *	createBeacon(const ed::string & name,const ed::string & callsign, BeaconType type,double frequency, const cPosition & pos, int system_ID, int site_ID, wBeaconStatus status = BEACON_ACTIVE, bool dynamic_site_building = false);
	IwRadioBeacon *	createChildBeacon(const ed::string & name, const ed::string & callsign, BeaconType type, double frequency, viObject * pOwnerIn, int system_ID);
	void	reloadBeacons();
	void	clear_beacons();
private:
	wRadioBeaconsBuilder * beaconsBuilder_;
};

#endif
