#ifndef __wConst_h__
#define __wConst_h__

enum wConst
{
	// Задачи самолета
	Undefined = -1,
	Default_Flight_,			// Полет по-умолчанию.
	WaitForWingmensTakeoff_,	// Ожидание взлёта ведомых
	Route_,						// Полет по маршруту.
	Departure_,					// Стандартная процедура вылета 
	Arrival_,					// Стандартная процедура прилета (ПМ).
	Air_Refuelling_,			// Дозаправка в воздухе - не танкер
	Go_Around_,					// Уход на 2-й круг 
	Holding_,					// Полет в зоне ожидания
	Destroy_Flight_,			// Полет разрушенного самолета
	Follow_Leader_,				// Полет за лидером.
	Follow_Leader_Safe_,		// Следование за лидером, пока тот атакует
	Holding_Safe_,              // Кружим не месте на безопасном расстоянии от угроз
	Orbit_Around_Point_,        // Кружим вокруг точки
	Circle_Orbit_,              // Кружим вокруг ППМ
	Race_Track_,                // Два по 180 между текущим и следующим ППМ
	Lag_Behind_,				// Отставание для выхода из строя. 

	// Общие задачи
	Intercept_,					// Перехват известной воздушной цели
	Air_Patrol_,				// Воздушное патрулирование
	Attack_Air_Target_2_,		// Атака воздушной цели 2м номером. 
	Air_Refuelling_T_,			// Дозаправка в воздухе - танкер
	AWACS_,
	Nothing_,
	AFAC_,
	Reconnaissance_,
	Escort_,
	Fighter_Sweep_,
	Ground_Alert_Intercept_, 

	Attack_Air_Target_,// Атака воздушной цели.  
	Bombing_,				// Бомбежка. 
	Missile_Gr_Attack_,		// Ракетная атака наземной цели. 
	Gun_Gr_Attack_,			// Aтака наземной цели из пушки. 
	Attack_Gr_Target_,		// Атака наземной цели всеми средствами. 
	Rocket_Attack_,			// Атака неуправляемями реактивными снарядами. 
	Missile_Evasion_,		// Уклонение от ракеты. 
	Plane_Evasion_,			// Уклонение от самолета противника. 

	Antiradar_, 
	Antiship_Strike_, 
	Close_Air_Support_, 
	Ground_Attack_,
	Pinpoint_Strike_,
	Runway_Attack_,
	Transport_,				// Транспортировка: взлет-маршрут-посадка 
	Aerobatic_,				// Акробатика 
	Tank_Attack_ ,			// Атака цели типа танка 
	Reconnaissance_Zone_,	// Доразведка зоны
	Weapon_Evasion_,		// Уклонение от огнестрельного оружия. 

	// Взлет - посадка
	Landing_,			// Посадка.
	Landing_Ship_,			// Посадка на авианосец.
	Landing_Heliport_,		// Посадка на вертолетную площадку.
	Landing_Ground_,		// Посадка на землю.
	Heliport_Refuel_,		// Дозаправка вертолета на вертолетной площадке.
	Take_Off_,				// Взлет.
	Take_Off_Ship_,			// Взлет c авианосца без трамплина.
	Take_Off_Ship_T_,		// Взлет c авианосца c трамплином.
	Take_Off_Heliport_,		// Взлет с вертолетной площадки.
	Take_Off_Ground_,		// Взлет с земли.

	// Наземные задачи	
	Taxi_,					// Рулежка.
	Waiting_,				// Ожидание перед взлетом.  
	Waiting_Adr_,			// Ожидание на аэродроме.  
	Taxi_Ship_,				// Рулежка на плывущем корабле.
	Taxi_ForTO_				// Рулежка на взлет на аэродроме.
};

inline bool isLanding(wConst task) { return task == Landing_ || task == Landing_Ship_ || task == Landing_Heliport_ || task == Heliport_Refuel_ || task == Landing_Ground_; }

inline bool isTakeoff(wConst task) { return task == Take_Off_ || task == Take_Off_Ship_ || task == Take_Off_Ship_T_ || task == Take_Off_Heliport_ || task == Take_Off_Ground_; }

inline bool isTaxing(wConst task) { return task == Taxi_ || task == Taxi_Ship_ || task == Taxi_ForTO_; }

inline bool isWaiting(wConst task) { return task == Waiting_ || task == Waiting_Adr_; }

inline bool isDepartureOrInbound(wConst task)
{
	return	isLanding(task) || isTakeoff(task) || isTaxing(task) || isWaiting(task) ||
			task == Arrival_ || task == Departure_ || task == Holding_ || task == Go_Around_;
}

