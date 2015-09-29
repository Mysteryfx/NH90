#pragma once
#include "Utilities\ccUtilities.h"
namespace cockpit
{
	class COCKPITBASE_API avMotor_Additive
	{
	public:
		avMotor_Additive();
	   ~avMotor_Additive(){}
		void   clear();
		void   set (bool flag_in,double v = 0);
		void   set_velocity  (double v,bool force = false);
		void   set_acceleration(double a)	{ acceleration = a;	}
		bool   active()			  const		{ return is_active; }
		double get()			  const		{ return value;}
		double get(double dtime);
		void   set_base(double b) { base = b;}
		double get_base() const { return base;}
		double get_step(double dtime);
		void   reset_output() { value = 0;}
		double get_velocity() const { return velocity; }
	public:
		double base;
		double value;
		double velocity;
		double velocity_initial;
		double acceleration;
		bool   is_active;
	};

	class COCKPITBASE_API avMotor //: public wTime
	{
	public:
		avMotor();
		~avMotor();
		void   set_unlimited(bool flag_in) { unlimited = flag_in;}
		void   set_target(double _target);
		void   set_velocity(double _velocity);
		void   set_limits(double min,double max);
		void   set_default(double d) { value_default = d;}
		void   set_default_velocity(double v) { velocity_default = v;}
		void   respat_to_minimum();//отработать на концевик - минимум
		void   respat_to_maximum();//отработать на концевик - максимум
		void   stop();
	
        double out(double dtime,
                   double err,
                   double last) const;
        void   update (double dtime, double err) {  value  = out(dtime,err,value);  }
		void   update (double dtime)		     {	update(dtime, get_error(dtime));	}
		bool   steady_state() const		{	return (value == target);	}

		double			   get()       const { return value;}
		double			   get_target() const { return target; }
		const  limits_d&   get_limit() const { return physical_limit;}
		limits_d&		get_limit()       { return physical_limit;}
		void			   clear();
		void               reset_velocity()  { velocity = velocity_default; }
		void               reset_target()	 { target   = value_default; }
		double			   get_velocity()     const { return velocity;}
		double             get_default_velocity() const { return velocity_default;}
		double             get_default() const			{ return value_default;}
		void			   force(double a)			{ value = a;} 
		double			   get_step(double dtime,double err) const;
		double			   get_error(double dtime) const;
	protected:
		void			   force_Pi(double & v) const;
	protected:
		double			 velocity;
		double			 value;
		double			 target;
		double			 value_default;
		double			 velocity_default;
		limits_d		 physical_limit;
		bool			 unlimited;
	};
}
