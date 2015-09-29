#pragma once
#include "Avionics/Sensors/avTVSensor.h"
#include "Avionics/Sensors/avRangefinder.h"
#include "Avionics/Sensors/avLaserSpotDetector.h"

namespace cockpit {


enum TGP_designation_source_enum
{
	TGP_DESIGNATION_LASER,
	TGP_DESIGNATION_INFRARED_POINTER,
	TGP_DESIGNATION_BOTH,
};

enum TGP_video_mode_enum
{
	TGP_VIDEOMODE_CCD,
	TGP_VIDEOMODE_WHOT,
	TGP_VIDEOMODE_BHOT,
};

enum TGP_track_mode_enum
{
	TGP_TRACK_MODE_NONE,
	TGP_TRACK_MODE_AREA,
	TGP_TRACK_MODE_POINT,
	TGP_TRACK_MODE_INR,
	TGP_TRACK_MODE_INR_AREA,
	TGP_TRACK_MODE_INR_POINT,
	TGP_TRACK_MODE_RATES,
};

enum TGP_master_mode
{
	TGP_MASTER_OFF,
	TGP_MASTER_MODE_STBY,
	TGP_MASTER_MODE_AG,
	TGP_MASTER_MODE_AA,
};

enum TGP_STATUS
{
	TGP_STATUS_UNK,
	TGP_STATUS_INIT,
	TGP_STATUS_STBY_INIT,
	TGP_STATUS_STBY_BIT,
	TGP_STATUS_FAILURE,
	TGP_STATUS_READY,
};

enum tgp_gain_level_enum
{
	TGP_GAINLEVEL_SELECTION_GAIN,
	TGP_GAINLEVEL_SELECTION_LEVEL,
	TGP_GAINLEVEL_SELECTION_COUNT
};

class  COCKPITBASE_API eqLitening_TGP : public avTVSensor
{
public:
	eqLitening_TGP(unsigned st);
	virtual ~eqLitening_TGP(void);

	//avBreakable interface
	virtual bool	set_damage(unsigned Failure_ID, const bool can_be_rejected = false);
	virtual void	repair();
	//end of interface

	void   setElecPower(bool flag_in);
	int	   get_screen() const;

	virtual void slew_left(double SlewCoeff = 1.0);
	virtual void slew_right(double SlewCoeff = 1.0);
	virtual void slew_up(double SlewCoeff = 1.0);
	virtual void slew_down(double SlewCoeff = 1.0);
	virtual void slew_stop();

	void update(double delta);

	void initialize();

	//bool align_in_progress();
	TGP_STATUS getStatus() const {return Status;}
	void setStatus(TGP_STATUS StatusIn) {Status = StatusIn;}

	bool is_TV_Available();
	int  get_designator_type() const { return designator_type;}

	void set_designator_type(int new_designator)
	{
		designator_type  = new_designator;
	}

	int  get_video_mode()	   const;
	void set_video_mode(int new_mode, bool force = false);
	PolarPosition get_polar_angles() const;
	PolarPosition get_polar_angles_local() const;

	void set_narrow(bool narrow);
	void set_narrow_LSS(bool narrow) {  LSS.set_narrow(narrow); }
	bool get_narrow() const { return is_narrow; };
	bool get_narrow_LSS() const { return LSS.get_narrow(); };

	void track();
	void track_inertial();
	unsigned    get_track_mode() const;

	virtual void stabilizeOnGround(bool with_objects = false);

	void        set_carrier		(woPointer new_carrier);

	void		set_laser_designation_code(unsigned new_code)	{		 laser.set_code(new_code);};
	unsigned    get_laser_designation_code() const				{ return laser.get_code();}
	
	void		set_laser_spot_search_code(unsigned new_code)	{		 LSS.set_code(new_code);};
	unsigned    get_laser_spot_search_code() const				{ return LSS.get_code();}


