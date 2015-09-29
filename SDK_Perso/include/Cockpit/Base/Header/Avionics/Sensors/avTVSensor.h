#pragma  once

#include "ICommandsCockpitDevice.h"
#include "viLight.h"
#include "Avionics/Sensors/avBasicSensor.h"
#include "edTerrain/PointOnSurfaceAdapter.h"

#ifdef _ED_HIDDEN_DEBUG 
#include "Renderer/DbgGraph.h"
#endif

namespace cockpit
{
	typedef enum Mode_TV
	{
		TV_SENSOR_Free,
		TV_SENSOR_GyroMoving,
		TV_SENSOR_Gyro,
		TV_SENSOR_Lock,
		TV_SENSOR_Prolongation,
		TV_SENSOR_Scan,
		TV_SENSOR_GyroSpace
	};

	struct COCKPITBASE_API sensor_frame
	{
		sensor_frame(): x(0), y(0){}
		sensor_frame(double _x, double _y) : x(_x),y(_y){}
		double size() const;

        sensor_frame randomize(double k = 0.075) const
        {
            float random_x = avDevice::device_random_evenly.get();
            float random_y = avDevice::device_random_evenly.get();

            return sensor_frame(x*(1.0 + k*random_x),
                                y*(1.0 + k*random_y));
        }

		double x;
		double y;
	};

    class COCKPITBASE_API track_simulation
    {
    public:
        track_simulation(): is_active(false),fov_on_start(1),distance_on_start(1){}

        void start(const sensor_frame & f,double fov,double zoom = 1.0,double d  = 1.0)
        {
             is_active         = true;
             frame             = f;
             frame.x          *= zoom;
             frame.y          *= zoom;
             fov_on_start      = fov;
             distance_on_start = d;
        }
        void clear()
        {
            is_active = false;
        }
        const sensor_frame & get_frame() const { return frame;}
        sensor_frame         get_frame(double current_fov,double d = 0) const
        {
            if (d < 1.0)
                d = distance_on_start;
            sensor_frame f    = frame;
                         f.x *= (fov_on_start / current_fov) * (distance_on_start / d);
                         f.y *= (fov_on_start / current_fov) * (distance_on_start / d);
            return       f;
        };
        bool          active() const { return is_active;};
    private:
        sensor_frame  frame;
        bool          is_active;
        double        fov_on_start;
        double        distance_on_start;
    };

	struct COCKPITBASE_API search_item
	{
        search_item (viObject * it = 0):item(it),intersected(false),lock_part(false)
		{
		};
		viObject *    item;
		dVector       local_pos;
		sensor_frame  size;
		PolarPosition polar_pos;
		bool		  intersected;
        bool          lock_part;

		friend bool operator < (const search_item& item_1, const search_item& item_2)
		{
			if (item_1.intersected && !item_2.intersected)
			{
				return false;
			}
			else if(!item_1.intersected && item_2.intersected)
			{
				return true;
			}

			double Angle1 = acos(cos(item_1.polar_pos.azimuth) * cos(item_1.polar_pos.elevation));
			double Angle2 = acos(cos(item_2.polar_pos.azimuth) * cos(item_2.polar_pos.elevation));

			if (item_1.polar_pos.distance < item_2.polar_pos.distance && Angle1 < Angle2)
				return true;
			else if (item_1.polar_pos.distance >= item_2.polar_pos.distance && Angle1 >= Angle2)
				return false;

			double DistAcrossLOS1 = item_1.polar_pos.distance * sin(Angle1);
			double DistAcrossLOS2 = item_2.polar_pos.distance * sin(Angle2);

			double DistAlongLOS1 = item_1.polar_pos.distance * cos(Angle1);
			double DistAlongLOS2 = item_2.polar_pos.distance * cos(Angle2);

			return (DistAcrossLOS1 - DistAcrossLOS2) + (DistAlongLOS1 - DistAlongLOS2) * 0.25 < 0.0;

			//float a1 = MAX(fabs(item_1.polar_pos.azimuth), fabs(item_1.polar_pos.elevation));
			//float a2 = MAX(fabs(item_2.polar_pos.azimuth), fabs(item_2.polar_pos.elevation));

			//return item_1.polar_pos.distance < item_2.polar_pos.distance;
		};
	};
	typedef ed::vector<search_item>  TV_SENSOR_search_cache;

	class COCKPITBASE_API avTVSensor : public avBasicSensor
	{
#ifndef USE_TERRAIN4
        mutable landscape::lPointOnSurface _terrain_visibility_check_pnt_;
#else
		mutable edterrain::PointOnSurfaceAdapter _terrain_visibility_check_pnt_;
#endif
	public:
		avTVSensor();
	   ~avTVSensor();
		virtual void			   initialize();
		virtual void	   setElecPower(bool flag_in);
		virtual void	   fill_camera(Graphics::Camera  & camera) const;
        PolarPosition      getPolarPosition()      const;
		double		       getCurrentZoom()        const   { return    zoom_axis.get();}
		int  		       getCurrentZoomLevel()   const   { return    zoom_level;}
		double			   getCurrentViewAngle()   const   { return    getDefaultCameraAngle() / getCurrentZoom();}
		double		       getDefaultCameraAngle() const   { return	   default_camera_angle;}
		unsigned char      getMode()			   const   { return    mode;}
        unsigned char      getPreviousMode()       const   { return    previous_mode;}
		const avTrackData* getTarget()			   const;

