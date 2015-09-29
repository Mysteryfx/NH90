#ifndef   _CCCLICKABLE_H
#define   _CCCLICKABLE_H

#include "IcClickable.h"
#include "wTime.h"
#include "ccClickableElementData.h"
#include "ed\unordered_set.h"

class Vector3;


namespace Lua
{
    class Config;
}

namespace cockpit
{
class avDevice;
class ccMainPanel;
class ccHelpWindow;
class SimpleOctTree;

class CockpitActionRepeater : public wTime
{
	int				repeated_command;
	int				after_stop_command;
	double          repeated_value;
	unsigned char   receiver_device;
public:
	CockpitActionRepeater();
	virtual ~CockpitActionRepeater();

	virtual void StopRepeat(float StopValue = 0.0f);
	virtual void StartRepeat(int Command,unsigned char device_id = 0,float value = 0,int after_stop_command = 0,bool repeatable = false );
	virtual void NextEvent();
};

class Clickable : public IcClickable
{
public:
	Clickable(const ed::string& name);
	virtual ~Clickable();
//Dialog
//Dialog
    void              setInterface(const char * name,void * data);
    void*             getInterface(const char * name);


	void			  set_init_conditions(wsInitData*);
	void			  set_init_conditions(wsPlaneJump*);
			
    MovingObject *    getCarrier() const;
	bool			  initialize(MovingObject *human,const char * custom_name = 0);
	//COCKPITBASE_API   void release(); // такое объявление для защиты
	void			  release();

	void			  SetCommand(int command , float value =0 );
	//void			  ProcessCommand(int command, float value =0);
	void			  SetGlobalCommand(int command, float value = 0);
    void              LogCommand      (int command);
    void              LogCommand      (int command, float value);

	void			  SetCockpitMessage(int device_id, int command, float value = 0);
	void			  OnCockpitEvent(const ed::string & event, const Graphics::DParamList &params = Graphics::DParamList());
	//void			  SetGlobalCommand(int command);//интерпретация команд старого кокпита

	bool			  checkClick();   //Возвращает false если не найдено зарегистрированного шейпа в непосредственной близости курсора
	bool              checkClickScreenSpace();
	void			  processClick(int action = PRIMARY_ACTION); //обработка найденного шейпа
	void			  setCursorMode(CursorMode mode);
    CursorMode        getCursorMode() const { return cursorMode; } 
	bool              isOnClickableItem(){return CursorOnClickableItem;};
	void              drawUtilities(); 
	void              createLua();
	void              parseCommon(const ed::string& script);
	bool              create_aircraft_data(MovingObject * human,const char * custom_name = NULL);
    void              draw_update();
	void              draw();
	void			  parse_for_render(Graphics::ModelParser &parser);
    void              draw_screenspace(bool independent_indicators = false);
    void              draw_independent_indicators();
//	void			  on_rendering_foreground(smCamera * cam);

    bool              have_arcade();

	void			  external_parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects);
// camera interface
	const Graphics::Camera & get_camera(int render_target);
	dVector			  get_camera_point(int render_target);
	TVScreenCondition get_screen_condition(int render_target);
	void			  get_screen_params(int render_target,Graphics::DParamList & args);
    bool              render_target_always(int n) const;

	int               get_render_count();
	void		   	  render_device(int render_target);
	int               get_current_element_type();
	bool              get_night_vision_goggles(NightVisionParams & out_params);
    const MirrorRenderParams * get_mirrors_camera() const;

	void              update_padlock();
	inline  const clickableElementData * get_current_item() const { return CurrentItem.get();}

	clickableElementData * get_item_by_name(const ed::string & name);
	void               check();
	void               maintenance_command(int command,float value = 0);
    void               maintenance_command(const ed::string & callback_function);
	bool			   get_show_hints()  const { return show_hints;}

	void				set_damage(unsigned slot,double value);
    void                on_planned_failure(const ed::string & failure_id,double probability, double start_, double interval_);

	ObjectID			get_target_of_interest() const;
	bool                getPointOfInterest(POINTS_OF_INTEREST kind, cPoint& pt) const;
	bool			    get_oxy_starvation_effects(float & effect1, float & effect2) const;

