//                    file wWeaponUtils.h
#pragma once

#include "WeaponsBase.h"

#include <complex>
#include <ed/vector.h>

#include "wTime.h"
#include "Lua/State.h"

struct InputEquation3
{
	double a, b, c, d;
};

struct InputEquation4
{
	double a, b, c, d, e;
};

typedef std::complex<double> ComplexDouble;

extern WEAPONSBASE_API void setInaccuracy(ComplexDouble& out, double length);
extern WEAPONSBASE_API bool equation3(InputEquation3& input, ed::vector<ComplexDouble>& output);
extern WEAPONSBASE_API bool equation4(InputEquation4& input, ed::vector<double>& output);

WEAPONSBASE_API void weapdebug_printf(const char* s, ...);

#ifdef USE_WEAP_DEBUG_LOG 
#define WEAP_DEBUG_LOG(args) weapdebug_printf##args
#else
#define WEAP_DEBUG_LOG(args) ((void)0)
#endif


////////////////// 13/1/2006 DMUT ///////////////////////////////////
// типы башен вынесены в отдельный список во избежании путаницы с названиями вооружений
typedef enum wsystem_type 
{
    wsType_Ship_ERROR   = 0, //для диагностики
    wsType_Ship_ASM,    // Противокорабельные ракеты
    wsType_Ship_UG,     // Универсальные орудия
    wsType_Ship_ZA,     // Зенитный автомат
    wsType_Ship_ZMC,    // Зенитный ракетный комплекс
    wsType_Ship_ZMG,    // Зенитный ракетно/пушечный комплекс
    wsType_Howitzer,    // indirect-fire guns
    wsType_Ship_ASM_AG  = 8, // ракеты типа тамагавк, стреляющие по надводным и наземным целям.
    wsType_MG           = 9, // пулеметы (те же автоматические пушки, введено для разделения приоритетов целей)
    wsType_HMG          = 10, // крупнокалиберные(зенитные) пулеметы
    wsType_ATGM         = 33,    // управляемая ракета
    wsType_RSZO         = 34,
    wsType_LastNonRadar = 34, // для фильтрации. при добавлении типа - изменить
    wsType_RadarCC      = 100, // виртуальные радары коммандных центров
    wsType_Radar        = 101,
    wsType_RadarTrack   = 102, // трек-радар работает по воздушным целям
    wsType_RadarTrackEx = 103, // трек-радар работает по наземным, морским и воздушным целям
    wsType_LastRadar    = 103, // для фильтрации. при добавлении типа - изменить
    wsType_Ship_LAST    = 103,   // для диагностики
};

typedef ed::vector<std::pair<int,ed::string>> ammo_t;

extern WEAPONSBASE_API Lua::State global_weapons_config_L;

class MovingObject;
class Vector3;
bool WEAPONSBASE_API checkVisibilityObjects(const Vector3& from_, const Vector3& to_, MovingObject* ofrom_, MovingObject* oto_);
extern WEAPONSBASE_API float round_up(float value_);

