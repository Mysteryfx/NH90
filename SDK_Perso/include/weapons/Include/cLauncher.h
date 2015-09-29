//                    file cLauncher.h
#pragma once

#include "ed/sstream.h"
#include <cAlgorithm.h>

#include "Weapons.h"
#include "wWeaponUtils.h"

#include "dPosition.h"
#include "mathDebug.h"
#include "wFloat.h"

#include "assert.h"

#include "wBarrel.h"
#include "wWeaponSystem.h"
#include "wMovingObject.h"
#include "wActivity.h"
#include "WorldSound.h"

#include "MovingPlatform.h" // для использования movingplatform
#include "cReloadingState.h"

#include "BC_InputData.h"

#define IID_ILauncher Common::identify<cLauncher>() 

class wStowage;
struct FireMsgData;

class WEAPONS_API cLauncher: public Common::FakeUnknown<Serializable> 
{
    friend class wWeaponSystemManager;
    friend class wWeaponSystem;
    friend class wWeaponSystemMail;
    friend class wBarrel;
    friend class cReloadingStateOrdinary;
    friend class cReloadingStateSimultaneously;
    friend class cReloadingStateSequentialy;

public:
    typedef wWeaponSystemManager::SystemID SystemID;
    typedef	ed::vector<wStowage*> Stowages;
    typedef	ed::vector<wBarrel*> Barrels;
    typedef unsigned char LauncherID;

    typedef				wActivity<cLauncher> wActivityLauncher;
    wActivityManager	activities;
    wtPointer           activity_fire;
    wtPointer           activity_releaseBarrelsLock;
	wtPointer			activity_sound;

    cLauncher();
    virtual~cLauncher();

    virtual inline wsystem_type getType() const { assert(type>wsType_Ship_ERROR && type<=wsType_Ship_LAST); return type;}

    virtual bool		preInit(woPointer& chassis_,wWeaponSystem* theSystem_, Lua::Config& c, wsystem_type type_, int id_);
    virtual void		postInit();

    virtual bool		maintain(); // polling низкой частоты
    virtual void        control_fire(wModelTime& timeStep); // активность стрельбы, стартует по мере необходимости
    virtual void        releaseBarrelsLock(wModelTime& timeStep); // разлочивание стволов для возможности использования их другими лаунчерами

	virtual void		barrelFire(const FireMsgData& fmd);
	int                 get_loaded_barrels_num() const;
    
	virtual bool        fire(){return false;}; // стреляет и выбирает ствол, из которого будет выполняться следующий выстрел
    virtual bool		fireStart(){return true;}; // сетевая стрельба
    virtual bool		fireStop(){return true;};
    virtual void	    net_fireStop(int actualAmmoCapacityInActiveStowage);// сетевая стрельба
    virtual void        aim();
	void                rotateToReloadPosition();

    virtual void        validatePointerOnObjectID(ObjectID ID_, woPointer woPnt); // related to player unit switch
    virtual bool		assignTargetEx(const wsTarget& target_);
    virtual	void		cancelAssign();

    virtual void		net_assignTargetEx(const wsTarget& target_);
    virtual	void		net_cancelAssign();

    LauncherID			getIdLauncher() const {return idLauncherInHost;};
    virtual ObjectID    getTargetID() const;
    virtual	void		getZone(float& hMin, float& hMax, float& dMin, float& dMax);
    virtual	bool		launcherEscortTarget() const {return false;}

    virtual bool        track_missile(ObjectID missileID_, ObjectID targetID_) {return false;};

    virtual cPosition&	Position(wModelTime t) const;
    virtual cPosition&	Position() const;
    virtual void		getPositionCMDBarrel( IN wBarrel* barrel_, IN bool global_, OUT dPosition& position_);
    virtual void		getPosBarrel( IN wBarrel* barrel_, IN bool global_, OUT dPosition& position_);
    virtual	float		ScalarVelocity() const {return host->ScalarVelocity();}
    virtual const cVector&	VectorVelocity() const {return host->VectorVelocity();}

