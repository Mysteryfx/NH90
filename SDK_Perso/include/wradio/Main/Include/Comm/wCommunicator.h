#ifndef __wCommunicator__
#define __wCommunicator__

#include <ed/map.h>
#include <ed/list.h>

#include "wRadio.h"

#include "Comm/wMessage.h"
#include "Comm/wCommNetFlags.h"
#include "wTime.h"
#ifdef DEBUG_TEXT
#include "Utilities/Printer.h"
#endif DEBUG_TEXT
#include "Randomizer/RandomObject.h"

class MovingObject;
class wCommunicator;
class wCommNet;
class wRadioReceiver;
class wRadioTransmitter;
class wMessage;
class Parameter;
class IwTransiver;

//Channel type
enum wChannelType {	CHANNEL_TYPE_VOID = -1,
					CHANNEL_TYPE_SIMPLEX_RECEIVE,
					CHANNEL_TYPE_SIMPLEX_TRANSMIT,
					CHANNEL_TYPE_HALF_DUPLEX,
					CHANNEL_TYPE_DUPLEX };

//The structure used to control message transmission
class wTransmittingMessageData
{
	//The timer used to finish transmission of the message if the message has limited duration
	class Activity : public wTime
	{
	public:
		Activity();
		Activity(wCommunicator * pCommunicatorIn);
		virtual ~Activity ();
		virtual void NextEvent();
	public:
		wCommunicator*		pCommunicator;
	};
public:
	wTransmittingMessageData();
	~wTransmittingMessageData();
	void startTransmition(wCommunicator * pCommunicator_,const wMessagePtr & message_, bool commandDialogsPanelIsOn = false);
	void finishTransmition(wCommunicator * pCommunicatorIn, bool commandDialogsPanelIsOn);
	bool isTransmitting() const;
	wMessagePtr		message;
	Activity		activity;
};

//The structure used to determine if the message is received or not
struct WRADIO_API wReceivingMessageData
{
	wReceivingMessageData();
	void updateStatus();
	wModelTime	lastCheckTime; //Last check time of legibility
	wModelTime	legibleTime; //The summary time when the message was legible
	bool		legible; //Is the message legible now
};

typedef ed::map<wMessagePtr, wReceivingMessageData> WRADIO_API wReceivingMessages;

enum RadioMessageEvent
{
	MESSAGE_START,
	MESSAGE_UPDATE,
	MESSAGE_FINISH, //Message interruption
	MESSAGE_FINISH_ON_TIME, //Message finish
	MESSAGE_TRANSMITTION //Transmission of an instant (with no duration) message
};

//The timer used to wait to next transmission attempt
class wWaitingActivity : public wTime
{
public:
	wWaitingActivity();
	wWaitingActivity(wCommunicator * pCommunicatorIn);
	virtual void NextEvent();
public:
	wCommunicator * pCommunicator;
};

/*
wCommunicator is the object that provides sending and receiving of messages (wMessage).
wCommunicator is responsible for message building (in some cases), message transmission, message receiving, control of broadcast (to avoid collisions).
wCommunicator can have IwTransiver - the physical object that operates with radio signals. If the communicator has the IwTransiver, that object determines is the message received or not.
*/
typedef ed::vector<IwTransiver*> IwTransivers_t;

class WRADIO_API wCommunicator : public wMessage::Sender
{
public:
	wCommunicator();
	wCommunicator(wChannelType typeIn);
	wCommunicator(wChannelType typeIn, wMessage::Sender::Callsign callsignIn, MovingObject * pUnitIn, const char * nameIn);	
	virtual ~wCommunicator();
	//initialization
	void			clear();
	void			clearMessageQueue();
    void            clearReceivingMessages();
	//set
	virtual void	setTransiver(IwTransiver * pTransiverIn, int i);
	void			setCommandDialogsPanelisOn(bool commandDialogsPanelIsOnIn); //Connect/disconnect dialog panel to/from the communicator
	//net routines
	virtual void	setTransmitReceiveCommNetFlags(wCommNetFlags flags);
	virtual void	setTransmitCommNetFlags(wCommNetFlags flags);
	inline wCommNetFlags	getTransmitCommNetFlags() const { return transmitFlags; }
	virtual void	setReceiveCommNetFlags(wCommNetFlags flags);
	inline wCommNetFlags	getReceiveCommNetFlags() const { return receiveFlags; }
	//status
	inline bool		isTransmitting() const {return transmittingMessageData.isTransmitting(); }
	inline bool		isWaitingForTransmittion() const {return !isTransmitting() && !messageQueue.empty();}	 
    inline bool		isReceiving() const { return receivingMessages.size() > 0; } //Is the communicator receiving messages
	virtual bool	cannotInterruptReceive(const wMessagePtr & message) const;
	//message transmittion
	virtual void	makeAndTransmitMessage(wMsg event, const Parameter * messageParam = NULL); //Makes (from the given parameters) and transmits the message
	virtual void	transmitMessage(const wMessagePtr & message); //Enqueues the message and will transmit it when the broadcast is free
	virtual void	transmitMessageNow(const wMessagePtr & message); //Transmits message now
	virtual void	tryTransmitCurrentMessage(); //Tries to transmit current message if the broadcast is free
	virtual void	finishTransmition(bool onTime); //Finishes the transmission. If onTime is false - interrupts the transmission.
	wMessagePtr		getCurrentMessage() const;
	//message receiving	
	virtual bool	receiveMessage(const wMessagePtr & message, RadioMessageEvent event); //Process the message event - tries to receive the message
	const wReceivingMessages & getReceivingMessages() const {return receivingMessages;}	
    IwTransivers_t& getTransivers() { return m_arTransivers; }
    IwTransiver*    getCurrentTransiver(int i) const { if (i < m_arTransivers.size()) return m_arTransivers[i]; return nullptr; }

