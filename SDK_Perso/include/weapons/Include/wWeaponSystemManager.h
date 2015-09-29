//                    file wWeaponSystemManager.h
#pragma once

#include "ed/sstream.h"

#include "Weapons.h"
#include "wWeaponUtils.h"
#include "../../Effects/ParticleSystem/inc/ParticleApi.h"
#include "wTime.h"
#include "wMovingObject.h"
#include "Randomizer\RandomObject.h"

#include "WorldMail.h"

#include "Lua/Config.h"

#include "dPosition.h"

#include "wArmour_scheme.h"
#include "AI\Detection\wDetector.h"

#include "wFloat.h"

#include "wcontrol.h"

#include "ArgumentAnimation.h"
#include "Movingplatform.h"
#include "wActivityManager.h"
#include "TargetPriorities.h"

#include "WeaponFlagEnum.h"

#include "Sounders/WeaponSounder.h"

#define RANDOM (wWeaponSystemManager::randomEvenly.get())
#define RANDOM_DELAY_01 (RANDOM*.1)
#define RANDOM_DELAY_001 (RANDOM*.01)

struct wsCommonTarget;
class cLauncher;
class wGroupTargetManager;
class wWeaponSystem;
class Box;

enum wsCommonTargetType  // виды и подвиды целей
{ 
    wscTarget_Nothing,	            // Не цель
    wscTarget_Ground,	            // Oбычная наземная цель (мост, дот)
    wscTarget_Air,	                // Воздушная цель
    wscTarget_Plane,				// Самолет или вертолет
    // Виды атаки
    wscTarget_Attack_Air_Target_,   // Атака воздушной цели (и подвид).  
    wscTarget_Bombing_,		        // Бомбежка. 
    wscTarget_Missile_Gr_Attack_,   // Ракетная атака наземной цели ( и подвид) 
    wscTarget_Gun_Gr_Attack_,       // Aтака наземной цели из пушки. 
    wscTarget_Attack_Gr_Target_,    // Атака наземной цели всеми средствами ( и подвид). 
    wscTarget_Rocket_Attack_,       // Атака неуправляемями реактивными снарядами. 
    wscTarget_Antiradar_,           // Antiradar (подвид)
    wscTarget_Antiship_Strike_,     // Корабль   (подвид)
    wscTarget_Close_Air_Support_,   // Close_Air_Support 
    wscTarget_Ground_Attack_,       // Ground_Attack
    wscTarget_Pinpoint_Strike_,     // Атака наземной цели точным оружием
    wscTarget_Runway_Attack_,       // Runway_Attack (подвид)
    wscTarget_Tank,                 // Танк (защищенная наземная цель)(подвид)
    wscTarget_Point,

    // Виды целей
    wscTarget_GroundTarget,	        // Вид - наземная цель
    wscTarget_AirTarget,	        // Вид - воздушная цель

    wscTarget_AirPlane,	            // Вид - самолет/вертолет
    wscTarget_Weapon,	            // Вид - мелкое оружие (ракета/бомба/снаряд)
    wscTarget_Missile,              // Вид - крупная ракета
    wscTarget_Helicopter            // Вид - вертолет
};

enum EmissionMode // режимы излучения
{
    emNone  = 0x0000,
    emScan  = 0x0001,
    emTWS   = 0x0002,
    emLock  = 0x0004,
    emLaser = 0x0008
};
enum EmissionStates
{
    esNA        = -1,
    esDisabled  = 0,
    esEnabled   = 1,
    esAlwaysOn  = 2
};
enum RearmingErrors
{
    rearmerr_Ok,
    rearmerr_NoWarehouseFound,
    rearmerr_IsRearming,
    rearmerr_hasImportantTarget,
    rearmerr_TargetAssigned,
    rearmerr_IsMoving,
    rearmerr_NoNeed
};

namespace Graphics { class Model; }
namespace PSEffects {class EffectHolder;}

struct WEAPONS_API wsTarget 
{
    unsigned int	    T_ID;       // Номер цели в мире
    woPointer			pTarget;
    wsCommonTargetType  T_Type;     // Вид цели (наземная/воздушная)
    wsCommonTargetType  T_SubType;  // Подвид цели
    double              radius;
    ObjectID            master;     // используется при внешнем прицеливании, например пусковые С-300 от трек-радара
    char			    master_ws;  // номер WS в машинке

    wsTarget();
    wsTarget(const wsTarget& arg);
    wsTarget(const wsCommonTarget& arg);
    bool operator<(wsTarget &) const;
    void setDiffInitStatus(Mail::Reader& stream);
    void getDiffInitStatus(Mail::Writer& stream);
};

struct wsNetStatus
{
	char wsId;
	float trgAngleY;
	float trgAngleZ;
    ObjectID manual_targetID;
};
typedef ed::vector<wsNetStatus> WeaponStatusVec;

