#pragma once
namespace plane_lights
{
	class  IwoLA_Light;
}

struct lua_State;

namespace cockpit
{
	class ccPanelEffect;
}

namespace Sound
{
	class Host;
	class Source;
}

struct  aircraft_property_value
{
	const char * str_value;
	float		 value;
};

#if defined(COCKPITBASE_EXPORTS) || defined(HAVE_ED_LIBS)
#include "CockpitBase.h"
//prototypes as they declared in CockpitBase.dll
extern "C" {
	COCKPITBASE_API void   ed_cockpit_set_draw_argument(int arg , float value);
	COCKPITBASE_API float  ed_cockpit_get_draw_argument(int arg);
	COCKPITBASE_API void   ed_cockpit_set_external_draw_argument(int arg , float value);
	COCKPITBASE_API float  ed_cockpit_get_external_draw_argument(int arg);
	COCKPITBASE_API float  ed_cockpit_get_base_sensor_output(unsigned index);

	COCKPITBASE_API void   ed_cockpit_get_self_coordinates(double & x, double & y, double & z);
	COCKPITBASE_API void   ed_cockpit_get_self_velocity   (double & x, double & y, double & z);
	COCKPITBASE_API void   ed_cockpit_get_self_airspeed   (double & x, double & y, double & z);
	
//	open/close lua state : create new lua state for your own needs
	COCKPITBASE_API lua_State *  ed_cockpit_open_lua_state (const char * name);
	COCKPITBASE_API void		 ed_cockpit_close_lua_state(lua_State  * L);
	
	//lights reference
	COCKPITBASE_API plane_lights::IwoLA_Light * ed_cockpit_external_lights_get_reference();
	COCKPITBASE_API plane_lights::IwoLA_Light * ed_cockpit_external_lights_get_child(plane_lights::IwoLA_Light * parent,unsigned child_number);
	COCKPITBASE_API void		  ed_cockpit_set_external_lights_power(plane_lights::IwoLA_Light * light,double power);
	COCKPITBASE_API double		  ed_cockpit_get_external_lights_power(plane_lights::IwoLA_Light * light);

	//internal lights

	COCKPITBASE_API cockpit::ccPanelEffect * ed_cockpit_internal_lights_get_reference(unsigned id);
	COCKPITBASE_API void ed_cockpit_internal_lights_get_reference_set_color		  (cockpit::ccPanelEffect * effect,float   r,float   g,float    b);
	COCKPITBASE_API void ed_cockpit_internal_lights_get_reference_get_color		  (cockpit::ccPanelEffect * effect,float & r,float & g,float  & b);

	COCKPITBASE_API void ed_cockpit_internal_lights_get_reference_set_cone		  (cockpit::ccPanelEffect * effect,float a1,float a2);
	COCKPITBASE_API void ed_cockpit_internal_lights_get_reference_set_attenuation (cockpit::ccPanelEffect * effect,float a0,float a1,float a2,float rng);

	//commands and track
	COCKPITBASE_API bool ed_cockpit_track_is_reading();
	COCKPITBASE_API bool ed_cockpit_track_is_writing();
	//actions . dispatched and recorded to track
	COCKPITBASE_API void ed_cockpit_dispatch_action_digital  (int command);
	COCKPITBASE_API void ed_cockpit_dispatch_action_analog   (int command,float value);
	COCKPITBASE_API void ed_cockpit_dispatch_action_to_device(unsigned char device,int command,float value);
	//actions . dispatched without recording to track
	COCKPITBASE_API void ed_cockpit_set_action_digital  (int command);
	COCKPITBASE_API void ed_cockpit_set_action_analog   (int command,float value);
	COCKPITBASE_API void ed_cockpit_set_action_to_device(unsigned char device,int command,float value);


	//sound system api
	COCKPITBASE_API void		    ed_cockpit_create_sound_host(const char * context, const char* name, float x, float y, float z);
	COCKPITBASE_API Sound::Source * ed_cockpit_create_sound_source(const char * host,const char * sdef_path);

	COCKPITBASE_API void		    ed_cockpit_sound_play_once	  (Sound::Source * source);
	COCKPITBASE_API void		    ed_cockpit_sound_play_continue(Sound::Source * source);
	COCKPITBASE_API void		    ed_cockpit_sound_stop(Sound::Source * source);
	COCKPITBASE_API bool		    ed_cockpit_sound_is_playing(Sound::Source * source);
	COCKPITBASE_API void		    ed_cockpit_sound_update(Sound::Source * source,float * gain,float * pitch,float * lowpass);
	
	COCKPITBASE_API void		    ed_cockpit_sound_host_update(const char* host_name,
																 float     *gain,
																 float     *pos_x,
																 float	   *pos_y,
																 float	   *pos_z);
	COCKPITBASE_API  aircraft_property_value  ed_cockpit_aircraft_get_property(const char * property_name);
}
#else

