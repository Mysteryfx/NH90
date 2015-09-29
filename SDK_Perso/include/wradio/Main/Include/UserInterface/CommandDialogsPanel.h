#ifndef __CommandDialogsPanel__
#define __CommandDialogsPanel__

#include "wRadio.h"
#include "ICommandDialogsPanel.h"

#include <ed/string.h>

struct lua_State;
namespace Lua
{
	class Config;
}
class Control;

//Implementation of ICommandDialogsPanel based on code in ./Scripts/UI/RadioCommandDialogPanel/RadioCommandDialogsPanel.lua

class CommandDialogsPanel : public ICommandDialogsPanel
{
	class InputLayerSwitcher
	{
	public:
		InputLayerSwitcher();
		void acquire();
		void free();
		void ensureLayerIsActive(); //To be used after track interruption and taking control 
	private:
		unsigned int requestCount;
	} inputLayerSwitcher;
public:
	WRADIO_API CommandDialogsPanel();

	//ICommandDialogsPanel
	virtual void initialize(ObjectID unitID);
	virtual void loadIntercom(const Intercom & intercom);
	virtual void loadInterphone(const Interphone & interphone);
	virtual void loadRadio(const Radio & radio);
	virtual void release();
	virtual void clear();
	virtual void setShowSubtitles(bool on);
	virtual void toggleMenu();
	virtual void toggleMenu(bool on);
	virtual void setCommunicatorId(unsigned char deviceId);
	virtual void setVoIP(bool on);
	virtual unsigned char getCommunicatorId() const;
	virtual void selectAndTuneCommunicatorFor(wCommunicator * pRecepient);
	virtual void onTrackInterrupt();
	virtual void onCommunicatorDeath(wCommunicator * pCommunicator);
	virtual void switchToMainMenu();
	virtual void switchDialog();
	virtual bool visible();
	virtual void onMsgStart(wMessage * pMessage, wCommunicator * pRecepient);
	virtual void onMsgFinish(wMessage * pMessage, wCommunicator * pRecepient);
	virtual void onMsgEvent(wMsg event, wCommunicator * pSender);
	virtual void selectMenuItem(int itemNum);
	virtual void onMenuItem(const ed::string & id, int itemNum);
	
	//Custom commands
	virtual Path addCustomCommand(const Path & path, const ed::string & name, unsigned int actionIndex, const ed::string & primaryMenuItem);
	virtual Path addCustomCommand(const Path & path, const ed::string & name, unsigned int actionIndex);
	virtual Path addCustomSubMenu(const Path & path, const ed::string & name, const ed::string & primaryMenuItem);
	virtual Path addCustomSubMenu(const Path & path, const ed::string & name);
	virtual void removeCustomItem(const Path & path, const ed::string & primaryMenuItem);
	virtual void removeCustomItem(const Path & path);
	virtual void clearCustomMenu(const ed::string & primaryMenuItem);
	virtual void clearCustomMenu();

	//Implementation
	void acquireCommandMenu();
	void freeCommandMenu();

	static void WRADIO_API regLua(lua_State * L);
};

#endif __CommandDialogsPanel__
