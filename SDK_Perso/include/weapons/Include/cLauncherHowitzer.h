//                    file cLauncherHowitzer.h
#pragma once

#include "Weapons.h"
#include "cLauncher.h"

#include "cannonsight.h"

#include "Shells/shell_descriptors.h"
#include "Shells/shell_group.h"

#define IID_ILauncherHowitzer Common::identify<cLauncherHowitzer>() 

class wShellGroup;

class WEAPONS_API cLauncherHowitzer : public cLauncher, public wShellExplosionListener
{
    // для арт-наводчика
    virtual void onShellExplosion(const Math::Vec3d& point);

public:
	cLauncherHowitzer();
	virtual~cLauncherHowitzer();

    virtual bool	    preInit(woPointer& chassis_,wWeaponSystem* theSystem_, Lua::Config& c, wsystem_type type_, int id_);
    virtual void	    postInit();

	void                barrelFire(const FireMsgData& fmd);
    virtual bool        fire();
    virtual bool	    assignTargetEx(const wsTarget& target_);
    virtual void		net_assignTargetEx(const wsTarget& target_);

    virtual	bool        checkTrgDistancePossible(const woPointer& trg_) const;

    // MAIL {
    virtual void	    getDiffInitStatus(Mail::Writer& stream);
    virtual void	    setDiffInitStatus(Mail::Reader& stream);
    // } MAIL

    virtual double      getArtilleryShellMaxHeight(ObjectID targetId) const;
    virtual double      getArtilleryDistByAngle(const double& angle_) const;
    virtual double      getArtilleryAngleByDist(const double& dist_);

    virtual inline void setManual(bool manual_);
    virtual inline const bool  hasAutomaticLoader() const { return automaticLoader; };

    virtual wsType      getAmmunition_wsType() const { return wsType(wsType_Weapon,wsType_Shell,0,0); };

protected:

    virtual	int	        conditionsOfShootingAreExecuted();
    virtual int			checkAimingDynamicParameters(const woPointer& trg_, const wWeaponSystem* trk_ ) const;	// проверяются динамические параметры цели, вызывается при прицеливании, 0 - ok, 1,2,3... - error codes
	virtual int		    calculateAimingParameters(const dPosition & mypos_, const woPointer& target_, dVector& r);	// 0 - ok, 1... - error code
    virtual bool        calculateAimingCorrections(double dist, double& azimuthCorr, double& elevCorr);
    virtual bool        calculateAimingCorrections(const BC_InputData& data, double& azimuthCorr, double& elevCorr);

    void                generate_new_delta(); // новые отклонения при стрельбе по площадям

    virtual bool        IsIndirect()const{return true;};

    // forward observer, артиллерийский наводчик
    bool            FO_use; // использоавать наводчика. только для точечных целей. для площадных не используется.
    double          FO_fork_max;
    double          FO_fork_min;
    double          FO_fork_threshold;

    // для стрельбы по площадям
    double          radius,dx,dy;

    dVector         prev_direct_to_target;
    dVector         correction_barrel_pos;
    double          correction_distn; // накопленная величина коррекции по дальности
    double          correction_angle; // накопленная величина коррекции по углу
    double          correction_step;
    bool            prev_perelet; // предыдущий выстрел был перелет = true, недолет = false;
    wModelTime      use_correction;
    wModelTime      fly_time;
    dVector         aiming_vector;

	wShellAimingTableDescriptor::targeting_data targeting_data;
	wShellGroup*					shellGroup_;
	wShellAimingTableDescriptor*	aimingTable_;
    bool            automaticLoader;
};