		virtual bool   is_TV_Available();

        bool   check_existence(viObject     *vis_obj) const;

        bool   can_be_detected_viObject(viObject * vis_obj,const dVector * local_pnt) const;

		virtual void   search();
        virtual bool   test_and_lockon(viObject* obj);
		virtual bool   searcher(viObject*, viSearch*);

		bool   is_inverted() const  {return inverted;}
		bool   is_distance_available()			const { return distance_available;}
		void   set_distance_available(bool flag_in) { distance_available = flag_in;}

		bool		   caged() const { return is_caged;}
		virtual void   uncage();
		virtual void   cage();


		void  respat_angles(double hAngle,double vAngle);
		virtual bool  is_LockOn()		 const  { return mode == TV_SENSOR_Lock; }
		virtual bool  is_ready_to_fire() const 	{ return mode == TV_SENSOR_Lock ||  mode == TV_SENSOR_Gyro; }

		const dVector & get_reference_point() const { return gyro_stabilize_point ;}
		PolarPosition   get_target_position() const				    {		return get_predicted_target_position(wTime::GetModelTime()); }
		PolarPosition   get_target_position_in_carrier_axis() const {		return get_predicted_target_position_in_carrier_axis(wTime::GetModelTime()); }

		bool			IsCaged()			  const { return caged() && mode == TV_SENSOR_Free; }
		PolarPosition   get_predicted_target_position(wModelTime time) const;
		PolarPosition   get_predicted_target_position_in_carrier_axis(wModelTime time) const;

		PolarPosition   get_forestall(wModelTime time) const;

		bool			arrested()			  const { return caged() && mode == TV_SENSOR_Free; }
		virtual void	set_external_designation(bool flag_in,bool success);
		bool			locked() const		   { return  lock_TDC;}
		void			lock_hat(bool flag_in) { lock_TDC = flag_in;}

		virtual bool    on_TV_SENSOR_move_horizontal_abs(float value, float dead_zone = 0.05, double SlewCoeff = 1.0);
		virtual bool    on_TV_SENSOR_move_vertical_abs(float value, float dead_zone = 0.05, double SlewCoeff = 1.0);
		const sensor_frame &	get_frame() const { return frame;};
		virtual dPosition   get_SPI() const;
		virtual void  stabilizeInSpace();
		virtual void  stabilizeOnGround(bool with_objects = false);

		virtual void  reset();

		virtual void zoom_in();
		virtual void zoom_out();

		virtual void slew_left(double SlewCoeff = 1.0);
		virtual void slew_right(double SlewCoeff = 1.0);
		virtual void slew_up(double SlewCoeff = 1.0);
		virtual void slew_down(double SlewCoeff = 1.0);
		virtual void slew_stop();
		virtual void on_axis_stop();
		bool		 in_slew() const { return slew_active;};

		virtual PolarPosition   slave_error(const dPosition & init_currrent,const dVector  & point) const;

        bool                    cursor_active() const;


		double					get_roll_stabilize_angle() const  { return roll_stabilize_angle;}

		virtual void  slave_to_point(const dVector & point);
		bool		  is_slaved() const { return slaved;}
		virtual void  update(double delta);
		static void   set_preffered_IR_effect(int tv_type) { preffered_IR_effect = tv_type;}
	protected:
        void          on_finish_search(search_item & available_target);
        void          update_slave_mode(const dVector & point);

		virtual void  initLimits();
		virtual void  initZoom();
		virtual void  clear();
		virtual void  on_power_on();
		virtual void  on_power_off();
		virtual PolarPosition calculate_target_angles(const dVector & pnt) const;

		bool  target_is_visible(viObject* item, bool refine_search_angels, search_item& inserted) const;

		virtual bool  on_absolute_axis(avMotor & axis, float value, float dead_zone = 0.05, double SlewCoeff = 1.0);
		virtual bool  on_relative_axis(avMotor & axis, float value, float dead_zone = 0.05);

		virtual void  set_gyro_moving();

		virtual	double get_distance_to_terrain() const;

      	bool			checkTargetTVvisibility(const dVector &target_pos,float MaxDistance) const; //проверка цели на условия оптической видимости (туман,ночь,облака)
		bool			isHiddenByBuilding(const avTrackData &target) const;//возвращает true если объект скрыт за зданием
		void			update_angles(double delta);
        PolarPosition   interpolate_angles(const dPosition & init_currrent,double delta)  const;// должно в принципе совпадать с update_angles