    virtual void		serialize(Serializer &);

    wWeaponSystem*	    getSystem() const { return system; }

    //фабрики 
    virtual void		addStowage(wStowage* stowage);
    virtual void        addBarrel(wBarrel* barrel_);
	wBarrel*			findBarrel(wBarrel::BarrelID);
    
    // Rearming/Reloading
    virtual const bool  is_reloading() const;
    void                collectBarrelsLoaded(Mail::Writer writer_);
    void                net_reloadingStart(Mail::Reader reader);
    void                net_reloadingMaintain(Mail::Reader reader_);
    void                net_reloadingFinish(int activeStowageID, int actualAmmoCapacity);
    bool                canDelayedReloadingBePerformed() const;
    virtual bool        needToBeRearmed() const;

    virtual const int   get_skill() const {return skill;};
    virtual woPointer	get_host() { return host;};
    virtual	bool        good_to_attack( const woPointer& pnewTarget_, float& trg_priority, const wWeaponSystem* trk_ = NULL, WeaponFlags allowedWeaponTypeFlags = ArmWeapon);
    virtual bool        lockBarrels(const wModelTime lockPeriod_ = 0.0); // лочим стволы, дабы не использовались одновременно разными лаунчерами
    virtual bool        unlockBarrels();
    virtual float       getNextShotDelay() const;

    virtual void		startedRocketHasDied(ObjectID){};
    virtual int         number_of_tracking_missiles()const{return 0;};
    virtual int         number_of_tracking_missiles(ObjectID target_)const{return 0;};
    virtual int         max_number_of_tracking_missiles()const{return 1;};
    virtual int         max_number_of_tracking_missiles_conditioned()const {return 1;};
    float               get_reflection_limit()const{return reflection_limit;};

    virtual double      getArtilleryShellMaxHeight(ObjectID targetId) const {return 0;};
    virtual double      getArtilleryDistByAngle(const double& angle_) const {return 0;};
    virtual double      getArtilleryAngleByDist(const double& dist_) {return 0;};

    virtual bool        IsDepends() const {return depends_on.size();};
    virtual bool        IsDependsOnExternal() const;
    virtual bool		IsDependsOn(const ed::string& unit_type_)const;

    virtual double		getMaxTacticalSpeed();
    virtual void        select_new_stowage();
    virtual void        select_stowage(int stowageNum);

    virtual bool        check_local_slaves() const;

    ed::vector<SystemID> local_slave_systems;

    virtual void        setIDS(IDs&) {return;};

    virtual wsType      getAmmunition_wsType() const { return wsType(255); };
	virtual void		getAmmo(Ammo & ammo) const;
    virtual void        GetAmmoCount(ammo_t & ammo) const;  // всё, для вывода на F10
    virtual void        getBarrelReadyState(std::pair<int,ed::string>& p, int* readyState) const;
    virtual void        GetAmmoCount1(ammo_t & ammo, int* readyState) const;  // только активный stowage
    virtual void        GetAmmoCount2(ammo_t & ammo) const;  // всё, кроме активного stowage
            bool        IsMyTargetInRange() const; // для отображения вхождения цели в зону поражения (в кабине)

    virtual dPosition   getCameraViewPoint(bool);
    virtual bool        can_manual() const;
    virtual const ed::string& getCockpitName() const {return cockpitName;};
    const unsigned char getSightMasterMode() const;
    virtual bool        getManualTargetAim(const  dPosition & pos, dVector& r);

    virtual inline void setManual(bool manual_);

    virtual const float getPriorityByType(const woPointer& pTarget_) const;
    virtual const float getPriorityByPosition(const woPointer& target_ptr) const;
    virtual const float getTargetPriority(const woPointer& trg_ptr_);
    const float         getCurrentTargetPriority() const;

    virtual bool        IsLockedAndReady() const; 

