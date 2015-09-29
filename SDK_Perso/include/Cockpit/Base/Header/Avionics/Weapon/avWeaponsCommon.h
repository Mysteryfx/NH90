#pragma once

#include "Avionics/avDevice.h"
#include "Utilities/ccUtilities.h"
#include "IwHumanPayload.h"
#include "CannonSight.h"
#include "Avionics/avAvionicsProxyDefault.h"

#include "Sights/nurs_sight.h"

#include <memory>

struct wLauncher;
struct Bomb_Const;
struct Rocket_Const;

class wAmmunitionDescriptor;
class wAmmunitionNURSDescriptor;
class wShellDescriptor;

namespace cockpit
{

struct station_number
{
	station_number(int st = -1, int sub = -1) : station(st), substation(sub) {}
	bool valid() const {return (station < 0 || substation < 0) ? false : true;} 
	int station;
	int substation;
};

typedef union 
{
	const Bomb_Const   *BOMB;	
	const Rocket_Const *MISSILE;
}
weapon_data; 

struct DynamicLaunchZone
{
	//DLZ
    DynamicLaunchZone():dMIN(0),dAUT(0),dMAX(0){}
	union 
	{
		struct
		{
			double         dMIN; // minimum    (safety)
			double         dAUT; // authorized (efficient)
			double         dMAX; // maximum    (ballistic)
		};
		double         array[3];
	};
};

struct SightingData
{
	SightingData() {  weapon.BOMB = 0; weapon.MISSILE = 0;}
	weapon_data       weapon;
};

enum Sight_enum
{
	sight_GUN			= 0,
	sight_ROCKET		= 1,
	sight_BOMB			= 2,
	sight_MISSILES		= 3,
	sight_CLUSTER_BOMBS = 4,
	sight_SIZE_MAX
};

struct piper_omega_lead_data
{
	std::queue<dVector>		omega;
	dVector					piper_rotation;
	void update(const dVector & curr_omega, double dt , double TOF)
	{
		piper_rotation -= curr_omega * dt;
		omega.push(curr_omega);
		while (omega.size() > TOF/dt)
		{
			dVector & om = omega.front();
			piper_rotation += om * dt;
			omega.pop();
		}	
	}
};


class COCKPITBASE_API SightDevice
{
public :

	SightDevice(const weapon_base::WindTableProxy* wind_proxy = nullptr) 
		: wind_proxy_(wind_proxy)
	{
		external_distance      = 0;
		external_aim_available = false;
		time_of_flight         = 0;
		can_calculate          = true;
		aim_point.Set(PolarPosition(0,0,0), wTime::GetModelTime());
		freezed				   = false;
		target_level			= 0.0;
	};

	virtual ~SightDevice(){};
	virtual void			 calculate() = 0;
	virtual bool calculate(PolarPosition& aim_pos){return false;};
	const PolarPosition&	 get_aim_point()  const		
	{
		return get_aim_point(wTime::GetModelTime());
	}
	const PolarPosition&	 get_aim_point(double time)  const		
	{
		static PolarPosition dummy;
		dummy = aim_point.GetValue(time);
		return				 dummy;
	}
	cVector get_aim_point_global() const {return aim_point_global;}

	const DynamicLaunchZone& get_DLZ()		  const						{ return DLZ;}
	void					 set_sight_data(const SightingData & data_) { data = data_;}
	void					 set_external_distance(double distance)		{ external_distance = distance;}
	double					 get_external_distance() const				{ return external_distance;}

	bool					 is_distance_available()			  const { return distance_available;}
	void					 set_distance_available(bool flag_in)		{ distance_available = flag_in;}

	virtual	bool			 launch_authorized() const = 0;
	virtual bool			 breakaway() const = 0;
	void					 set_external_aim_point(const PolarPosition & p) { external_aim_point = p;}
	void					 set_external_aim_point_available(bool flag_in)  { external_aim_available = flag_in;}
	void					 set_target_level(double target_level_)			 { target_level = target_level_;}
	double					 get_target_level() const { return target_level;}
	void					 forestall(const PolarPosition &  forestalling);
	wModelTime				 get_time() const								 { return time_of_flight; }
	void                     set_can_calculate  (bool flag_in)               { can_calculate			= flag_in; }
	void                     freeze(); 
	bool                     is_freezed() const { return freezed; }
	void                     unfreeze();
    const SightingData    &  get_sight_data() const { return data;}

    double                   get_self_height() const;
protected:

	const weapon_base::IWindTablePtr get_wt_ptr();	/* return reference to an actual wind table.*/
	const weapon_base::IWindTablePtr get_wt_ptr(const dVector& current_wind); /* return reference to a wind table */
	
	Math::Vec3d get_real_wind(double altitude = 0) const;																							

    dPosition          get_human_position() const;
    dVector            get_human_point() const;
	wPolar			   aim_point;
	PolarPosition      external_aim_point;
	cVector			   aim_point_global;
	DynamicLaunchZone  DLZ;
	SightingData       data;

	bool               distance_available;
	bool               external_aim_available;
	bool               can_calculate;
	bool               freezed;
	double             external_distance;
	wModelTime		   time_of_flight;
	double             target_level;

