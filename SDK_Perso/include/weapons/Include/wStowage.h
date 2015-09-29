#pragma once

#include "ed/sstream.h"
#include "Lua/Config.h"

#include "Weapons.h"

#include "cPosition.h"
#include "Common.h"

#include "WorldMail.h"
#include "wsType.h"
#include "wTime.h"

class cLauncher;
class wMissileOnPilon;
class wShellDescriptor;
class wAmmunitionNURSDescriptor;

#define IID_IStowage Common::identify<wStowage>() 

// хранилище боеприпасов разного типа с разным временем доступа
class WEAPONS_API wStowage : public Common::FakeUnknown<Serializable>
{
    //friend class wWeaponSystemManager;
    friend class cLauncher;
public:

	typedef unsigned char StowageID;

	wStowage();
	virtual ~wStowage();

    void            preInit(cLauncher* launcher_, Lua::Config& c, int id_);
	bool			maintain();
    bool            startRearm();
    void            stopRearm();

    int				getTotalAmmoCapacity() const {return ammo_capacity;};
    int             getActualAmmoCapacity() const {return actual_ammo_capacity;};
	StowageID		getID() {return id;}
	
    void            remove_round();
    void            getCurrentAmmunition(wShellDescriptor** shell_, wAmmunitionNURSDescriptor** rocket_, wsType& missileType, ed::string& missileName);
    bool            is_empty() const {return actual_ammo_capacity<=0; };
    bool            is_available() const { return !numOfPortionsToReload && actual_ammo_capacity>0; };

    wModelTime      prepareRearming();
    inline int      getPortionAmmoCapacity() const { return portionAmmoCapacity; }
    wModelTime      setPortionRearmFinishTime();
    wModelTime      getPortionRearmFinishTime() const { return portionRearmFinishTime; };

    wModelTime      get_shot_delay() const {return shot_delay;};
    wModelTime		getSwitchOnDelay() const {return switchOnDelay;};
    wModelTime      get_time_rearm() const;
    inline const unsigned int getMyFeedSlotID() const {return myFeedSlotID;}
    inline const bool   hasAutomaticLoader() { return automaticLoader; }

    // для cLauncherAutogun, cLauncherGun, cLauncherHowitzer
    typedef         ed::vector<wShellDescriptor*> ammo_set_t; //{A,A,A,B...}, etc
    ammo_set_t      ammo;
	typedef			ed::vector<std::pair<wShellDescriptor*, unsigned int>> AmmoCount;
	AmmoCount		ammoCount;

    // для cLauncherRocket
    wAmmunitionNURSDescriptor* rocketDescriptor_;

    // для cLauncherMissile
    wsType          typeAmunition;
    ed::string      nameAmunition;

    wModelTime      shot_delay;
    wModelTime      shot_delay_lofi;

protected:
    // table values, consts
    cLauncher*		launcher;
    StowageID		id;
    bool            automaticLoader;
	wModelTime		switchOnDelay;
    wModelTime		timeOvercharge;
    wModelTime      portionReloadTime;
    int             ammo_capacity;
    unsigned int    myFeedSlotID;

    // run-time variables
    int             numOfPortionsToReload;
    int             actual_ammo_capacity;
    int             portionAmmoCapacity;
    wModelTime		portionRearmFinishTime;
    ed::deque<wModelTime> rearmDelays;
    wShellDescriptor* get_current_shell() const {return ammo.size()>0 ? ammo[(ammo_capacity-actual_ammo_capacity) % ammo.size()] : NULL;};
    void            net_setActualAmmoCapacity(int actAmmoCap);

public:
#ifdef DEBUG_TEXT
    virtual void    debug_text(ed::ostringstream& out);
#endif
// MAIL {
	void	        getDiffInitStatus(Mail::Writer& stream);
	void	        setDiffInitStatus(Mail::Reader& stream);
// } MAIL
private:
    void            rearm_all();
};

