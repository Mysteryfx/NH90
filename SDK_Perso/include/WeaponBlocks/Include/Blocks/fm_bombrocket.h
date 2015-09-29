#ifndef __FM_BOMBROCKET__
#define __FM_BOMBROCKET__

#include "Math/Position.h"

#include "fm_base.h"
#include "cx_interpolator.h"
#include "block_parameter.h"

#include "../attachable.h"

#ifndef NO_REAL_LOCKON
#include "Randomizer/RandomObject.h"
#endif


class wDatabaseLoader;
class wFMBombRocketDescriptor;

class wFMBombRocketDescriptor;

// Аэродинамика для бомб и ракет
class WEAPON_BLOCKS_API wFMBombRocket : public wFMBase
{
#ifdef _ED_HIDDEN_DEBUG 
    bool freeze;
#endif
	DECLARE_BLOCK(wFMBombRocket, wFMBase, wFMBombRocketDescriptor)
public:
#ifndef NO_REAL_LOCKON
	static void init_randoms(int offset);
	static void serialize_randoms(Serializer& s);
#endif

public:
	typedef wFMBombRocketDescriptor ParametersType;

public:
	wFMBombRocket(const wFMBombRocketDescriptor* fmData, wSimulationSystem* system);
	~wFMBombRocket();

	// v - в глобальной системе координат
	// Надо вызывать после того, как ориентация  будет установлена
	virtual void setRotation(const Math::Rot3d& val);
	virtual void setAngularVelocity(const Math::Vec3d& v);
	virtual void onWarheadPenetration(const Math::Vec3d& v);
	virtual void onRebound(const Math::Vec3d& vel);

	// Сделать шаг
	virtual double simulate(double time);
	virtual void simulateDt(double time, double dt);

	// Сериализация
/*#ifndef WEAPONS_BENCH
	virtual void serialize(Serializer &serializer);
#endif*/

protected:
	virtual void calcForceTorque_(const Math::Pos3dd& pos, const Math::Vec3d& v, double sndSpeed, double rho, Math::Vec3d& R, Math::Vec3d& M);
	virtual Math::Vec3d calcForce_(const Math::Pos3dd& pos, const Math::Vec3d& v, double sndSpeed, double rho);	// Сила (без учета mg)
	virtual Math::Vec3d calcTorque_(const Math::Pos3dd& pos, const Math::Vec3d& v, double sndSpeed, double rho);	// Момент

	double getMass_();

protected:
	INPUT_WIRES(2, (
		WIRE(double, thrust_),
		WIRE(double, fuel_mass_)
	))

	OUTPUT_WIRES(1, (
		WIRE(Math::Vec3d,omega_)						// Угловая скорость - в связанной системе
	))

	INPUT_PORT(Math::Vec3d, pomega_);
	INPUT_PORT(Math::Vec3d, warheadPosition_);
	INPUT_PORT(Math::Vec3d, rebound_);

	double flightTime_;	// Считается с момента создания
	double addWindY_, addWindZ_;	// Дополнительный ветер (для рассеивания)

	Math::Rot3d realRot_;		// Вращение для физики (без учета omega_x)
	Math::Vec3d penetrationPos_; //при наличии замедлителя во взрывателе
	double fuel_mass_0;

#ifndef NO_REAL_LOCKON
	static RandomObject rndNormal_;
#endif

	bool first_iter_;
	Math::Vec3d current_vel_;
};

// Аэродинамические параметры бомб и ракет
class WEAPON_BLOCKS_API wFMBombRocketDescriptor : public wFMBaseDescriptor, public wAttachableDescriptor<wFMBombRocketDescriptor>
{
	DECLARE_BLOCK_DESCRIPTOR(wFMBombRocket);

public:
	virtual void serialize(wBlockSerializer& s);
	double getCy(double mach) const;

	const ed::string& getModelName() const { return shapeName.value(); }

	virtual wFMBombRocket* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);


public:
	DECL_PARAM(double, dt);				// Шаг времени
	DECL_PARAM(double, mass);			// Полная масса снаряда масса, кг
	DECL_PARAM(double, caliber);		// Калибр, мм
	DECL_PARAM(double, L);				// Характеристическая длина
	DECL_PARAM(double, I);				// Момент инерции относительно поперечной оси
	DECL_PARAM(double, Ma);				// Градиент коеффициента момента по углу атаки
	DECL_PARAM(double, Mw);				// Градиент коэффициента момента по угловой скорости
	DECL_PARAM(double, wind_time);		// "ветер разброса" - время действия
	DECL_PARAM(double, wind_sigma)		// "ветер разброса" - среднеквадратичное отклонение
	DECL_PARAM(double[5], cxCoeff);		// Исходные коэффициенты для Cx
	DECL_PARAM(ed::string, shapeName);	// Имя шейпа - для

	double Sm;							//Площадь миделя, м^2
	CxInterpolator cxInterp;			// Вычислитель cx
};
#endif
