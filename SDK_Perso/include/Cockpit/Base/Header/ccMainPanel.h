// ccMainPanel.h: interface for the ccMainPanel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ccMainPanel_h
#define _ccMainPanel_h

#include "cLinear.h"
#include "ClipVolumes/ClipVolumePlanes.h"
#include "Avionics/avDevice.h"
#include "Modeler/ModelInstance.h"
#include "Modeler/ModelParser.h"
#include "Modeler/ModelLight.h"
#include "IwHumanPLane.h"

#include "Math/IIR_Filter.h"

struct lua_State;
class wHumanCommunicator;

namespace EagleFM { class DynamicBody;};

namespace Graphics
{
	class  Material;
	class  ModelParser;
};

namespace cockpit
{

class ccPanelGauge;
class ccPanelEffect;
class ccMainPanel;

typedef ed::vector<ccPanelGauge*>        gauge_list;
typedef ed::vector<ccPanelEffect*>      effect_list;

typedef ed::set<Graphics::Material*> material_set;

enum model_format_
{
    MF_CMD,
    MF_LOM,
    MF_EDM,
};


enum draw_pilot_level
{
	DO_NOT_DRAW_CREW,
	DRAW_CREWMEN_BUT_NOT_SELF,
	DRAW_WHOLE_CREW
};

enum crewman_level_test
{
	DO_NOT_DRAW_THIS_CREWMAN,
	DRAW_THIS_CREWMAN_WITHOUT_HEAD,
	DRAW_THIS_CREWMAN_FULL,
};


class flash_light_effect;

class COCKPITBASE_API mirrors_data : public wTime
{
public:
	mirrors_data()
	{
		def_position_ = Vector3(0.755f,0.05f,0); // зеркала слегка впереди пилота
		def_width_	  = 0.8; 
		def_rotate_   = 0;
		def_rotate_y_ = 0;
		max_angle_    = 175.0/2.0;

		present_	  = false;
		active_		  = false;

		animation_speed_   = 2.0;
		animation_state_   = 0;
		connector		   = 0;
		arg_value_when_on  = 0;
	}

	void set_state_from_arg_value(float value)
	{
		if (arg_value_when_on < 0.1)
			set_on(value < 0.1);
		else
			set_on(value > 0.1);
	}

	void  set_on(bool value);

	float get_state() const;
	bool  update_animation();

	bool  animated() const
	{
		return animation_speed_  > 0;
	}

	void NextEvent()
	{
		if (update_animation())
			wakeUpTime += 0.02;
	}

	MirrorRenderParams  render_;
	Vector3				def_position_;
	double				def_width_;
	double				def_rotate_;
	double				def_rotate_y_;
	double				max_angle_;
	bool				present_;
	bool				active_;
	float				animation_speed_;
	float				animation_state_;
	double				animation_last_time_;
	float				arg_value_when_on;
	Graphics::ModelConnector * connector;
};


class PanelRenderParser : public Graphics::RenderParser
{
	bool                   bAlfaBlend;
	bool                   bAlfaTest;
	material_set           holed_materials_alfablend_disable;
	Graphics::Material    *mirrors;
    ClipVolumePlanes      *clipVolume;

	typedef ed::vector<Graphics::RenderObject *> distanceList;
	typedef ed::vector<Graphics::RenderObject *> keyList;

	distanceList distObjects;	// объекты, отсортированные по дальности
	keyList keyObjects;			// объекты, отсортированные по ключам
    

    unsigned model_format;
public:
	PanelRenderParser();
	PanelRenderParser(const material_set & mat_set, bool _bAlfaBlend,bool _bAlfaTest);

    model_format_ get_model_format() const { return (model_format_)model_format; }
    void set_model_format(model_format_   f) { model_format = (unsigned)f; }

