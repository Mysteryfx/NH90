#ifndef __AMMUNITION_GUIDED_H__
#define __AMMUNITION_GUIDED_H__

#include "Weapons.h"

#include "Ammunition/ammunition_descriptor.h"
#include "Ammunition/ammunition_new.h"

#include "IWeapon.h"
#include "IWorldNet.h"

#include "WorldMail.h"

struct GuidedWeaponNetMessage;

class WEAPONS_API wAmmunitionGuided : public wAmmunition, public IwMissile
{
public:
	wAmmunitionGuided();
	virtual ~wAmmunitionGuided();

	//virtual double simulate();

	virtual woPointer	getHost();
    virtual void setLauncher(MovingObject* obj);

    // From IwMissile
	virtual void setTarget(ObjectID id);
    virtual void MissileSetTarget(const cVector& tpos);
    virtual wModelTime	getShotTime();

    virtual void PostCollisionProcessing(wsCollisionData* colData, ObjectID objId);
	virtual viFlag getViFlag();

protected:	
	virtual void initCommon_(MovingObject * launcher,const wAmmunitionDescriptor* desc);
    void onNetMessage(const GuidedWeaponNetMessage& msg);
	virtual bool updateTargetPnt_();

protected:
    ObjectID    targetId_;
	woPointer   carrier_;
	Math::Vec3d targetPnt_;
	bool		hasTargetPnt_;
    bool        targetDirectlySet_;

protected:
    friend class wGuidedWeaponMail;
};

struct GuidedWeaponNetMessage
{
	enum
	{
		MSG_SET_TARGET_ID,
		MSG_SET_TARGET_POINT,
	};

	unsigned char messageType;
	union
	{
		struct
		{
			ObjectID objectID;
		};
		struct
		{
			float x, y, z;
		};
	};
};

WEAPONS_API Mail::Stream& operator >> (Mail::Stream& stream, GuidedWeaponNetMessage& msg);

namespace {
	typedef Mail::GenMessage<wWeaponMsgID2 + 2, Mail::WeaponMsg, ObjectID, GuidedWeaponNetMessage> MsgGuidedWeapon;
}

class wGuidedWeaponMail
{
public:
	wGuidedWeaponMail()
	{
		globalMail->reg( Mail::GenWorldHandler<MsgGuidedWeapon, wAmmunitionGuided>()(&wAmmunitionGuided::onNetMessage) );

	}
	
    ~wGuidedWeaponMail()
    {
        globalMail->unreg(MsgGuidedWeapon::ID);
    }

	void setTargetIDMessage(wAmmunitionGuided* vikhr, ObjectID obj)
	{
		GuidedWeaponNetMessage msg;
		msg.messageType = GuidedWeaponNetMessage::MSG_SET_TARGET_ID;
		msg.objectID = obj;

		globalWorldNet->netSend(0, MsgGuidedWeapon(vikhr->ID(), msg));
	}

	void setTargetPointMessage(wAmmunitionGuided* vikhr, double x, double y, double z)
	{
		GuidedWeaponNetMessage msg;
		msg.messageType = GuidedWeaponNetMessage::MSG_SET_TARGET_POINT;
		msg.x = x;
		msg.y = y;
		msg.z = z;

		globalWorldNet->netSend(0, MsgGuidedWeapon(vikhr->ID(), msg));
	}
};


class wAmmunitionGuidedDescriptor : public wAmmunitionDescriptor
{
public:
	wAmmunitionGuidedDescriptor(const ed::string& name);
    virtual ~wAmmunitionGuidedDescriptor();

protected:
	virtual wAmmunition* create_() const;
};


#endif
