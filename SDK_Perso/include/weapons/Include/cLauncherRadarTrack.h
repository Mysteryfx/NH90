//                    file cLauncherRadarTrack.h
#pragma once

#include "cLauncher.h"

#define IID_ILauncherRadarTrack Common::identify<cLauncherRadarTrack>() 

class cLauncherRadarTrack : public cLauncher
{
public:

	cLauncherRadarTrack();
	virtual~cLauncherRadarTrack();

    virtual bool	    preInit(woPointer& chassis_,wWeaponSystem* theSystem_, Lua::Config& c, wsystem_type type_, int id_);
    virtual void	    postInit();

    virtual bool	    maintain(); // polling низкой частоты

	virtual bool	    launcherEscortTarget() const;
    virtual void	    startedRocketHasDied(ObjectID RocketID_);

	virtual int		    calculateAimingParameters(const dPosition & mypos_, const woPointer& target_, dVector& r);	// 0 - ok, 1... - error code
	virtual	bool        good_to_attack( const woPointer& pnewTarget_, float& trg_priority, const wWeaponSystem* trk_ = NULL, WeaponFlags allowedWeaponTypeFlags = ArmWeapon);

	// MAIL {
    virtual void	    getDiffInitStatus(Mail::Writer& stream);
    virtual void	    setDiffInitStatus(Mail::Reader& stream);
    // } MAIL

    virtual bool        track_missile(ObjectID missileID_, ObjectID targetID_);
    virtual int         number_of_tracking_missiles()const{return idTargetAndMissiles.size();};
    virtual int         number_of_tracking_missiles(ObjectID target_)const;
    virtual int         max_number_of_tracking_missiles()const { return max_number_of_missiles_channels; };
    virtual int         max_number_of_tracking_missiles_conditioned()const;

    virtual bool        check_local_slaves() const;

    virtual void        setIDS(IDs&);

#ifdef DEBUG_GEOMETRY_ENABLED
    virtual void                DrawTrackerDirector();
#endif

    virtual wsType      getAmmunition_wsType() const { return wsType(255); }; // проверка по типу вооружения для радарных должна всегда проходить

protected:
	virtual	int	        conditionsOfShootingAreExecuted();
    virtual	int	        conditionsOfTrackingAreExecuted(wBarrel::BarrelID idBarrel_ = 0) const;
    virtual int			checkAimingDynamicParameters(const woPointer& trg_, const wWeaponSystem* trk_ ) const;	// проверяются динамические параметры цели, вызывается при прицеливании, 0 - ok, 1,2,3... - error codes
    virtual bool        checkTrgDistancePossible(const woPointer& trg_) const;

    virtual double      get_max_acquiring_distance() const {return distanceMax;};
    virtual const float getPriorityByPosition(const woPointer& target_ptr) const;

    int                 max_number_of_missiles_channels;
};
