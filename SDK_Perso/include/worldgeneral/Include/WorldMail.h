#ifndef __WorldMail_h__
#define __WorldMail_h__


#include "cLinear.h"
#include "GlobalMail.h"
#include "Registry/RegisterManager.h"
#include "dPosition.h"

using Mail::GenMessage;


enum {
	SystemMsgID         = 0x00,
	woCarMsgID          = 0x20,
	woShipMsgID         = 0x28,
	wWeaponMsgID        = 0x30,
	woLAMsgID           = 0x40,
	wWeaponSystemMsgID  = 0x80,
	wPointMsgID         = 0x88,
	woCivMsgID          = 0x90,
	wColumnMsgID		= 0x95,//20 сообщений колонне
	wNucleusMsgID       = 0xA9,
	woLandMsgID         = 0x100,
    woSpotMsgID         = 0x106,
    woAirdromeMsgID     = 0x110,
	wTriggerMsgID       = 0x120,
    woCarMsgID2         = 0x150,
	wWeaponMsgID2		= 0x170, //оставить не менее ... 100
	woLAMsgID2          = 0x270,
	woShipMsgID2        = 0x280,

	viewUpdateMsgID		  = 1000,
	wWorldDataMsgID		  = 1010,
	wMissionResourceMsgID = 1024,

	wWorldEventMsgIDStart	  = 1536,
	wWorldEventMsgIDend 	  = 1664,

	WorldMissionCommandMsgID        = 1665,
    TransportMissionFunctionMsgID   = 1680,//10 message
	AutoSmokeMarkerMsgID			= 1695,//2 message
    wCargoMsgID                     = 1700,
	wPartsExtMsgID                  = 1710,
    woSnareMsgID                    = 1720,


    woMaxMsgID            = 2048, // это последний + 1 (то есть его можно использовать)
};


namespace Mail {

enum {
	StatusMsg = Unreliable|0,
	DamageMsg = Reliable|0,
	DamageAllMsg = Reliable|Broadcast|0,
	WeaponMsg = Reliable|Urgent|0,
	CreateMsg = Reliable|Urgent|0,
	DestroyMsg = Reliable|0,
	TriggerMsg = Reliable|0,
	EffectMsg = Reliable|0,//?Unreliable?

	UnknownMsg = 0
};


// handler generator stuff
template<class M_, class O_, class F_>
class _WorldHandler : public Mail::Handler {
	F_ func;
public:
		_WorldHandler(F_ f, const char* name) : Mail::Handler(M_::ID, M_::Flags, name), func(f) {}

	virtual bool handle(const Mail::Message& msg) {
		typename M_::Args args;
		Mail::Reader reader(msg);
		reader >> args;
		ObjectID id = args.value;
		Registered* r = RegisterManager::instance->Get(id);
		O_* obj = dynamic_cast<O_*>(r);
		if( obj==0 )
			return false;
		Mail::call(obj, func, args.next);
		return true;
	}
}; // class _WorldHandler


// generator
template<class M_, class O_>
struct GenWorldHandler {

template<class F_> inline
Mail::HandlerPtr operator()(F_ func, const char* name=0)
{
	return Mail::HandlerPtr( new _WorldHandler<M_, O_, F_>(func, name) );
}

};//


// handler generator stuff
template<class M_, class O_, class F_>
class _WorldMsgHandler : public Mail::Handler {
	F_ func;
public:
		_WorldMsgHandler(F_ f, const char* name) : Mail::Handler(M_::ID, M_::Flags, name), func(f) {}

	virtual bool handle(const Mail::Message& msg) {
		ObjectID id;
		Mail::Reader(msg) >> id;
		Registered* r = RegisterManager::instance->Get(id);
		O_* obj = dynamic_cast<O_*>(r);
		if( obj==0 )
			return false;
		(obj->*func)(msg);
		return true;
	}
}; // class _WorldHandler


// generator
template<class M_, class O_>
struct GenWorldMsgHandler {

template<class F_> inline
Mail::HandlerPtr operator()(F_ func, const char* name=0)
{
	return Mail::HandlerPtr( new _WorldMsgHandler<M_, O_, F_>(func, name) );
}

};//


// sub-object handler
template<class M_, class P_, class F_>
class _ChildHandler : public Mail::Handler {
	F_ func;
public:
		_ChildHandler(F_ f, const char* name) : Mail::Handler(M_::ID, M_::Flags, name), func(f) {}

	virtual bool handle(const Mail::Message& msg) {
		typename M_::Args args;
		Mail::Reader reader(msg);
		reader >> args;
		ObjectID& id = args.value;
		Registered* reg = RegisterManager::instance->Get(id);
		if( reg==0 )
			return false;
		P_& path = args.next.value;
		typename P_::Type* obj = path.find(reg);
		if( obj==0 )
			return false;
		Mail::call(obj, func, args.next.next);
		return true;
	}
}; // class _WorldHandler


// generator
template<class M_, class P_>
struct GenChildHandler {

template<class F_> inline
Mail::HandlerPtr operator()(F_ func, const char* name=0)
{
	return Mail::HandlerPtr( new _ChildHandler<M_, P_, F_>(func, name) );
}

};//


inline
Stream& operator >> (Stream& stream, cVector& v)
{
	return stream >> v.x >> v.y >> v.z;
}

inline
Stream& operator >> (Stream& stream, dVector& v)
{
    return stream >> v.x >> v.y >> v.z;
}

inline
Stream& operator >> (Stream& stream, cPosition& v)
{
    return stream >> v.x >> v.y >> v.z >> v.p;
}

inline
Stream& operator >> (Stream& stream, dPosition& v)
{
    return stream >> v.x >> v.y >> v.z >> v.p;
}

} // Mail namespace

#endif /* __WorldMail_h__ */
