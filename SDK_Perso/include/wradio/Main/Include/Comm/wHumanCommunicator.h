#ifndef __wHumanCommunicator__
#define __wHumanCommunicator__

#include "Comm/wCommunicator.h"
#include "Comm/wTransiverStorage.h"
#include "Sound/Sounder.h"
#include "WorldSound.h"

class wRadioReceiverSource;

/*
Communicator used in player-controlled units.
Player-controlled units may have several radios and receivers.
The devices are usually modeled in details.
The communicator is connected to sound system to play the messages.
*/
class WRADIO_API wHumanCommunicator : public wCommunicator
{
public:
	//Message handler used by radio devices of the cockpit to handle message events
	class MessageHandler
	{
	public:
		virtual void	updateMessage(const wMessagePtr & message) = 0;
		virtual void	finishMessage(const wMessagePtr & message, bool onTime) = 0;
	};

	//Callback used by wHumanCommunicator to check what SNR the cockpit device(s) provide(s) for the message
	class ReceivingMessageHandler : public MessageHandler
	{
	public:
		virtual float	getMessageSNR(const wRadioTransmitter * pTransmitter) const = 0;
	};
	
	//Transiver with children transivers (radios) and receivers
	class wTransiverStorage : public ::wTransiverStorage
	{
	public:
		wTransiverStorage() : transiverForTunningFromOutside_(NULL) {;}
		virtual void clear();
		inline void removeTransiver(wTransiver * transiver)
		{
			::wTransiverStorage::removeTransiver(transiver);
			if(transiver == transiverForTunningFromOutside_)
				transiverForTunningFromOutside_ = NULL;
		}
		inline void setTransiverForTunningFromOutside(wTransiver * transiverForTunningFromOutsideIn)
		{
			assert(transiverForTunningFromOutsideIn == NULL || std::find(transivers_.begin(), transivers_.end(), transiverForTunningFromOutsideIn) != transivers_.end());
			transiverForTunningFromOutside_ = transiverForTunningFromOutsideIn;
		}
		virtual void setAntenna(const wRadioAntenna * antenna);
        virtual void setDevices(double innerNoiseIn, double powerIn);
		virtual void setFrequency(float frequencyIn);
		virtual void setBandWidth(float bandWidthIn);
		virtual void setModulation(wModulation modulation);
	private:
		wTransiver *	transiverForTunningFromOutside_;
	};
    wHumanCommunicator();
	wHumanCommunicator(wMessage::Object::Callsign callsignIn, woPointer pUnitIn, const char * nameIn);
	~wHumanCommunicator();
	virtual bool	cannotInterruptReceive(const wMessagePtr & message) const;
	virtual void	tryTransmitCurrentMessage();
	virtual void	startMessageTransmition(const wMessagePtr & message);
	virtual void	finishTransmition(bool onTime);
	virtual bool	isMessageLegible(const wMessagePtr & message, bool moment) const;
	inline wTransiverStorage & getTransiverStorage() { return transiverStorage_; }
	virtual bool	canBreakReceive(const wMessagePtr & message1, const wMessagePtr & message2) const;
	virtual void	onReceiving(const wMessagePtr & message, RadioMessageEvent event, bool legible, bool new_message);
	void			addMessageHandler(ReceivingMessageHandler * pMessageHandler);
	void			removeMessageHandler(ReceivingMessageHandler * pMessageHandler);
#ifdef DEBUG_TEXT
	virtual size_t	print(char * buff, size_t size) const;
#endif	
private:
	//Call MessageHandler members functions
	void			handleMessageUpdate_(const wMessagePtr & message);
	void			handleMessageFinish_(const wMessagePtr & message, bool onTime);
protected:
	typedef ed::list<ReceivingMessageHandler*>	ReceivingMessageHandlers;
	ReceivingMessageHandlers			receivingMessageHandlers_;
	wTransiverStorage					transiverStorage_;
	Sound::Host							selfHost_;
	Sounder::MessageSource				transmittingMessagesSource_; //player must always hear himself
};

#endif