    // manual mode
    virtual void        lockoff();
    virtual woPointer   lockon() { return lockon(false, 1.0); };
    woPointer           lockon(bool ignoreBeamWidth, double ECM_K);
    virtual bool        check_lock(woPointer obj);
    bool                check_lock(woPointer obj, double ECM_K);
    virtual void        selectFeedSlot(const unsigned int slotID);

    inline int          getReloadingType() const { return reloadingState ? reloadingState->getReloadingType() : 0; }

	// panov {
	// время оставшееся до следующего выстрела (без времени перезарядки орудия)
	virtual wModelTime	getNextShot() const { return (m_wTimeStep + m_wTimeShot - wTime::GetModelTime());  }
	// panov }

    virtual inline const bool  hasAutomaticLoader() const { return false; };

protected:
    // SOUND {
    ed::string fnameCycleShot;
    ed::string fnameSingleShot;
    ed::string fnameEndBurst;
	ed::string fnameStartReload;
	ed::string fnameEndReload;

	// panov {
	wModelTime			m_wTimeStep; // Задержка до выстрела
	wModelTime			m_wTimeShot; // Время выстрела
	// panov }

    struct snd {
        Sound::Source single_shot;
        Sound::Source cycle_shot;
        Sound::Source end_burst;
		Sound::Source start_reload;
		Sound::Source end_reload;
    } _snd;

    virtual void sound_load(Lua::Config &conf);
    virtual void sound_init();
    virtual void sound_uninit();
    virtual void sound_update();

	// Звук перезарядки
	void reloadSoundUpdate(wModelTime& timeStep);

	wModelTime			fullTimeToReloadAfrerFire;
	wModelTime			firstReloadSoundTime;
	wModelTime			secondReloadSondTime;
	bool				needPlayFirst;
	bool				needPlaySecond;
	// Звук перезарядки
    // } SOUND

    typedef struct { ed::string type; wWeaponSystem::SystemID ws_id; } depends_atom_t; // 
    typedef ed::vector<depends_atom_t> depend_AND_vector_t;
    typedef ed::vector<depend_AND_vector_t> depend_OR_vector_t;
    depend_OR_vector_t depends_on;

    void                net_pTargetInit(); // получение pTarget из tmp_targetID для сетевых клиентов после старта мира

    virtual wModelTime  get_aiming_delay()const {return 0.5;}; // поллинг цикла стрельбы
    virtual void		uninitLauncher();
    virtual wStowage*	findOnID(wStowage::StowageID id);
    virtual int			calculateAimingParameters(const dPosition & mypos_, const woPointer& target_, dVector& r);	// 0 - ok, 1... - error code
    virtual bool        updateAimingCorrections(double dist, double& azimuthCorr, double& elevCorr);
    virtual bool        calculateAimingCorrections(double dist, double& azimuthCorr, double& elevCorr) { return true; };
    virtual bool        calculateAimingCorrections(const BC_InputData& data, double& azimuthCorr, double& elevCorr) { return true; };
    virtual int         check_trackers();

    virtual	int		    conditionsOfShootingAreExecuted();
    virtual	int	        conditionsOfTrackingAreExecuted(wBarrel::BarrelID idBarrel_ = 0) const { return 0; }

    dPosition           get_noised_target_pos(const dPosition& host_pos_, const dPosition& t_pos_) const; // получить позицию цели с учетом ошибок
    cVector             get_noised_target_vel(const cVector& vel_) const;

    virtual	bool        check_dependences( ObjectID targetID_, const wWeaponSystem* trk_ = NULL) const;
    const wWeaponSystem* get_suitable_trk( ObjectID targetID_, const wWeaponSystemManager* trk_) const;

    virtual double      get_max_acquiring_distance() const {return distanceMax*2.;};
    void                calculate_new_deviations(bool renew_ = false);
    //void                calculate_sight_deviations(double& sight_dev_y, double& sight_dev_z, bool init_ = false);

	void                reload_barrels();
    void                onReloadPosition();
    void                releaseReloadPosition();
    void                selectActiveBarrel();

    void                mailReloadingStart();
    void                mailReloadingMaintain(std::queue<int>& reloadedBarrels_);
    void                mailReloadingFinish();