	virtual bool  on_TV_SENSOR_move_horizontal_abs(float value, float dead_zone = 0.05, double SlewCoeff = 1.0);
	virtual bool  on_TV_SENSOR_move_vertical_abs(float value, float dead_zone = 0.05, double SlewCoeff = 1.0);
	void		  on_axis_stop();
	void		  zoom_stop();
	void		  zoom_in();
	void		  zoom_out();
	void		  set_master_mode(unsigned new_master_mode) { master_mode = new_master_mode;}
	unsigned	  get_master_mode() const					{ return master_mode;}

	void		  start_LSS();
	void		  stop_LSS();
	void          stop_LSS_with_area_track();
	void          stop_LSS_with_inr_track();

	const	avLaserSpotDetector & get_laser_spot_detector() const {	return LSS;}
	const	avRangefinder		& get_laser()			    const {	return laser;}
	avRangefinder				& get_laser()					  {	return laser;}

	const	sensor_frame & get_default_frame() const { return default_frame;}

	double		  get_distance_under_LOS() const;

	dPosition     get_point_under_LOS() const {return point_under_LOS;}
	void          update_point_under_LOS();

	void		  cage();
	void		  set_designator_on();
	void		  set_designator_off();
	bool		  get_designator_fire() const;
	void		  set_laser_power(bool flag_in);
	bool		  get_laser_power();
	unsigned	  get_laser_mode() const { return laser.get_mode();}
	void		  add_masked_area(const ed::vector<Vector3> & area,bool outlined = false);

	const ed::vector< ed::vector<Vector3> > & get_masked_areas(bool outlined = false) const { return  outlined ? outlined_masked_areas : masked_areas;}

	bool		  get_masked() const		 { return LOS_masked;}
	bool		  get_masked_outline() const { return LOS_masked_outline;}

	void		  increase_focus();
	void		  decrease_fucus();

	void		  start_focus_adjustment();
	double		  get_focus() const {return focus;}
	double		  getfocusBest() const {return focusBest;}
	bool		  get_focus_adjustment() const { return focus_adjustment;}
	bool		  get_flir_hot() const;
	
	bool		  getStbyInitState2() const;
	bool		  getStbyBIT_FLR_Info() const;
	bool		  getStbyBIT_MSK_Info() const;

	unsigned	  get_station() const { return station;	}

	PolarPosition get_tgp_angles_from_polar(const double azimuth,const double elevation) const;
	PolarPosition get_polar_angles_from_tgp(const double roll   ,const double elevation) const;
	PolarPosition get_tgp_angles() const;

	const avTrackData & get_priority_target() const { return priority_target;}

	double        roll() const { return azimuth();}

	void		  slave_to_point(const dVector & point);
	void		  set_caged_positon(double elev);
	void		  setPlatformInitialLocalPosition(const dPosition & new_local_pos);
	void		  set_use_native_axis(bool flag_in) { use_native_axis = flag_in;}
	void		  on_hot_air_start();
	void		  fill_camera(Graphics::Camera  & camera) const;
	void		  set_digital_terrain_available(bool flag_in,double dst = 0)
	{
		digital_terrain_available = flag_in;
		digital_terrain_distance  = dst;
	};
	void		  set_use_digital_zoom(bool flag_in) { use_digital_zoom = flag_in;}
	bool		  get_use_digital_zoom() const		 { return use_digital_zoom;}
	double		  get_digital_zoom() const			 { return digital_zoom;}

	void		  set_preffered_IR_effect(int tv_type) { preffered_IR_effect = tv_type;}

	unsigned      get_gain_level_selection_value(int kind) const;
	void          gain_level_selection_value_increase(int kind);
	void          gain_level_selection_value_decrease(int kind);
	unsigned	  get_gain_selection_value() const;
	unsigned	  get_level_selection_value() const;

	float		  get_ir_sharpness_factor() {return ir_sharpness_factor;}
	void		  set_ir_sharpness_factor(float valIn) {ir_sharpness_factor = valIn;}
	
	float         get_ir_level() {return (1.0*get_level_selection_value()-4.0) / ir_levels_count;}
	float         get_ir_gain() {return (1.0*get_gain_selection_value()-4.0) / ir_gains_count;}
	
	void          set_ir_levels_count(int valIn) {ir_levels_count = valIn;}
	void          set_ir_gains_count(int valIn) {ir_gains_count = valIn;}

