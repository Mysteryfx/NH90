//                    file cLauncherRocket.h
#pragma once

#include "Weapons.h"
#include "cLauncher.h"

#include <memory>

#include "Sights/nurs_sight.h"

#define IID_ILauncherRocket Common::identify<cLauncherRocket>() 

class wMLRSRail;


class WEAPONS_API cLauncherRocket : public cLauncher
{
public:
	cLauncherRocket();
	virtual~cLauncherRocket();

    virtual bool	    preInit(woPointer& chassis_,wWeaponSystem* theSystem_, Lua::Config& c, wsystem_type type_, int id_);
    virtual void	    postInit();

	void                barrelFire(const FireMsgData& fmd);
	virtual bool        fire();
	virtual bool	    fireStart();
	virtual bool	    fireStop();
    virtual float       getNextShotDelay() const;

    // MAIL {
    virtual void	    getDiffInitStatus(Mail::Writer& stream);
    virtual void	    setDiffInitStatus(Mail::Reader& stream);
    // } MAIL

	void                acceptImpulse(const dVector& pnt, const dVector& imp);
	void                removeRail(wMLRSRail* rail);
    virtual double      getArtilleryAngleByDist(const double& dist_);

    virtual bool	    assignTargetEx(const wsTarget& target_);
    virtual void		net_assignTargetEx(const wsTarget& target_);

	// panov {
	virtual wModelTime	getDelayTimeActiveBarrel() { 
		return (*activeBarrel)->get_delay_to_ready();
	}
	// panov }

    virtual wsType      getAmmunition_wsType() const;

protected:

	virtual int		    calculateAimingParameters(const dPosition & mypos_, const woPointer& target_, dVector& r);	// 0 - ok, 1... - error code
    virtual int			checkAimingDynamicParameters(const woPointer& trg_, const wWeaponSystem* trk_ ) const;	// проверяются динамические параметры цели, вызывается при прицеливании, 0 - ok, 1,2,3... - error codes
    virtual	int	        conditionsOfShootingAreExecuted();
    
    bool                calculateAimingEx_(const double& dist, double& zReq);
    bool			    calculateAiming_(const dVector& target_pos, dVector & r);
	Math::Vec3d		    calcPoint_(double y0, double y1, double angle, const Math::Vec3d& wind);
	double			    calcDist_(double y0, double y1, double angle, const Math::Vec3d& wind);
	double			    calcDev_(double y0, double y1, double angle, const Math::Vec3d& wind);
	

    virtual bool        IsIndirect()const{return true;};

    // run-time values
	//wModelTime		timeShot; // panov - move to base class
    wModelTime		next_launch;
	wModelTime		timeSmokeCloud;
	wModelTime		dTimeSmokeCloud;

	// Прицеливание
	bool			aiming;
    dVector         aiming_vector;
    cPosition       prev_hostPos;
	dVector			prev_targetPos; // кэшированые значения для прицеливания

    // table values
    double          launch_delay;

	// NURS descriptor
	ed::vector<wMLRSRail*> rails_;
	double railLength_;

	// NURS sight
	std::auto_ptr<wNURSSight> sight_;

	// Активности

	typedef wActivity<cLauncherRocket> wActivityRocket;
	wActivityManager rocketActivities;
	wtPointer rocketFireActivity;
	
	void fireActivityHandler(wModelTime &deltaTime);

	// Активности
};



