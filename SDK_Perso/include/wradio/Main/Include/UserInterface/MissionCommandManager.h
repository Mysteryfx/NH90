#ifndef _MissionCommandManager_h_
#define _MissionCommandManager_h_

/*
Custom mission command system (F10. Other).
Custom commands are located in command dialog panel in "F10. Other" submenu.
Custom commands are used to perform custom actions: trigger actions, scripts.
Custom commands are organized in submenus and have hierarchical structure
Each custom command or submenu can be corresponded to all units, or only to specific coalition or only to specific group.
Custom command menu is dynamic and can be modified run-time.
*/

#include "ed/vector.h"
#include "ed/list.h"
#include "ed/map.h"

#include "Exception.h"

#include "wRadio.h"
#include "IwNetObject.h"
#include "WorldMail.h"
#include "CoalitionInfo.h"

struct lua_State;

namespace Mail
{
	class Message;
}

//Interface of mission command
class WRADIO_API IMissionCommandAction
{
public:
	virtual ~IMissionCommandAction();
	virtual void operator()() = 0;
};

/*
Reference counter for the mission command.
Several command items may refer to the same mission command. This class is used to determine life time of the command.
*/

class MissionCommandActionRefered
{
public:
	MissionCommandActionRefered();
	MissionCommandActionRefered(IMissionCommandAction * action);
	MissionCommandActionRefered(const MissionCommandActionRefered & other);
	MissionCommandActionRefered & operator = (const MissionCommandActionRefered & other);
	~MissionCommandActionRefered();
	void set(IMissionCommandAction * action);
	inline void addRef() { ++refCount_; }
	bool releaseRef();
	inline IMissionCommandAction * get() { return action_; }
private:
	mutable IMissionCommandAction * action_;
	unsigned int refCount_;
};

//Mission command item
struct MissionCommand
{
	ed::string caption;
	unsigned int actionIndex;
};

//Mission command menu
struct MissionCommandMenu
{
	ed::string caption;
	
	struct Item
	{
		enum Type { TYPE_COMMAND, TYPE_MENU };
		Type type;
		unsigned int index;
	};
	typedef ed::vector<Item> Items;
	Items items;

	typedef ed::vector<MissionCommandMenu> Submenus;
	Submenus subMenus; //Children submenus
	typedef ed::vector<MissionCommand> Commands;
	Commands commands; //Children commands
};

typedef ed::list<ed::string> MissionCommandMenuPath;

class MissionCommandManager;

//Generator of network messages
class MissionCommandMail : public IwNetObject
{
public:
	MissionCommandMail(MissionCommandManager * missionCommandManager);
	~MissionCommandMail();

	void onMissionStart();
	void onMissionFinish();
	virtual bool netGetCreate(Mail::Message& msg) const;

	void sendMsgAddCommand(const MissionCommandMenuPath & path, const ed::string & name, unsigned int actionIndex);
	void sendMsgAddSubMenu(const MissionCommandMenuPath & path, const ed::string & name);
	void sendMsgRemoveItem(const MissionCommandMenuPath & path);

	void sendMsgAddCommandForCoalition(wcCoalitionName coalition, const MissionCommandMenuPath & path, const ed::string & name, unsigned int actionIndex);
	void sendMsgAddSubMenuForCoalition(wcCoalitionName coalition, const MissionCommandMenuPath & path, const ed::string & name);
	void sendMsgRemoveItemForCoalition(wcCoalitionName coalition, const MissionCommandMenuPath & path);

	void sendMsgAddCommandForGroup(wcCoalitionName coalition, unsigned int groupMissionID, const MissionCommandMenuPath & path, const ed::string & name, unsigned int actionIndex);
	void sendMsgAddSubMenuForGroup(wcCoalitionName coalition, unsigned int groupMissionID, const MissionCommandMenuPath & path, const ed::string & name);
	void sendMsgRemoveItemForGroup(wcCoalitionName coalition, unsigned int groupMissionID, const MissionCommandMenuPath & path);

	void sendMsgDoAction(unsigned int actionIndex);
private:
	MissionCommandManager * missionCommandManager_;
};

//Custom commands menus: common, for different coalitions, for different groups
struct MissionCommandManagerData
{
	MissionCommandManagerData();
	MissionCommandMenu		commandMenuCommon; //For all
	bool					commandMenuForCoalitionFlag[COALITION_MAX];
	MissionCommandMenu		commandMenuForCoalition[COALITION_MAX]; //For different coalitions
	typedef ed::map<unsigned int, MissionCommandMenu> CommandMenuForGroups;
	CommandMenuForGroups	commandMenuForGroups[COALITION_MAX]; //For different groups
	MissionCommandMenu	    commandMenuForDescent; //For different types
};	

//Manager of mission custom commands
class WRADIO_API MissionCommandManager
{
	typedef IMissionCommandAction Action;
	typedef MissionCommandActionRefered ActionRef;
	typedef MissionCommand Command;
	typedef MissionCommandMenu CommandMenu;
	typedef MissionCommandMenuPath Path;
	typedef MissionCommandManagerData Data;	
	typedef MissionCommandMail Mail;
	
	MissionCommandManager();
public:
	static MissionCommandManager & instance();
	
	void onMissionStart();
	void onMissionFinish();
	
	bool netGetCreate(::Mail::Message& msg) const;	 //Generates initialization message for the clients

	void setMenu(wcCoalitionName coalition, unsigned int groupMissionID); //Initializes the manager by the player's group.

