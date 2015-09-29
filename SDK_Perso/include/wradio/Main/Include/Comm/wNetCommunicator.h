#ifndef _W_COMM_MAIL_H_
#define _W_COMM_MAIL_H_

#include "wRadio.h"
#include "Comm/wCommunicator.h"
#include "Comm/wCommNetFlags.h"
#include "Comm/wTransiverOwner.h"

#include "Utilities/LuaTable.h"

struct CommNetState
{
	CommNetState()  : enable(false), modulation(MODULATION_VOID), power(0.0f), sensitivity(0.0f), frequency(0.0f), bandWidth(0.0f)  {;}
	bool		enable;
	wModulation modulation;
	float		power;
	float		sensitivity;
	float		frequency;
	float		bandWidth;
};

struct WRADIO_API VoiceCommNetState : public CommNetState
{
	VoiceCommNetState() : CommNetState(), callsign(-1), transmitFlags(COMM_NET_FLAG_NULL), receiveFlags(COMM_NET_FLAG_NULL) {;}
	int callsign;
	int voice;
	wCommNetFlags transmitFlags;
	wCommNetFlags receiveFlags;
};

enum wMsg;
class MovingObject;

struct wMessageData
{
	wMessage::Type type;
	wMessage::PlayMode playMode;
	wModelTime birthTime, startTime;
	ObjectID recepientID;
	wMsg msgId;
	LuaTableElement luaTableElement;
	ed::string paramStr;
	ed::string subtitle;
	wModelTime duration;
};

void WRADIO_API makeMessageData(wMessagePtr message, wMessageData & messageData);

void WRADIO_API transmitMessage(wCommunicator * communicator, const wMessageData & messageData);

class WRADIO_API SendMessageTimer : public wTime
{
public:
	SendMessageTimer(ObjectID unitID, const wMessageData & messageData);
	virtual void NextEvent();
private:
	ObjectID unitID_;
	wMessageData messageData_;
};

//Local transmition (transmit-only communicator)
class WRADIO_API wNetVoiceCommunicator : public wCommunicator
{
public:
	wNetVoiceCommunicator(MovingObject * mo);
	virtual ~wNetVoiceCommunicator();
	void setOnOff(bool on);
	void setNetState(const VoiceCommNetState & voiceCommNetState);
	void setTransiverState(bool enable, float power, float frequency, float bandWidth, wModulation modulation);
	wTransiverOwner	transiver;
private:
	bool enabled;
};

void setTransiverNetState(wTransiver * pTransiver, float power, float sensitivity, float frequency, float bandWidth, wModulation modulation);

void setTransiverNetState(wTransiver * pTransiver, const CommNetState & commNetState);

void getTransiverNetState(IwTransiver * transiver, CommNetState & commNetState);

void WRADIO_API getVoiceCommunicatorNetState(const wCommunicator * communicator, VoiceCommNetState & voiceCommNetState);

namespace Mail
{
	class Stream;
}

Mail::Stream & operator >> (Mail::Stream& stream, CommNetState & commNetState);

WRADIO_API Mail::Stream & operator >> (Mail::Stream& stream, VoiceCommNetState & voiceCommNetState);

WRADIO_API Mail::Stream & operator >> (Mail::Stream& stream, wMessageData & messageData);

#endif _W_COMM_MAIL_H_