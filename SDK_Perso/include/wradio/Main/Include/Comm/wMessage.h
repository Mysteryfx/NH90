#ifndef __wMessage__
#define __wMessage__

#include "Mem/Shared.h"
#include "Utilities/LuaTable.h"
#include "Utilities/Parameter.h"

#include "wTime.h"
#include "IwDatalinkTerminal.h"
#include "wMovingObject.h"

enum wMsg;
class wRadioTransmitter;

//wMessage is the object that contains logical information (for AI untis and avionics) or/and sound/text information for the players.
class WRADIO_API wMessage : public Mem::Shared
{
public:

	enum Type
	{
		TYPE_NULL = 0,
		TYPE_CONSTRUCTABLE, //Sound list, subtitle string and duration will be constructed from data.
		TYPE_CONSTRUCTABLE_REMOTE, //Sound list, subtitle string will be constructed from data, but duration is received from the host.
		TYPE_MORZE, //Sound list, subtitle string and duration will be constructed from string that must be represented with Morze code.
		TYPE_FILE_AND_SUBTITLE, //Sound and subtitle are parameters of the message. Sound file is stored in Sounds directory.
		TYPE_MISSION_FILE_AND_SUBTITLE, //Sound and subtitle are parameters of the message. Sound file is stored in the mission file.
	};
	enum PlayMode
	{
		PLAY_MODE_NULL,
		PLAY_MODE_UNLIMITED_DURATION, //No duration. The message will never be finished.
		PLAY_MODE_LIMITED_DURATION, //The message has limited duration and  will be finished when the time is over.
		PLAY_MODE_LOOPED //The message is looped so it will never be finished.
	};
	//wMessage::Object is base class for both message senders and receivers.
	class WRADIO_API Object
	{
	public:
		typedef int Callsign;
		enum { NO_CALLSIGN = -1, DEFAULT_CALLSIGN = 0 };
		Object();
		Object(Callsign callsignIn, woPointer pUnitIn, const char * nameIn);		
		void			createLuaObject(lua_State * L) const;
		bool			valid() const;
		bool			operator == (const Object & object) const;
		bool			operator != (const Object & object) const;
		ed::string		name;
#ifdef DEBUG_TEXT
		ed::string		print() const;
#endif
		static Object*(*getById)(ObjectID objectID);
		Callsign		callsign; //Callsign
		woPointer		pUnit; //Reference to unit
	};

	//wMessage::Sender - the message sender
	class WRADIO_API Sender : public Object
	{
	public:
		Sender();
		Sender(Callsign callsignIn, woPointer pUnitIn, const char * nameIn);
		unsigned int		voice; //Voice identifier
	};
	typedef Object Receiver;

	typedef wMsg						Event;
	typedef LuaTable					Parameters;
	typedef ed::vector<ed::string>		Files;

	//Void message
	wMessage(Type typeIn = TYPE_NULL, PlayMode playModeIn = PLAY_MODE_NULL);
	
	//Message represented by event id and parameters (TYPE_CONSTRUCTABLE or TYPE_CONSTRUCTABLE_REMOTE)
	wMessage(const Sender & senderIn, const Event & eventIn, PlayMode playModeIn = PLAY_MODE_LIMITED_DURATION);
	wMessage(const Sender & senderIn, const Event & eventIn, const Parameters & parameters, PlayMode playModeIn = PLAY_MODE_LIMITED_DURATION, wModelTime durationIn = 0.0);
	
	//Message represented by string to convert it into Morze code (TYPE_MORZE)
	wMessage(const Sender & senderIn, const Event & eventIn, const ed::string & lettersStr, PlayMode playModeIn = PLAY_MODE_LIMITED_DURATION);
	
	//Message with sound file name and subtitle string (TYPE_FILE_AND_SUBTITLE, MISSION_TYPE_FILE_AND_SUBTITLE)
	wMessage(const Sender & senderIn, const Event & eventIn, const ed::string & fileName, bool missionFile, const ed::string & subtitleIn, PlayMode playModeIn = PLAY_MODE_UNLIMITED_DURATION, wModelTime durationIn = 0.0);
	virtual ~wMessage();

	bool operator == (const wMessage & message) const;

	void				start(); //Start the message

	//Sound interface begin
	const wModelTime	getDuration() const;
	const ed::string &	getDirectory() const;
	const Files &		getFiles() const; //Get sound file names relative to the directory.
	const ed::string &	getSubtitle() const;	
	//Sound interface end

	void				fromState(lua_State * L, int index); //Load wMessage from the lua state
	void				toState(lua_State * L) const; //Convert wMessage to lua table and push it into lua state

#ifdef DEBUG_TEXT
	ed::string			print() const; //Debug print
#endif
private:
	void				buildContent_() const; //Build message from event id and parameters (for TYPE_CONSTRUCTABLE, TYPE_CONSTRUCTABLE_REMOTE and TYPE_MORZE only)
public:
	Type				type;
	PlayMode			playMode;
	Sender				sender;
	Receiver			receiver;
	//Content
	Event				event; //Event id. Indicates how to process/understand the message
	ed::string			paramStr; //String that contains morze string or file name
	Parameters			parameters; //Message parameters. This structure is organized like lua table
    ed::vector<wRadioTransmitter*> pRadioTransmitters; //Transmitters that is used to transmit the message
	//Synchronization
	wModelTime			birthTime;
	wModelTime			startTime;
private:
	//Output (lazy evaluated)
	mutable bool		contentBuildt; //Flag of content readiness (for TYPE_CONSTRUCTABLE, TYPE_CONSTRUCTABLE_REMOTE and TYPE_MORZE only)
	mutable wModelTime	duration; //The message duration
	mutable	ed::string	directory; //The directory for the message's sound files
	mutable Files		files; //The message sound files
	mutable ed::string	subtitle; //Subtitle string
	static int			subtitleLanguage;
	static int			speechLanguage;
};

typedef Mem::Ptr<wMessage> wMessagePtr;

//wMessageDatalink is the datalink message. It contains datalink datagram in datalink_msg
class WRADIO_API wMessageDatalink : public wMessage
{
public:
	wMessageDatalink();
	wMessageDatalink(const wMessage::Sender & senderIn, const datalink_msg & msg_);
	void set(const datalink_msg &msg_) {param = msg_;}
	datalink_msg & get() {return param;}
private:
	datalink_msg param;
};

#endif
