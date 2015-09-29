#pragma once

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include "Common/System.h"
#include "Factory/StandardFactory.h"
#include "wListener.h"
#include "Notificator/Evoker.h"

struct lua_State;

class WORLDGENERAL_API WorldManager :	public Evoker<wListener, Link<wListener> >,
				                        public System
{
	unsigned int humanID;
	woPointer humanPtr;
	unsigned int humanPilotID; //пришлось пока так вот сделать...
	woPointer humanPilotPtr;
	bool bMissionRandomization;
	int iYear;
	int iMonth;
	int iDay;
	
	bool manualVehicleControl;
	bool TrashWaterOn;				   //всплески на воде от мелких обломков (мусора)
	wModelTime TrashGroundOnLifeTime;  //время жизни приземлившихся обломков (мусора)
	wModelTime PartsGroundOnLifeTime;  //время жизни приземлившихся обломков (больших)
	lua_State *luaWorld;
public:
	WorldManager::WorldManager();
	virtual ~WorldManager();

    virtual void    create(const Common::Identifier &identifier, Common::Identifiable **identifiable);
    virtual void	serialize(Serializer &);
	woPointer		Get_woPointer(unsigned int id);
	unsigned int	ID_OF_Human();
	woPointer		HumanPtr(bool Pilot = false);
	void			SetHuman(woPointer p, bool Pilot = false);
	void			ResetHuman(woPointer p, bool Pilot = false);
	unsigned int	GetNearestAirdrome(cPosition &pos, float &Dist);
	void			ParseConfig();
	bool			GetMissionRandomization();
	bool			GetMissionDate(int &Year, int &Month, int &Day);
	bool			GetTrashWaterOn() { return TrashWaterOn; }
	wModelTime      GetTrashGroundOnLifeTime() { return TrashGroundOnLifeTime; }
	wModelTime      GetPartsGroundOnLifeTime() { return PartsGroundOnLifeTime; }
	bool			GetManualVehicleControl() {return manualVehicleControl; }
	wModelTime      ConflagrationTime(const ed::string & shape);


    void            addListener(wListener *l);
    void            removeListener(wListener *l);
    void            notifyBirth(Registered *wobject);
    void            notifyDeath(Registered *wobject);
};

extern WORLDGENERAL_API WorldManager *globalWorldManager;

template <class T>
class WorldFactory: public Common::StandardFactory<T>
{
public:
    WorldFactory() {}
	// Factory must implement IUnknown:
	// TO DO: .. CreateInstance(..) { ... new ...Unknown<T>; }
};
#define REGISTER_WORLD_FACTORY(factory) REGISTER_FACTORY(getRegistry, factory);
#define REGISTER_WORLD_FACTORY_EX(factory, name) REGISTER_FACTORY_EX(getRegistry, factory, name);