    Stowages			stowages;
    unsigned int        currentFeedSlotID; // слот питания боеприпасов (Для пушек имеющих возможность выбора типа боеприпаса. Обычно это ББ и ОФ)
    unsigned int        maxFeedSlotID; //  ID крайнего слота питания
    Barrels				barrels;
    Barrels::iterator   activeBarrel; // очередной ствол для выстрела
    wWeaponSystem*	    system; // chassis or another turret
    MovingObject*	    host; // chassis
    MovingPlatform*     platform; // interface to moving platform
    cReloadingState*    reloadingState;

    // table values, consts
    float			    deviationAngleMax;
    int                 skill;
    LauncherID			idLauncherInHost;
    float				xc;
    wsystem_type		type;
    int					fireAnimationArgument;
    float				distanceMin; 
    float				distanceMax; 
    float				heightMin;
    float				heightMax;
    wModelTime			reactionTime;		// Время прицеливания после переноса огня на новую цель.
    bool                external_tracking_awacs;
    bool                show_external_missile;
    float               reflection_limit; // ограничение по отражаемой поверхности цели
    bool                connectorFire; // стрельба из коннектора модели
    double              platform_max_speed;
    int                 barrelsReloadType; // тип перезарядки стволов (1 - обычный режим, 2 - все разом, когда все пусты,...)
    float               beamWidth;
    float               ECM_K; // коэффициент для максимальной дальности захвата при работе по цели с включенными помехами

    dPosition		    viewPoint; // для action-камеры
    ed::string		    cockpitName; // для action-камеры

    unsigned char       sightMasterMode; // номер основного режима прицела связанного с WS

    Graphics::ModelConnector* connectorPointer; // коннектор первого барреля, если есть, для камеры.

    // разброс ошибок сенсоров  
    double              deviation_error_azimuth;
    double              deviation_error_elevation;
    double              deviation_error_speed_sensor;
    double              deviation_error_stability;
    double              deviation_error_distance;

    // ошибки выверки прицела (для управляемых систем)
    // обновляются после каждого выстрела для крупных калибров или после каждой очереди для мелких
    double              sight_deviation_y;
    double              sight_deviation_z;

    // run-time variables
    double				angleYRequired;
    double				angleZRequired;
    //bool                reloading;
	wModelTime			last_successful_aiming;
    wModelTime			last_AimingDynamicParametersCheck;
    int					currentStowageNum;
    int                 firstStowageForCurrentFeedSlot;
    int                 last_check_result;

    // сгенерированые текущие ошибки
    double              base_multiplayer; // базовый множитель ошибок, уменьшается при пристрелке
    double              targeting_v_deviation; // ошибки по скорости скаляр
    double              targeting_vx_deviation; // ошибки по скорости X
    double              targeting_vy_deviation; // ошибки по скорости Y
    double              targeting_x_deviation; // ошибки по азимуту
    double              targeting_z_deviation; // ошибки по углу места
    double              targeting_d_deviation; // ошибки по определению расстояния до цели
    woPointer           pTarget;

    // стрельба начнется после aimingTime+newTargetTime
	wModelTime			newTargetTime; // время прицеливания для текущей цели
    wModelTime			aimingTime; // глобальное время начала прицеливания

	wModelTime		    fireTime; // глобальное время крайнего выстрела, для отработки пламени из ствола.

    // для организации зависимости между системами
    ObjectID            trackerID; // host ID
	char				tracker_ws; // host WS ID

    int                 fire_solution_err; // номер последней ошибки прицеливая, если 0 то ошибки нет, проверяются условия для стрельбы

    bool                manual; // true when operation under manual control

    // кэшированные позиции
    dPosition           cache_pos;
    wModelTime          cache_postime;

    dVector             cache_ManualTargetAim;
    wModelTime          cache_ManualTargetAimtime;


