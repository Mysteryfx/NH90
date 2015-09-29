#ifndef WO_CAR_MAIL_H
#define WO_CAR_MAIL_H

#include "WorldMail.h"
#include "Comm/wNetCommunicator.h"

enum wMsg;
#include "woCar.h"

class woCarMail {
public:
	typedef GenMessage<woCarMsgID+1, Mail::StatusMsg, ObjectID, woCar::Status> MsgStatus;
    typedef GenMessage<woCarMsgID+2, Mail::DamageMsg, ObjectID, float, ObjectID, ObjectID> MsgDamage;
	typedef GenMessage<woCarMsgID+3, Mail::CreateMsg, woCar::InitNetCar, Mail::Reader> MsgCreate;
	typedef GenMessage<woCarMsgID+4, Mail::Reliable,  ObjectID, unsigned int> MsgSetIDPusk;
	typedef GenMessage<woCarMsgID+5, Mail::Reliable,  ObjectID> MsgRecharge;
    typedef GenMessage<woCarMsgID+6, Mail::CreateMsg,  ObjectID> MsgDestroy;
    typedef GenMessage<woCarMsgID+7, Mail::CreateMsg,  ObjectID, int32_t> MsgActivate;
    typedef GenMessage<woCarMsgID2+1, Mail::CreateMsg,  ObjectID> MsgMasterArmOn;
    typedef GenMessage<woCarMsgID2+2, Mail::CreateMsg,  ObjectID> MsgMasterArmOff;

	// Net comm
	typedef GenMessage<woCarMsgID2+3, Mail::Reliable, ObjectID, int, float, float, float, wModulation>	MsgCommCreateUpdate;
	typedef GenMessage<woCarMsgID2+4, Mail::Reliable, ObjectID, unsigned int, unsigned int>				MsgCommChangeCommNetFlags;	
	typedef GenMessage<woCarMsgID2+5, Mail::Reliable, ObjectID, wMessageData>							MsgCommStartMessage;
	typedef GenMessage<woCarMsgID2+6, Mail::Reliable, ObjectID, bool>									MsgCommFinishMessage;
	typedef GenMessage<woCarMsgID2+7, Mail::Reliable, ObjectID, bool, ed::string>						MsgDatalinkStatusMsg;

	typedef GenMessage<woCarMsgID2+8, Mail::Reliable, ObjectID, bool, bool, int>						MsgHumanTakeControl;

    typedef GenMessage<woCarMsgID2+9, Mail::WeaponMsg, ObjectID, Mail::Reader >    						MsgWeaponStatus;
	typedef GenMessage<woCarMsgID2+10, Mail::CreateMsg,  ObjectID, int32_t> MsgActivateAI;

	typedef GenMessage<wNucleusMsgID+1, Mail::Reliable, ObjectID, cVector, int> MsgColorSmokeData;
	typedef GenMessage<wNucleusMsgID+2, Mail::Reliable, ObjectID, int> MsgColorFlareData;

		woCarMail();
		~woCarMail();

	static void sendDamage(woCar* car, float deltaLife, ObjectID shooter, ObjectID weapon);
	static bool getBaseline(const woCar *, woCar::InitNetCar&, Mail::Message&);
	static bool sendStatus(const woCar*, int dest);
    static void sendWeaponStatus(woCar* car, int dest = -1);
	static void setIDPusk(const woCar*, unsigned int id_);
	static void recharge(const woCar*);
    static void destroy(const woCar*);
    static void activate(const woCar*,int32_t);
	static void activateAI(const woCar*,int32_t);
    static void master_arm(const woCar*, bool arm_);

	static void sendDatalinkStatusMsg(const woCar* obj, bool onOff, const ed::string & ownTN);

	static void commCreateUpdate(const woCar*, int callsign, float power, float frequency, float bandWidth, wModulation modulaiton);
	static void commChangeCommNetFlags(const woCar*, unsigned int transmitFlags, unsigned int receiveFlags);
	static void commStartMessage(const woCar*, const wMessagePtr & message);
	static void commFinishMessage(const woCar*, bool onTime);
	static void sendHumanTakeControl(const woCar*, bool, bool);

	static void sendColorSmokeData(const woCar* car, cVector smokeSpawnPoint, int smokeColorCode);
	static void sendColorFlareData(const woCar* car, int flareColorCode);
}; // class woCarMail

namespace Mail 
{
    Stream& operator >> (Stream& stream, woCar::Status& data);
	Stream& operator >> (Stream& stream, woCar::InitNetCar& data);
}

#endif /* WO_CAR_MAIL_H */
