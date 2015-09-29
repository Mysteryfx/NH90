#pragma once
#include "CockpitBase.h"
#include "Avionics/avLuaDevice.h"
#include "Avionics/Sensors/avLinkToTargetResponder.h"
#include "FMElectricSystems/Items/Wire.h"
#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"
#include "Utilities/ccPolarPosition.h"
#include "Utilities/ccTriggerSystem.h"
#include "Utilities/ccUtilities.h"
#include "Avionics/avSimpleElectricSystem.h"

class woSpot;

namespace cockpit {

struct detect_status
{
	bool  can_be_locked;
	bool  can_be_detected;
	bool  can_feel_radiation;
	float RCS;
};	

enum PRF_value {
	MEDIUM_PRF,
	HIGH_PRF,
};


struct scan_pattern_data
{
	scan_pattern_data();
	double  volume_vertical;
	double	volume_horizontal;
	double  volume_origin_horizontal;
	double  volume_origin_vertical;
};

struct STT_target
{
	void clear()
	{
		id		  = 0;
		RCS		  = 0;
		time_stamp = 0;
	}
	STT_target()
	{
		clear();
	}
	unsigned		id;
	Position3		pos;	
	Vector3			velocity;
	Vector3			acceleration;
	double			RCS;
	double			time_stamp;
	PolarPosition	polar;
};

struct scan_contact
{
	dVector			point;
	PolarPosition   polar_position;
	dVector			velocity;
	float			RCS;
	int				friendly;
	std::string		NCTR_result;
	double			time_stamp;
	float			service_randomization;
	bool			noise_mark;
};

struct contact_export
{
	triggers::double_param_proxy time_handle;
	triggers::double_param_proxy range_handle;
	triggers::double_param_proxy azimuth_handle;
	triggers::double_param_proxy elevation_handle;
	triggers::double_param_proxy friendly_handle;
	triggers::double_param_proxy rcs_handle;
	triggers::string_param_proxy nctr_handle;
	triggers::double_param_proxy velocity_x;
	triggers::double_param_proxy velocity_y;
	triggers::double_param_proxy velocity_z;
	triggers::double_param_proxy service_randomization;
	triggers::double_param_proxy is_it_noise_mark;

	void update_with_contact(const scan_contact & contact, int index);
	void mark_invalid();
};

struct range_result
{
	double range;
	float  RCS;
};


struct acqusition_cache
{
	acqusition_cache()
	{
		spotted_count		  = 0;
		distance_from_center  = 0;
	}
	int		spotted_count;
	float   distance_from_center;
};

enum radar_work_mode
{
	NONE,
	SCAN,
	ACQUISITION,
	TRACKING,
	BUILT_IN_TEST,
};

class avSimpleRadarTimer;

class COCKPITBASE_API avSimpleRadar : public avLuaDevice,
									  public avLinkToTargetResponder
{

	landscape::lPointOnSurface surf_info;
	landscape::lPointOnSurface surf_info_2;

	woSpot				*spot;

	ed::vector<dVector> spot_cache;// temporal cache for spot creation

	void				update_spot();
	void				kill_spot();

	avSimpleRadarTimer  * p_search_timer;

	ed::vector<woPointer> jamm_sources;
public:
	avSimpleRadar();
	virtual void initialize(unsigned char ID, const std::string& Name, const std::string& script_name);
	virtual void post_initialize();
	virtual void release();
	//////////////////////////////////////////////////////////////////////////
	virtual void update_radar();
	virtual void SetCommand(int command, float value = 0);
	//////////////////////////////////////////////////////////////////////////
	virtual bool is_tracking(ObjectID ID, unsigned weapon_type) const ;
	virtual ObjectID get_target_to_engage() const
	{
		if (mode == TRACKING)
			return stt.id;
		return 0;
	}

	double   get_target_range() const
	{
		if (mode == TRACKING)
			return stt.polar.distance;
		return -1;
	}



	//////////////////////////////////////////////////////////////////////////
	virtual void		setElecPower(bool flag_in) { power_switch.setOnOff(flag_in); }
	virtual bool		getElecPower() const {return power_line.GetVoltage() > 10.0;}
	//////////////////////////////////////////////////////////////////////////

	void external_parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects);
protected:

	virtual void l_register_functions(lua_State *Lua_s,int index);
	static int   l_set_power		 (lua_State *Lua_s);
	static int   l_get_power		 (lua_State *Lua_s);

