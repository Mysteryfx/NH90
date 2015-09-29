#ifndef __NURS_SIGHT_H__
#define __NURS_SIGHT_H__

#include "WeaponsBase.h"
#include "wWeaponUtils.h"
#include "wsType.h"

#include "Math/Vector.h"
#include "Math/Rotation3.h"

#include "cx_interpolator.h"
#include "wind_table.h"

template <class T> class wInputPort;

using weapon_base::IWindTablePtr;

class WEAPONSBASE_API wNURSSight
{
public:
	enum AimType
	{
		BY_TARGET_HEIGHT,
		BY_DIST,
		BY_HOR_DIST
	};
    struct FMData
    {
        CxInterpolator cx;
        double mass, Sm, L, I, Ma, Mw;
    };

    struct EngineData
    {
        double thrust, fuel_mass, fuel_rate, engine_time;
    };

public:
	// Создание прицела - !!!!КРАЙНЕ!!!! длительная операция!!
	wNURSSight();
	~wNURSSight();

    void setData(const FMData& fm_data, const EngineData& eng_data);
	void setWsType(wsType type) {curType_ = type; valid_ = false;}
	const wsType& getCurrent_wsType() const {return curType_;}
	
	// Точка пересечения с плоскостью y = target_height
	// v0 - в связанной системе координат
	bool calculateImpactPoint(double height, 
		const Math::Rot3d& rot, 
		const Math::Vec3d& v0, 
		AimType aimType,
		double targetParam,
		double rail_length, 
		Math::Vec3d & IP,
		wModelTime & TOF, 
		const IWindTablePtr wt_ptr = nullptr,
		double maxTime = 60.0);

	bool valid() const { return valid_; }

protected:
	void simulate_(double t, double dt, 
		Math::Vec3d& pos, 
		Math::Vec3d& vel,
		Math::Rot3d& rot, 
		Math::Vec3d& omega);

protected:
	// Данные fm
	CxInterpolator cx;
	double mass, Sm, L, I, Ma, Mw;

	// Данные двигателя
	double thrust, fuel_mass, fuel_rate, engine_time;

	wsType curType_;
	bool valid_;
};

#endif