    void            setKillingActivity(wTime* pAct) { m_pKillingActivity = pAct; }
    void            destroyKillingActivity();

	//wRadioDevice::EventHandler interface end
	static wCommunicator * getById(ObjectID objectID);
#ifdef DEBUG_TEXT
	virtual size_t	print(char * buff, size_t size) const; //Debug print
	Common::Printer * getPrinter() { return &printer; }
#endif
	static RandomObject		randomEvenly;
protected:
	//message transmittion
	bool			addMessage(const wMessagePtr & message); //Adds ongoing message to the queue
	virtual void	startWaiting(wModelTime dt); //Start waiting for the next transmission attempt
	virtual wModelTime howLongWaitForTransmittion(const wMessagePtr & message) const;  // to be overriden
	virtual void	startTransmition();
	virtual void	startMessageTransmition(const wMessagePtr & message);
	virtual void	transmitZeroDurationMessage(const wMessagePtr & message);
	virtual void	sendMessage(const wMessagePtr &  message, RadioMessageEvent event); //Sends message event to the broadcast
	void			sendMessageFinish(const wMessagePtr & message, bool onTime); //Sends message finish event to the broadcast
	virtual void	startSignal(const wMessagePtr & message);
	enum Interruption { DONT_INTERRUPT, INTERRUPT_AND_DELAY, INTERRUPT_AND_FINISH };
	virtual Interruption canInterrupt(const wMessagePtr & message1, const wMessagePtr & message2) const;//Returns if message1 can interrupt transmission of message2 - to be overriden
	virtual bool	canRemove(const wMessagePtr & message1, const wMessagePtr & message2) const; //Returns if message1 can remove message2 from the queue of ongoing messages - to be overriden
	virtual bool	canDelay(const wMessagePtr & message1, const wMessagePtr & message2) const; //Returns if message1 can delay message2 from the queue of ongoing messages - to be overriden
	virtual bool	canBreakReceive(const wMessagePtr & message1, const wMessagePtr & message2) const; //Returns if message1 can break receiving of message2 - to be overridden
	//message receiving	
	virtual void	onReceiving(const wMessagePtr & message, RadioMessageEvent event, bool legible, bool new_message); //Message receive handler - to be overridden
	virtual bool	isMessageLegible(const wMessagePtr & message, bool moment) const; //Returns if the message is legible now
	virtual double	getMinSNR() const;
	//message processing
	virtual void	processMessage(const wMessagePtr & message); //Processes the received message - to be overridden
protected:
	const wChannelType			type;
	wCommNetFlags				transmitFlags; //Flags to filter the messages that are allowed to transmit
	wCommNetFlags				receiveFlags; //Flags to filter the messages that are allowed to receive

    IwTransivers_t	            m_arTransivers;
	class RadioDeviceCallback;
	RadioDeviceCallback *		radioDeviceCallback; //Callback for physical radio devices in IwTransiver
	
	typedef ed::list<wMessagePtr> MessageQueue;
	MessageQueue				messageQueue;
	wTransmittingMessageData	transmittingMessageData;

	wReceivingMessages			receivingMessages; //All current messages that were legible to the communicator at least once

	wWaitingActivity			waitingActivity; //Timer for waiting for the next transmission attempt
	bool						commandDialogsPanelIsOn;
    wTime *                     m_pKillingActivity;
#ifdef DEBUG_TEXT
	Common::ObjectPrinter<wCommunicator, 2000> printer;
#endif
public:
	static wCommNet * commNet; //The logical broadcast
};

#endif