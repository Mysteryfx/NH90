#ifndef __AMMUNITION_BALLUTE_H__
#define __AMMUNITION_BALLUTE_H__

#include "Weapons.h"

#include "Ammunition/ammunition_descriptor.h"
#include "Ammunition/ammunition_new.h"

#include "IWeapon.h"
#include "IWorldNet.h"
#include "WorldMail.h"

struct BalluteNetMessage;

class WEAPONS_API wAmmunitionBallute : public wAmmunition
{
public:

	wAmmunitionBallute();
	virtual ~wAmmunitionBallute();

	virtual woPointer	getHost();
	virtual void setLauncher(MovingObject* obj);
	void SetBalluteSuppress(bool suppress_ballute);

protected:	
	virtual void initCommon_(MovingObject * launcher,const wAmmunitionDescriptor* desc);
	void OnNetworkMessage(const BalluteNetMessage& msg);

protected:
	ObjectID    targetId_;
	woPointer   carrier_;

protected:
	friend class wBalluteMail;
};

struct BalluteNetMessage
{
	bool suppress_ballute;
};

WEAPONS_API Mail::Stream& operator >> (Mail::Stream& stream, BalluteNetMessage& msg);

namespace {
	typedef Mail::GenMessage<wWeaponMsgID2 + 5, Mail::WeaponMsg, ObjectID, BalluteNetMessage> MsgBallute;
}

class wBalluteMail
{
public:
	wBalluteMail()
	{
		globalMail->reg( Mail::GenWorldHandler<MsgBallute, wAmmunitionBallute>()(&wAmmunitionBallute::OnNetworkMessage) );

	}

	~wBalluteMail()
	{
		globalMail->unreg(MsgBallute::ID);
	}

	void SendBalluteSuppress(wAmmunitionBallute* bomb, bool suppress_ballute)
	{
		BalluteNetMessage msg;
		msg.suppress_ballute = suppress_ballute;

		globalWorldNet->netSend(0, MsgBallute(bomb->ID(), msg));
	}

};


class wAmmunitionBalluteDescriptor : public wAmmunitionDescriptor
{
public:
	wAmmunitionBalluteDescriptor(const ed::string& name);
	virtual ~wAmmunitionBalluteDescriptor();

protected:
	virtual wAmmunition* create_() const;
};


#endif
