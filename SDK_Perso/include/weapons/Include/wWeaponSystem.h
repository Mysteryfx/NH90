//                    file wWeaponSystem.h
#pragma once

#include "ed/sstream.h"

// если определить, при ручном наведении выступающие части будут автоматически обводиться стволом
#define OBSTACLE_AUTO_AVOIDANCE
#include "Weapons.h"
#include "wWeaponUtils.h"
#include "Common.h"
#include "wFloat.h"
#include "wWeaponSystemManager.h"
#include "wStowage.h"
#include "Math/math.h"
#include "wGroupTargetManager.h"
#include "wActivityManager.h"
#include "wDrawActivity.h"
#include "AngleLimits.h"
#include "cLaserRangefinder.h"

class	cLauncher;
struct FireMsgData;

struct wIds
{
    ObjectID	idTarget;
    ObjectID	idRocket;

    void getDiffInitStatus(Mail::Writer& stream)
    {
        stream << idTarget << idRocket;
    };
    void setDiffInitStatus(Mail::Reader& stream)
    {
        stream >> idTarget >> idRocket;
    };
    wIds()
    {
        idTarget = 0;
        idRocket = 0;
    };
};
typedef			ed::vector<wIds> IDs;

class CPID 
{
public:
    double integrator, error;
    bool first_run;
    CPID() : integrator(0), error(0), first_run(true) {};
    void clear() { integrator = 0; error = 0; first_run = true; };
    double step(double err, double dt, double k[3], double limit, double externalCorrection);
};

inline double norma(double angle_)
{
    angle_ = fmod(angle_, 2.0*M_PI);

    if (angle_ < -M_PI)
        angle_ += 2.0*M_PI;
    else if (angle_ > M_PI)
        angle_ -= 2.0*M_PI;
    return angle_;
}

inline double norm_arg(double arg_)
{
    if (arg_ < -1.0)
        arg_ += 2.0;
    else if (arg_ > 1.0)
        arg_ -= 2.0;
    return arg_;
}

typedef void (*voidHandler)(void);

class WEAPONS_API wWeaponSystem
{
    friend class wWeaponSystemManager;
    friend class cLauncher;
    friend class cLauncherAutogun;
    friend class cLauncherGun;
    friend class cLauncherHowitzer;
    friend class cLauncherMissile;
    friend class cLauncherMissileAT;
    friend class cLauncherRadarTrack;
	friend class cLauncherRocket;
public:

    enum CorrectionsApplyingState {
        NOT_AVAIL = 0,
        IN_TRANSITION = 1,
        IN_POSITION = 2
    };
	
	typedef wWeaponSystemManager::SystemID SystemID;
	typedef unsigned char LauncherID;

    wWeaponSystem();
	~wWeaponSystem();
	
	// panov: 31/08/12 {
	/*
	cLauncher*      	getLauncherOnIdx( int nIdx ) const {
							if( launchers.size() - 1 >= nIdx )
								return launchers[nIdx];
							return NULL;
						}
	int      			getLaunchersSize() { return launchers.size(); }
	*/

	bool				getStartFireTime(ObjectID targetId, wModelTime &wTimeStart) const; // оставшееся время наведеня
	bool				getNextShot(ObjectID targetId, wModelTime &wTimeShot) const; // оставшееся время до выстрела следующего снаряда.
	// panov: }

    void		        preInit(wWeaponSystemManager *pManager, woPointer host_, Lua::Config& c, int id_);

    void		        postInit();

    void                PrintTrace();	// Печать текущего состояния объекта в wTime
	
    cPosition&      	Position(wModelTime t) const;
    cPosition&	        Position() const;
    void		        control(wModelTime& timeStep);
    void		        maintain(wModelTime& timeStep);
    const bool          is_rearming() const;
    const bool          needToBeRearmed() const;

	void		        addLauncher(cLauncher* launcher);
	void		        removeLauncher(cLauncher* launcher);
	cLauncher*      	findLauncherOnID(LauncherID launcherID) const;
    bool                can_master_arm_on() const;	
    bool                can_master_arm_off() const;	

	void		        net_passiveAssign(wsTarget& target_ ); 
    void		        net_passiveCancel(); 

	bool		        assignTargetEx(wsTarget& target_, const wWeaponSystemManager* trk_ = 0 );
    void		        cancelAssign(); 
    bool		        cancel();
    bool                can_cancel();
    bool                IsTargetAssigned() const;
    inline bool         IsTargetAssigned(ObjectID idTarget_) const;
	bool		        can_assign_more(ObjectID idTarget_) const;
    bool                has_base() const {return base != NULL;};
    bool                good_to_attack( const woPointer& pnewTarget_, const wWeaponSystemManager* trk_ = NULL, WeaponFlags allowedWeaponTypeFlags = ArmWeapon);
    int                 number_of_tracking_missiles() const;
    int                 number_of_tracking_missiles(ObjectID target_) const;
    int                 max_number_of_tracking_missiles() const;
    int                 max_number_of_tracking_missiles_conditioned() const;

