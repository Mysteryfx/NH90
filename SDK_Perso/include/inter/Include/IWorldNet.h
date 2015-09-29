#ifndef IWorldNet_h
#define IWorldNet_h


#include "Inter.h"
#include <ed/string.h>
#include <stdint.h>


namespace Mail { class Message; }

enum { NET_NEWBIES_ID = 0xFFFF };

class IwNetObject;


enum { NET_NO_CREATE = 0x8000,  NET_NO_DELETE_ON_DISCARD = 0x4000};


class IWorldNet {
protected:
	virtual ~IWorldNet() = 0 {}

public:
	virtual bool netIsRunning() const = 0;
	virtual bool netIsServer() const = 0;

	virtual int netServerID() const = 0;
	virtual int netLocalID() const = 0;

	// send a message
	// toHost==0 - broadcast message
	virtual void netSend(int toHostID, const Mail::Message&) = 0;

	virtual void netAddObject(IwNetObject* obj, int mode) = 0;
	virtual void netDelObject(IwNetObject* obj) = 0;

    virtual ed::string netGetPlayerName(int host) const = 0;

    virtual uint64_t registerSerial(const ed::string& starforce_id) = 0;
    virtual void unregisterSerial(uint64_t handle) = 0;

    virtual bool netAllowObjectExport() const = 0;
};


extern INTER_API IWorldNet *globalWorldNet;


#endif /* IWorldNet_h */
