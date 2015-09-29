#pragma once

#include "Weapons.h"
#include "cLauncher.h"
#include "wSignaller.h"

#include "cannonsight.h"

#define IID_ILauncherAutoGun Common::identify<cLauncherAutogun>() 

class wShellGroup;

class WEAPONS_API cLauncherAutogun : public cLauncher
{
	ed::string			name; //for debriefing
public:
    cLauncherAutogun();
    virtual~cLauncherAutogun();

    virtual bool	    preInit(woPointer& chassis_,wWeaponSystem* theSystem_, Lua::Config& c, wsystem_type type_, int id_);
    virtual void	    postInit();

    virtual bool        fire();
    virtual bool	    fireStart();
    virtual bool	    fireStop();
    virtual void	    net_fireStop(int actualAmmoCapacityInActiveStowage);
    virtual	void	    cancelAssign();
    virtual bool	    assignTargetEx(const wsTarget& target_);
    virtual void		net_assignTargetEx(const wsTarget& target_);

	virtual	bool        good_to_attack( const woPointer& pnewTarget_, float& trg_priority, const wWeaponSystem* trk_ = NULL, WeaponFlags allowedWeaponTypeFlags = ArmWeapon);
    
    virtual void        select_new_stowage();

    // MAIL {
    virtual void	    getDiffInitStatus(Mail::Writer& stream);
    virtual void	    setDiffInitStatus(Mail::Reader& stream);
    // } MAIL

#ifdef DEBUG_GEOMETRY_ENABLED
    virtual void                DrawTrackerDirector();
#endif

    virtual inline void setManual(bool manual_);

	void                barrelFire(const FireMsgData& fmd);
    virtual inline const bool  hasAutomaticLoader() const { return true; };

    virtual wsType      getAmmunition_wsType() const { return wsType(wsType_Weapon,wsType_Shell,0,0); };

protected:
    wModelTime          get_aiming_delay()const{return aiming_delay;}; // поллинг цикла стрельбы
    void			    uninitGunLauncher();
    virtual bool        calculateAimingCorrections(double dist, double& azimuthCorr, double& elevCorr);
    virtual bool        calculateAimingCorrections(const BC_InputData& data, double& azimuthCorr, double& elevCorr);
	virtual int		    calculateAimingParameters(const dPosition & mypos_, const woPointer& target_, dVector& r);	// 0 - ok, 1... - error code
    virtual	int	        conditionsOfShootingAreExecuted();
    virtual int			checkAimingDynamicParameters(const woPointer& trg_, const wWeaponSystem* trk_ ) const;	// проверяются динамические параметры цели, вызывается при прицеливании, 0 - ok, 1,2,3... - error codes

    int                 get_burst_len(const double distance_)const; 
    double              get_burst_delay(const double distance_)const; 
    void                prepareNewQueue(); // остановка текущей и подготовка параметров новой очереди
	void				queueSetState(const bool st);
    int                 findStowageForArmoredTarget( float distToTarget, const wIArmoredTarget::ArmorData& armorData ) const;

    double              aiming_delay;
    int                 QueueType; // тип прицеливания текущей очереди: 0 - в расчетную точку, 1 - протяжка, 2 - заградительный
    wSignaller		    queue;
    wModelTime		    endQueueTime;
    wModelTime		    StartQueueTime;
    wModelTime          lastQueueMessageTime; // время последней отправки сообщения on_fire
    cVector             barrel_pos;
    int                 newStowageNum; // номер stowage для атаки цели определенный на этапе good_to_attack
    int				    numShell;
    int				    averageLengthQueue; 
    int				    CurrLengthQueue;
    wShellGroup*	    shellGroup_;
    bool                high_rate;
    CannonSight*         cannonSight;
};