	void		        updateDrawArguments(wModelTime t);

    double              get_bestY(double angleY_, double angleZ_)const;
    bool                is_in_angles(double angleZ_) const;
    bool                is_in_angles(double angleY_,double angleZ_) const;
    bool                is_in_firePermittedAngles(int* l_r, int* u_d) const;

    bool                angleY_within_limits(const double angle_) const;
	bool				angleDiviationIsSmall(double angleY_, double angleZ_);

    double              get_moveable() const { return moveable;};
    double              get_deviation(double,double) const;
    double              get_deviation() const;

    double              get_currY_fast_(wModelTime t) const; // без ограничителей углов
    double              get_currZ_fast_(wModelTime t) const; // без ограничителей углов
    double              get_currY_in_hostLCS(wModelTime t); //угол по азимуту относительно оси x хоста
    double              get_currY_fast(wModelTime t) const;
    double              get_currZ_fast(wModelTime t) const;

    bool                isTargetInBeam(const dVector&) const;

    inline const ObjectID get_targetID() const { return target.T_ID;};
	
	inline SystemID             getIdSystem() const {return idSystemInHost;}
	inline woPointer			getHost() const {return host;}
    inline wWeaponSystemManager*getManager() const {return manager;}
    inline wsystem_type 		getType() const { return type; };
    bool                hasType(wsystem_type) const;
    bool                realAttacker() const {return _realAttacker || number_of_tracking_missiles();}; // not a wsType_Radar or wsType_RadarCC or wsType_RadarTrack
    //void				getZone(float& hMin, float& hMax, float& dMin, float& dMax) const;
	
	bool				isEscortTargetByMissile();
    float               GetMaximumWeaponDistance(wsystem_type wsTypeFilter);
	float               GetMaximumWeaponHeight();

	bool                IsDepends()const;
    bool                IsDependsOnExternal()const;
 	bool				IsDependsOn(const ed::string& unit_type_)const;
 
	bool		        accept_point_target() const;

    // для трекинга ракет
    bool                track_missile(ObjectID missileID_, ObjectID targetID_);
    void	            startedRocketHasDied(ObjectID RocketID_);

    typedef			    ed::vector<cLauncher*> Launchers;
    Launchers			launchers;
	int					getLaunchersCount() const { return launchers.size(); }

    bool                lock_on(ObjectID target_) const;

    // barrels locking
    bool                lockBarrel(const ed::string& connName_, cLauncher* launcher_, const wModelTime lockPeriod_ = 0.0);
    bool                unlockBarrel(const ed::string& connName_, cLauncher* launcher_);
    const wModelTime    getBarrelsLockDelay(const cLauncher* const ln) const;

	const dPosition&	getMountingPos() const { return mountingPos; }

	void				on_fire(cLauncher * launcher, ObjectID target_id, const wsType & weaponType, void * weaponData, ObjectID weapon_id);
    wModelTime          my_timestep; // константный step поллинга, вычисляется в postInit

    double              getArtilleryShellMaxHeight(ObjectID targetId) const;

	void                setAI_ON();
	void                setAI_OFF();

    bool                netIsMaster();
	bool				isHumanControlled();

	void				getAmmo(Ammo & ammo) const;
    void                GetAmmoCount(ammo_t & ammo) const; 
    void                GetActiveLNAmmoCount1(ammo_t & ammo, int* readyState) const; 
    void                GetActiveLNAmmoCount2(ammo_t & ammo) const; 
    bool                IsLockedAndReady() const; 

    bool                can_manual() const;
	inline void         setManual(bool manual_);

    const float         getMaxTargetPriority(const woPointer& target_ptr);
    void                setCurrentTargetPriority(const float pr);
    const float         getCurrentTargetPriority();

	void				netfire(const FireMsgData& fmd);

    Graphics::ModelConnector* connectorPointer; // для прицеливания

