#ifndef AVDEVICE_H
#define AVDEVICE_H

#include "CockpitBase.h"
#include "Common.h"
#include "Utilities/ccLuaLoader.h"
#include "Renderer/DynamicParams.h"
#include "wMovingObject.h"
#include "dPosition.h"
#include "Randomizer/RandomObject.h"
#include "Utilities/avDevLink.h"
#include <assert.h>
#include "Utilities/avBasicTimer.h"
#include "Utilities/avLuaRegistrable.h"
#include "KeyCommandsHandler.h"
#include "ccCockpitContext.h"

class  IwHumanPlane;
class  wHumanCommunicator;
struct wsInitData;
struct wsPlaneJump;

struct lua_State;

namespace Lua
{
    class Config;
}

namespace cockpit
{

class clickableElementData;
class ccCockpitContext;

class avDevice;

class COCKPITBASE_API avDevice_BasicTimer : public avBasicTimer<avDevice>
{ 
    avDevice_BasicTimer();
public:
	avDevice_BasicTimer(avDevice* _parent, wModelTime _dtime) : avBasicTimer<avDevice>(_parent,_dtime){};
	virtual void NextEvent();
};

struct animation_element_data
{
	animation_element_data() : 
		arg(0), clickable_command(0), arg_increment(0.0f),
		arg_lim_min(0.0f), arg_lim_max(0.0f),direct_set(false) {};
	
	int arg, clickable_command;
	float arg_increment,
		  arg_lim_min,
		  arg_lim_max;
    bool  direct_set;

    void  read(Lua::Config & config);
};

typedef ed::map<int, ed::vector<animation_element_data> > animation_map;

class COCKPITBASE_API DeviceMessage
{
public:
	DeviceMessage():id(0){}
	int		     id;
	Graphics::DParamList args;
};

class COCKPITBASE_API avDevice : public ccContextRelatedObject,
                                 public avLuaRegistrable
{
public:
	avDevice();
	virtual ~avDevice();
	virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void release();
	virtual void serialize(Serializer& serializer);
	virtual void SetCommand(int command, float value = 0);
	virtual void OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params);
    virtual void update() {assert(!"Why you do need EMPTY activity ?");}
	void		 insertCallbackElement(int command,clickableElementData *data);
	unsigned char ID() const {return InstanceID;}
	const ed::string& Name() const {return InstanceName;}
	
	virtual void post_initialize();
	virtual bool checkCallbackKeyboard(int command, float& value);
	virtual bool checkCallbackClickable(int command, float& value); /*return true when it needs to call SetCommand. */
	// Vous pouvez également vérifier si vous avez besoin de changer val. argument
	virtual bool checkAnimationConditions(int arg_num, int command, float& value){return true;};
	void performClickableAction(int command, float value, bool echo = false /*set to true if you only want to move element w/o SetCommand call*/);

    void         unregister();

    void         start(unsigned step = 0);  //старт     
    bool         is_active() const { return active;} //J'ai commencé

	virtual void external_parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects) {};

// STATIC
	static avDevice*	create(ccCockpitContext * context_,const ed::string&, unsigned char, const ed::string& name, const ed::string& script_name ="");

	static ed::string     common_path; // путь к общим файлам
	static ed::string     path; // полный путь
    static void			   init_commands(avDevice*, const ed::string&);
protected:

	lua_State *  ensured_Lua(const char * dbg_state_name = NULL);
    void         close_LuaState();
	void         make_default_activity(double delta_time = 0.05);

public:
	static  int l_set_command(lua_State*		L_state);
	static  int l_get_device (lua_State*		L_state);
	static  int l_get_self   (lua_State*		L_state);
	static  int l_perform_clickable_action(lua_State*		L_state);
    static  int l_set_global_command(lua_State*		L_state);
	static  int l_listen_command(lua_State*		Lua_s);
	static int  l_listen_event(lua_State*		Lua_s);

    void        lua_call_SetCommand(int command, float value);
    void        hide_callback_element(int key, bool value);

    clickableElementData* getClickableElement(int command);

	class UpdateHandler
	{
	public:
		virtual void onUpdate(avDevice* Initiator, int Event) = 0;
	};

	avDevice::UpdateHandler* getUpdateHandler() {return UpdHndl_;}
	void setUpdateHandler(avDevice::UpdateHandler* HndlIn) {UpdHndl_ = HndlIn;}

	static RandomObject						device_random_orderly;
	static RandomObject						device_random_evenly;
	bool									first_update;
	
	friend class ILinkVar;
	typedef ed::map<avDevice*, dev_links>   DeviceVarsGlobalMap;
	static DeviceVarsGlobalMap				LinkVarsSpace;
	
	// Were moved here as we need them just as flags in some devices
	// Are loaded from "difficulty" table
	static bool EasyFlight;
	static bool EasyRadar;

protected:

	void switcher_value_iterate(int target_command,int direction = 1);
	void switcher_value_iterate_reversible(int target_command,int* current_direction);
	void axis_value_iterate(int target_command,float direction_speed = 1.0);
	void button_value_iterate  (int target_command, bool inverse = false );
	void button_value_ON_OFF  (int target_command, bool On);

	unsigned char							InstanceID;
	ed::string								InstanceName;
	static avDevice*						LastCreatedDevice;

	lua_State*								L;
	avDevice_BasicTimer*					p_device_timer;	
	ed::map<int, clickableElementData*>    ClickElements;
	animation_map							animation_data;

	virtual void register_in_script(lua_State *Lua_s);

	virtual const char * l_metatable_name() const { return "device_meta";}
	virtual void		 l_register_functions(lua_State *Lua_s,int index);
	
    bool                 read_init_script_from_mission;//прочитать скрипт из миссии если он там есть , либо записать туда при записи трека и prepare

	avDevice::UpdateHandler* UpdHndl_;
private: 
    bool    have_script_name;
    bool    active;
	int		have_set_command_in_state;
};

enum device_mode_enum
{
	LEVEL_MASTER,
	LEVEL_SECOND,
	LEVEL_THIRD,
	LEVEL_FOURTH
};

struct COCKPITBASE_API Device_Mode
{
	Device_Mode(unsigned char _master = 0,
				unsigned char _lev2   = 0,
				unsigned char _lev3   = 0,
				unsigned char _lev4   = 0) :
		master(_master), level_2(_lev2), level_3(_lev3), level_4(_lev4) {}
	
	Device_Mode(const Device_Mode &mode)
	{
		*this = mode;	
	}

	void clear_sub_levels()
	{
		level_2 = 0;
		level_3 = 0;
		level_4 = 0;
	}

	Device_Mode& operator = (const Device_Mode &mode)	
	{
		if (this != &mode)
			long_representation = mode.long_representation;
		
		return *this;
	}

	bool operator != (const Device_Mode &mode) const
	{
		return !(*this == mode); 
	}

	bool operator == (const Device_Mode &mode) const 
	{
		return long_representation == mode.long_representation; 
	}

	unsigned char& operator[](int i)	   { return char_array[i];} 
	unsigned char  operator[](int i) const { return char_array[i];} 
	union 
	{
		struct 
		{
			unsigned char master;
			unsigned char level_2;
			unsigned char level_3;
			unsigned char level_4;
		};
		unsigned int   long_representation;
		unsigned char	char_array[4];
	};

	friend Serializer& operator << (Serializer& serializer, Device_Mode& mode) {return serializer << mode.long_representation;}

	void save_to_state  (lua_State* L) const;
    void load_from_state(Lua::Config & config);
};

}

#endif //AVDEVICE_H
