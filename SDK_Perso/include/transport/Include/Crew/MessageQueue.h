#pragma once

#include "CarCrew.h"

#include "wActivity.h"
#include "wActivityManager.h"

class MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();

public:
	int addMsgToQueue(CarCrew::CrewMessage* msg);

private:
	// ����������

	typedef wActivity<MessageQueue> msgQueueActivities;
	wActivityManager actManager;
	wtPointer qActivity;

	void doQueue(wModelTime& modelTime);
	void startActivity();
	void stopActivity();

	// ����������

	float delayTime;

	ed::vector<CarCrew::CrewMessage*> msgQueue;

	// ����

	virtual void initSound();
	virtual void soundGetParams(Sound::HostParams* sHP);
	virtual void updateSoundParams();

	Sound::Host soundHost;
	Sound::Source soundSource;

	float playedTime;
	int lastMsgId;

	// ����
};