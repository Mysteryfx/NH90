#pragma once

#include "Common.h"
#include <ed/string.h>

#include "Vector4.h"
#include "Renderer/Material.h"
#include "Renderer/Camera.h"
#include "AuxMath.h"

#include "IcClickable.h"
#include "Avionics/avDevice.h"

#include "Indicators/ccElement.h"
#include "Randomizer/RandomObject.h"
#include "Utilities/avLuaRegistrable.h"


namespace cockpit
{

struct indicator_geometry
{
    indicator_geometry()
    {
        delta_size_x  = 0;
        delta_size_y  = 0;
        render_target = -1;
    }
	ed::vector<ed::string> shape_names;
	Vector3					 delta_pos;
	Vector3					 delta_rot;
    Vector3					 delta_pos_l;
	Vector3					 delta_rot_l;
	int						 render_target;
	float					 delta_size_x;
	float					 delta_size_y;
};

class ccDrawable;
class ceFont;

typedef ed::unordered_map<ed::string, Graphics::Material*>		ccMaterialMap;
typedef ed::unordered_map<ed::string, cockpit::ceFont*>			ccFontCockpitMap;
typedef ed::vector<ccDrawable*>						ccRenderTargets;

typedef ed::unordered_map<ed::string, void (*)(Element*, ccDrawable*, const Graphics::DParamList& args)> ccElementsControllersMap;

class COCKPITBASE_API ccDrawable_BasicTimer : public avBasicTimer<ccDrawable>
{
public:
    ccDrawable_BasicTimer(ccDrawable* _parent,wModelTime _dtime):avBasicTimer<ccDrawable>(_parent,_dtime){}
    virtual ~ccDrawable_BasicTimer(){};
	virtual void NextEvent();
};



struct COCKPITBASE_API indicator_viewport
{
    indicator_viewport(double x_ = 0.0,
                       double y_ = 0.0,
                       double w_ = 100.0,
                       double h_ = 100.0):x(x_),y(y_),w(w_),h(h_){}
    double x;
    double y;
    double w;
    double h;
};

class ccCockpitContext;

class COCKPITBASE_API ccDrawable : public ccContextRelatedObject,
                                   public avLuaRegistrable

{
public:
	ccDrawable();

	virtual ~ccDrawable();

	virtual void initialize(avDevice*, unsigned char, const ed::string&);
	virtual void post_initialize() {}
	virtual void release();
	virtual void serialize(Serializer&) = 0;
	virtual void draw() = 0;
    virtual void draw(unsigned char level)=0;
    virtual void draw_screenspace() = 0;
    virtual void draw_to_viewport(const indicator_viewport & viewport) = 0;
	virtual void draw_to_render_target()
	{
		update(DRAW_LEVEL_RENDER_TARGET);
		draw(DRAW_LEVEL_RENDER_TARGET);
	}
    virtual bool is_draw_screenspace() const= 0;
	virtual void update() = 0;
    virtual void update(unsigned char level) = 0;
	virtual void control() = 0;
    virtual void start(unsigned step = 0);
    virtual void SetCommand(int command,float value = 0){};
	virtual void OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params) {};

    //force indicator to draw at this purpose
    virtual void push_purpose(int purpose) = 0;
    virtual void pop_purpose() = 0;

    //////////////////////////////////////////////////////////////////////////
    void      set_controller(avDevice* controller_in){controller = controller_in;};
    avDevice* get_controller() {return controller;};
    //////////////////////////////////////////////////////////////////////////
    virtual void              set_geometry(const indicator_geometry & geom) = 0;
	virtual const Graphics::Camera & get_camera() = 0;
	virtual dVector			  get_camera_point() = 0;
	virtual TVScreenCondition get_screen_condition() = 0;
	virtual void			  get_screen_params(Graphics::DParamList & args) = 0;
    virtual bool              get_render_target_always() const = 0;
    virtual void              draw_temporal_set(Element *start_point,unsigned temporal_set,unsigned char level) = 0;
	virtual int               getType() const= 0;
	//virtual void update_element(Element*) = 0;
	virtual const ccElementsControllersMap& get_elements_controllers() const = 0;
	virtual ccElementsControllersMap&       get_elements_controllers_ref()= 0;
	virtual const int get_render_target() const = 0;

    virtual Element * get_element_on_current_page(const char * name) = 0;


    virtual const Position3& get_position(bool dynamic = false) const = 0;
    virtual const Position3& get_initpos() const = 0;

    void                    make_default_activity(double delta = 0.05);
	// data definitions
	typedef ed::vector<ccDrawable*> ccDrawableList;
    static  int l_get_indicator(lua_State*		L_state);
protected:
	virtual void            RegisterInScript(lua_State*		L_state){};
	lua_State*			    L;
	ccDrawable_BasicTimer*  p_indicator_timer;
	friend class ILinkVar;
    avDevice *              controller;
#ifdef _ED_HIDDEN_DEBUG 
public:
    bool test_indication;
#endif
	//static functions:
public:
	static float            get_random();
	static void				initMaterials(Lua::Config & config);
protected:
	//static data:
public:
    static ccMaterialMap	materials;
    static ccFontCockpitMap fonts_cockpit;
    static ccRenderTargets	render_targets;
    static int              render_count;

    static Graphics::Material* find_material(const ed::string & name);
	typedef ed::map<ccDrawable*, dev_links> IndVarsGlobalMap;
    static IndVarsGlobalMap LinkVarsSpace;
protected:
	static RandomObject     randomEvenly;
    static ccDrawable*		LastCreatedIndicator;

};

}
