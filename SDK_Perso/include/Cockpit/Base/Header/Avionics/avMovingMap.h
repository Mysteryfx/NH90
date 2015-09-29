#pragma once

#include "CockpitBase.h"
#include "Utilities/ccUtilities.h"
#include "Avionics/avDevice.h"
#include "Renderer/Camera.h"

namespace cockpit
{
	class  COCKPITBASE_API avMovingMap_Cursor 
	{
	public:
		avMovingMap_Cursor();
		virtual ~avMovingMap_Cursor();
		virtual void  clear(); 
		void		  clear_moving();

		float getX() const	 { return cursorX;}
		float getY() const	 { return cursorY;}
		void  setX(float  x) { cursorX = x; }
		void  setY(float  y) { cursorY = y; }

		void  set_from_axis_absolute_X(float value,bool limited = true);
		void  set_from_axis_relative_X(float value,bool limited = true);
		void  set_from_axis_absolute_Y(float value,bool limited = true);
		void  set_from_axis_relative_Y(float value,bool limited = true);

		const limits_d & get_limit_X() const {return lim_X;}
		const limits_d & get_limit_Y() const {return lim_Y;}

		void  set_limits_X(limits_d L) {lim_X = L;}
		void  set_limits_Y(limits_d L) {lim_Y = L;}
		void  set_limits_X(float min_,float max_) 
		{
			lim_X.min = min_;
			lim_X.max = max_;
		};
		void  set_limits_Y(float min_,float max_)
		{
			lim_Y.min = min_;
			lim_Y.max = max_;
		};
		void  to_limit();
		bool  in_limit();

		bool  update_X(double delta,bool limited);
		bool  update_Y(double delta,bool limited);	

		void  stop_X() { cursor_moving_X_active = false; cursor_velocity_X = 0;};
		void  stop_Y() { cursor_moving_Y_active = false; cursor_velocity_Y = 0;};
		void  start_X(double v) { cursor_moving_X_active = true; cursor_velocity_X = v;};
		void  start_Y(double v) { cursor_moving_Y_active = true; cursor_velocity_Y = v;};
		bool  in_slew() const	{ return cursor_moving_X_active || cursor_moving_Y_active;};
	protected:
		limits_d	lim_X;
		limits_d	lim_Y;
		float		  cursorX; // The cursor moves relatively
		float		  cursorY; // The cursor moves relatively
		float         cursor_velocity_X;
		float         cursor_velocity_Y;
		bool		  cursor_moving_X_active;
		bool		  cursor_moving_Y_active;
	};

	enum enum_MovingMapOrientation
	{
		MAP_LIKE_PLANE,
		MAP_PLANE_VELOCITY,
		MAP_TRUE_NORTH_UP,
		MAP_MAG_NORTH_UP,
		MAP_ORIENTATION_MANUAL,
	};

	class  COCKPITBASE_API avMovingMap
	{
	public:
		avMovingMap();
		virtual ~avMovingMap(){};

		Vector3			   get_map_center();

		virtual bool	   get_map_rotation(double& heading) { return false; };
		virtual double	   get_volume_size()	   const = 0;

		virtual const Graphics::Camera & get_camera() { return get_camera(false,-1.0f,1.0f,-1.0f,1.0f);};

		const Graphics::Camera & get_camera(bool		change_volume,
											const float scale_x_min,
											const float scale_x_max,
											const float scale_y_min,
											const float scale_y_max);
	protected://functions
		Vector3		     relative_position_map(const Vector3 & pnt);

		void			 check_camera_orientation();
		void			 check_camera_volume(bool		change_volume		= false,
											 const float scale_x_min		= -1.0f,
											 const float scale_x_max		=  1.0f,
											 const float scale_y_min		= -1.0f,
											 const float scale_y_max		=  1.0f);

		virtual Vector3	 get_camera_point()    const = 0;
		virtual void     check_camera_scale()		 = 0;
		virtual void	 check_camera_position()	 = 0;
	protected:// data
		Graphics::Camera	saved_camera;
		double				update_camera_time;
		Vector3				map_center;
	};
}