	float     get_screen_aspect() const { return aspect;}
	float     get_screen_height() const { return heightScreen;}
	float     get_screen_width () const { return widthScreen;}	

    IcChartManager & getChartManager();
    IcGroundManager& getGroundCockpitManager();
	
	bool			 link_to_target(const unsigned int TargetID, const unsigned int weapon_type);

	virtual AI::wPlayerTaskIndicator * getTaskHandler() const;

	void	  set_screenspace_camera();
	
    void      start();// для активации у же готового кокпита ( после создания сразу , или при отложенном старте)

	// old sound
	virtual Sound::Host& getSoundHost(const char* name);
	// new sound
	virtual void setSounder(ed::SounderRef sndrIn);

	__declspec(dllexport)  void  parseGlobalConfig();

protected: //functions

    bool loadElementData(Lua::Config & config,clickableElementData &shapeData);

	void loadClickableData(const ed::string& script);

    void load_elements(Lua::Config & config);

	void  processElement_Like_Button     (clickableElementData &data,int action = PRIMARY_ACTION,float x_error = 0,float y_error = 0);
	void  processElement_Like_SwingButton(clickableElementData &data,int action = PRIMARY_ACTION,float x_error = 0,float y_error = 0);
	void  processElement_Like_Switcher   (clickableElementData &data,int action = PRIMARY_ACTION,float x_error = 0,float y_error = 0);
	void  processElement_Like_Axis       (clickableElementData &data,int action = PRIMARY_ACTION,float x_error = 0,float y_error = 0);
	
	void  processElement_Like_VirtualAxis(clickableElementData &data,int action ,int command,float value);

	Position3 modelPosition();
	Position3 cameraPosition();
    E2::Vector calculateError(clickableElementData &data);
	E2::Vector screenPoint(const Vector3 &vec);           //точка на экране по мировым координатам 
	bool	   worldSpaceSightLine(float X_screen,float Y_screen,Vector3& start,Vector3& end);

	void      set_clickable_mode(CursorMode mode);
 	void	  reg_in_script(lua_State *Ls);
	void	  freeSharedResources();


	void 	  set_attribute(const ed::string & property_name) { attributes.insert(property_name); };
	bool	  has_attribute(const ed::string & property_name) { return attributes.find(property_name) != attributes.end(); }

protected: //data
	ed::unordered_set<ed::string>  attributes;
	lua_State             *L;
	Mem::Ptr<clickableElementData>  LastClickedItem;
	Mem::Ptr<clickableElementData>  CurrentItem;

	int                    LastAction;

    float				   mouseStateX;
	float                  mouseStateY;
	float                  mouseSpeed;

	CursorMode             cursorMode;
	void				   onCursorModeChanged(CursorMode mode);
	bool                   CursorOnClickableItem;
	bool                   check_send();// передавать ли команду или как (при проигрывании трека не передавать)
	bool                   use_pointer_name;

	CockpitActionRepeater		*repeater;
	ccHelpWindow			*helpWindow;
	float aspect;       // отношение горизонтальногo раствора к вертикальному
	float widthScreen;  // разрешение экрана приведенное к float
	float heightScreen;

	SimpleOctTree  *elementsTree;

	bool           show_element_boxes;
	bool           show_tree_boxes;
	bool           show_element_parent_boxes;
    bool           show_other_pointers;
    bool           show_indicator_borders;

	bool		   show_hints;

	class Clickable_checker : public SimpleOctTree_check_object
	{
	public:

		virtual bool valid(SimpleOctTreeLoad *obj)
		{
			result.push_back(static_cast<clickableElementData *>(obj));
			return false;
		}
		ed::vector<clickableElementData*> result; 
	};

	Clickable_checker clickable_checker_;

	void onCommandCockpitClickModeOnOff(float value);

private:
    ed::unordered_map<ed::string,void*> interfaces;

    void           checkIndependentIndicators();
};

}

COCKPITBASE_API inline cockpit::Clickable* ClickableInstance()
{
    return static_cast<cockpit::Clickable*>(cockpit::instance());
}


#endif // _CCCLICKABLE_H
