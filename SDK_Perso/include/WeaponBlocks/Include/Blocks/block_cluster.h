#ifndef __BLOCK_RAMP_H__
#define __BLOCK_RAMP_H__

#include "simulation_block_disc.h"

#include "cx_interpolator.h"

#include "WeaponBlocks.h"

#ifndef NO_REAL_LOCKON
	#include "lPointOnSurface.h"
	#include "edTerrain/PointOnSurfaceAdapter.h"
	#include "Randomizer/RandomObject.h"
#endif

class wAmmunition;

class wBlockClusterDescriptor;

class wBomblet;

class WEAPON_BLOCKS_API wBlockCluster : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wBlockCluster, wSimulationBlockDisc, wBlockClusterDescriptor);
public:
#ifndef NO_REAL_LOCKON
	static void init_randoms(int offset);
	static void serialize_randoms(Serializer& s);

	static RandomObject& getRandom() { return rndNormal_; }
#endif

public:
	wBlockCluster(const wBlockClusterDescriptor* desc, wSimulationSystem* system, Lua::Loader* runtime_cfg);
	virtual ~wBlockCluster();

	virtual double simulate(double time);

	wAmmunition* getParent();
	const wBlockClusterDescriptor* getDesc() const;

protected:
	void spawnBomblets_(const Math::Vec3d& pos, const Math::Vec3d& vel, const Math::Rot3d& par_rot, const Math::Rot3d& init_rot, const Math::Vec3d& omega, const Math::Vec3d* initial_pos);

	bool testGroundCollision_(const Math::Vec3d& prev_pos, const Math::Vec3d& pos, const Math::Vec3d& vel, bool effect);

protected:
	wBomblet* bomblets_;	// Array of bomblets
	bool*	  alive_;		// Array: is bomblet alive??

#ifndef NO_REAL_LOCKON
#ifndef USE_TERRAIN4
	landscape::lPointOnSurface point_;		// Collision handling
#else 
	edterrain::PointOnSurfaceAdapter point_;
#endif
#endif

protected:
	INPUT_WIRES(3, (
		WIRE(double, rho_),		// Все бомблеты на одной высоте
		WIRE(double, M_),
		WIRE(Math::Vec3d, wind_)
	))

	OUTPUT_WIRES(9, (
		WIRE(Math::Vec3d, pos_),
		WIRE(Math::Vec3d, vel_),
		WIRE(Math::Rot3d, rot_),
		WIRE(Math::Vec3d, omega_),

		WIRE(Math::Vec3d, colPos_),
        WIRE(Math::Vec3d, colNorm_),
		WIRE(int, colObj_),
		WIRE(Math::Vec3d, colVel_),
		WIRE(int, objPartName_)
	))

	INPUT_PORT_OBJ_PTR(wAmmunition, owner_);

	OUTPUT_PORT(bool, explode_);
	OUTPUT_PORT(bool, died_);

#ifndef NO_REAL_LOCKON
	static RandomObject rndNormal_;
#endif
};

class wBlockClusterDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wBlockCluster)

public:
	virtual ~wBlockClusterDescriptor();

	virtual void serialize(wBlockSerializer& ser);

	virtual wBlockCluster* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(int, count);				// Количество бомблетов
    DECL_PARAM(int, effect_count);      // Сколько рисуется эффектов

	// FM
	DECL_PARAM(double, dt);				// Шаг времени
	DECL_PARAM(double, mass);			// Полная масса снаряда масса, кг
	DECL_PARAM(double, caliber);		// Калибр, мм
	DECL_PARAM(double, L);				// Характеристическая длина
	DECL_PARAM(double, I);				// Момент инерции относительно поперечной оси
	DECL_PARAM(double, Ma);				// Градиент коеффициента момента по углу атаки
	DECL_PARAM(double, Mw);				// Градиент коэффициента момента по угловой скорости
	DECL_PARAM(double[5], cxCoeff);		// Исходные коэффициенты для Cx

	CxInterpolator Cx;
	double Sm;

	DECL_PARAM(double, wind_sigma);
	DECL_PARAM(double, impulse_sigma);
	DECL_PARAM(double, moment_sigma); 

	DECL_PARAM(ed::string, model_name);
};

#endif
