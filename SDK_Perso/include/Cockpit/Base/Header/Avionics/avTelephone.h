#ifndef _avTelephone_h
#define _avTelephone_h

#include "CockpitBase.h"
#include "Sound/Sounder.h"
#include "Comm/wHumanCommunicator.h"

namespace cockpit
{

// Приёмная часть проводной связи
class COCKPITBASE_API avTelephone
{
public:
	avTelephone();
	
	void			initialize(const char * name_);
	void			post_initialize();
	void			release();

	inline bool		isOn() const { return switcher.isOn(); }
	
	void			setOnOff(bool on);
	void			setMicrophoneOnOff(bool on);
	
	inline Sounder::Element & getSoundElement() { return volume; }
private:
	bool microphoneOn;
	Sounder::MessagesSource	messagesSource;
	Sounder::Switcher	switcher;
	Sounder::Regulator	volume;
	
	class MessageHandler : public wHumanCommunicator::ReceivingMessageHandler
	{
	public:
		MessageHandler(avTelephone * telephone_);
		virtual void	updateMessage(const wMessagePtr & message);
		virtual void	finishMessage(const wMessagePtr & message, bool onTime);
		virtual float	getMessageSNR(const wRadioTransmitter * pTransmitter) const;
	private:
		avTelephone *	telephone;
	};
	friend class MessageHandler;
	std::auto_ptr<MessageHandler>	messageHandler;
};

}

#endif _avTelephone_h