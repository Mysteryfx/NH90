#ifndef IwNetObject_h
#define IwNetObject_h


namespace Mail { class Message; }


enum {
	NET_UPDATE_HIGH = 1,
	NET_UPDATE_NORMAL = 2,
	NET_UPDATE_LOW = 4,

	NET_UPDATE_PRIOS = 3
};

class IwNetObject {
protected:
	virtual ~IwNetObject() = 0 {}

public:
	/**
	 * Get master hostID
	 */
	virtual int netGetHost() const { return 0; }

	/**
	 * Get create message
	 */
	virtual bool netGetCreate(Mail::Message&) const { return false; }

	/**
	 * Get periodic update message.
	 */
	virtual bool netSendUpdate(int dest, int prio, int& cookie) { return false; }

	/**
	 * Discad object.
	 */
	virtual void netDiscard() {}

};


#endif /* IwNetObject_h */