	void operator()(const material_set & mat_set, bool _bAlfaBlend,bool _bAlfaTest);
	void InitFrame(int *objCounter, int *triCounter);
	void DrawAll(void* context);
	Graphics::RenderObject *GetNextObject(void* context);
	void OnObjectReady(Graphics::RenderObject *obj);
	void set_mirrors_material(Graphics::Material *mat) { mirrors = mat;}
    void setClipVolume(ClipVolumePlanes *volume) { clipVolume = volume;}
};

class COCKPITBASE_API ccMainPanel :  public avDevice , public Graphics::ModelInstance 
{
    //Z test;
    float f_near;
    float f_far;

    unsigned livery_handle;
public:
	ccMainPanel();
	virtual ~ccMainPanel();
	
	//------------
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize() {}
	virtual void release();
	virtual void serialize(Serializer&);
	virtual void SetCommand(int, float value = 0);
    virtual void draw_update();
	virtual void parse_for_render(Graphics::ModelParser & parser);
	bool         have_arcade() const { return is_draw_arcade; }
	//------------
//	virtual void on_rendering_foreground(smCamera * cam){};

	void			   dbg_parse     (Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects);
	virtual void       external_parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects);
	const Position3&			 GetPosition();	  // получить позицию
	const Graphics::DParamList&  GetDrawArguments();// получить список аргументов рисования
	Graphics::Model             *GetModel();		  // получить модель

    virtual void         setLiveryHandle(unsigned int liveryHandle){ livery_handle = liveryHandle;}
    virtual unsigned int getLiveryHandle()const { return livery_handle; };


	void                SetArgument(int i,float value);
	float               GetArgument(int i);
	float               GetRotFactor()  {return rot_factor;};

	bool			    is_new_model_format(){return new_model_format;};

	virtual ObjectID    get_target_of_interest() const { return 0;}
	virtual bool        getPointOfInterest(int kind, cPoint& pt) const {return false;}
	virtual bool		get_oxy_starvation_effects(float & effect1, float & effect2) const { effect1 = effect2 = 0.f; return false;}

    void                AddInternalGauge    (ccPanelGauge  *gauge);//mainpanel will be full owner
	void                AddExternalGauge    (ccPanelGauge  *gauge);//mainpanel will be only able to call update
    void                RemoveExternalGauge (ccPanelGauge  *gauge);
    void                AddInternalEffect   (ccPanelEffect *effect);//mainpanel will be full owner
    void                AddExternalEffect   (ccPanelEffect *effect);//mainpanel will be only able to call update
    void                RemoveExternalEffect(ccPanelEffect *effect);


	bool			    get_mirrors() const;;

    virtual const MirrorRenderParams *  get_mirrors_camera() const;

    const Vector3       getCockpitLocalPoint() const { return cockpit_local_point; }

	ccPanelEffect*		get_lights_reference(unsigned id)
	{
		if (id < panel_effects.size())
			return panel_effects[id];
		return nullptr;
	}
private:
    void                     add_global_effects();
    void                     add_effects_from(effect_list &eff);
	void		             set_arguments();       // выставить аргументы отрисовки
    void		             set_arguments_from(gauge_list  &gauges_);       // выставить аргументы отрисовки
    void		             set_arguments_from(effect_list &gauges_);       // выставить аргументы отрисовки
    void                     update_effects(); 
protected: // functions
     void							 initialize_mirrors(Lua::Config & config);
	void                             initialize_day_night_textures_subsets();

	void							 register_in_script		   (lua_State*);
	static	int						 register_controls_in_state(lua_State *L);
	virtual void                     register_gauge_controllers(lua_State* L,int index);

	static void						 day_night_texture_switcher(ccPanelGauge* gauge, ccMainPanel* panel);
	static void                      mirrors_draw(ccPanelGauge* gauge, ccMainPanel* panel);
	static void						 pilot_draw_controller(ccPanelGauge* gauge, ccMainPanel* panel);