class WEAPONS_API wDamageArea
{
public:
	wDamageArea();
    virtual ~wDamageArea();
	ed::string area_name;
	int arg; 
	double life;
	double life0;

    cPoint firePos;
    float fireSize;
    Mem::Ptr<PSEffects::EffectHolder> areaFire;
};

// class WEAPONS_API wDamageClient // объект принимает повреждения и связывает их с изменениями в модели
// {
// public:
// 	ed::vector<wDamageArea> areas;
// 	virtual bool update_args();
// 	virtual bool process_damage(const ed::string& area_, double ammount_ );
// };

class WEAPONS_API wSmokeManager
{
private:
	ed::vector<ed::string> smoke_con; //mortar connectors
	int charges_left;
	woPointer host; // chassis
public:
	wSmokeManager();
	virtual ~wSmokeManager();
	void	init(Lua::Config& c, woPointer& host);
	bool popup_next();
	int		get_n() const {return charges_left;};

	// MAIL {
	void	getDiffInitStatus(Mail::Writer& stream);
	void	setDiffInitStatus(Mail::Reader& stream);
	// } MAIL
};

typedef ed::set<ObjectID> potential_targets;
typedef	ed::map<ObjectID, wsCommonTarget> known_targets;
typedef known_targets::iterator known_targets_itr;
typedef known_targets::const_iterator known_targets_citr;

