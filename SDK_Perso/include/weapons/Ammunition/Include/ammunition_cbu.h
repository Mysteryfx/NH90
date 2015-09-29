#ifndef __AMMUNITION_CBU_H__
#define __AMMUNITION_CBU_H__

#include "Weapons.h"

#include "Ammunition/ammunition_descriptor.h"
#include "Ammunition/ammunition_new.h"

#include "IWeapon.h"
#include "IWorldNet.h"

#include "WorldMail.h"

struct CbuNetMessage;

class WEAPONS_API wAmmunitionCbu : public wAmmunition
{
public:

	wAmmunitionCbu();
	virtual ~wAmmunitionCbu();

	//virtual double simulate();

	virtual woPointer	getHost();
	virtual void setLauncher(MovingObject* obj);
	void SetCbuParametrs(const Math::Vec3d& des_wx, double drop_height, double drop_time, bool fzu39 , bool has_wcmd , const Math::Vec3d& target_pos );

protected:	
	virtual void initCommon_(MovingObject * launcher,const wAmmunitionDescriptor* desc);
	void OnNetworkMessage(const CbuNetMessage& msg);

protected:
	ObjectID    targetId_;
	woPointer   carrier_;

protected:
	friend class wCbuMail;
};

struct CbuNetMessage
{
	Math::Vec3d  desired_wx;
	double drop_height;
	double drop_time;
	bool   fzu39;
    bool   has_wcmd;
	Math::Vec3d target_pos;
};

WEAPONS_API Mail::Stream& operator >> (Mail::Stream& stream, CbuNetMessage& msg);

namespace {
	typedef Mail::GenMessage<wWeaponMsgID2 + 3, Mail::WeaponMsg, ObjectID, CbuNetMessage> MsgCbu;
}

class wCbuMail
{
public:
	wCbuMail()
	{
		globalMail->reg( Mail::GenWorldHandler<MsgCbu, wAmmunitionCbu>()(&wAmmunitionCbu::OnNetworkMessage) );

	}

	~wCbuMail()
	{
		globalMail->unreg(MsgCbu::ID);
	}

	void SendCbuParametrs(wAmmunitionCbu* bomb, Math::Vec3d des_wx, double drop_height, double drop_time, bool fzu39 , bool has_wcmd, Math::Vec3d target_pos)
	{
		CbuNetMessage msg;
		msg.desired_wx  = des_wx;
		msg.drop_height = drop_height;
		msg.drop_time   = drop_time;
		msg.fzu39		= fzu39;
		msg.has_wcmd    = has_wcmd;
		msg.target_pos   = target_pos ;

		globalWorldNet->netSend(0, MsgCbu(bomb->ID(), msg));
	}

};


class wAmmunitionCbuDescriptor : public wAmmunitionDescriptor
{
public:
	wAmmunitionCbuDescriptor(const ed::string& name);
	virtual ~wAmmunitionCbuDescriptor();

protected:
	virtual wAmmunition* create_() const;
};


#endif
