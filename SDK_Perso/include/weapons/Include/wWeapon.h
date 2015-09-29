#ifndef wWeapon_H
#define wWeapon_H

#include "Weapons.h"

#include "IWeapon.h"
#include "IwInfo.h"

#include "Ammunition/ammunition_name.h"
#include "Lua/State.h"

//#define WEAPON_DRAW_ARGS_MAX 6


class lLandObject;

enum ExplosionEnv;
enum ExplosionType;
struct wsCollisionData;

class wSimulationSystem;
class wSimulationSystemScheme;

class wWeaponMail;

void WEAPONS_API weaponRegLua(lua_State * L);

class wWeapon : public IwWeapon, public wINamedAmmunition, public wINamedAmmunitionDescriptor
{
	mutable ed::string  original_name;
	mutable ed::string  display_name;
public:
	
	wWeapon(); 
	~wWeapon(); 

	virtual void			initCamera();	
	virtual const gBox&		LocalBox(wModelTime t)	{	return box;}
	virtual void			SpaceRegister(wModelTime dt, float use_size = -1.f); // пространственная регистрация, следующая - через dt
	virtual unsigned int	GunId() const { return Gun; } // кто выпустил
	virtual void			serialize(Serializer &serializer);
	virtual void			Control();
	const bool				In_Air_(void)   { return true;} // В воздухе
	// сгенерировать взрыв с заданными параметрами и удалить объект.
	void					explode(const cPoint& , ObjectID id_=0);	
	void					destroy(bool immediately = false);// испариться не причиняя зла

	virtual void			PostCollisionProcessing(wsCollisionData*, ObjectID);
	// IWeapon
	virtual woPointer		getHost();
	virtual wModelTime		getShotTime() { return creatingTime; }

	virtual ed::vector<cVector> get_impact_points(){ed::vector<cVector> stub; return stub;};

	virtual void MissileSetTarget(const cVector& tpos)  {};

    // Named ammunition
    virtual const wINamedAmmunitionDescriptor* getNameDescriptor() const;
    virtual ObjectID getLauncherId() const;
    virtual const ed::string & getOriginalName() const;
    virtual const ed::string & getDisplayName() const;
    virtual const ed::string & getTypeDisplayName() const;

	wSimulationSystem *  getSystem() 
	{
		return netIsMaster() ? warheadServer_ : warheadClient_;
	}

protected:

	void			WeaponCheckImpact(viFlag mask); // Проверка столкнoвений.
	void			WeaponCheckImpact(viFlag mask, viObject * pTarget); // Проверка столкнoвений.
	virtual void	Check_Ground_Collision(); // Проверка попадания в рельеф

    void            initWarheads_(wSimulationSystemScheme* server,
                                  wSimulationSystemScheme* client);

    ObjectID    	Gun;          // мировой ИД пусковой установки.
	gBox			box;

	wModelTime		creatingTime; // абсолютное время создания (для pEvent)  

	bool			firstAfterSave;

    wSimulationSystem* warheadServer_, *warheadClient_;

// MAIL {
protected:
	friend class wWeaponMail;
	static wWeaponMail mail;

	void					doExplode(const cPoint& p) { doExplode(p, 0); }
	virtual void			doExplode(const cPoint& , ObjectID id);
	virtual void			doExplodeV(const cPoint& pos, const cVector& vel);
	void					doDestroyV();
// } MAIL
}; // class wWeapon


#endif // wWeapon_H
