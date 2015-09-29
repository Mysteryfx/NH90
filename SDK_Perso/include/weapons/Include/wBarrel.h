#pragma once

#include "ed/sstream.h"
#include "Lua/Config.h"

#include "Weapons.h"
#include "cPosition.h"
#include "Common.h"
#include "Modeler/ModelConnector.h"
#include "wMovingObject.h"
#include "WorldMail.h"
#include "wActivity.h"
#include "wActivityManager.h"
#include "wFloat.h"
#include "wStowage.h"


class MovingObject;
class cLauncher;
class wMissileOnPilon;
class wShellDescriptor;
class wAmmunitionNURSDescriptor;

#define IID_IBarrel Common::identify<wBarrel>() 

// хранилище описания стволов
class WEAPONS_API wBarrel : public Common::FakeUnknown<Serializable> 
{
    friend class wWeaponSystemManager;
    friend class cReloadingStateOrdinary;
    friend class cReloadingStateSimultaneously;
    friend class cReloadingStateSequentialy;
public:
    typedef unsigned char BarrelID;
protected:
    void                load_byorder(wStowage* stowage, wModelTime readyTime);
    void                setReadyTime(wModelTime redyTime_);

private:
    typedef Graphics::ModelConnector ModelConnector;

    ed::string			name;
    ModelConnector*		modelConnector;	// для новых моделей
    cLauncher*		    launcher;
    wMissileOnPilon*	missile;
    MovingObject*		parent;
    int					drawArgument;

    typedef				wActivity<wBarrel> wActivityBarrel;
    wActivityManager	activities;
    wtPointer           activity_recoil; 
    int					recoilArgument;
    double              recoilTime;

    BarrelID            barrelID;

    void                SetExternalMissile();

    // run-time values
    wModelTime          delayToSwitchOn; 
    wModelTime          timeReady;
    bool                ammo_loaded;
    wShellDescriptor*   current_shell; // loaded shell
    wAmmunitionNURSDescriptor* current_rocket; //loaded rocket
    wsType              current_missType; // loaded missile wsType
    ed::string         current_missName; // loaded missile name
    int					currentStowageNum;
    wFloat			    recoilArg; // приведение в боеготовность
    bool                reloadAfterShot; // необходимость перезарядки непосредственно после выстрела

public:
    wBarrel();
    virtual ~wBarrel();

    cVector			    mountingPos; // для старых моделей
    const ed::string&  getConnectorName() const {return name;} // возвращает название коннектора (для лока стволов)
    ModelConnector*	    getModelConnector() const {return modelConnector;} // для новых моделей

    void                preInit(cLauncher* launcher_, woPointer host_, Lua::Config& c, int id_);

    void			    getPosBarrel(dPosition& position); // point and direction

    bool			    maintain(); // малая частота вызова
    void                net_setLoaded(bool loaded_); // не переопределяет время готовности ствола, служит только для синхронизанции отображения
    //bool                reload(); // перезарядка
    //bool                instantReload(int tsNum); // переброска боеприпаса из пилона в ствол
    void		        recoil(wModelTime& timeStep);

    void                shot();
    BarrelID            getID() const { return barrelID; };
    bool			    IsReady() const;
    inline bool         is_empty() const {return !ammo_loaded;};
    wModelTime          get_delay_to_ready()const{return timeReady - wTime::GetModelTime();};
    wShellDescriptor*   get_current_shell() const {return current_shell;};
    wAmmunitionNURSDescriptor* get_current_rocket() const {return current_rocket;};
    void                get_current_missile(wsType& missType_, ed::string& missName_) const {missType_ = current_missType; missName_ = current_missName;};

    void                switch_to_stowage(const int currentStowageNum, const wModelTime delayToSwitchOn_);
    void                setReloadAfterShot(bool reloadAfterShot_) {reloadAfterShot = reloadAfterShot_;};

	// panov {
	wModelTime			getTimeReady() const { return timeReady; }
	// panov }

#ifdef DEBUG_TEXT
    virtual             void debug_text(ed::ostringstream& out);
#endif

    // MAIL {
    void	getDiffInitStatus(Mail::Writer& stream);
    void	setDiffInitStatus(Mail::Reader& stream);
    // } MAIL
};

