#pragma once 

#include "WorldGeneral.h"
#include "Registry\Registered.h"
#include "cLinear.h"
#include "cPosition.h"
#include <ed/list.h>

struct datalink_msg
{
public:
	datalink_msg() : 
	  senderID(0), receiverID(0), targetID(0), pos(0), msg_type(0), param1(0), param2(0), orientation_known(false), localOnly(false) {};

	  datalink_msg(ObjectID s, ObjectID r, ObjectID t, const dVector & p,	char m, int p1, int p2):
	  senderID(s), receiverID(r), targetID(t), pos(p), msg_type(m), param1(p1), param2(p2), orientation_known(false), localOnly(false) {};

	  datalink_msg(ObjectID s, ObjectID r, ObjectID t, const dVector & p,	char m, int p1, int p2, const ed::string &txt):
	  senderID(s), receiverID(r), targetID(t), pos(p), msg_type(m), param1(p1), param2(p2), message(txt), is_text_info(true), orientation_known(false), localOnly(false) {};

	  datalink_msg(ObjectID s, ObjectID r, ObjectID t, const ed::string &text):
	  senderID(s), receiverID(r), targetID(t), message(text), is_text_info(true), localOnly(false) {}

	  ObjectID	senderID;
	  ObjectID	receiverID;
	  ObjectID	targetID;
	  dVector	pos;
	  char		msg_type;
	  int		param1;
	  int		param2;

	  cPosition	orientation;
	  bool		orientation_known;

	  bool      is_text_info;
	  ed::string message;

	  bool		localOnly;
};

typedef ed::list<datalink_msg> datalink_msgs_queue;

struct DatalinkState;

class WORLDGENERAL_API IwDatalinkTerminal
{
public:
	IwDatalinkTerminal() : Ka50VCUHostHumber(-1), Ka50VCULeaderFlag(false) {;}
	virtual ~IwDatalinkTerminal() = 0;
	
	inline void setSADLNetId(const ed::string & SADLnetIdIn) { SADLnetId = SADLnetIdIn; }
	inline const ed::string & getSADLNetId() const { return SADLnetId; }
	inline void setKa50VCUHostNumber(int Ka50VCUHostHumberIn) { Ka50VCUHostHumber = Ka50VCUHostHumberIn; }
	inline int getKa50VCUHostNumber() const { return Ka50VCUHostHumber; }
	inline void setKa50VCULeaderFlag(bool ka50VCULeaderFlagIn) { Ka50VCULeaderFlag = ka50VCULeaderFlagIn; }
	inline bool getKa50VCULeaderFlag() const { return Ka50VCULeaderFlag; }

	virtual	void send_msg(const datalink_msg & msg) = 0;
	virtual	void receive_msg(const datalink_msg & msg) = 0;
	virtual	datalink_msgs_queue & get_msgs_gueue() = 0;
	virtual void getNetState(DatalinkState & datalinkState) const {;}
private:
	ed::string SADLnetId;
	int Ka50VCUHostHumber;
	bool Ka50VCULeaderFlag;
};
