#pragma once

#include "Avionics/Sensors/avTVSensor.h"
#include "edTerrain/PointOnSurfaceAdapter.h"

namespace cockpit {

	class  COCKPITBASE_API eqTVGuidedWeapon : public avTVSensor
	{
	public:
		 eqTVGuidedWeapon(const wsType & type_ = wsType(0,0,0,0));
		~eqTVGuidedWeapon(void);
		 
		 const wsType & Type() const { return type;}

		 void update(double delta);
		 
		 virtual void tracking(double delta);

		 void slave_to_point(const dVector & point);
		 bool align_in_progress();
		 bool is_TV_Available();

		 virtual void zoom_in();
		 virtual void zoom_out();

		 void slew_left(double SlewCoeff = 1.0);
		 void slew_right(double SlewCoeff = 1.0);
		 void slew_up(double SlewCoeff = 1.0);
		 void slew_down(double SlewCoeff = 1.0);
		 void slew_stop();
		 virtual void lockon();
		 virtual void free_lock();
		 virtual void break_lock();

		 bool gimbal_limits(const double k = 1.05) const;
		 void change_polarity(bool flag_in);
		 int  get_screen() const;
		 void set_default_azimuth  (double a) { get_h_axis().set_default(a);};
		 void set_default_elevation(double e) { get_v_axis().set_default(e);};
		 void set_digital_terrain_available(bool flag_in,double dst = 0)
		 {
			 digital_terrain_available = flag_in;
			 digital_terrain_distance  = dst;
		 };

         PolarPosition getPolarPosition() const;
		 unsigned int launch(IwHumanPlane * airplane);
		 virtual void uncage() {avTVSensor::uncage();}
		 virtual void cage();
		 virtual void stabilizeInSpace();
		 virtual void stabilizeOnGround(bool with_objects = false);
		 
		 void  enableForceCorrelate();
		 void  disableForceCorrelate();
		 void  startForceCorrelateTrack();
		 bool  get_force_correlate() const  {return force_correlate;}

		 bool  is_LockOn()			 const	{return mode == TV_SENSOR_Lock;}
		 bool  is_ready_to_fire()	 const 	{return is_LockOn();}
		 void  on_axis_stop();
		 void  set_station(int p,int s = 0) 
		 {
			 pilon	  = p;
			 subpilon = s;
		 }
		 bool    on_TV_SENSOR_move_horizontal_abs(float value, float dead_zone = 0.05, double SlewCoeff = 1.0);
		 bool    on_TV_SENSOR_move_vertical_abs(float value, float dead_zone = 0.05, double SlewCoeff = 1.0);
		 dPosition  get_point_under_LOS() const;
		 double  default_search_distance() const;
		 void	 on_hot_air_start();
		 bool	 isTrainingWeapon() const;
		 void	 respat_angles(double hAngle,double vAngle);

		 bool	 is_break_lock() const { return break_lock_mode;}
		 bool    is_in_search() const;

		 static	void  set_preffered_IR_effect(int tv_type) { preffered_IR_effect = tv_type;}

         int     get_station()    const { return pilon; };
         int     get_substation() const { return subpilon; };

		 void	 set_gyro_stabilize_point_to_ground(); //use in the case if there isn't any target during launch (force correlate mode)
	protected:
		void   update_frame();
        virtual unsigned char check_size(const sensor_frame& frame_self, const sensor_frame& frame_item) const;
		void   initLimits();
		void   initZoom();
		
		void   check_ATA();
		void   check_gimbal_limits();
		void   check_polarity();
		virtual double get_slew_velocity() const;
		virtual double get_velocity_by_axis_value(float value, const float dead_zone) const;

		virtual bool use_volume_search() const {return true;} // не проверять пересечение с осью камеры в поиске
		virtual bool useLockOnParts() const;
		virtual bool toLockOnlyParts() const;
		virtual double get_distance_to_terrain() const;

		double get_real_distance_to_terrain() const;

		bool forceCorrelateIsReady() const;
		
		wsType type;
		int    pilon;
		int    subpilon;
		double default_frame;
		double TDC_move_time;
		double Gyro_start_time;
		bool   force_correlate;

		bool   digital_terrain_available;
		double digital_terrain_distance;
		bool   to_change_polarity;
		bool   multiple_polarity;
		bool   ATA_enabled;
		bool   break_lock_mode;
        
		bool   forceCorrelateIsApplicable;
		bool   forceCorrelateEnabled;
        wModelTime forceCorrelateEnableTime;

#ifndef USE_TERRAIN4
		mutable landscape::lPointOnSurface surfInfo;
#else
		mutable edterrain::PointOnSurfaceAdapter surfInfo;
#endif
	};
}
