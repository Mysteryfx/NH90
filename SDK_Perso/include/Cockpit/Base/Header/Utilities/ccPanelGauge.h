#pragma once
#include "CockpitBase.h"
#include "Renderer/DynamicParams.h"
#include "GraphMath/position.h"

struct lua_State;
namespace Lua
{ 
    class Config;
}
namespace Graphics
{ 
    class VolumeEffect;
    class ModelConnector;
}

namespace cockpit
{ 
    class ccMainPanel;
    class COCKPITBASE_API ccPanelGauge
    {
    public:
	     ccPanelGauge();
         virtual ~ccPanelGauge(){};
	     virtual float  getArgValue(float input) const;
	     int            getArgNumber() const { return arg_number;}
	     virtual void   update(ccMainPanel *panel) = 0;
         float          getParam(int i) const;

         void           read_from_state(Lua::Config & config);
         virtual void   onAddTo     (ccMainPanel *panel) {}
         virtual void   onRemoveFrom(ccMainPanel *panel) {}

		 virtual void   setArgs(ccMainPanel * panel,float value_in);
    protected:
	    int			            arg_number;

		HeapVector<float>             x;
	    HeapVector<float>             y;
   	    Graphics::DParamList        params;
    };


    struct gauge_controller
    {
	    void (*func)(ccPanelGauge*, ccMainPanel*);
    };

    class COCKPITBASE_API ccPanelGauge_internal : public ccPanelGauge
    {
    public:
	     ccPanelGauge_internal();
	     static  void   register_gaugemeta(lua_State *L);
	     virtual void	setValue(lua_State *L, const ed::string& field);
	     void           update(ccMainPanel *panel);
    protected:
	    gauge_controller  controller;
    };

	class COCKPITBASE_API ccPanelGaugeCycled : public ccPanelGauge_internal
	{
		float cycle;
	public:
		ccPanelGaugeCycled()	{ cycle = 1.0; }
		void	setValue(lua_State *L, const ed::string& field);
		float   getArgValue(float input) const;
	};

	class COCKPITBASE_API ccPanelGaugeForParameter : public ccPanelGauge_internal
	{
		void * param_handle;
		static void parameter_gauge(ccPanelGauge* gauge, ccMainPanel* panel);
	public:
		ccPanelGaugeForParameter();
		void   setValue(lua_State *L, const ed::string& field);
	};

	class COCKPITBASE_API ccPanelGaugeExternalArgToCockpit : public ccPanelGauge_internal
	{
		int external_arg;
		static void map_external_shape_arg_to_cockpit(ccPanelGauge* gauge, ccMainPanel* panel);
	public:
		ccPanelGaugeExternalArgToCockpit();
		void   setValue(lua_State *L, const ed::string& field);
	};



    class COCKPITBASE_API ccPanelEffect : public ccPanelGauge
    {
    public:
		struct state //for debug_purposes
		{
			state()
			{
				light_type	= 0xFFFF;
				att0		= 0;
				att1		= 0;
				att2		= 0;
				rng			= 0;
				azimuth		= 0;
				elevation	= 0;
				roll		= 0;
				angle		= 0;
				angle2		= 0;
			};
			unsigned  light_type;
			float	  att0;
			float	  att1;
			float	  att2;
			float	  rng;

			Vector3	  pos;
			//spot
			float	  azimuth;
			float	  elevation;
			float	  roll;
			float	  angle;
			float	  angle2;
		};
	    ccPanelEffect();
        virtual ~ccPanelEffect();
        virtual void              update(ccMainPanel *panel);
        void                      read_from_state(Lua::Config & config);
        Graphics::VolumeEffect  * add_to_effects(ccMainPanel *panel);      
        virtual void              set_color(float R,float G,float B);
		inline  void              set_color(const Vector3 & color)
		{
			set_color(color.x,color.y,color.z);
		}
		Vector3 get_color();
		state	get();
		void	set(const state & state_);

		void    set_cone(float cone1,float cone2);
		void    set_attenuation(float att0,float att1,float att2,float rng);
    protected:
        Graphics::VolumeEffect  *  effect;
        bool                       effect_active;
        Position3                  effect_position;
        Graphics::ModelConnector * connector;
    };
}