	void on_power_on (const lwl::CSignalInfo* pInfo);
	void on_power_off(const lwl::CSignalInfo* pInfo);

	static bool			handler_	 (viObject *item, viSearch *search, void* data)
	{
		return static_cast<avSimpleRadar*>(data)->handler_impl_(item,search); 
	}
	bool				handler_impl_(viObject *item, viSearch *search);


	const cPosition	&   get_antenna_position() const { return antenna_pos; } 
	double				get_max_available_distance() const;

	detect_status		get_detection_status(const woPointer  & theTargetPointer) const;//видит ли Human цель радаром?

	double				get_RCS_for_target(const woPointer  & theTargetPointer) const;

	range_result		get_range_for_target(const woPointer  & theTargetPointer) const;
	range_result		get_range_for_target(double RCS,
											 const cVector & point,
											 const cVector & target_nose_dir,
											 const cVector & target_velocity) const;


	unsigned			get_pulse_repeat_frequency() const;


	bool				is_scan_for_targets_available() const { return true; }
	radar_work_mode		get_mode() const { return mode; };
	
	void				scan_for_targets();
	void				make_ground_spots();
	void				make_jammer_spots();
	void				make_ground_spot(const dVector & pnt);
	double				synthetic_RCS_for_land(const dVector & pnt,int surfType) const;
	void				update_acquisition();
	void				update_STT();

	void				update_export();

	const scan_pattern_data     &  get_current_pattern() const
	{
		if (mode == ACQUISITION)
			return TDC_aqusition;
		else
			return scan_pattern;
	}

	avElectricSourceParamDriven param_bus;

	EagleFM::Elec::Switch		power_switch;
	EagleFM::Elec::Wire			power_line;


	cPosition				antenna_pos;
	cPosition				antenna_pos_local;
	cPosition				antenna_pos_scan_origin;
	double					antenna_azimuth;
	double					antenna_elevation;
	double					scan_beam;
	double					scan_speed;
	double					max_available_distance;
	double					dead_zone;

	scan_pattern_data		scan_pattern;
	scan_pattern_data		TDC_aqusition;

	bool		horizontal_scan_direction;

	radar_work_mode		mode;
	STT_target			stt;

	ed::vector<scan_contact>			contacts;
	ed::vector<contact_export>			contacts_exported;
	ed::map<ObjectID,acqusition_cache>  acquistion_data;

	void		next_scan_step();
	void		next_scan_line();

//////////////////////////////////////////////////////////////////////////
//I/O VALUES
//
	triggers::double_param_proxy TDC_range;
	triggers::double_param_proxy TDC_azimuth;
	triggers::double_param_proxy TDC_range_carret_size;


	triggers::double_param_proxy STT_azimuth;
	triggers::double_param_proxy STT_elevation;
	triggers::double_param_proxy STT_range;

	triggers::double_param_proxy RDR_mode_export;

	triggers::double_param_proxy SCAN_volume_origin_azimuth;
	triggers::double_param_proxy SCAN_volume_origin_elevation;
	triggers::double_param_proxy SCAN_volume_azimuth;
	triggers::double_param_proxy SCAN_volume_elevation;

	triggers::double_param_proxy ACQUSITION_volume_azimuth;

	triggers::double_param_proxy IFF_interrogator_is_on;
	triggers::double_param_proxy PITCH_BANK_stabilization;
	triggers::double_param_proxy BIT_in_progress;
	triggers::double_param_proxy ACQUSIOTIN;


	limits						 tracking_azimuth;
	limits						 tracking_elevation;

	limits						 roll_compensation_limits;
	limits						 pitch_compensation_limits;

	double						 antenna_roll_compensation;
	double						 antenna_pitch_compensation;

	bool						 render_debug_info;

	struct ground_clutter_
	{
		Vector3 sea;
		Vector3 land;
		Vector3 artificial;
		float   max_distance;
	} ground_clutter;
};


class COCKPITBASE_API avSimpleRadarTimer : public avBasicTimer<avSimpleRadar>
{ 
public:
	avSimpleRadarTimer(avSimpleRadar* _parent, wModelTime _dtime = 0.02) : avBasicTimer<avSimpleRadar>(_parent,_dtime){};
	virtual void NextEvent()
	{
		if (!parent || to_cancel)
		{
			Cancel();
			return;
		}
		parent->update_radar();
		wakeUpTime = wTime::GetModelTime() + dtime;
	}
};

}