    virtual inline cLauncher* getManualLN() const { return manualLN; };
    woPointer           getManualTarget() const {return manual_target;};
    void                netSetManualTarget(ObjectID manualTrgID);
    woPointer           getManualTargetTemp() const {return manual_target_temp;};
    wModelTime          getManualTargetTime() const {return manual_lockon_time;};
    bool                getManualStabilizer() const {return manual_stabilizer;};
    bool                hasLaserRangefinder() const { return laserRanger; };
    void                enableRangefinder(bool enable_ = true);
    int                 getCorrectionsApplyingState() const;
    bool                getRangeWithLaser(double &dist) const;
    double              takeRange() const;
    double              getLaserMaxRange() const;
    void                setElevationCorrectionByRange(double dist); // возвышение по дальномеру
    void                setAimingCorrectionsByInputData(); // выставляем коррекцию ствола на основе имеющихся данных с датчиков
    double              getElevationCorrection() const { return sightElevationCorrection; };
    double              getAzimuthCorrection() const { return sightAzimuthCorrection; };
    char                getMasterSightMode() const;

    void                validatePointerOnObjectID(ObjectID ID_, woPointer woPnt); // related to player unit switch
    const bool          canEmission() const;
    const bool          trackingEmissionIsOn() const { return (type == wsType_RadarTrack || type == wsType_RadarTrackEx) ? trackingEmissionIsOn_ : true; };
    void                trackingEmissionRequest(bool reqState);
    void                net_TrackingEmissionTurnOn();
    const unsigned      getEmissionModeForTarget(woPointer const target_IN, bool iAmTR = false) const;

protected:
#ifdef DEBUG_TEXT
    void                debug_text(ed::ostringstream& out);
#endif
    typedef				wDrawActivity<wWeaponSystem> wActivityWeaponSystem;
    wActivityManager	activities;
    wtPointer           activity_control; 
    wtPointer           activity_maintain;
    wtPointer           activity_rotate; 
#ifdef DEBUG_GEOMETRY_ENABLED
	wtPointer			activity_drawTrackerDirector; //for Tracker Director drawing
	void				DrawTrackerDirector(wModelTime& t);
#endif // DEBUG_GEOMETRY_ENABLED

    void	            uninitSystem();
	ed::string			dm_area; // damage modell area name

	mutable int         depends; // unit has launcher which takes targeting data from another unit
    mutable int         _can_manual;

    wsystem_type		type;
	woPointer			host; // chassis
    wWeaponSystem*	    base; // chassis or turret, to take position
    wWeaponSystemManager* manager; 
    int                 base_type; // 1 - baseY, 2 - baseZ, 3 - baseYZ

    bool                PointedToTarget() const;

    void                rotationLauncher(wModelTime& timeStep);
    void                rotationLauncher_manual(wModelTime& timeStep);
    void                rotateToReferencePosition();
    void                rotateToReloadPosition(cLauncher* listener);
    void                releaseReloadPosition(cLauncher* listener);

    dPosition           getCameraViewPoint(bool) const;
    const ed::string&   getCockpitName() const;
    bool                getManualTargetAim(const  dPosition & pos, dVector& r);
    //void                calculate_sight_deviations(bool init_ = false);

    void                manualFireTry(cLauncher* manLN);

    //////////////////////////////////////////////////////////////////////////
    //table values
    int				drawArgument1;
    int				drawArgument2;
    double			refAngleY;
    double			refAngleZ;
    double          reloadAngleY;
    double          reloadAngleZ;
    bool            new_cinematic;
    bool            new_rotation;
    bool			moveable;
    bool            mount_before_move;
    bool            inReferencePosition;
    bool            inReloadPosition;
    cLauncher*      reloadPositionListener;

    ed::string		name; // connector name
    dPosition		mountingPos;

    ed::string      cockpitName;
    cVector         sightPoint;

mutable cPosition   cachedPos;
mutable wModelTime  tcachedPos;

    SystemID		idSystemInHost;
    int				board;
    bool            laser; // weapon system has laser range-finder or laser targeting device
    cLaserRangefinder*   laserRanger;

    float			maximumDistance;	
    float			maximumHeight;

    float			minimumDistance;
    float			minimumHeight;
    double          pidY_k[5]; // P,I,D,inn,vel
    double          pidZ_k[5]; // P,I,D,inn,vel

    bool            _realAttacker; // wstype is not a wsType_Radar or wsType_RadarCC or wsType_RadarTrack

    Graphics::ModelConnector* connectorCenterWS;
    Graphics::ModelConnector* connectorPointerView; // для камеры

    //runtime values
    int             alarm_state_arg;
    ArgumentAnimation	anmAlarmState; // арг приведения в боевое для отдельных систем

    wsTarget        target;
    woPointer       manual_target;
    woPointer       manual_target_temp; // used for delayed manual lock-on process
    wModelTime      manual_lockon_time; // start time of manual lock-on
    bool            stabilizerPresent;
    bool            manual_stabilizer;  // stabiilizer on\off
    dVector         manual_stabilizer_vec;

