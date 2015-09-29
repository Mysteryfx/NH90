#ifndef _ICCLICKABLE_H
#define _ICCLICKABLE_H

#include "CockpitBase.h"
#include "WorldPlugin.h"
#include "WorldPluginManager.h"
#include <ed/string.h>
#include "IwHumanPlane.h"
#include "Randomizer/RandomObject.h"
#include "Renderer/DynamicParams.h"
#include <ed/SounderAPI.h>

class smCamera;
struct wsInitData;
struct wsPlaneJump;
class wHumanCommunicator;
class IcGroundManager;

// Old sound
namespace Sound 
{ 
    class Host;
}

namespace AI
{
	class wPlayerTaskIndicator;
}

enum CursorMode
{
	CURSOR_MODE_UNDEFINED = -1,
	CURSOR_MODE_CLICKABLE,
	CURSOR_MODE_CLICKABLE_AND_CAMERA,
	CURSOR_MODE_CAMERA,
};

class  IcChartManager;
struct NightVisionParams;
namespace cockpit 
{
	class clickableElementData;
}


enum POINTS_OF_INTEREST
{ 
	SENSORS_STARE_PT,
	FLYTO_PT,
};

class COCKPITBASE_API IcClickable : public WorldPlugin
{
public:
			 IcClickable(const ed::string& name) : WorldPlugin(name) {}
	virtual ~IcClickable(){}

    virtual void  setInterface(const char * name,void * data) = 0;
    virtual void* getInterface(const char * name) = 0;

	virtual void  set_init_conditions(wsInitData*) = 0;
	virtual void  set_init_conditions(wsPlaneJump*) = 0;
	
    virtual MovingObject * getCarrier() const = 0;
	virtual bool  initialize(MovingObject * human,const char * custom_name = 0) = 0;//if custom_name specified then load from Aircrafts/custom_name
	virtual void  release()= 0;
    virtual void  start()=0;

	virtual void  SetCommand	  (int command, float value = 0) = 0;
	virtual void  SetGlobalCommand(int command, float value = 0) = 0;//интерпретация команд старого кокпита
    virtual void  LogCommand      (int command) = 0;//лог для макросов 
    virtual void  LogCommand      (int command, float value) = 0;//лог для макросов 

	//virtual void  SetCockpitMessage(int device_id, int command, float value = 0) = 0;
	virtual void  OnCockpitEvent(const ed::string & event, const Graphics::DParamList &params = Graphics::DParamList()) = 0;

    virtual void  draw_update() = 0;
	virtual void  draw() = 0;
    virtual void  draw_screenspace(bool independent_indicators = false) = 0;
    virtual bool  have_arcade() = 0;
	virtual void  parse_for_render(Graphics::ModelParser &parser) = 0;
//	virtual void  on_rendering_foreground(smCamera * cam) = 0;

	virtual void  external_parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects)=0;

	virtual const Graphics::Camera & get_camera(int render_target) = 0;
	virtual dVector			  get_camera_point(int render_target) = 0;
	virtual TVScreenCondition get_screen_condition(int render_target) = 0;
	virtual int               get_render_count() = 0;
	virtual void			  get_screen_params(int render_target,Graphics::DParamList & args) = 0;
	virtual void		   	  render_device(int render_target) = 0;
	
	virtual bool              get_night_vision_goggles(NightVisionParams & out_params)=0;
    virtual const MirrorRenderParams * get_mirrors_camera() const = 0;

	virtual void              update_padlock()=0;
	virtual void              maintenance_command(int command,float value = 0)=0;

	virtual void			  reg_in_script(lua_State *Ls)= 0;

	virtual void			  set_damage(unsigned slot,double value) = 0;

    virtual void              on_planned_failure(const ed::string & failure_id,double probability, double start_, double interval_) = 0;

	virtual ObjectID		  get_target_of_interest() const = 0;
	virtual bool			  getPointOfInterest(POINTS_OF_INTEREST kind, cPoint& pt) const = 0;

	virtual bool			  get_oxy_starvation_effects(float & effect1, float & effect2) const = 0;

    virtual bool              render_target_always(int n) const = 0;

	// old sound
	virtual Sound::Host&      getSoundHost(const char* name) = 0;
	// new sound
	virtual void              setSounder(ed::SounderRef sndrIn) = 0;

    virtual IcChartManager&   getChartManager() = 0;
    virtual IcGroundManager&  getGroundCockpitManager() = 0;

	virtual bool			  link_to_target(const unsigned int TargetID, const unsigned int weapon_type) = 0;

	virtual void* GetInterface(const char * name_of_interface)  {	return this; };

	virtual AI::wPlayerTaskIndicator * getTaskHandler() const = 0;
	
	virtual bool			  has_attribute(const ed::string & property_name) { return false; }
	
	inline		bool		  isClickableMode()	{return getCursorMode() != CURSOR_MODE_CAMERA;};
	virtual		void		  setCursorMode(CursorMode mode) = 0;
	virtual		CursorMode    getCursorMode() const = 0;
	virtual		cockpit::clickableElementData    * get_item_by_name(const ed::string & name) = 0;
};




namespace cockpit
{

	inline IcClickable * getAsPlugin()
	{
		WorldPlugin * plug = worldPluginManager->GetPlugin("ClickableCockpit");
		if (plug)
			return	(IcClickable*)plug->GetInterface();
		return nullptr;
	}

	extern COCKPITBASE_API IcClickable* instance();

}

#endif //_ICCLICKABLE_H