	void          set_ir_init_gain(unsigned valIn);
	void          set_ir_init_level(unsigned valIn);

	void		  set_aircraft_altitude(double altitude);

protected:
    void          start_tracking()
    {
        set_mode(TV_SENSOR_Lock);
    }

	void		  set_mode(unsigned char new_mode)
	{
		avTVSensor::set_mode(new_mode);
		slaved = false;
	}
	
	void		  updateStatus();
	void		  update_frame();
	void		  update_mask();
	void		  update_focus();
	void		  update_cooling(double delta);
	void	      set_track_mode(unsigned new_track);
	void		  set_area_track();
	void		  set_point_track();
	
	bool		  try_to_track_area();

	void		  tracking(double delta);
	void		  prolongate(double delta);
	void		  scan(double delta);
	void		  scan_stop();
	void		  scan_start();
	double	      scan_zone_width();
	void		  start_prolongation(unsigned track_type);
	void		  start_acquisition(unsigned track_type);
	void          start_rates();

	void		  update_angles(double delta);
	void		  initLimits();
	void		  initZoom();
	double		  get_FOV() const { return get_FOV(get_narrow()); } ;
	double        get_FOV(bool narrow) const;

	double        terrain_intersection_distance(const Graphics::Camera & camera) const;

    virtual unsigned char check_size(const sensor_frame& frame_self, const sensor_frame& frame_item) const;

	virtual double get_slew_velocity() const;
	double		   get_slew_acceleration(double SlewCoeff) const;
	virtual double get_velocity_by_axis_value(float value, const float dead_zone) const;
	
	void	   update_priority_target();
	double     default_search_distance() const;
	double     get_search_angle() const;
	void	   orient(dPosition &pos,double roll,double elevation) const;

	bool	   check_track_by_angle(const dVector & point,double angle) const;

	virtual double     get_delta_height() const;

	void       check_area_LSS_toggle(double delta);
	
	void       update_digital_zoom();

    dVector    get_tracked_point() const; // возвращает точку за которой следит ПКС в истинной системе
    virtual bool use_volume_search() const; // не проверять пересечение с осью камеры в поиске

	virtual bool target_can_be_detected(const avTrackData & trg) const;

	int			  designator_type;
	bool	      is_narrow;
	sensor_frame  default_frame;
	avRangefinder laser;
	
	unsigned      track_mode_before_slew;
	int			  zoom_start;
	bool          zoom_increase;

	unsigned	  master_mode;
	TGP_STATUS	  Status;
	
	wModelTime	  StbyInitTime;
	wModelTime	  StbyBIT_Time;
	static const wModelTime InitTimersK;
	
	avLaserSpotDetector LSS;
	void		  set_simulated_point_track(bool sim);

	ed::vector< ed::vector<Vector3> > masked_areas;
	ed::vector< ed::vector<Vector3> > outlined_masked_areas;

    bool		  masked(double azimuth, double elevation, const ed::vector< ed::vector<Vector3> > & mask_regions) const;
	bool          acquisitionTimeExpired() const;
	
	bool		  LOS_masked;
	bool		  LOS_masked_outline;
	bool		  focus_adjustment;
	double		  focus_adjustment_start_time;
	double		  focus;
	double		  focusBest;
	double		  flir_temperature;

	unsigned	  station;

	double        update_prioritisation_time;
	avTrackData   priority_target;
	
	bool		  use_native_axis;

	double        update_mask_time;
	bool   digital_terrain_available;
	double digital_terrain_distance;

	dVector       lss_start_location;
	bool		  enter_area_after_respat;
	bool          use_digital_zoom;
	double		  digital_zoom;
    void          update_triggers();
    void          initialize_triggers();

	unsigned	  gain_level_selection_value[TGP_GAINLEVEL_SELECTION_COUNT];

	float         ir_sharpness_factor;
	int           ir_levels_count;
	int           ir_gains_count;

	dPosition     point_under_LOS;

	double		  aircraft_altitude_;

	unsigned char prevTgtSizeResult;
	wModelTime    prevTgtSizeChkTime;
};

}
