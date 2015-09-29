#ifndef __ICommandDialogsPanel__
#define __ICommandDialogsPanel__

#include "Inter.h"
#include "Registry/Registered.h"
#include "ed/vector.h"
#include "ed/list.h"

class wCommunicator;
class wMessage;
enum wMsg;

/*
Command dialog panel is GUI element used to communicate with different objects: wingmens, ATC, AWACS e.t.c. 
and to activate custom mission commands.
The panel consists of one or more dialogs each with command menu and message log. Dialogs organized in tabs.
Each time only one dialog is visible.
The main dialog with main menu is always available.
Secondary dialogs are open as long as interaction with a service (ATC, Tanker, JTAC) is active.
Command dialog panel knows about on board radios and may tune them onto the required frequency automatically if
"Easy Comm" option is enabled.
*/

class INTER_API ICommandDialogsPanel
{
public:
	enum {	COMMUNICATOR_VOID = (unsigned char)-1,
			COMMUNICATOR_AUTO = (unsigned char)-2 };
	//Intercom data
	struct Intercom
	{
		Intercom() : id(COMMUNICATOR_VOID) {;}
		unsigned char id; //Device id in the cockpit
	};
	//Interphone data
	struct Interphone
	{
		Interphone() : id(COMMUNICATOR_VOID) {;}
		unsigned char id; //Device id in the cockpit
		ed::string displayName;
	};
	//Radio data
	struct Radio
	{
		Radio() : id(COMMUNICATOR_VOID), AM(false), FM(false) {;}
		unsigned char id; //Device id in the cockpit
		ed::string displayName;
		struct FrequencyRange
		{
			float min;
			float max;
		};
		typedef ed::vector<FrequencyRange> FrequencyRanges;
		FrequencyRanges frequencyRanges;
		typedef ed::vector<float> Channels;
		Channels channels;
		bool AM, FM;
	};
	virtual ~ICommandDialogsPanel() {;}
	virtual void initialize(ObjectID unitID) = 0;  //Initialize the panel for the unit
	virtual void loadIntercom(const Intercom & intercom) = 0; //Load intercom data
	virtual void loadInterphone(const Interphone & interphone) = 0; //Load interphone data
	virtual void loadRadio(const Radio & radio) = 0; //Load radio data
	virtual void release() = 0; //Releases the panel
	virtual void clear() = 0;
	virtual void setShowSubtitles(bool on) = 0; //Enables/disables subtitles
	virtual void toggleMenu() = 0; //Toggles menu
	virtual void toggleMenu(bool on) = 0;
	virtual void setCommunicatorId(unsigned char deviceId) = 0; //Sets current communicator id
	virtual void setVoIP(bool on) = 0;
	virtual unsigned char getCommunicatorId() const = 0;
	virtual void selectAndTuneCommunicatorFor(wCommunicator * pRecepient) = 0; //Selects and tunes the radio for the recepient
	virtual void onTrackInterrupt() = 0;
	virtual void onCommunicatorDeath(wCommunicator * pCommunicator) = 0;
	virtual void switchToMainMenu() = 0; //Switches the panel to the main dialog
	virtual void switchDialog() = 0; //Switches to the next dialog
	virtual bool visible() = 0;
	virtual void onMsgStart(wMessage * pMessage, wCommunicator * pRecepient) = 0; //Notify incoming message start
	virtual void onMsgFinish(wMessage * pMessage, wCommunicator * pRecepient) = 0; //Notify incoming message finish
	virtual void onMsgEvent(wMsg event, wCommunicator * pSender) = 0; //Notify message even if it was not received by the communciator
	virtual void selectMenuItem(int itemNum) = 0; //
	//Custom commands
	typedef ed::list<ed::string> Path;
	virtual Path addCustomCommand(const Path & path, const ed::string & name, unsigned int actionIndex, const ed::string & primaryMenuItem) = 0;//Add Command with primary menu punkt name "menu"
	virtual Path addCustomCommand(const Path & path, const ed::string & name, unsigned int actionIndex) = 0;
	virtual Path addCustomSubMenu(const Path & path, const ed::string & name, const ed::string & primaryMenuItem) = 0;
	virtual Path addCustomSubMenu(const Path & path, const ed::string & name) = 0;
	virtual void removeCustomItem(const Path & path, const ed::string & primaryMenuItem) = 0;
	virtual void removeCustomItem(const Path & path) = 0;
	virtual void clearCustomMenu(const ed::string & primaryMenuItem) = 0;
	virtual void clearCustomMenu() = 0;
};

extern INTER_API ICommandDialogsPanel * globalCommandDialogsPanel;

#endif __ICommandDialogsPanel__