//минимальное значение радиальной скорости цели для обнаружения радаром
//const float RadialVelocityMin = 30.0f;
const float RadialVelocityMin = 40.0f;
const float RelativVelocityMin = 14.0f;
const float FiveNM = 9260.0f; // 5 морских миль в метрах.

typedef enum ROUTE_TASKS
{	
    PRT_TAKEOFF_FROMRUNWAY	= 13,
	PRT_TAKEOFF_FROMPARKING	= 50,
	PRT_LAND	= 14,
	PRT_ATTACK	= 2,
	PRT_REFUELING	= 8,
	PRT_TURNINGPOINT	= 0,
	PRT_FLYOVER	= 1,
	PRT_FINPOINT = 3,
	PRT_LOCKALTITUDE	= 9,
	PRT_UNLOCKALTITUDE	= 10,
	PRT_BEGINLOOP	= 11,
	PRT_ENDLOOP	= 12,
	SRT_TURNINGPOINT	= 10,
	SRT_ATTACK	= 11,
	CRT_ATTACK_ONROAD	= 10,
	CRT_ATTACK_SHERENGA	= CRT_ATTACK_ONROAD + 1,
	CRT_ATTACK_OFFROAD	= CRT_ATTACK_SHERENGA + 1,
	CRT_ATTACK_CONE	= CRT_ATTACK_OFFROAD + 1,
	CRT_TURNINGPOINT_ONROAD	= 100,
	CRT_TURNINGPOINT_SHERENGA	= CRT_TURNINGPOINT_ONROAD + 1,
	CRT_TURNINGPOINT_OFFROAD	= CRT_TURNINGPOINT_SHERENGA + 1,
	CRT_TURNINGPOINT_CONE	= CRT_TURNINGPOINT_OFFROAD + 1,
} ROUTE_TASKS;

enum COMMANDER_SKILLS
{	
    SKILL_ANY	= -1,
    SKILL_AI_EXCELLENT,
    SKILL_AI_HIGH,
    SKILL_AI_GOOD,
	SKILL_AI_AVERAGE,
	SKILL_AI_MAX,
	SKILL_HUMAN = SKILL_AI_MAX,
	SKILL_CLIENT,
};

enum LandingType
{
	LANDING_TYPE_AIRDROME = 0,
	LANDING_TYPE_SHIP,
	LANDING_TYPE_HELIPORT,
	LANDING_TYPE_GROUND,
	LANDING_TYPE_MAX
};

enum Options {
	OPTION_ROE,
	OPTION_THREAT_REACTION,
	OPTION_RADAR_USING = 3,
	OPTION_FLARE_USING,
	OPTION_FORMATION,
	OPTION_RTB_ON_BINGO,
	OPTION_SILENCE,
	OPTION_DISPERSE_ON_ATTACK,
	OPTION_ALARM_STATE,
	OPTION_RTB_ON_OUT_OF_AMMO,
	OPTION_AWARNESS_LEVEL,
	OPTION_FOLLOWING,
	OPTION_ECM_USING,
};

enum ROEType				{ ROE_WEAPON_FREE, ROE_OPEN_FIRE_WEAPON_FREE, ROE_OPEN_FIRE, ROE_RETURN_FIRE, ROE_WEAPON_HOLD };
enum ReactionOnThreatType	{ THREAT_NO_REACTION, THREAT_PASSIVE_DEFENCE, THREAT_EVADE_FIRE, THREAT_BYPASS_AND_ESCAPE, THREAT_ALLOW_ABORT_MISSION };
enum RadarUsingType			{ RADAR_USING_NEVER, RADAR_USING_FOR_ATTACK_ONLY, RADAR_USING_FOR_SEARCH_IF_REQUIRED, RADAR_USING_FOR_CONTINUOUS_SEARCH };
enum FlareUsingType			{ FLARE_USING_NEVER,  FLARE_USING_AGAINST_FIRED_MISSILE, FLARE_USING_WHEN_FLYING_IN_SAM_WEZ, FLARE_USING_WHEN_FLYING_NEAR_ENEMIES };
enum AwarnessLevel			{ AWARNESS_LEVEL_SAFE, AWARNESS_LEVEL_AWARE , AWARNESS_LEVEL_DANGER };
enum FormationType			{ NO_FORMATION = 0 };
enum GTMAlarmType			{ ALARM_STATE_AUTO, ALARM_STATE_GREEN, 	ALARM_STATE_RED };
enum ECMUsingType			{ ECM_USING_NEVER, ECM_USE_IF_ONLY_LOCK_BY_RADAR, ECM_USE_IF_DETECTED_LOCK_BY_RADAR, ECM_USING_ALWAYS };

enum { MAX_WING_MEMBERS_NUM = 4 };

#endif