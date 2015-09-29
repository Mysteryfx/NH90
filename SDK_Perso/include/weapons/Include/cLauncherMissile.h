//                    file cLauncherMissile.h
#pragma once

#include "cLauncher.h"
#include "cLauncherRadarTrack.h"

#define IID_ILauncherMissile Common::identify<cLauncherMissile>() 

struct Rocket_Const;

struct TargetData 
{
	TargetData()
	{
		targetPointer = nullptr;
		mePointer = nullptr;
		pMissileData = nullptr;
	}

	MovingObject* targetPointer;
	MovingObject* mePointer;
	Rocket_Const* pMissileData;
};

class cLauncherMissile : public cLauncherRadarTrack
{
public:

	cLauncherMissile();
	virtual~cLauncherMissile();

    virtual bool	    preInit(woPointer& chassis_,wWeaponSystem* theSystem_, Lua::Config& c, wsystem_type type_, int id_);
    virtual void	    postInit();

    virtual bool	    maintain(); // polling низкой частоты

	virtual void	    startedRocketHasDied(ObjectID RocketID_);
    virtual bool	    assignTargetEx(const wsTarget& target_);
    virtual void		net_assignTargetEx(const wsTarget& target_);

	bool			    findTarget(unsigned int idTarget_);
	void                barrelFire(const FireMsgData& fmd);
	virtual bool        fire();
    virtual float       getNextShotDelay() const;

	// MAIL {
    virtual void	    getDiffInitStatus(Mail::Writer& stream);
    virtual void	    setDiffInitStatus(Mail::Reader& stream);
    // } MAIL

    virtual bool	    launcherEscortTarget() const;
    virtual bool        IsIndirect()const;
    virtual woPointer   lockon();
    virtual bool        check_lock(woPointer obj);

	virtual inline void setManual(bool manual_);

    virtual wsType      getAmmunition_wsType() const;

protected:

    double              getTimeToHit(const cPosition& myPos, const cPosition& trgPos, const cVector& trgVel) const;

    virtual bool        updateAimingCorrections(double dist, double& azimuthCorr, double& elevCorr);
	virtual int		    calculateAimingParameters(const dPosition & mypos_, const woPointer& target_, dVector& r);	// 0 - ok, 1... - error code
    virtual	int	        conditionsOfShootingAreExecuted();
    virtual int			checkAimingDynamicParameters(const woPointer& trg_, const wWeaponSystem* trk_ ) const;	// проверяются динамические параметры цели, вызывается при прицеливании, 0 - ok, 1,2,3... - error codes
    virtual bool        checkTrgDistancePossible(const woPointer& trg_) const;
    virtual bool        checkTrgDistanceReal(const woPointer& trg_) const;
    virtual bool        checkTrgAltitude(const woPointer& trg_) const;

    virtual double      get_max_acquiring_distance() const;
    virtual const float getPriorityByPosition(const woPointer& target_ptr) const;

    // table  values
    Rocket_Const*       pMiss_Data;
    double              launch_delay;
    double              distanceMaxMissile;
    double              distanceMaxMissile_lofi;
    double              min_launch_angle;
    double              inclination_correction_upper_limit;
    double              inclination_correction_bias;

    // run-time values
	wModelTime		    next_launch;
	wModelTime		    timeSmokeCloud;
	wModelTime		    dTimeSmokeCloud;
    wModelTime          m_timeToDistanceMaxMissile;

	// ПЗРК

	typedef wActivity<cLauncherMissile> wActivityLauncherMissile;
	wActivityManager launcherMissileActivities;
	wtPointer irSearchActivity;

	wModelTime deltaTime; // Дельта времени
	wModelTime lastActivityTime; // Последнее время обновления

    ed::string fnameIrLowQuiet;
    ed::string fnameIrLockTarget;
    struct snd {
        Sound::Source ir_low_quiet;
        Sound::Source ir_lock_target;
    } _snd;

    Sound::Host irSoundHost; // Вывод звука в наушники для ИК ракет

	void                        irActivityHandler(wModelTime& modelTime);
    virtual void                sound_load(Lua::Config &conf);
    virtual void                sound_init();
    virtual void                sound_uninit();
	void                        soundUpdate(wModelTime& deltaTime);
    void                        headphonesSoundsStop();

	// ПЗРК
};