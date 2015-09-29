#ifndef __AMMUNITION_NET_H__
#define __AMMUNITION_NET_H__

#include "wsType.h"
#include "Mail/Message.h"
#include "WorldMail.h"

/*enum eAmmunitionNetMsgs
{
	AMMO_NETMSG_FIRST	= wNewWeaponFirstMsgID,

	AMMO_NETMSG_CREATE	= AMMO_NETMSG_FIRST,
};*/

struct wNetCreateInfo
{
	// тип
//	int lev1, lev2, lev3, lev4;

	ObjectID id;
	ObjectID carrier_id;
};

/*template <eAmmunitionNetMsgs ID, int FL> class wAmmunitionMessage : public Mail::Message
{
public:
	wAmmunitionMessage() : Mail::Message(ID, FL) {};
};

typedef wAmmunitionMessage<AMMO_NETMSG_CREATE, Mail::CreateMsg> wAmmunitionCreateMessage;*/

#endif