		virtual void	update_frame();
		sensor_frame	get_frame_of_object(viObject * item) const;
		sensor_frame	get_frame_of_OBB(const Box & box,const Position3 & pos,const Position3 & camera_pos) const;
		Box				get_local_angular_box(const Box & box,const Position3 & pos);
		Box				get_frame_box();
        virtual double  get_slew_acceleration(double slewCoeff) const;
        virtual double  get_slew_velocity()     const;

        enum check_size_result
        {
            TARGET_SIZE_INVALID,
            TARGET_SIZE_LESS,
            TARGET_SIZE_EQUAL,
            TARGET_SIZE_MORE,
        };

		virtual unsigned char check_size(const sensor_frame & frame_self,const sensor_frame & frame_item) const;

		virtual double   get_velocity_by_axis_value(float value,const float dead_zone = 0.05) const;

		void             slave(const dVector & pnt,double delta);

		PolarPosition	 in_sensor_axis(const PolarPosition & in_carrier_axis) const;

		avMotor_Additive	h_additive;
		avMotor_Additive	v_additive;

		virtual bool		use_gyro_base() const;
        dVector             cursor_slave_point(dPosition pos_base) const;

        dPosition           get_cursor_base_position(const dPosition & init_currrent,const dVector * gyro_base = 0) const;

		dVector				gyro_stabilize_point;
		unsigned char       mode;
		unsigned char       previous_mode;
		int  				zoom_level;
		double				default_camera_angle;
		avMotor				zoom_axis;
		ed::vector<double> zoom_data;
		bool                inverted;
		bool                distance_available;
		bool                is_caged;
		bool                stabilize_after_respat;
		avTrackData         target;
		double              aspect;
		sensor_frame        frame;
		bool				slew_active;
		mutable double  	roll_stabilize_angle;
		bool                roll_stabilized;

        track_simulation    part_locked_simulated;
        track_simulation    point_track_simulated;

        virtual void        start_tracking()
        {
            set_mode(TV_SENSOR_Lock);
			slaved = false;
            lock_hat(true);
        }
		virtual void        set_mode(unsigned char new_mode)
		{
			previous_mode = mode;
			mode          = new_mode;
		};
		virtual dPosition  inertial_system_computed_position() const;
		virtual void	   tracking(double delta);
		virtual void	   scan_stop() {};
		virtual void       scan(double delta);
		virtual void       prolongate(double delta){};
		virtual void	   clear_AC_mode_factors();
		double			   distance_by_triangulation() const;
		double			   distance_by_triangulation(double delta_height,double summary_elevation) const;
		virtual double     default_search_distance() const;
		virtual double     get_search_angle() const;
		virtual void	   on_update_position(double delta);
		virtual double	   roll_stabilize(dPosition & dpos) const;

		virtual void       update_gyro_space_base_point();
		virtual void	   update_gyro_base_point(double delta);
		void               update_slew(double delta);
        virtual void       recalculate_gyro_mode() {};
		virtual double     get_delta_height() const;

		double			   max_distance_by_triangulation;

        virtual void       on_target_disappear(bool outside_fov = false);
		double	   last_caging_off_time;
		bool	   slaved;
        wVector    slave_point;

        static bool        search_handler(viObject *item, viSearch *search, void* data);//handler

        virtual bool       target_can_be_detected(const avTrackData & trg) const;

        virtual dVector    get_tracked_point() const; // возвращает точку за которой следит ПКС в истинной системе

        virtual bool       use_volume_search() const {return false;} // не проверять пересечение с осью камеры в поиске

		// To allow lock on parts of large objects.
		// If enabled the tracker works like force correlate track in Maverick.
		// If disabled it works like centroid track in Maverick or point track in Litening.
		virtual bool       useLockOnParts() const {return true;}

		// Is used to imitate of Forced Correlated track of moving objects.
		virtual bool       toLockOnlyParts() const {return false;}

        double     prolongation_start;
        TV_SENSOR_search_cache search_cache;
        limits_d     search_azimuth;
        limits_d     search_elevation;
        bool       search_refine_angles;
        bool	   axis_use_velocity;
        dVector    prolongation_velocity;
        wModelTime power_switch_time;
        bool       lock_TDC;
        bool       infrared;
        dVector    saved_gyro_space_orientation;
        bool       arcade_enable;

        double     slew_velocity_base;
        double     slew_acceleration_base;
        avSensorLimits      limits_sensor;

#ifdef _ED_HIDDEN_DEBUG 
        dbg_line*  dbg_robj;
		//dbg_obj*   dbg_pos;
	public:
		void parse(Graphics::RenderParser & parser,const Position3 & pos);
#endif
		static int  preffered_IR_effect;
	};

	struct COCKPITBASE_API LightData
	{
		float				  LightStrengthSum;
		ed::vector<viLight*> LightVector;
		Vector3  			  target_pos;
	};
}
