#pragma once

#include "Math/Vector.h"

#include "Weapons.h"
#include "cLinear.h"
#include "cPosition.h"
#include "dPosition.h"
#include "wTime.h"

#include "wind_table.h"

class wShellDescriptor;

struct WEAPONS_API FEDS_data 
{   
    FEDS_data():
    pos (0,0,0),
    vel (0,0,0),
    wind(0,0,0),
    started_(false),
    intersect_(false)
    {
    }
    bool         started_;
    bool         intersect_;
    Math::Vec3d  pos;
    Math::Vec3d  vel;
    Math::Vec3d  wind;
};

using weapon_base::IWindTablePtr;

class WEAPONS_API CannonSight
{
public:
	
	CannonSight();
	explicit CannonSight(const wShellDescriptor& desc);
	~CannonSight();

	// Это ТЯЖЕЛАЯ операция
	void loadShell(const wShellDescriptor& desc);

	// KA: еще нужно время полета
	int sightAlt(double sourceHeight, // высота самолета для правильного учета атмосферы в ФМ
					double targetHeight, // высота цели
					const dVector& shellVelocity, // начальная скорость снаряда
					const dVector& wind, 
					dVector& distance,
					wModelTime& time,
					wModelTime maxTime = 15.0);

	int sightAlt(double sourceHeight, // высота самолета для правильного учета атмосферы в ФМ
		         double targetHeight, // высота цели
		         const dVector& shellVelocity, // начальная скорость снаряда
		         const IWindTablePtr wind_t_ptr, //wind table; wind = f(altitude)
		         dVector& distance,
		         wModelTime& time,
		         wModelTime maxTime = 15.0);

	int sightDist(double sourceHeight, // высота самолета для правильного учета атмосферы в ФМ
			double targetDist, // дистанция до цели
			const dVector& shellVelocity, // начальная скорость снаряда
			const dVector& wind, 
			dVector& distance,
			double& time,
			wModelTime maxTime = 15.0);

	int sightDist(double sourceHeight, // высота самолета для правильного учета атмосферы в ФМ
		          double targetDist, // дистанция до цели
		          const dVector& shellVelocity, // начальная скорость снаряда
				  const IWindTablePtr wind_t_ptr,  //wind table; wind = f(altitude)
		          dVector& distance,
		          double& time,
		          wModelTime maxTime = 15.0);

    int sightTime(double         sourceHeight,// высота самолета для правильного учета атмосферы в ФМ
                  wModelTime     targetTime,//время до цели
                  const dVector& shellVelocity, // начальная скорость снаряда
                  const IWindTablePtr wind_t_ptr,  //wind table; wind = f(altitude)
                  dVector      & distance,
                  wModelTime     maxTime = 15.0);

	bool calculateImpactPoint(const cPosition & gun_pos, const cVector & self_vel, const cPoint & target_point, const cVector & wind, cVector & v, wModelTime & dtime);

	//A. Kovalenko прицеливание для пушки
	cVector Gun_Aiming(	const cPosition & gun_pos, const cVector & V, const cVector & vWind, const cVector & Targ_Pos, const cVector & Target_V, 
						const cVector & Target_Acc, bool &flag_shoot, bool &flag_aiming, bool &flag_in_angles, 
						int Skill, cVector * dir_to_impact_point = NULL, wModelTime * dt = NULL);
    cVector Gun_AimingForGroundUnits(	const cPosition & gun_pos, const cVector & V, const cVector & vWind, const cVector & Targ_Pos, const cVector & Target_V, 
        const cVector & Target_Acc, cVector * dir_to_impact_point = NULL, wModelTime * dt = NULL);

	cVector Gun_Vehicle_Aiming(	const cPosition & gun_pos, const cVector & V, const cVector & vWind, const cVector & Targ_Pos, const cVector & Target_V,
								const cVector & Target_Acc, int Skill, cVector * dir_to_impact_point = NULL, wModelTime * dt = NULL);

	cVector Gun_Aiming_Time(const cPosition & gun_pos, const cVector & V, const cVector & vWind, const cVector & Targ_Pos, const cVector & Target_V, 
							const cVector & Target_Acc, bool & flag_shoot, bool &flag_aiming, bool &flag_in_angles, int Skill, wModelTime dt = 0.0, double _c1=-0.0010796, double _c2=9.4858);

    cVector Gun_Aiming_Table(const cPosition & gun_pos, const cVector & V, const cVector & vWind, const cVector & Targ_Pos, bool &flag_shoot, bool &flag_aiming, bool &flag_in_angles, int Skill);

    bool simulate_shot( const dVector& pos, 
                        const dVector& vel_, 
                        const dVector& wind_, 
                        wModelTime& max_time_,
                        dPoint& hit_pos_);

    bool updateFEDS(FEDS_data & FEDS,wModelTime dtime,bool ground,double target_altitude);

private:
    void updateFM_(Math::Vec3d& pos, Math::Vec3d& vel, Math::Vec3d& wind, double dt);

private:  

	cVector prev_Tang_aiming;

	cVector old_targ_pos;
	cVector old_gun_pos;
	bool m_oldPermission;
	int countNoCalc;

	wModelTime m_fCorrectionTime;
	float m_TargSpeedCorrect;

	const wShellDescriptor* shell_;
};

// Стрельба по движущейся цели
// Найти время, при котором цель и снаряд будут на одинаковом удалении от точки выстрела
// Начало координат - в точке выстрела
// Возвращает:
//  true - если есть решение, false - если нет

// Рассчитать время полета (воздушная цель)
bool WEAPONS_API CalculateTOFToAirTarget(
					 const Math::Vec3d& vD0,		// Положение цели
					 const Math::Vec3d& vV,			// Скорость цели
					 const Math::Vec3d& vA,			// Ускорение цели
					 double H,						// Высота стрельбы
					 double Vc,						// Скорость носителя
					 const wShellDescriptor& shell,			// Чем стреляем
					 wModelTime& time				// [out] Время встречи
					 );