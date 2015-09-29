#ifndef __woShipMail_h__
#define __woShipMail_h__

#include "woShip.h"
#include "WorldMail.h"

enum wMsg;

class woShipMail {
	friend class woShip;
public:
	typedef GenMessage<woShipMsgID+1, Mail::StatusMsg, ObjectID, woShip::Status> MsgStatus;
	typedef GenMessage<woShipMsgID+2, Mail::DamageMsg, ObjectID, float, unsigned char, ed::string, ObjectID, ObjectID> MsgDamage;
	typedef GenMessage<woShipMsgID+3, Mail::CreateMsg, woShip::InitNetShip, Mail::Reader> MsgCreate;
    typedef GenMessage<woShipMsgID+4, Mail::CreateMsg, ObjectID> MsgDestroy;
    typedef GenMessage<woShipMsgID+5, Mail::CreateMsg, ObjectID> MsgActivate;

	typedef GenMessage<woShipMsgID2+1, Mail::Reliable,  ObjectID, PathPointsNAV> MsgNAVSetPathToAI;
	typedef GenMessage<woShipMsgID2+2, Mail::Reliable,  ObjectID, AI::Route*> MsgNAVSetPathUpdate;
	typedef GenMessage<woShipMsgID2+3, Mail::Reliable,  ObjectID, int> MsgNAVSetGotoWaypointIdx;
	typedef GenMessage<woShipMsgID2+4, Mail::Reliable,  ObjectID, int> MsgNAVFormation;
	typedef GenMessage<woShipMsgID2+5, Mail::Reliable,  ObjectID, int> MsgNAVSetFormationUpdate;
	typedef GenMessage<woShipMsgID2+6, Mail::Reliable,  ObjectID, double> MsgNAVSpeed;
	typedef GenMessage<woShipMsgID2+7, Mail::Reliable,  ObjectID, double> MsgNAVSetSpeedUpdate;
	typedef GenMessage<woShipMsgID2+8, Mail::Reliable,  ObjectID, TargetPointNAV> MsgNAVSetTargetToAI;
	typedef GenMessage<woShipMsgID2+9, Mail::Reliable,  ObjectID, ObjectID> MsgNAVRemoveTargetToAI;
	typedef GenMessage<woShipMsgID2+10, Mail::Reliable,  ObjectID, int> MsgNAVROE;
	typedef GenMessage<woShipMsgID2+11, Mail::Reliable,  ObjectID, int> MsgNAVSetROEUpdate;

	typedef GenMessage<woShipMsgID2+12, Mail::Reliable,  ObjectID, int> MsgColorFlareData;

		woShipMail();
		~woShipMail();

	static void sendDamage(woShip*, float deltaLife, unsigned char element, const ed::string& area, ObjectID shooter, ObjectID weapon);
	static bool getBaseline(const woShip *, woShip::InitNetShip&, Mail::Message&);
	static bool sendStatus(const woShip*, int dest);
    static void destroy(const woShip*);
    static void activate(const woShip*);

	// path
	static void sendSetPathToAI(const woShip* host, const PathPointsNAV& );
	static void sendSetPathUpdateAI(const woShip* host, AI::Route *pRoute );
	static void sendGotoWaypointIdxAI( const woShip* host, int nIdx );

	// control
	static void sendRoeToAI( const woShip* host, int nRoe );
	static void sendSetRoeUpdateAI( const woShip* host, int nRoe );
	static void sendSpeedToAI(const woShip* host, double speed);
	static void sendSetSpeedUpdateAI( const woShip* host, double speed );
	static void sendFormationToAI( const woShip* host, int nFormation );
	static void sendSetFormationUpdateAI( const woShip* host, int nFormation );


	static void sendSetTargetToAI( const woShip* host, const TargetPointNAV& opt);
	static void sendRemoveTargetToAI( const woShip* host, ObjectID targetID );

	static void sendColorFlareData(const woShip* ship, int flareColorCode);

}; // class woShipMail


namespace Mail 
{
	Stream& operator >> (Stream& stream, woShip::Status& data);
	Stream& operator >> (Stream& stream, woShip::InitNetShip& data);
}

#endif /* __woShipMail_h__ */
