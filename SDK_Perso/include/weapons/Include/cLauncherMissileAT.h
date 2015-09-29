//                    file cLauncherMissileAT.h
#pragma once

#include "cLauncher.h"
#include "cLauncherRadarTrack.h"

#define IID_ILauncherMissileAT Common::identify<cLauncherMissileAT>() 

struct Rocket_Const;

class cLauncherMissileAT : public cLauncherRadarTrack
{
public:

	cLauncherMissileAT();
	virtual~cLauncherMissileAT();

    virtual bool	    preInit(woPointer& chassis_,wWeaponSystem* theSystem_, Lua::Config& c, wsystem_type type_, int id_);
    virtual void	    postInit();

    virtual bool	    maintain(); // polling низкой частоты

	virtual void	    startedRocketHasDied(ObjectID RocketID_);
    virtual bool	    assignTargetEx(const wsTarget& target_);
    virtual	void		cancelAssign();
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
    virtual ObjectID    getTargetID() const;

    virtual const float getPriorityByPosition(const woPointer& target_ptr) const;

    virtual void        lockoff();
    virtual bool        informTargetIfInRRCBeam(const dVector& dirToTarget_, woPointer pTrg) const;

    virtual wsType      getAmmunition_wsType() const;
/*
#ifdef DEBUG_GEOMETRY_ENABLED
    virtual void                DrawTrackerDirector();
#endif
*/

protected:

    virtual bool        updateAimingCorrections(double dist, double& azimuthCorr, double& elevCorr);
	virtual int		    calculateAimingParameters(const dPosition & mypos_, const woPointer& target_, dVector& r);	// 0 - ok, 1... - error code
    virtual	int	        conditionsOfShootingAreExecuted();
    virtual int			checkAimingDynamicParameters(const woPointer& trg_, const wWeaponSystem* trk_ ) const;	// проверяются динамические параметры цели, вызывается при прицеливании, 0 - ok, 1,2,3... - error codes
    virtual bool        checkTrgDistanceReal(const woPointer& trg_) const;
    virtual bool        checkTrgAltitude(const woPointer& trg_) const;
    virtual bool        checkTrgVisibility(const woPointer& pTarget_) const;

    virtual double      get_max_acquiring_distance() const;

    typedef				wDrawActivity<cLauncherMissileAT> wActivityLauncherAT;
    wActivityManager	activities;
    wtPointer           activity_controlmissile; 
    void		        controlmissile(wModelTime& timeStep);

    // table  values
    Rocket_Const*       pMiss_Data;
    double              launch_delay;
    double              distanceMax_lofi;
    double              min_launch_angle;
    double              inclination_correction_upper_limit;
    double              inclination_correction_bias;

    // run-time values
	wModelTime		    next_launch;
	wModelTime		    timeSmokeCloud;
	wModelTime		    dTimeSmokeCloud;

    wModelTime		    aim_time;
    wModelTime		    control_time;
    wModelTime          missileControlInterval;
    double              radialDisperse;
    dVector             missile_relative_position_calculation_disperse;
    double              dispertionReductionFactor;
    bool                missilesAreRRC;
    bool                maintain_after_tracking;
};