    CPID            pidY;
    double          curr_velY;

    CPID            pidZ;
    double          curr_velZ;

    CPID            pidCorrZ;
    CPID            pidCorrY;
    double          azimuthCorrVel;
    double          elevCorrVel;

    double			angleYWant; // defined by user input or AI calculations
    double			angleZWant;
    double			angleYCould; // limited by mechanical features
    double			angleZCould;
    double          prevYCould;
    double          prevZCould;
    double			manual_stab_angleY; // stabilizer direction
    double			manual_stab_angleZ;
    double          sightElevationCorrection; // required value, calculated by ballistic computer
    wDouble         currentElevationCorrection; // interpolator for animation, gives actual value for sight correction angles in particular moment of simulation
    double          sightAzimuthCorrection; // the same for the azimuth angle
    wDouble         currentAzimuthCorrection;
    // sight angles
    wDouble			angleY; // 
    wDouble			angleZ;
    wModelTime      argumentsUpdateTime; // время, когда были закэшированы аргументы
    wModelTime      anglesUpdateTime; // время когда были 

    double          currentAimingDeviation; // угловое отклонение от линии прицеливания
    double          sight_dev_y;
    double          sight_dev_z;
    wModelTime		Time;
    wModelTime		cancel_timer;
    LauncherID      ws_owner;

    // ручное управление

	double baseTurretRotationSpeedX; // Базовая скорость вращения башни с клавиатуры по горизонтали
	double baseTurretRotationSpeedZ; // Базовая скорость движения ствола по вертикали с клавиатуры

	double maxTurretRotationSpeedX; // Максимальная скорость вращения башни с клавиатуры по горизонтали
	double maxTurretRotationSpeedZ; // Максимальная скорость движения ствола по вертикали с клавиатуры

	double curTurretRotationSpeedX; // Текущая скорость движения ствола по горизонтали с клавиатуры
	double curTurretRotationSpeedZ; // Текущая скорость движения ствола по вертикали с клавиатуры

	wModelTime turretDtX; // dT для вращения башни по горизонтали
	wModelTime prevTurretTimeX;
	wModelTime turretDtZ; // dT для вращения бащни по вертикали
	wModelTime prevTurretTimeZ;

    void	        CommandSet(int command);
    void	        CommandSet(int command, float value);
    cLauncher*      manualLN;
    cLauncher*      manualLNsecondary;
	bool            manual;
    float			cursorY, cursorZ;

    float           currentTargetPriority;
    cLauncher*      topPriorLauncher;
    const wWeaponSystem*  topPriorTrk;
    float           topPrior;

	LauncherID		owner() const {return ws_owner;};
    bool            takeOwnership(LauncherID);
    bool            isOwner(LauncherID);
    void            releaseOwnership(LauncherID);

    void            prepare_angles(double angleY_, double angleZ_);

    void            rotate(LauncherID owner_, double angleYRequired_, double angleZRequired_);
	double			getMaxTacticalSpeed();
    //////////////////////////////////////////////////////////////////////////

    int             launchersRearming;
    void            onLauncherRearming();
    void            onLauncherRearmed();

    // MAIL {
public:
	void	        getDiffInitStatus(Mail::Writer& stream);
	void	        setDiffInitStatus(Mail::Reader& stream);
    // } MAIL

private:
	// New sounders

	void sound_init();
	void sound_uninit();

	// New sounders

	void stopRotation();

    typedef struct
    {
        ed::string connName;
        cLauncher* launcher;
        wModelTime releaseTime;
    } _barrelLock;
    typedef ed::vector<_barrelLock> LockedBarrels;
    LockedBarrels lockedBarrels;

    AngleLimits angles; // углы прицеливания
    AngleLimits angles_mech; // механические ограничения по углам поворота

    void copy_missiles(LauncherID,IDs&);

public:
	void sound_update();
	
private:
	friend	class cLauncher;
	friend	class wWeaponSystemMail;
	static	wWeaponSystemMail mail;
    double          arg_to_angleY(double arg_) const;
    double          angleY_to_arg(double angle_) const;
    double          arg_to_angleZ(double arg_) const;
    double          angleZ_to_arg(double angle_) const;
    void            get_currY_Z(double&,double&);
    void            get_sight_currY_Z(double&Y_,double&Z_);
    double          angleY_cached, angleZ_cached;

    void            rearm_all();
    void            adjustLauncherAimingTime(float relLife);
    bool            trackingEmissionIsOn_;

    float mouse_Y_move_value;
}; 