enum base_sensor
{
	base_sensor_RadarAltitude,
	base_sensor_BarometricAltitude,
	base_sensor_AngleOfAttack,
	base_sensor_AngleOfSlide,
	base_sensor_VerticalVelocity,
	base_sensor_TrueAirSpeed,
	base_sensor_IndicatedAirSpeed,
	base_sensor_MachNumber,
	base_sensor_VerticalAcceleration,
	base_sensor_HorizontalAcceleration,
	base_sensor_LateralAcceleration,
	base_sensor_RateOfRoll,
	base_sensor_RateOfYaw,
	base_sensor_RateOfPitch,
	base_sensor_Roll,
	base_sensor_MagneticHeading,
	base_sensor_Pitch,
	base_sensor_Heading,
	base_sensor_EngineLeftFuelConsumption,
	base_sensor_EngineRightFuelConsumption,
	base_sensor_EngineLeftTemperatureBeforeTurbine,
	base_sensor_EngineRightTemperatureBeforeTurbine,
	base_sensor_EngineLeftRPM,
	base_sensor_EngineRightRPM,
	base_sensor_WOW_RightMainLandingGear,
	base_sensor_WOW_LeftMainLandingGear,
	base_sensor_WOW_NoseLandingGear,
	base_sensor_RightMainLandingGearDown,
	base_sensor_LeftMainLandingGearDown,
	base_sensor_NoseLandingGearDown,
	base_sensor_RightMainLandingGearUp,
	base_sensor_LeftMainLandingGearUp,
	base_sensor_NoseLandingGearUp,
	base_sensor_LandingGearHandlePos,
	base_sensor_StickRollPosition,
	base_sensor_StickPitchPosition,
	base_sensor_RudderPosition,
	base_sensor_ThrottleLeftPosition,
	base_sensor_ThrottleRightPosition,
	base_sensor_HelicopterCollective,
	base_sensor_HelicopterCorrection,
	base_sensor_CanopyPos,
	base_sensor_CanopyState,
	base_sensor_FlapsRetracted,
	base_sensor_SpeedBrakePos,
	base_sensor_FlapsPos,
	base_sensor_TotalFuelWeight,

	base_sensor_count
};

#endif

//prototypes for function pointers
typedef void   (*PFN_ED_COCKPIT_SET_DRAW_ARG)			 (int arg , float value);
typedef float  (*PFN_ED_COCKPIT_GET_DRAW_ARG)			 (int arg);
typedef float  (*PFN_ED_COCKPIT_GET_BASE_SENSOR_OUTPUT)  (unsigned index);
typedef void   (*PFN_ED_COCKPIT_GET_VECTOR)				 (double & x,double & y,double & z);


typedef lua_State * (*PFN_ED_COCKPIT_OPEN_LUA)  (const char * name);
typedef void		(*PFN_ED_COCKPIT_CLOSE_LUA) (lua_State  * L);


//lights reference
typedef plane_lights::IwoLA_Light * (*PFN_ED_COCKPIT_EXTERNAL_LIGHTS_GET_REFERENCE)();
typedef plane_lights::IwoLA_Light * (*PFN_ED_COCKPIT_EXTERNAL_LIGHTS_GET_CHILD)(plane_lights::IwoLA_Light * parent,unsigned child_number);
typedef void		  (*PFN_ED_COCKPIT_SET_EXTERNAL_LIGHTS_POWER)(plane_lights::IwoLA_Light * light,double power);
typedef double		  (*PFN_ED_COCKPIT_GET_EXTERNAL_LIGHTS_POWER)(plane_lights::IwoLA_Light * light);

//internal lights

typedef cockpit::ccPanelEffect * (*PFN_ED_COCKPIT_INTERNAL_LIGHTS_GET_REFERENCE)(unsigned id);
typedef void					 (*PFN_ED_COCKPIT_INTERNAL_LIGHTS_GET_REFERENCE_SET_COLOR)		 (cockpit::ccPanelEffect * effect,float   r,float   g,float    b);
typedef void					 (*PFN_ED_COCKPIT_INTERNAL_LIGHTS_GET_REFERENCE_GET_COLOR)		 (cockpit::ccPanelEffect * effect,float & r,float & g,float  & b);

typedef void					 (*PFN_ED_COCKPIT_INTERNAL_LIGHTS_GET_REFERENCE_SET_CONE)		 (cockpit::ccPanelEffect * effect,float a1,float a2);
typedef void					 (*PFN_ED_COCKPIT_INTERNAL_LIGHTS_GET_REFERENCE_SET_ATTENUATION) (cockpit::ccPanelEffect * effect,float a0,float a1,float a2,float rng);


//commands and track
typedef bool (*PFN_ED_COCKPIT_TRACK_STATUS)();//for read write type pointer
//actions pointers
typedef void (*PFN_ED_COCKPIT_ACTION_DIGITAL)  (int command);
typedef void (*PFN_ED_COCKPIT_ACTION_ANALOG)   (int command,float value);
typedef void (*PFN_ED_COCKPIT_ACTION_TO_DEVICE)(unsigned char device,int command,float value);

//sound system api

typedef void		    (*PFN_ED_COCKPIT_CREATE_SOUND_HOST)		(const char * context, const char* name, float x, float y, float z);
typedef Sound::Source * (*PFN_ED_COCKPIT_CREATE_SOUND_SOURCE)	(const char * host,const char * sdef_path);
typedef void		    (*PFN_ED_COCKPIT_SOUND_PLAY_ONCE)		(Sound::Source * source);
typedef void		    (*PFN_ED_COCKPIT_SOUND_PLAY_CONTINUE)	(Sound::Source * source);
typedef void		    (*PFN_ED_COCKPIT_SOUND_STOP)			(Sound::Source * source);
typedef bool		    (*PFN_ED_COCKPIT_SOUND_IS_PLAYING)		(Sound::Source * source);
typedef void		    (*PFN_ED_COCKPIT_SOUND_UPDATE)			(Sound::Source * source,float * gain,float * pitch,float * lowpass);
typedef void		    (*PFN_ED_COCKPIT_SOUND_HOST_UPDATE)		(const char* host_name,float     *gain,float     *pos_x,float	   *pos_y,float	   *pos_z);


//various stuff 
//get aircraft property from AddPropAircraft
typedef aircraft_property_value  (*PFN_ED_COCKPIT_AIRCRAFT_GET_PROPERTY)(const char * property_name);