//                    file cLauncherGun.h
#pragma once

#include "Weapons.h"
#include "cLauncher.h"

#include "cannonsight.h"

class IwWeapon;

#define IID_ILauncherGun Common::identify<cLauncherGun>() 

class wShellGroup;

class WEAPONS_API cLauncherGun : public cLauncher
{
public:
	cLauncherGun();
	virtual~cLauncherGun();

    virtual bool	    preInit(woPointer& chassis_,wWeaponSystem* theSystem_, Lua::Config& c, wsystem_type type_, int id_);
    virtual void	    postInit();

	void                barrelFire(const FireMsgData& fmd);
    virtual bool        fire();

    virtual bool	    assignTargetEx(const wsTarget& target_);
    virtual void		net_assignTargetEx(const wsTarget& target_);

    // MAIL {
    virtual void	    getDiffInitStatus(Mail::Writer& stream);
    virtual void	    setDiffInitStatus(Mail::Reader& stream);
    // } MAIL

    virtual inline const bool  hasAutomaticLoader() const { return automaticLoader; };

#ifdef DEBUG_GEOMETRY_ENABLED
    virtual void                DrawTrackerDirector();
#endif
    virtual wsType      getAmmunition_wsType() const { return wsType(wsType_Weapon,wsType_Shell,0,0); };

protected:
    virtual bool        checkAimingDeviation(const double distance_) const;

    virtual int			checkAimingDynamicParameters(const woPointer& trg_, const wWeaponSystem* trk_ ) const;	// проверяются динамические параметры цели, вызывается при прицеливании, 0 - ok, 1,2,3... - error codes
	virtual int		    calculateAimingParameters(const dPosition & mypos_, const woPointer& target_, dVector& r);	// 0 - ok, 1... - error code
    virtual bool        calculateAimingCorrections(double dist, double& azimuthCorr, double& elevCorr);
    virtual bool        calculateAimingCorrections(const BC_InputData& data, double& azimuthCorr, double& elevCorr);

    cVector             barrel_pos;
    double              c1, c2;

	wShellGroup*	    shellGroup_;
    CannonSight*        cannonSight;
    bool                automaticLoader;
};