	//Local menu control (server)
	Path addCommand(const Path & path, const ed::string & name, Action * action);
	Path addSubMenu(const Path & path, const ed::string & name);
	void removeItem(const Path & path);

	Path addCommandForCoalition(wcCoalitionName coalition, const Path & path, const ed::string & name, Action * action);
	Path addSubMenuForCoalition(wcCoalitionName coalition, const Path & path, const ed::string & name);
	void removeItemForCoalition(wcCoalitionName coalition, const Path & path);

	Path addCommandForGroup(wcCoalitionName coalition, unsigned int groupMissionID, const Path & path, const ed::string & name, Action * action);
	Path addSubMenuForGroup(wcCoalitionName coalition, unsigned int groupMissionID, const Path & path, const ed::string & name);
	void removeItemForGroup(wcCoalitionName coalition, unsigned int groupMissionID, const Path & path);

	void doAction(unsigned int actionIndex); //Call the action by index

	//Remote menu control (client)
	void netInitialize(const Data & data); //Initializes the manager on the client by message from manager on the server
	
	//Remote menu modification (client)
	void netAddCommand(const Path & path, const ed::string & name, unsigned int actionIndex);
	void netAddSubMenu(const Path & path, const ed::string & name);
	void netRemoveItem(const Path & path);

	void netAddCommandForCoalition(wcCoalitionName coalition, const Path & path, const ed::string & name, unsigned int actionIndex);
	void netAddSubMenuForCoalition(wcCoalitionName coalition, const Path & path, const ed::string & name);
	void netRemoveItemForCoalition(wcCoalitionName coalition, const Path & path);	

	void netAddCommandForGroup(wcCoalitionName coalition, unsigned int groupMissionID, const Path & path, const ed::string & name, unsigned int actionIndex);
	void netAddSubMenuForGroup(wcCoalitionName coalition, unsigned int groupMissionID, const Path & path, const ed::string & name);
	void netRemoveItemForGroup(wcCoalitionName coalition, unsigned int groupMissionID, const Path & path);

	void netDoAction(unsigned int actionIndex);

    wcCoalitionName getCoaltionName() { return coalition_; }
    unsigned int	getGroupMissionId() { return groupMissionID_; }

private:
	void setMenu_(const CommandMenu & commandMenu);

	//Menu control operations
	Path addCommand_(const Path & path, const ed::string & name, unsigned int actionIndex, bool addAction);
	Path addSubMenu_(const Path & path, const ed::string & name);
	void removeItem_(const Path & path, bool removeActions);
	void doAction_(unsigned int actionIndex);

	CommandMenu & getCommandMenuforCoalition_(wcCoalitionName coalition, bool incRefs);
	Path addCommandForCoalition_(wcCoalitionName coalition, const Path & path, const ed::string & name, unsigned int actionIndex, bool addAction);
	Path addSubMenuForCoalition_(wcCoalitionName coalition, const Path & path, const ed::string & name, bool copyActions);
	void removeItemForCoalition_(wcCoalitionName coalition, const Path & path, bool removeActions);

	CommandMenu & getCommandMenuforGroup_(wcCoalitionName coalition, unsigned int groupMissionID, bool incRefs);
	Path addCommandForGroup_(wcCoalitionName coalition, unsigned int groupMissionID, const Path & path, const ed::string & name, unsigned int actionIndex, bool addAction);
	Path addSubMenuForGroup_(wcCoalitionName coalition, unsigned int groupMissionID, const Path & path, const ed::string & name, bool copyActions);
	void removeItemForGroup_(wcCoalitionName coalition, unsigned int groupMissionID, const Path & path, bool removeActions);

	Path addCommandTo_(CommandMenu & commandMenu, const Path & path, const ed::string & name, unsigned int actionIndex, bool addAction, bool applyToPanel, const ed::string & primaryMenuItem);
	Path addCommandTo_(CommandMenu & commandMenu, const Path & path, const ed::string & name, unsigned int actionIndex, bool addAction, bool applyToPanel);
	Path addSubMenuTo_(CommandMenu & commandMenu, const Path & path, const ed::string & name, bool applyToPanel, const ed::string & primaryMenuItem);
	Path addSubMenuTo_(CommandMenu & commandMenu, const Path & path, const ed::string & name, bool applyToPanel);
	void removeItemFrom_(CommandMenu & commandMenu, const Path & path, bool removeActions, bool applyToPanel, const ed::string & primaryMenuItem);
	void removeItemFrom_(CommandMenu & commandMenu, const Path & path, bool removeActions, bool applyToPanel);

	//Action reference counter operations
	ActionRef & acquireActionRef_(unsigned int & actionIndex);
	void releaseActionRef_(unsigned int actionIndex);

	//Command menus operations
	void copyCommandMenu_(CommandMenu & dst, const CommandMenu & src, bool incRefs);
	void incrementCommandsRefs_(const CommandMenu & commandMenu);
	void clearMenu_(CommandMenu & commandMenu, bool removeActions);

	Mail			mail_;
	Data			data_;
	wcCoalitionName coalition_; //Coalition of the player's unit
	unsigned int	groupMissionID_; //Mission id of the player's group
	typedef ed::vector<ActionRef> ActionsRefs;
	ActionsRefs		actionsRefs_; //Actions of mission commands are stored in separate container.
	typedef ed::vector<unsigned int> FreeActionIndexes;
	FreeActionIndexes freeActionIndexes_; //List of free action indexes that is used to assign index for new actions.
};



#endif _MissionCommand_h_
