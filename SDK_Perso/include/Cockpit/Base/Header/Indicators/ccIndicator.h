#ifndef ccIndicator_H
#define ccIndicator_H

#include "Renderer\Material.h"
#include "Renderer\Camera.h"
#include "AuxMath.h"
#include "Indicators\ccDrawable.h"
#include "Indicators\ccIndicationRenderParser.h"

namespace gui 
{
	class Window;
}

namespace cockpit
{

typedef unsigned char pageID;
typedef unsigned char setID;

const static pageID PAGE_NULL = 0;

class ccDrawable;
class ccIndicatorPage;

typedef ed::map<setID, ccIndicatorPage*>  ccLuaElementSetsMap;
typedef ed::vector<ccIndicatorPage*>	   ccLuaElementSet;
typedef ed::map<pageID, ccLuaElementSet>  ccLuaPageMap;



enum curr_purpose_mode
{
    CURR_PURPOSE_GENERAL,
    CURR_PURPOSE_HUD_ONLY_VIEW,
    CURR_PURPOSE_SCREENSPACE_INSIDE_COCKPIT,
    CURR_PURPOSE_SCREENSPACE_OUTSIDE_COCKPIT,
	CURR_PURPOSE_GROUND_UNIT_OPERATOR,
	CURR_PURPOSE_GROUND_UNIT_OPERATOR_SCREENSPACE
};


class COCKPITBASE_API 
ccIndicator : public ccDrawable
{
	gui::Window * wnd;//helper window for positioning
public:
	ccIndicator();
	virtual ~ccIndicator();

	// ccDrawable interface;
	virtual void initialize(avDevice*, unsigned char, const ed::string&);
	virtual void post_initialize() {}
	virtual void release();
	virtual void serialize(Serializer&);
	virtual void draw();
    virtual void draw_screenspace();
    virtual void draw_to_viewport(const indicator_viewport & viewport);
    virtual bool is_draw_screenspace() const;
    virtual void draw(unsigned char level);
	virtual void update();
    virtual void update(unsigned char level);
	virtual void control(){ assert(!"why you need EMPTY activity");};
	
	virtual void draw_temporal_set(Element *start_point,unsigned temporal_set,unsigned char level) {};
	// end of ccDrawable interface;

	// une position centrale et dimensions
    void                    set_geometry(const indicator_geometry & geom);
	void					initGeometry();

	void					initPage(const setID, const char*);
	virtual void			initPages();
	void					SetCurrentPage(pageID NewPage);
	ccLuaElementSet*		GetCurrentPage();
	float					GetHalfHeight(){return HalfHeight;};
	float					GetHalfWidth (){return HalfWidth;};
	int						getType()  const    {return type;};

	const int				get_render_target() const { return render_target_current;};
	// camera data
	virtual const Graphics::Camera & get_camera();
	virtual dVector			  get_camera_point();
	virtual TVScreenCondition get_screen_condition();
	virtual void			  get_screen_params(Graphics::DParamList & args){};
    const Position3&          get_position(bool dynamic = false) const
    {
        return (dynamic) ? dynamic_center_pos : center_pos;
    }

	Position3& get_position_ref(bool dynamic = false)
	{
		return (dynamic) ? dynamic_center_pos : center_pos;
	}

    void log_current_page(ed::string & out);

    const Position3& get_initpos() const
    { 
        return init_pos;
    }
    virtual Element * get_element_on_current_page(const char * name);

    virtual bool              get_render_target_always() const { return render_target_always; }

    virtual void check_page(Device_Mode ModeIn);
	
		
	const ccIndicatorPage *   get_page_by_ID(setID id) const
	{
		ccLuaElementSetsMap::const_iterator set = ElementSets.find(id);
		if (set != ElementSets.end())
		{
			return set->second;
		}
		return 0;
	}
	
	ccIndicatorPage *   get_page_by_ID(setID id)
	{
		ccLuaElementSetsMap::iterator set = ElementSets.find(id);
		if (set != ElementSets.end())
		{
			return set->second;
		}
		return 0;
	}
	// controllers
	virtual void create_elements_controllers_map();
	virtual const ccElementsControllersMap& get_elements_controllers() const {return elements_controllers;}
	virtual ccElementsControllersMap& get_elements_controllers_ref() {return elements_controllers;}

	static void remove_orientation(Element*, ccDrawable*, const DParamList&);
	static void rotate_element(Element*, ccDrawable*, const DParamList&);
	static void move_element(Element*, ccDrawable*, const DParamList&);
	static void scale_element(Element*, ccDrawable*, const DParamList&);

	static void fov_control(Element*, ccDrawable*, const DParamList&);
	static void draw_sight(Element* element, ccDrawable* parent, const DParamList& args);
	static void txt_txt_control(Element*, ccDrawable*, const DParamList&);
	static void	txt_lo_version     (Element*, ccDrawable*, const DParamList&);
	static void	txt_lo_resource    (Element*, ccDrawable*, const DParamList&);
	static void	txt_lo_serialnumber(Element*, ccDrawable*, const DParamList&);
		
	static void	blinking(Element*, ccDrawable*, const DParamList&);
	static void	blinking_direct(Element*, ccDrawable*, const DParamList&);

	static void render_purpose(Element*, ccDrawable*, const DParamList&);
    static void external_view_control(Element*, ccDrawable*, const DParamList&);

    static void screenspace_position_control(Element* element, ccDrawable* parent, const DParamList& args);
    

	static void set_draw_by_enum(Element* element,const DParamList& args,unsigned value);
	static void set_draw_boolean(Element* element,const DParamList& args,bool value);

    static void bound_by_box(Element* element, ccDrawable* parent, const DParamList& args);
    static void bound_by_circle(Element* element, ccDrawable* parent, const DParamList& args);
    static void bound_by_circle(Vector3 & p,float R1,float R2);

    static void bound_by_circle_without_root(Element* element, ccDrawable* parent, const DParamList& args);

    static void draw_argument_in_range(Element* element, ccDrawable* parent, const DParamList& args);
    static void txt_UTF8_substr(Element* element, ccDrawable* parent, const DParamList& args);
    
    static void increase_render_target_counter(Element* element, ccDrawable* parent, const DParamList& args);

	static void rotate_using_parameter(Element* element, ccDrawable* parent, const DParamList& args);
	static void move_up_down_using_parameter(Element* element, ccDrawable* parent, const DParamList& args);
	static void move_left_right_using_parameter(Element* element, ccDrawable* parent, const DParamList& args);
	static void parameter_in_range(Element* element, ccDrawable* parent, const DParamList& args);
	static void compare_parameters(Element* element, ccDrawable* parent, const DParamList& args);
	static void parameter_compare_with_number(Element* element, ccDrawable* parent, const DParamList& args);

	static void change_color_when_parameter_equal_to_number(Element* element, ccDrawable* parent, const DParamList& args);


	static void text_using_parameter(Element* element, ccDrawable* parent, const DParamList& args);
	static void opacity_using_parameter(Element* element, ccDrawable* parent, const DParamList& args);
	static void line_object_set_point_using_parameters(Element* element, ccDrawable* parent, const DParamList& args);
	static void change_texture_state_using_parameter(Element* element, ccDrawable* parent, const DParamList& args);


    static bool global_point_to_screen_space(const dVector & pnt,Element* element,bool restrict = true);
    static bool global_point_to_screen_space(const dVector & ray,float & x,float & y,bool restrict = true);
    static bool to_screen_space             (const dVector & ray,Element* element,bool restrict = true);
    static bool to_screen_space             (const dVector & ray,float & x,float & y,bool restrict = true);
	int                   get_render_purpose() const { return get_render_purpose(screen_space_now); }
	bool                  check_purpose(int type_of_screenspace) const;

	inline const Vector3  get_collimator_default_distance_factor() const { return collimator_default_distance_factor; };

protected:
    virtual void          l_register_functions(lua_State *Lua_s,int index);
	static int			  l_get_self   (lua_State*		Lua_s);

    static int            l_add_purpose(lua_State*		Lua_s);
    static int            l_remove_purpose(lua_State*   Lua_s);  
	static int            l_set_page(lua_State*   Lua_s);  

    virtual void		  RegisterInScript(lua_State*		L_state);

	virtual void          PrepareParser();
	void				  set_render_materials();

    //force indicator to draw at this purpose
    virtual void push_purpose(int purpose);
    virtual void pop_purpose();
	virtual void remove_purpose(int purpose);


    int                   get_render_purpose(bool screenspace_now) const;
    void                  update_geometry();
    void                  update_position(int purpose,const indicator_viewport * v = NULL);
	
	ccLuaElementSet*	  current_page;			// page info actuelle MFD
	ccLuaElementSetsMap   ElementSets;          // jeux de cartes d'éléments
	ccLuaPageMap          PageMap;              // mapper les pages logiques

	//Отрисовка
	Graphics::Material*   render_material;
	Graphics::Camera      render_camera;	    // TV и радар
	TVScreenCondition     render_screen_type;

	//Геометрия 
	Position3			  init_pos;		    // La position initiale du centre
	Position3			  center_pos;       // Current (converti) en position centrale
	Position3			  dynamic_center_pos;
	float				  HalfWidth;
	float				  HalfHeight;
	unsigned char         type;
	int                       render_target_current;
	ccIndicationRenderParser  render_parser;
	ed::string               render_mask;

    bool                  render_target_always;

	ccElementsControllersMap elements_controllers;
	
    ed::vector<int>      purposes;

    indicator_viewport    screenspace_viewport;
    indicator_viewport    screenspace_viewport_arcade;

	bool				  dynamically_update_geometry;

    indicator_geometry    geometry_; 
private:
    bool                  screen_space_now;
    void                  set_full_view_port_coverage(const indicator_viewport & vp);


	static void utility_set_origin_to_cockpit_shape( Element * elem,ccDrawable * parent, const DParamList& args );

	Vector3				  collimator_default_distance_factor;
	bool				  collimator_default_distance_present;
};
}

#endif // ccIndicator_H