	//common shake feature 
	static void						 shake_panel_Y(ccPanelGauge* gauge, ccMainPanel* panel);
	static void						 shake_panel_Z(ccPanelGauge* gauge, ccMainPanel* panel);
	static void						 Panel_Rot_X  (ccPanelGauge* gauge, ccMainPanel* panel);


	crewman_level_test get_level_crewman(unsigned seat);



	inline float get_crewman_arg_value(unsigned seat)
	{
		unsigned lev =	get_level_crewman(seat);
		if (!lev)
			return 1.0;//full off
		else if (lev == DRAW_THIS_CREWMAN_WITHOUT_HEAD)
			return 0.1f;
		return 0.0f;
	}

public:   //data
	unsigned		draw_pilot_level;
	bool			use_external_views; //чистый эксперимент отрисовка кокпита в основной проход рендера вместе с самолетом
    bool            use_external_shape;
protected://data
	Position3               center_pos;
	Graphics::DParamList    args;
	Graphics::Model			*model;
	ed::vector<Graphics::ModelLight>	modelLights;
	Graphics::Model			*glass;
	float		            dusk_border;
	float	                rot_factor;
	bool	                new_model_format;
	PanelRenderParser       panel_parser;

	float                   day_texture_set_value;
	float                   night_texture_set_value;

	Vector3                 cockpit_local_point;

	int                     external_model_canopy_arg;

	mirrors_data		*   mirrors;

    bool                    parse_shape_to_render;

    Graphics::effectState   effects;       
private:
    bool                is_draw_arcade;
	gauge_list          gauges;
    gauge_list          gauges_external;
    effect_list         panel_effects;
    effect_list         panel_effects_external;

	flash_light_effect	*   flashlight;
protected:
   	const char * l_metatable_name() const { return "base_main_panel_meta";}
	void		 l_register_functions(lua_State *Lua_s,int index);
private:
	static int   l_update_arguments(lua_State *Lua_s);
    static int   l_get_argument_value(lua_State *Lua_s);
    static int   l_set_argument_value(lua_State *Lua_s);
	static int   l_get_light_reference(lua_State *Lua_s);

public:
	float getPanelShakeAccelZ() const;
	float getPanelShakeAccelY() const;
	float getPanelShakeAccelX() const;
	float getPanelShakeAccelLeftY() const;
	float getPanelShakeAccelRightY() const;

protected:
	//panel shaking
	void panel_shake_init(Lua::Config& config);

	float get_shake_Z() const;
	float get_shake_Y() const;
	float get_rot_X() const;

	virtual void updatePanelAccelerations(wModelTime dt);

	virtual Math::Vec3f  getLocalShakeAccel() const;

	class Shaking_timer : public avBasicTimer<ccMainPanel>
	{
	public:
		Shaking_timer(ccMainPanel* p, wModelTime _dtime) : avBasicTimer<ccMainPanel>(p, _dtime){};
		virtual ~Shaking_timer() {}
		virtual void NextEvent();
	};

	struct PanelShake
	{
		PanelShake ()
			: panel_shake_rot_x_(.0f), panel_shake_rot_x_k_(.0f),
			cross_accel_(.0f), panelAccelY(.0f), p_shaking_timer(nullptr)
		{} 
		Math::Vec3f panel_local_point_;
		EagleFM::DynamicBody* pDBody;

		Math::IIR_Filter	shake_filter_Y_Left_;
		Math::IIR_Filter	shake_filter_Y_Right_;
		Math::IIR_Filter	shake_filter_Z_;
		Math::IIR_Filter	shake_filter_X_;

		float panel_shake_rot_x_;
		float panel_shake_rot_x_k_;
		float cross_accel_;

		class Accel
		{
		public:
			Accel() : out(0.0), filter2(0.0), filter3(0.0) {}

			float out, filter2, filter3;
		};

		Accel panelAccelZ, panelAccelX, panelAccelLeftY, panelAccelRightY; 
		float panelAccelY;

		Shaking_timer*  p_shaking_timer;
	} shake;
};

}
#endif //_ccMainPanel_h
