#ifndef __AMMUNITION_LASER_HOMING_H__
#define __AMMUNITION_LASER_HOMING_H__

//#include "Ammunition/ammunition_descriptor.h"
#include "ammunition_self_homing.h"

struct LaserHomingWeaponNetMessage;

class wAmmunitionLaserHoming : public wAmmunitionSelfHoming
{
public:
	wAmmunitionLaserHoming();
	void WEAPONS_API setLaserCode(unsigned laserCode);
protected:
	virtual void initCommon_(MovingObject * launcher, const wAmmunitionDescriptor* desc);
    void OnNetworkMessage(const LaserHomingWeaponNetMessage& msg);
	friend class wLaserHomingWeaponMail;
private:
	unsigned * laserCode_;
};

struct LaserHomingWeaponNetMessage
{
	enum
	{
		MSG_SET_LASER_CODE
	};

	unsigned char messageType;
	unsigned laserCode;
};

WEAPONS_API Mail::Stream& operator >> (Mail::Stream& stream, LaserHomingWeaponNetMessage& msg);

namespace {
	typedef Mail::GenMessage<wWeaponMsgID2 + 4, Mail::WeaponMsg, ObjectID, LaserHomingWeaponNetMessage> MsgLaserHomingWeapon;
}

class wLaserHomingWeaponMail
{
public:
	wLaserHomingWeaponMail()
	{
		globalMail->reg( Mail::GenWorldHandler<MsgLaserHomingWeapon, wAmmunitionLaserHoming>()(&wAmmunitionLaserHoming::OnNetworkMessage) );
	}

	~wLaserHomingWeaponMail()
	{
		globalMail->unreg(MsgLaserHomingWeapon::ID);
	}

	void setLaserCode(wAmmunitionLaserHoming* ammunitionLaserHoming, int laserCode)
	{
		LaserHomingWeaponNetMessage msg;
		msg.messageType = LaserHomingWeaponNetMessage::MSG_SET_LASER_CODE;
		msg.laserCode = laserCode;

		globalWorldNet->netSend(0, MsgLaserHomingWeapon(ammunitionLaserHoming->ID(), msg));
	}
};

class wAmmunitionLaserHomingDescriptor : public wAmmunitionDescriptor
{
public:
	wAmmunitionLaserHomingDescriptor(const ed::string& name);
	virtual ~wAmmunitionLaserHomingDescriptor();

protected:
	virtual wAmmunition* create_() const;
};

#endif __AMMUNITION_LASER_HOMING_H__
