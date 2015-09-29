#pragma once

#include "Inter.h"
#include "cPosition.h"
#include "Registry\Registered.h"

// Коэффициент, Джоулей в килограмме тратила
const float JouleInTNT = 4.18E6f;

// тип взрыва
enum ExplosionType
{
	etDefault,
	etNapalm,
	etVolume,
	etPlane,
	etHelicopter,
    etLightEffect, /// легкий эффект для снарядов
};

// среда взрыва (для оптимизации)
enum ExplosionEnv
{
	eeAuto,		// автоопределение по координате
	eeGround,
	eeAir,
	eeWater,
	eeShallow,
	eeObject,	// если попали в объект - координаты взрыва в его ЛСК
};

// флаги - что делать в функции взрыва (если несколько флагов - через | )
enum ExplosionFlags
{
	efNothing	= 0,
	efVisual	= 2,
	efSound		= 4,
	efCrater	= 8,
	efEffect	= efVisual | efSound | efCrater,

	efDamage	= 16,

	efDefault	= efEffect | efDamage
};

struct INTER_API ExplosionInfo
{
	cPoint p;
	cVector v;
    // power используется для фугасного эффекта, cumulative_power - для повреждения только объекта в который попали
	float power, cumulative_power; 
    float decay_factor_based_on_fragments_mass;
	float visual_power_correction;//костыль для увеличения внешнего эффекта без перенастройки повреждений
	ExplosionType type;
	ObjectID generator;
	ExplosionEnv env;
	ObjectID object;
	bool pierced;
	ExplosionFlags flags;
	ed::string partName;
};

// Менеджер создания и обслуживания взрывов
class INTER_API wExplosionManager
{
public:


	// конструктор/деструктор
//	static wExplosionManager *Create();
	virtual ~wExplosionManager();

	// Функция генерации взрыва
    virtual void Explosion(ExplosionInfo& info, bool do_not_log = false);
	virtual void Explosion(
		const cPoint& p,				// точка взрыва (если env = eeObject то в ЛСК объекта)
		const cVector& v,				// скорость источника взрыва
		float power,					// мощность в кг. тротила
        float cumulative_power,         // Это только объекту передастся
		ExplosionType type = etDefault,	// тип взрыва
		ObjectID generator = 0,			// ID взорвавшегося объекта
		ExplosionEnv env = eeAuto,		// среда в которой произошел взрыв
		ObjectID object = 0,			// если env = eeObject: ID объекта, в который попали
		bool pierced = false,			// броня объекта object уже пробита болванкой		
		unsigned flags = efDefault,		// флаги функциональности (ExplosionFlags)
		const ed::string &partName = " ",		// subshape name
        float decayFactor = 1.0f        // коэффициент затухания силы взрыва с расстоянием
		);
};

extern INTER_API wExplosionManager* globalExplosions;
