#ifndef __BOMB_SIGHT_H__
#define __BOMB_SIGHT_H__

#include "WeaponsBase.h"
#include "Math/Vector.h"
#include "cx_interpolator.h"
#include "wind_table.h"

// Бомбовый прицел
using weapon_base::IWindTablePtr;

class WEAPONSBASE_API wBombSight
{
public:
	wBombSight();
	~wBombSight();

	static wBombSight* instance();

	// Прицелиться
	bool aim(	double sourceHeight,		// высота самолета для правильного учета атмосферы в ФМ
				double targetHeight,		// высота цели
				double trueAirVelocity,		// скорость самолета
				const Math::Vec3d& wind,	// ветер в системе самолета
				double pitch,				// угол тангажа
				double angleOfAttack,		// угол атаки
				double bank,				// угол крена
				double yaw,					// угол скольжения
				double charTime,			// Характеристическое время
                double& time,               // Время полета
				Math::Vec3d& distance);	    // [out]расстояние, которое пролетит бомба

	bool cluster_aim(	double sourceHeight,		// высота самолета для правильного учета атмосферы в ФМ
		double targetHeight,		// высота цели
		double trueAirVelocity,		// скорость самолета
		const Math::Vec3d& wind,	// ветер в системе самолета
		double pitch,				// угол тангажа
		double angleOfAttack,		// угол атаки
		double bank,				// угол крена
		double yaw,					// угол скольжения
		double charTime,			// Характеристическое время
		double charTimeBomblets,	  // Характеристическое время бомблетов
		double open_height,			  // Заданная высота раскрытия кассеты
		double& time,               // Время полета
		Math::Vec3d& distance);	    // [out]расстояние, которое пролетит бомба

    // Прицелиться
    bool simple_aim(double sourceHeight,		  // высота самолета для правильного учета атмосферы в ФМ
                    double targetHeight,		  // высота цели
                    const Math::Vec3d & air_speed,//вектор воздушной скорости в точке сброса
                    double charTime,			  // Характеристическое время
                    double& time,                 // Время полета
                    Math::Vec3d  & distance,	  // [out]расстояние вдоль вектора air_speed , которое пролетит бомба
					const IWindTablePtr wind_t_ptr = nullptr);

	bool cluster_simple_aim(double sourceHeight,		  // высота самолета для правильного учета атмосферы в ФМ
		double targetHeight,		  // высота цели
		const Math::Vec3d & air_speed,//вектор воздушной скорости в точке сброса
		double charTime,			  // Характеристическое время
		double charTimeBomblets,	  // Характеристическое время бомблетов
		double open_height,			  // Заданная высота раскрытия кассеты
		double& time,                 // Время полета
		Math::Vec3d  & distance,	  // [out]расстояние вдоль вектора air_speed , которое пролетит бомба
		const IWindTablePtr wind_t_ptr = nullptr);

protected:
	bool intersect_(double y0, const Math::Vec3d& prevPos, Math::Vec3d& curPos);

protected:
	// Не вынести ли все это в статик?
	double s_;
	CxInterpolator cxInterp_;
};
	

#endif // __BOMB_SIGHT_H__