	//////////////////////////////////////////////////////////////////////////
	// for target velocity vector calculation
	cVector             update_true_target_vel(const dVector& pos_);
	dVector				prv_trg_pos; 
	wModelTime			prv_trg_pos_time; // for target velocity vector calculation
	dVector				prv_trg_vel; // for target velocity vector calculation
	//////////////////////////////////////////////////////////////////////////

    virtual bool        checkTrgDistanceReal(const woPointer& trg_) const;
    virtual bool        checkTrgDistancePossible(const woPointer& trg_) const;
    virtual bool        checkTrgAltitude(const woPointer& trg_) const;
    virtual bool        checkTrgAngleReal(const woPointer& trg_) const;
    virtual bool        checkTrgAnglePossible(const woPointer& trg_) const;
    virtual bool        checkTrgSpeed(const woPointer& trg_) const;
    virtual bool        checkTrgReflectionLimit(const woPointer& trg_) const; 
    virtual bool        checkTrgVisibility(const woPointer& pTarget_) const;
    virtual bool        checkAimingDeviation(const double distance_) const;
    virtual double      getDeviation() const;

    virtual float       getBeamWidth() const {return beamWidth;}; // для радаров ширина луча
    virtual bool        isTargetInBeam(const dVector&) const;
    virtual bool        informTargetIfInRRCBeam(const dVector&, woPointer) const {return false;};

    virtual int			checkAimingStaticParameters(const woPointer& trg_) const;	// проверяются неизменные параметры цели, вызывается только при назначении цели, 0 - ok, 1,2,3... - error codes
    virtual int			checkAimingDynamicParameters(const woPointer& trg_, const wWeaponSystem* trk_ ) const;	// проверяются динамические параметры цели, вызывается при прицеливании, 0 - ok, 1,2,3... - error codes

    virtual bool        IsIndirect()const{return false;};

#ifdef DEBUG_TEXT
    virtual void        debug_text(ed::ostringstream& out);
#endif
#ifdef DEBUG_GEOMETRY_ENABLED
    virtual void                DrawTrackerDirector() {};
#endif
    ObjectID            tmp_targetID; // для сетевого целеназначения
    ObjectID            targetID;
    float               currentTargetPriority;

    Vector3             vWind; // мировой ветер, обновляется и запоминается при назначении цели

    IDs				    idTargetAndMissiles;
    dVector             isoview_offset;

public:
    void                updateDrawArguments(wModelTime t) const;
// MAIL {
	virtual void	    getDiffInitStatus(Mail::Writer& stream);
	virtual void	    setDiffInitStatus(Mail::Reader& stream);
// } MAIL

	// panov: {
	virtual wModelTime	getStartFireTime( ) { return aimingTime+newTargetTime; }		// Время наведения
	virtual wModelTime	getDelayTimeActiveBarrel() { return this->getNextShotDelay(); }	// Время (счетчик) перезарядки текущего ствола
	virtual wModelTime	getTimeReadyActiveBarrel() {
		if( activeBarrel != barrels.end() )
			return (*activeBarrel)->getTimeReady(); 
		else
			return -1;
	}
	/*
	int					getBarrelsSize() { return barrels.size(); }
	wBarrel*			getBarrelIdx( int nIdx ) { 
							if( barrels.size() - 1 >= nIdx )
								return barrels[nIdx]; 
							return NULL;
						}
	*/
	// Есть ли текущая цель
	bool				isActivityFire() { 
							if( activity_fire ) {
								wActivityLauncher* pA = dynamic_cast<wActivityLauncher*>(activity_fire.Obj());
								if( pA && pA->host )
									return true;
							}
							return false;
						}
	// panov: }
    virtual void        rearm_all();
    void                interrupt_ammo_maintenance();
    void                onLauncherRearming();
    void                onLauncherRearmed();
    void                adjustAimingTime(float relLife);

private:
	wModelTime	timeToWait; // Время ожидания перед высрелом во время короткой остановки.
	bool delayBeforeFiring; // Флаг необходимости ожидания перед выстрелом.

public:
	double maxTrackingSpeed;
	double maxShootingSpeed;
};