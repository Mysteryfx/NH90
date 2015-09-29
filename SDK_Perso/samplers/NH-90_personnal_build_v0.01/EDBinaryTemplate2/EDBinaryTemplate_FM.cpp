#include "stdafx.h"
#include "EDBinaryTemplate_FM.h"
#include "Avionics/avDevice.h"
#include "WorldManager.h"
#include "ed_log.h"
#include "wTime.h"
#include "cLinear.h"

//input events enum
#include "iCommand.h"
#include "Actions.h"

dVector	common_moment;
dVector	common_force;
dVector center_of_gravity;
dVector	wind;
dVector	velocity_world_cs;
dVector omega;
double  throttle		  = 0;
double  stick_roll		  = 0;
double  stick_pitch		  = 0;

double  internal_fuel     = 0;
double  fuel_consumption_since_last_time  = 0;
double  atmosphere_density = 0;
double  aoa = 0;
double  speed_of_sound = 320;

double test_gear_state = 0;
double desired_gear_state = 0;

double mach_table[] = {
	0,	
	0.2,
	0.4,
	0.6,
	0.7,
	0.8,
	0.9,
	1,	
	1.05,
	1.1,
	1.2,
	1.3,
	1.5,
	1.7,
	1.8,
	2,	
	2.2,
	2.5,
	3.9,
};

double cx0[] =
{
	0.0165,
	0.0165,
	0.0165,
	0.0165,
	0.0170,
	0.0178,
	0.0215,
	0.0310,
	0.0422,
	0.0440,
	0.0432,
	0.0423,
	0.0416,
	0.0416,
	0.0416,
	0.0410,
	0.0395,
};

double Cya[] = {
	   0.077,	
	   0.077,	
	   0.077,	
	   0.080,	
	   0.083,	
	   0.087,	
	   0.091,	
	   0.094,	
	   0.094,	
	   0.091,	
	   0.085,	
	   0.068,	
	   0.051,	
	   0.043,	
	   0.037,	
	   0.036,	
	   0.033,	
};

double B[] ={
	0.1,	
	0.1,	
	0.1,	 
	0.094,	
	0.094,	
	0.094,	
	0.11,	
	0.15,	
	0.15,	
	0.14,	
	0.17,	
	0.23,	
	0.23,	
	0.08,	
	0.16,	
	0.25,	
	0.35,	
};

double B4[] ={
	0.032,	
	0.032,	 
	0.032,	
	0.043,	
	0.045,	
	0.048,	
	0.050,	
	0.1,	
	0.1,	
	0.1,	
	0.096,	
	0.09,	
	0.38,	
	2.5,	
	3.2,	
	4.5,	
	6.0,	
};

double CyMax[] = {
	1.6,
	1.6,
	1.6,
	1.5,
	1.45,
	1.4,
	1.3,
	1.2,
	1.1,
	1.05,
	1.0,
	0.9,
	0.7,
	0.55,
	0.4,
	0.4,
	0.4,
};


double lerp(double * x,double * f, unsigned sz, double t)
{
	for (unsigned i = 0; i < sz; i++)
	{
		if (t <= x[i])
		{
			if (i > 0)
			{
				return ((f[i] - f[i - 1]) / (x[i] - x[i - 1]) * t +
					(x[i] * f[i - 1] - x[i - 1] * f[i]) / (x[i] - x[i - 1]));
			}
			return f[0];
		}
	}
	return f[sz-1];
}