class WEAPONS_API wWeaponSystemManager: 
	public wArmour_scheme, 
	public wDetector //Обнаружитель целей
{
    friend class wWeaponSystem;
public:
    static RandomObject randomEvenly;
    static RandomObject randomOrderly;

	typedef char SystemID;

	wWeaponSystemManager();
	virtual ~wWeaponSystemManager();

	virtual void PrintTrace();	// Печать текущего состояния объекта в wTime

	virtual void			addSystem(wWeaponSystem* system);
	virtual void			removeSystem(wWeaponSystem* system);
	wWeaponSystem* wWeaponSystemManager::get_base(int id_);

	int						getSkill() const { return skill; }
	virtual bool			allowAssign() const; 
	virtual bool			allowShoot() const; 
	bool			        isHuman() const { return human;}
    bool                    IsTargetAssigned() const;
	bool                    IsTargetAssigned(ObjectID targetID_) const;
	virtual	bool			accept_point_target() const;

    virtual bool            isTargetInBeam(const woPointer&) const; // попадает ли цель в раструб облучения

	void					set_manager(wGroupTargetManager* manager_) {group_manager = manager_;};
	virtual bool            IsDepends()const;
	virtual bool            IsDependsOn(const ed::string& unit_type_ )const;
    virtual bool            IsDependsOnExternal()const; // for ABRIS code
	wWeaponSystem*			getSystem(SystemID systemID);
	Box*					getCmdBox(viObjectShape *pObject);

	virtual bool			assign_target(wsCommonTarget& target_,const wWeaponSystemManager* trk_ = NULL, WeaponFlags allowedWeaponTypeFlags = ArmWeapon);

	virtual const wWeaponSystem* is_TR_tracking_target(SystemID ws_, ObjectID TargetID) const;
	virtual int             number_of_tracking_missiles(ObjectID TargetID) const;
	virtual int             max_number_of_tracking_missiles(ObjectID TargetID) const;
    virtual int             GetNumberOfAttackersWS(int targetID_, wsystem_type wstype_) const;

	virtual double			getRangeFindingTarget()const{return rangeFindingTarget;}
    virtual double          getRangeFindingTargetRaw() const {return rangeFindingTargetRaw;}
    virtual float           getManagerSensorHeight()const{return sensorHeight;}

	virtual void            setActive(bool active_) {active=active_;}; // активность относительно состава колонны, зависимость от радаров
	virtual bool            getActive() const {return active;}; // активность относительно состава колонны, зависимость от радаров
	virtual bool            can_child_cancel(int wsID_);
	virtual void            cancel_base(int wsID_);
	virtual void            cancel(); // безусловная отмена стрельбы по цели

	// ручное управление
	virtual void			CommandSet(int command);
	virtual void			CommandSet(int command, float value);

	// отдача
	virtual void			recoil(int, const cVector& power, const cVector& pnt) = 0;
	virtual float           GetMaximumWeaponDistance();
	virtual float			GetMaximumWeaponHeight();

	virtual bool            master_arm_on();
	virtual bool            master_arm_off();
	virtual bool            is_master_arm_on()const{return true;};
    virtual bool            is_master_arm_off()const{return false;};

	virtual void	        startedRocketHasDied(ObjectID RocketID_);

	unsigned char	        getRadarType()const{return Radar_Type;}
	woPointer				host; // chassis

	virtual void	        formDrawArgument(){};

	virtual void            sensors_on();
	virtual void            sensors_off();

	virtual double          getArtilleryShellMaxHeight(ObjectID targetId) const;

	virtual void			process_damage(const ed::string& area_, double life );

	virtual void			add_target(wsCommonTarget& unit_);
	virtual void			remove_target(ObjectID ID_);

	virtual void			updateTacticalSpeed(wModelTime& timeStep);

	virtual void			setAI_ON();
	virtual void			setAI_OFF();

	// temporarily public
	virtual int				sizeSystem() const;
	virtual wWeaponSystem*  Item(int index)const;
	virtual wGroupTargetManager* get_group_manager()const{ return group_manager; };
    virtual MovingPlatform* getPlatform()const{assert(platform); return platform;};
	//////////////////////////////////////////////////////////////////////////
    void                    clean_up_obsolete_targets();
    void                    validatePointerOnObjectID(ObjectID ID_, woPointer woPnt); // related to player unit switch
    bool                    check_potentialTarget(woPointer& pTarget);
    void                    remove_potentialTarget(const ObjectID trgID);
    void                    findTargets(wModelTime &timeStep);
    bool                    isInKnownTargetsList(const ObjectID trgID) const;
    inline const wModelTime getFindTargetsDelay() const {return findTargetsDelay;};
	float                   getTargetPriority(wsCommonTarget& common_target, bool search);
    void		            checkShooter(woPointer& target_ptr, wsCommonTarget& common_target, const wsType & weaponType, void * weaponData, ObjectID targetID);

    virtual bool            is_area_alive(const ed::string& dm_area) const;
    
    // for manual HUD
	const Ammo &			getAmmo() const;
    void                    GetAmmoCount(ammo_t & ammo) const; 
    void                    GetActiveLNAmmoCount1(ammo_t & ammo, int* readyState) const; 
    void                    GetActiveLNAmmoCount2(ammo_t & ammo) const; 
    bool                    IsLockedAndReady() const;
    bool                    is_in_firePermittedAngles(int* l_r, int* u_d) const;

    virtual inline bool     can_manual() const { return manualWS != 0; };
    virtual inline bool     getManual() const { return manual; };
    virtual inline wWeaponSystem* getManualWS() const { return manualWS; };
    virtual inline void     setManual(bool manual_);
            inline bool     isHumanWeaponController() const; // управляет ли игрок оружием с этого компьютера (сеть)
            inline bool     isWeaponHumanControlled() const; // управляется ли оружие юнита игроком (сеть)
    virtual void            humanWeaponControlTake( int controllerHostID);
    virtual void            humanWeaponControlRelease();

    virtual const ed::string&    getCockpitName() const;
    virtual woPointer       getManualTarget() const;
    virtual bool            getManualTargetAim(const  dPosition & pos, dVector& r);
    const float             getTargetPriority(const woPointer& target_ptr);

    virtual void            we_launch(const wsType&, const double next_launch_delay_ = 0); // register launch
    virtual bool            can_we_launch(const wsType&); // return time to next launch

    int                     getGearNum() const;
    virtual void            get_swings(double& swing_p, double& swing_v, double& swing_r) const;

    ////// FOR F10 view only, don't use it for AI code
    virtual float           getKillingDistance()const{return threatRange;} //used for F10 circles only!
    virtual float           getKillingDistanceMin()const{return threatRangeMin;} //used for F10 circles only!
    virtual float		    getFindingDistance()const{return detectionRange;} // used for F10 circles only!
    /////

	virtual void			setReconMode(bool on);
    const unsigned          getEmissionModeForTarget(woPointer const target) const;
    virtual void            setEmissionEnabled(bool enabled_);
    inline const bool       isEmissionEnabled() const { return emissionEnabled >= esEnabled; }
    inline const short      getEmissionState() const { return emissionEnabled;}
    inline bool             isScanningWithRadar() const { return isEmissionEnabled() && (emissionMode & emScan); }
    virtual	void			updateDrawArguments(wModelTime t);

	bool					getFireOnMarch() const { return fire_on_march; }

protected:					
	mutable int				depends; // unit has launcher which takes targeting data from another unit
    mutable int				dependsOnExternal; // unit has launcher which takes targeting data from another unit
	bool				    human;
	bool                    active; // активность относительно состава колонны, зависимость от радаров

    //table values
    unsigned char		    Radar_Type;	// Константa для сообщений об облучении
	int						radar_rotation_type; // 0 - always, 1 - after masterarm_on
	double					rangeFindingTarget;	
    double                  rangeFindingTargetRaw;
    double					rangeFilteringTargets;
    unsigned                emissionMode; // enum EmissionMode
    EmissionStates          emissionEnabled;

    // FOR F10 view only, don't use it for AI code
    double                  threatRange;
    double                  threatRangeMin;
    double                  detectionRange;
    ///// 

    bool                    fire_on_march; // приводить ли в боевое состояние на марше
    float                   sensorHeight;

    wWeaponSystem*          manualWS;

	wSmokeManager			smoke_manager;
	wGroupTargetManager*	group_manager;
    MovingPlatform*         platform;

    typedef wActivity<wWeaponSystemManager> wActivityWeaponSystemManager;
    wActivityManager	    activities;
    wtPointer               activity_findTargets;
    wtPointer               activity_updateTacticalSpeed;
    wModelTime              findTargetsDelay;

    int					    humanWeaponControlHostId;//id клиента управляющего оружием

	virtual void			deleteSystems();
	virtual void			update_systems_by_damage(const ed::string& dead_area_ );
	virtual void			controlSystems(); // polling высокой частоты
    virtual	void			setSkill(int skill_);
	virtual bool			isExternalLavaliere() {return externalLavaliere;}

	virtual bool			assign_target_to_ws(wsCommonTarget& target_, wWeaponSystem* ws_, float priority, const wWeaponSystemManager* trk_ = NULL, WeaponFlags allowedWeaponTypeFlags = ArmWeapon);
    virtual RadarMode		isRadiatedByRadar(ObjectID targetID, float & radar_detection_distance) const;

    virtual dPosition       getCameraViewPoint(bool);

    potential_targets       potentialTargets;
	potential_targets::iterator curPotentialTarget;
    void                    initialSearchForPotentialTargets();
    inline const bool       isInPotentialTargetsList(const ObjectID trgID) const
    {
        return potentialTargets.find(trgID) != potentialTargets.end();
    };
    static bool             search_hanler(viObject *item, viSearch *search, void* data);

    known_targets			targets;
    WSTargetPriorityMap     targets_priorities;
    const float             pickTargetPriorityForWS(const unsigned int targetID, const unsigned int wsID);

    void                    update_target_priority(const unsigned int targetID, float priority_, unsigned int wsID);
    bool                    getCommonTarget(const ObjectID ID_, wsCommonTarget& trg);
    void                    after_remove_target(ObjectID ID_);
    void                    clear_targets();

    virtual int             RearmingAllowed() const;
    void                    onWeaponSystemRearming();
    void                    onWeaponSystemRearmed();

	typedef					ed::vector<wWeaponSystem*> Systems;
	Systems					systems;
    int                     weaponSystemsRearming;
	int						skill;
    int                     skill_for_bot;
    bool                    botAI_ON; // состояние ИИ бота может изменить триггер в момент нахождения юнита под управлением игрока
	bool					externalLavaliere;
	bool					reconMode;

	typedef					ed::vector<wDamageArea> t_DamageAreas;
	t_DamageAreas			damage_areas;
	ArgumentAnimation		anmAlarmState;		// приведение в боеготовность
	mutable Ammo			ammo;

    // ручное управление
	bool                    manual;
	//void nextManualLauncher();

    wModelTime              next_launch;

	void					setHost(woPointer host_)	{ host = host_; platform = dynamic_cast<MovingPlatform*>((MovingObject*)host); };
    bool                    add_weapon_system( Lua::Config& c, int i);
#ifdef DEBUG_TEXT
    virtual void            debug_text(ed::ostringstream& out);
#endif

    virtual void            formDrawArgument_sim() = 0;
    virtual void            switchToCockpit(const ed::string& cockpitName);
public:

	// panov: {
	const t_DamageAreas *getDamageAreas() { return &damage_areas; }
	
	size_t getDamageAreaSize() { return damage_areas.size(); }
	const wDamageArea *getDamageAreaIdx( int nIdx ) { 
		if( damage_areas.size() < nIdx+1 )
			return NULL;
		return &damage_areas[nIdx]; 
	}

	const wDamageArea *getDamageAreaMax() {
		if( damage_areas.size() )
			return &damage_areas[0];
		return NULL;
	}
	const wDamageArea *getDamageAreaMin() {
		if( damage_areas.size() > 1 )
			return &damage_areas[1];
		return NULL;
	}
	// panov: }

// MAIL {
	virtual void	        getDiffInitStatus(Mail::Writer& stream);
	virtual void	        setDiffInitStatus(Mail::Reader& stream);
// } MAIL

	void	getWeaponStatus(WeaponStatusVec& status);
	void	setWeaponStatus(const WeaponStatusVec& status);

    // init
    bool                    add_weapon_system_manager(Lua::Config& c);

    // SOUND {
    void sound_update();

	WeaponSounder* getSounder();
    // } SOUND

    int                     rearm_all();
    bool                    is_rearming() const;
    void                    adjustLaunchersAimingTime(float relLife);

private:
	WeaponSounder* sounder;
};
