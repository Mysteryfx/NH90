#pragma once

#include "Avionics/Weapon/avWeaponsCommon.h"

class wBombSightDataDescriptor;

namespace cockpit {

class COCKPITBASE_API BombSight : public SightDevice
{
    void* acceleration_x_w_handle;//sensor output
    void* acceleration_y_w_handle;//sensor output
    void* acceleration_z_w_handle;//sensor output
public:
    BombSight(const weapon_base::WindTableProxy* wind_proxy = nullptr);
    ~BombSight(){};
    void calculate();
    bool calculate          (PolarPosition& aim_pos);

    bool calculate          (double Pitch,double self_level,double PitchZad, PolarPosition& aim_pos);
    bool calculate_lead     (wModelTime lead_time, PolarPosition& pos, bool Release, bool ElevationIsFixed);
    bool calculate_lead_alt_vel_acc(double Alt, PolarPosition& pos, wModelTime& lead_time, bool Release, bool ElevationIsFixed);
	bool calculate_lead_alt_vel(double Alt, PolarPosition& pos, wModelTime& lead_time, bool Release, bool ElevationIsFixed); //расчёт без учёта ускорения
    bool calculate_lead_time(wModelTime Time, PolarPosition& pos, wModelTime& lead_time, bool Release, bool ElevationIsFixed);
    bool calculate_lead     (float PitchZad, wModelTime lead_time, PolarPosition& pos, bool Release, bool ElevationIsFixed);
    bool calculate_lead_alt_vel(float PitchZad, double Alt, PolarPosition& pos_lead, wModelTime& lead_time, bool Release, bool ElevationIsFixed);
    bool calculate_lead_time(double Pitch, float PitchZad, wModelTime Time, PolarPosition& pos, wModelTime& lead_time, bool Release, bool ElevationIsFixed);
	cVector calculateDist_by_Time(const cVector	& Velocity, wModelTime timeOfFlight);
    cVector calculateDist_by_Time(double Pitch, double PitchZad, const cVector & Velocity, wModelTime timeOfFlight);
    double  calculateDist_by_Alt(double Alt);
    // точка на земле отнесенная на заданное расстояние вперед (центр серии)
    bool calculate_lead_dist(double Dist, PolarPosition& pos, wModelTime& lead_time, bool Release, bool ElevationIsFixed);
	
    bool launch_authorized() const;
    double get_max_bomb_ht();
    bool breakaway() const;

    double get_lead_time() const { return lead_time_of_flight; }
    const PolarPosition& get_lead_aim_point()  const		
    {
        static PolarPosition dummy;
        dummy = lead_aim_point.GetValue(wTime::GetModelTime());
        return dummy;
    }
    void set_lead_target_level(double lead_target_level_) { lead_target_level = lead_target_level_;}
    const cVector & get_lead_aim_point_global() const {return lead_aim_point_global;}

    const PolarPosition& get_center_of_bomb_serie() const		
    {
        static PolarPosition dummy;
        dummy = center_of_bomb_serie.GetValue(wTime::GetModelTime());
        return dummy;
    }

	double BombFlyInitAlt(wModelTime desiredTOF, const cVector& Vel);

    void setBombType(const wsType & type);

protected:
  virtual  bool  calculate_aim_point(const cPoint  & p,
        const cVector & air_speed,
        double          target_level_,
        double  & ToF,
        cVector & aim_point);

    void get_lead_release_point_vel_acc(const dPosition& start_,
        const dVector& start_velocity_,
        double lead_time_,
        dPosition& out,
        dVector& out_velocity);

	dVector get_lead_acceleration();

    cVector calculate_velocity_by_pitch(const cVector & start_velocity,                           
        double Pitch,
        double PitchZad);

	double BombFlyInitAlt_Old(wModelTime desiredTOF, const cVector& Vel);
	double BombFlyInitAlt_New(wModelTime desiredTOF, const cVector& Vel);
	
	double BombFlyHorizontalDist_New(const cVector& Vel, double launchAltitude);
	double BombFlyHorizontalDist(wModelTime desiredTOF, const cVector& Vel, double launchAltitude);

	virtual void updateFilters();

    wModelTime lead_time_of_flight;
    wPolar	lead_aim_point;
    double  lead_target_level;
    cVector lead_aim_point_global;
    wPolar	center_of_bomb_serie;
	
	dVector accelerationFiltered;
	wModelTime prevFiltersUpdateTime;

    wBombSightDataDescriptor* bomb_desc;	
};


class COCKPITBASE_API ClusterBombSight : public BombSight
{
public:
	ClusterBombSight(const weapon_base::WindTableProxy* wind_db_ptr = nullptr);
	virtual ~ClusterBombSight();

	void SetOpenHeight(double height_) {open_height = height_;}
	double GetOpenHeight() {return open_height;}

protected:

	virtual  bool  calculate_aim_point(const cPoint  & p,
		const cVector & air_speed,
		double          target_level_,
		double  & ToF,
		cVector & aim_point);


	double open_height;
};

}


