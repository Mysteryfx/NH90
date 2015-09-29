#ifndef __woAirMail_h__
#define __woAirMail_h__

#include "woATC.h"
#include "wAirdrome.h"

enum ATCMessage;
enum wMsg;
struct ShelterState;
//typedef ed::vector<ShelterState> SheltersStatus;
class wHeliport;
struct wHeliportState;

struct woAirMail {
		woAirMail();
		~woAirMail();
	
	static void openDoor(woATC* atc, unsigned n_term_index);
	static void closeDoor(woATC* atc, unsigned n_term_index);
	static void sendAirbaseInitStatus(const wAirbase* airbase, Mail::Message& msg);
	static void sendAirdromeInitStatus(const wAirdrome* airdrome, Mail::Message& msg);
	static void sendClientMessage(woATC* atc, ObjectID client, ATCMessage message);
	static void sendRadioMessage(woATC* atc, ObjectID client, wMsg msg);
	static void setClientStatus(woATC* atc, ObjectID client, ClientStatus status);
	static void setCountry(wAirbase* air, wcCoalitionName coalition);
	static void setInitCountry(wAirbase* air, wcCoalitionName coalition);
	static void damage(wAirdrome* air, float modelTime, unsigned short dmg);
    static void terminalStatus(woATC* atc, wsAdrTerminal * term);
	static void helTerminalStatus(woATC* atc, int termIndex, wsHAI_terminal * term);
	static void createHeliport(ObjectID heliportID, const wHeliportState & state);

	static bool getHeliportBaseline(const wHeliport * heliport, Mail::Message& msg);
};



#endif /* __woAirMail_h__ */
