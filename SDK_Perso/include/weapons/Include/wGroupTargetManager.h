#pragma once

#include <ed/vector.h>
#include "WorldGeneral.h"
#include "wTime.h"
#include "wWeaponSystemManager.h"
#include "AI\Detection\wTargetDetectionStatus.h"
#include "wListTargetManager.h"
#include "Utilities/Parameter.h"
#include "LandGroup.h"
#include "TargetPriorities.h"
#include "WeaponFlagEnum.h"

struct WEAPONS_API wsCommonTarget 
{
    unsigned int	    T_ID;       // Номер цели в мире
	woPointer			pTarget;
    wsCommonTargetType  T_Type;     // Вид цели (наземная/воздушная)
    wsCommonTargetType  T_SubType;  // Подвид цели
    wModelTime          last_detect; // время крайнего детектирования цели каким либо членом колонны
    double              radius;
    ObjectID            master;     // используется при внешнем прицеливании, например пусковые С-300 от трек-радара
	char			    master_ws;  // номер WS в машинке
    int                 waypoint;   // используется при указании целей из редактора.
    wTargetDetectionStatus target_det_status;

    wsCommonTarget();
    wsCommonTarget(const wsCommonTarget& arg);
	bool operator<(wsCommonTarget & commonTarget) const;
};

typedef	ed::map<ObjectID, wsCommonTarget> common_targets;
typedef common_targets::iterator common_targets_itr;
typedef common_targets::const_iterator common_targets_citr;

class WEAPONS_API wGroupTargetManager: public wControl, public Igroup
{
public:

    typedef ed::vector<wWeaponSystemManager *> military_units;
    typedef military_units::iterator military_units_itr;
    typedef military_units::const_iterator military_units_citr;

    wGroupTargetManager();
    virtual ~wGroupTargetManager();

    //////////////////////////////////////////////////////////////////////////
    virtual void    addTargetUnit(ObjectID unit, bool known);
    virtual void    addTargetUnit(woPointer& unit, bool known);
    virtual void    addTargetGroup(wControl* group, bool known);
    virtual void    removeTargetUnit(ObjectID unit);
    virtual void    removeTargetGroup(wControl* group);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // launch manager
private:
    wModelTime next_launch;
public:
    virtual void    we_launch(const wsType&, const double next_launch_delay_ = 0); // register launch
    virtual bool    can_we_launch(const wsType&); // return time to next launch
    //////////////////////////////////////////////////////////////////////////

    virtual void    add_member(wWeaponSystemManager * unit_);
    virtual void    remove_member(wWeaponSystemManager * unit_);
    virtual void    add_target(wsCommonTarget& trg_, WeaponFlags allowedWeaponType = ArmWeapon);
    virtual void    add_target(wsCommonTarget& trg_, float priority, woPointer& unit_);
    virtual common_targets_itr remove_target(const ObjectID ID_);
    virtual void	remove_target_priority(const ObjectID ID_, woPointer& unit_);
    virtual void    clean_up_obsolete_targets();
    virtual void    clear_targets();
    virtual bool    has_point_target();
    virtual bool    accept_point_target() const;
    virtual int     GetLastWaypointId()const{return 0;};
	virtual bool	is_target_in_list(ObjectID ID_);
            void    getDetectedTargets(Targets & targets) const;
            bool    isTargetDetected(ObjectID targetID, wTargetDetectionStatus & targetDetectionStatus) const;

	virtual const wWeaponSystem* find_trk_by_type_ws(const ed::string& type_, int ws_id_, ObjectID targetID_);
    void			setOption(unsigned int name, const Option & value);
    const int       getAlarmStateOption() const;

	// внешнее управление огнем колонны
	virtual	void	enableFire(bool enable) {enforce_fire = enable; };
	virtual	bool	getEnableFire() const { return enforce_fire; };

    void			onShoot(wcCoalitionName coalition, ObjectID shooterID, const wsType & weaponType, void * weaponData, ObjectID weaponID, ObjectID targetID, const cPoint & targetPoint, wModelTime impactTime);
    virtual void    onMyUnitShoot(wcCoalitionName coalition, ObjectID shooterID, const wsType & weaponType, void * weaponData, ObjectID weaponID, ObjectID targetID, const cPoint & targetPoint, wModelTime impactTime = 0.0);

    virtual bool    AllowMoreAttackers(wsCommonTarget& target_, int assignedWS, wsystem_type wstype_);
    virtual int     GetNumberOfAttackersWS(const ObjectID targetID_, wsystem_type wstype_) const;
    void            sheduleMasterArms_Off();
    void            check_potentialTarget(const ObjectID trgID); // является ли цель потенциальной для группы
    static void		targetTypeDetermination(wsCommonTargetType& type, wsCommonTargetType& subType, woPointer& pTarget);
    bool            getCommonTarget(const unsigned int ID_, wsCommonTarget& trg);
    const ed::list<ObjectID>& get_CoalitionTargets() const;
    virtual void    validatePointerOnObjectID(ObjectID ID_, woPointer woPnt); // related to player unit switch
protected:

	bool			enforce_fire; // принудительное запрещение огня
	wcCoalitionName	my_coalition;      
    military_units  units;

    common_targets  targets;
    UnitTargetPriorityMap     targets_priorities; // список приоритетов целей для группы, упорядоченный по приоритету
    TargetsPriorities          new_targets_priorities;
    ed::map<ObjectID, WeaponFlags> weaponTypeFlagsOnTarget;
    wModelTime      master_arms_off_timer;
    int				numMailToHuman;

    virtual void	maintain();

	virtual void	assignTargets(wModelTime& timeStep);
    virtual void	mailToHumanPlanes(wModelTime& timeStep);

    virtual void    postInit();

    CoalitionTargetManager* myCoalitionTargetManager;

    void            update_target_priority(wsCommonTarget& trg_, float priority, woPointer& unit_);
private:

    virtual void  mailToHuman(wWeaponSystemManager* unit,
                              woPointer           & target);      

    virtual bool    assign_target_to_unit(wWeaponSystemManager* pUnit, wsCommonTarget& _target, const wWeaponSystemManager* trk = NULL, WeaponFlags allowedWeaponTypeFlags = ArmWeapon);

    int             GetNumberOfAttackers(ObjectID targetID_) const;
    static int      maxNumberofAttackers(wsCommonTargetType type_, wsCommonTargetType subType_);
    static int      maxNumberofAttackersWS(const wsCommonTarget& target_, wsystem_type wstype_ );
};