	const weapon_base::WindTableProxy* wind_proxy_; 
	weapon_base::SingleVectorTable def_wt_; /*default wind table*/
};

class COCKPITBASE_API GunSight  : public SightDevice 
{
public :
	GunSight(const weapon_base::WindTableProxy* wind_proxy = nullptr); 
	~GunSight();
	void calculate();
	bool calculate(PolarPosition& aim_pos);
	bool launch_authorized() const;
	bool breakaway() const;
	void setShellDesc(const wShellDescriptor* desc) { shellDesc_ = desc; }
	bool calculate_funnel(float Dist, wModelTime& Time, dVector& FunPos);
	bool calculate_funnel(wModelTime Time, dVector& FunPos);
	bool calculate_depression(wModelTime Time, dVector& DetPos,double pitch,double Vsc);
	bool calculate_air_to_ground(wModelTime& Time, dVector& GroundPos, const wShellDescriptor*	he_desc = NULL);
	bool getFEDSPoint(int ind, dVector& fp);
	void launchFEDS(bool Ground, wModelTime LifeTime, float deltaZ,double target_altitude);
	void updateFEDS(wModelTime dtime);
	bool getLastGroundFEDS(dVector &fp);
protected:

	const wShellDescriptor* shellDesc_;
	struct GunFEDS
	{
        FEDS_data   data;
		bool        ground;
		wModelTime  time;
        double      target_level;
	};
	ed::vector <GunFEDS*> FEDSVector;
	dVector lastGroundFEDS;
	bool Ground;
};

class COCKPITBASE_API RocketSight  : public SightDevice 
{
public :
		RocketSight(const weapon_base::WindTableProxy* wind_proxy = nullptr);
		virtual ~RocketSight();
		void calculate();
		void calculate(cPosition& launch_position, const cVector& VectorVelocity, bool Rough);
		bool launch_authorized() const;
		bool breakaway() const;

		void setRocketType(wsType type);
private:
	std::auto_ptr<wNURSSight> sight_;
	const wAmmunitionNURSDescriptor* desc_;
	wsType type_;
};

class COCKPITBASE_API MissileSight : public SightDevice 
{
public :
	MissileSight(const weapon_base::WindTableProxy* wind_proxy = nullptr);
	
	~MissileSight(){};
	void calculate();
	bool launch_authorized() const;
	bool breakaway() const;

    void setMissileData(const wsType & type);
private:

	double drag (double v);
	double boost(wModelTime time);
	void   integrate(wModelTime t, wModelTime& t_last, double& v, double& d);
	wModelTime time_by_distance(double d, double v0);
	double distance_by_time(wModelTime t, double v0);
	bool   maverick(unsigned char name) const; 

    wNURSSight                 * sight;
    wAmmunitionDescriptor      * desc;
};		

class COCKPITBASE_API SimpleGyroSight
{

public:
	SimpleGyroSight();
	float time_by_distance(float d);

	void setAltitude(float new_h)
	{
		H_km_ = new_h* 0.001f;
	}
	void setAircraftSpeed(float new_V_h)
	{
		V_H_ = new_V_h;
	}
private:
	float k1_;  //coef from bd
	float V_H_; //plane speed
	float V_c_; //shell speed 
	float H_km_; //plane height

};

class COCKPITBASE_API avWeaponsCommon
{
public:
	static void fire_cannon(bool fire, int burst_length = -1, bool disable_gun_cutoff = false);
	static void fire_cannon(bool fire, unsigned gun_num, int burst_length = -1, bool disable_gun_cutoff = false);

	static void launch_weapon(bool);
	static void fire_gunpods(bool fire,const ed::vector<int>& pilons);
	static void fire_multigun_gunpod(bool fire, int pilon, const ed::vector<int>& gunsID);
	static void fire_gunpods(bool fire);
	static int  check_sight(SightDevice* sights[]);
    static int  check_sight(const wsType & selection,SightDevice* sights[]);
	static void set_gun_failure(unsigned int gun_number, bool failed);
	static void set_gun_mount_failure(char gun_number, bool failed, 
		float azimuth_deviation = .0f /*in degr*/, float elevation_deviation = .0f /*in degr*/);
	static void sync_gun_T(char gun_number, char station = -1);
};

class avWeap_ReleaseTimer_Activity;
class COCKPITBASE_API avRippReleaseCapable
{
public:
	avRippReleaseCapable() : release_timer(0) {}
	virtual ~avRippReleaseCapable();
	virtual void Launch(bool) = 0;
	void		 set_timer(avWeap_ReleaseTimer_Activity * timer)
	{
		release_timer = timer;
	}
protected:
	avWeap_ReleaseTimer_Activity * release_timer;
};

class COCKPITBASE_API avWeap_ReleaseTimer_Activity : public wTime
{
	public:
		avWeap_ReleaseTimer_Activity(avRippReleaseCapable* system_) : system(system_), dtime(0), to_cancel(true)
		{
			if (system)	system->set_timer(this);
		}

		~avWeap_ReleaseTimer_Activity()
		{
			if (system)	system->set_timer(0);
		}

		void NextEvent();
		void start_unloading_with_first_shot_delay(wModelTime delay_, wModelTime dtime_);

		inline void start_unloading_single(wModelTime dtime_)
		{
			start_unloading_with_first_shot_delay(dtime_,0);
		}

		inline void start_unloading(wModelTime dtime_)
		{
			start_unloading_with_first_shot_delay(dtime_,dtime_);
		}
		void stop_unloading();
		void SetParent(avRippReleaseCapable* p)
		{
			if (system)
				system->set_timer(0);

			system = p;
			if (system)
				system->set_timer(this);
		}

		void Die()
		{
			system = nullptr;
			Cancel();
			KillMe();
		}

	protected:
		avRippReleaseCapable* system;
		wModelTime dtime;
		bool   to_cancel;
};
}