void add_local_force(const dVector & Force, const dVector & Force_pos)
{
	common_force += Force;
	
	dVector delta_pos    = Force_pos - center_of_gravity;
	dVector delta_moment = delta_pos ^ Force; //cross product

	common_moment += delta_moment;
}
extern "C" {;

 void simulate_fuel_consumption(double dt)
{
	fuel_consumption_since_last_time =  10 * throttle * dt; //10 kg persecond
	if (fuel_consumption_since_last_time > internal_fuel)
		fuel_consumption_since_last_time = internal_fuel;
	internal_fuel -= fuel_consumption_since_last_time;
}

void ed_fm_add_local_force(double & x,double &y,double &z,double & pos_x,double & pos_y,double & pos_z)
{
	x = common_force.x;
	y = common_force.y;
	z = common_force.z;
	pos_x = center_of_gravity.x;
	pos_y = center_of_gravity.y;
	pos_z = center_of_gravity.z;
}

void ed_fm_add_global_force(double & x,double &y,double &z,double & pos_x,double & pos_y,double & pos_z)
{

}

void ed_fm_add_global_moment(double & x,double &y,double &z)
{

}

void ed_fm_add_local_moment(double & x,double &y,double &z)
{
	x = common_moment.x;
	y = common_moment.y;
	z = common_moment.z;
}

void ed_fm_simulate(double dt)
{
	common_force  = dVector();
	common_moment = dVector();

	dVector airspeed = velocity_world_cs - wind;

	dVector thrust_pos(-5.0,0,0);
	dVector thrust(throttle * 5000, 0 , 0);

	double V_scalar =  airspeed.length();

	double Mach		= V_scalar/ speed_of_sound;

	double CyAlpha_ = lerp(mach_table,Cya  ,sizeof(mach_table)/sizeof(double),Mach);
	double Cx0_     = lerp(mach_table,cx0  ,sizeof(mach_table)/sizeof(double),Mach);
	double CyMax_   = lerp(mach_table,CyMax,sizeof(mach_table)/sizeof(double),Mach);
	double B_	    = lerp(mach_table,B    ,sizeof(mach_table)/sizeof(double),Mach);
	double B4_	    = lerp(mach_table,B4   ,sizeof(mach_table)/sizeof(double),Mach);


	double Cy  = (CyAlpha_ * 57.3) * aoa;
	if (fabs(aoa) > 90/57.3)
		Cy = 0;
	if (Cy > CyMax_)
		Cy = CyMax_;

	double Cx  = 0.05 + B_ * Cy * Cy + B4_ * Cy * Cy * Cy * Cy;

	double q	   =  0.5 * atmosphere_density * V_scalar * V_scalar;
	const double S = 25;
	double Lift =  Cy * q * S;
	double Drag =  Cx * q * S;
	
	dVector aerodynamic_force(-Drag , Lift , 0 );
	dVector aerodynamic_force_pos(-0.1,0,0);

	add_local_force(aerodynamic_force,aerodynamic_force_pos);
	add_local_force(thrust			 ,thrust_pos);

	dVector aileron_left (0 , 0.05 * Cy * (stick_roll) * q * S , 0 );
	dVector aileron_right(0 ,-0.05 * Cy * (stick_roll) * q * S , 0 );

	dVector aileron_left_pos(0,0,-5.0);
	dVector aileron_right_pos(0,0, 5.0);


	add_local_force(aileron_left ,aileron_left_pos);
	add_local_force(aileron_right,aileron_right_pos);

	static const double mza   = -0.1;
	static const double mzwz  = -0.1;


	common_moment.x += -0.1 * omega.x * q * 10;
	common_moment.z += (mzwz * omega.z  + mza * aoa) *  q * 3;
	common_moment.y += -0.1 * omega.y * q * 10;


	simulate_fuel_consumption(dt);


	if (desired_gear_state > 0)
	{
		test_gear_state += 2.0 * dt;
		if (test_gear_state > 1.0)
			test_gear_state = 1.0;
	}
	else
	{
		test_gear_state -= 2.0 * dt;
		if (test_gear_state < 0.0)
			test_gear_state = 0.0;
	}
}

void ed_fm_set_atmosphere(double h,//altitude above sea level
							double t,//current atmosphere temperature , Kelwins
							double a,//speed of sound
							double ro,// atmosphere density
							double p,// atmosphere pressure
							double wind_vx,//components of velocity vector, including turbulence in world coordinate system
							double wind_vy,//components of velocity vector, including turbulence in world coordinate system
							double wind_vz //components of velocity vector, including turbulence in world coordinate system
						)
{
	wind.x = wind_vx;
	wind.y = wind_vy;
	wind.z = wind_vz;

	atmosphere_density = ro;
	speed_of_sound     = a;
}
/*
called before simulation to set up your environment for the next step
*/
void ed_fm_set_current_mass_state (double mass,
									double center_of_mass_x,
									double center_of_mass_y,
									double center_of_mass_z,
									double moment_of_inertia_x,
									double moment_of_inertia_y,
									double moment_of_inertia_z
									)
{
	center_of_gravity.x  = center_of_mass_x;
	center_of_gravity.y  = center_of_mass_y;
	center_of_gravity.z  = center_of_mass_z;
}
/*
called before simulation to set up your environment for the next step
*/
void ed_fm_set_current_state (double ax,//linear acceleration component in world coordinate system
							double ay,//linear acceleration component in world coordinate system
							double az,//linear acceleration component in world coordinate system
							double vx,//linear velocity component in world coordinate system
							double vy,//linear velocity component in world coordinate system
							double vz,//linear velocity component in world coordinate system
							double px,//center of the body position in world coordinate system
							double py,//center of the body position in world coordinate system
							double pz,//center of the body position in world coordinate system
							double omegadotx,//angular accelearation components in world coordinate system
							double omegadoty,//angular accelearation components in world coordinate system
							double omegadotz,//angular accelearation components in world coordinate system
							double omegax,//angular velocity components in world coordinate system
							double omegay,//angular velocity components in world coordinate system
							double omegaz,//angular velocity components in world coordinate system
							double quaternion_x,//orientation quaternion components in world coordinate system
							double quaternion_y,//orientation quaternion components in world coordinate system
							double quaternion_z,//orientation quaternion components in world coordinate system
							double quaternion_w //orientation quaternion components in world coordinate system
							)
{
	velocity_world_cs.x = vx;
	velocity_world_cs.y = vy;
	velocity_world_cs.z = vz;
}


void ed_fm_set_current_state_body_axis(double ax,//linear acceleration component in body coordinate system
	double ay,//linear acceleration component in body coordinate system
	double az,//linear acceleration component in body coordinate system
	double vx,//linear velocity component in body coordinate system
	double vy,//linear velocity component in body coordinate system
	double vz,//linear velocity component in body coordinate system
	double wind_vx,//wind linear velocity component in body coordinate system
	double wind_vy,//wind linear velocity component in body coordinate system
	double wind_vz,//wind linear velocity component in body coordinate system

	double omegadotx,//angular accelearation components in body coordinate system
	double omegadoty,//angular accelearation components in body coordinate system
	double omegadotz,//angular accelearation components in body coordinate system
	double omegax,//angular velocity components in body coordinate system
	double omegay,//angular velocity components in body coordinate system
	double omegaz,//angular velocity components in body coordinate system
	double yaw,  //radians
	double pitch,//radians
	double roll, //radians
	double common_angle_of_attack, //AoA radians
	double common_angle_of_slide   //AoS radians
	)
{
	aoa = common_angle_of_attack;

	omega = dVector(omegax,omegay,omegaz);
}
/*
input handling
*/
void ed_fm_set_command (int command,
							float value)
{

	switch (command)
	{
	case iCommandPlaneThrustCommon:
		throttle = 0.5 * (-value + 1.0);
		break;
	case iCommandPlanePitch:
		stick_pitch		  = value;
		break;
	case iCommandPlaneRoll:
		stick_roll		  = value;
		break;
	case iCommandPlaneGear:
		if (desired_gear_state > 0)
			desired_gear_state = 0;
		else
			desired_gear_state = 1.0;
		break;
	}
}
/*
	Mass handling 

	will be called  after ed_fm_simulate :
	you should collect mass changes in ed_fm_simulate 

	double delta_mass = 0;
	double x = 0;
	double y = 0; 
	double z = 0;
	double piece_of_mass_MOI_x = 0;
	double piece_of_mass_MOI_y = 0; 
	double piece_of_mass_MOI_z = 0;
 
	//
	while (ed_fm_change_mass(delta_mass,x,y,z,piece_of_mass_MOI_x,piece_of_mass_MOI_y,piece_of_mass_MOI_z))
	{
	//internal DCS calculations for changing mass, center of gravity,  and moments of inertia
	}
*/
bool ed_fm_change_mass  (double & delta_mass,
						double & delta_mass_pos_x,
						double & delta_mass_pos_y,
						double & delta_mass_pos_z,
						double & delta_mass_moment_of_inertia_x,
						double & delta_mass_moment_of_inertia_y,
						double & delta_mass_moment_of_inertia_z
						)
{
	if (fuel_consumption_since_last_time > 0)
	{
		delta_mass		 = fuel_consumption_since_last_time;
		delta_mass_pos_x = -1.0;
		delta_mass_pos_y =  1.0;
		delta_mass_pos_z =  0;

		delta_mass_moment_of_inertia_x	= 0;
		delta_mass_moment_of_inertia_y	= 0;
		delta_mass_moment_of_inertia_z	= 0;

		fuel_consumption_since_last_time = 0; // set it 0 to avoid infinite loop, because it called in cycle 
		// better to use stack like structure for mass changing 
		return true;
	}
	else 
	{
		return false;
	}
}
/*
	set internal fuel volume , init function, called on object creation and for refueling , 
	you should distribute it inside at different fuel tanks
*/
void   ed_fm_set_internal_fuel(double fuel)
{
	internal_fuel = fuel;
}
/*
	get internal fuel volume 
*/
double ed_fm_get_internal_fuel()
{
	return internal_fuel;
}
/*
	set external fuel volume for each payload station , called for weapon init and on reload
*/
void  ed_fm_set_external_fuel (int	 station,
								double fuel,
								double x,
								double y,
								double z)
{

}
/*
	get external fuel volume 
*/
double ed_fm_get_external_fuel ()
{
	return 0;
}

void ed_fm_set_draw_args (EdDrawArgument * drawargs,size_t size)
{
	drawargs[28].f   = (float)throttle;
	drawargs[29].f   = (float)throttle;


	drawargs[611].f = drawargs[0].f;
	drawargs[614].f = drawargs[3].f;
	drawargs[616].f = drawargs[5].f;
}


void ed_fm_configure(const char * cfg_path)
{
	
}

double ed_fm_get_param(unsigned index)
{
	if (index <= ED_FM_END_ENGINE_BLOCK)
	{
		switch (index)
		{
		case ED_FM_ENGINE_0_RPM:			
		case ED_FM_ENGINE_0_RELATED_RPM:	
		case ED_FM_ENGINE_0_THRUST:			
		case ED_FM_ENGINE_0_RELATED_THRUST:	
			return 0; // APU
		case ED_FM_ENGINE_1_RPM:
			return throttle * 3000;
		case ED_FM_ENGINE_1_RELATED_RPM:
			return throttle;
		case ED_FM_ENGINE_1_THRUST:
			return throttle * 5000 * 9.81;
		case ED_FM_ENGINE_1_RELATED_THRUST:
			return throttle;
		}
	}
	else if (index >= ED_FM_SUSPENSION_0_RELATIVE_BRAKE_MOMENT &&
			 index < ED_FM_OXYGEN_SUPPLY)
	{
		static const int block_size = ED_FM_SUSPENSION_1_RELATIVE_BRAKE_MOMENT - ED_FM_SUSPENSION_0_RELATIVE_BRAKE_MOMENT;
		switch (index)
		{
		case 0 * block_size + ED_FM_SUSPENSION_0_GEAR_POST_STATE:
		case 1 * block_size + ED_FM_SUSPENSION_0_GEAR_POST_STATE:
		case 2 * block_size + ED_FM_SUSPENSION_0_GEAR_POST_STATE:
			return test_gear_state;
		}
	}
	return 0;

}



void ed_fm_cold_start()
{
	test_gear_state = 1.0;
	desired_gear_state = 1.0;
}

void ed_fm_hot_start()
{
	test_gear_state = 1.0;
	desired_gear_state = 1.0;
}

void ed_fm_hot_start_in_air()
{
	test_gear_state = 0;
	desired_gear_state = 0;
}
}

