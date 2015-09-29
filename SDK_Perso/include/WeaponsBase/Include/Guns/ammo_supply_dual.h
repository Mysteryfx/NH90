#ifndef __AMMO_SUPPLY_DUAL_H__
#define __AMMO_SUPPLY_DUAL_H__

#include "WeaponsBase.h"

#include "ammo_supply.h"

class wAmmoSupplyDualDescriptor;

class WEAPONSBASE_API wAmmoSupplyDual : public wAmmoSupply
{
public:
    wAmmoSupplyDual(const wAmmoSupplyDualDescriptor* desc, Lua::Loader* cfg);
	virtual ~wAmmoSupplyDual();

	virtual void setAmmoCount(int ammoCount);

	virtual void prepareBurst(Mail::Stream& stream);

	virtual const wShellDescriptor* getCurrentShell() const;
	virtual const wShellDescriptor* getShellByName(const ed::string & shells_name) const;
	virtual const wShellDescriptor* popNextShell();
	virtual bool isEmpty();

	virtual double getMass();
	virtual Math::Vec3d getCenterOfMass();		

	virtual int getRoundsCount();
	virtual int getInitialRoundsCount();

	virtual void reset(float gun_load_factor = 1.0f);

    virtual void discharge(); 

	void switchChain();
	void setChain(int new_chain);

	const wShellDescriptor* getShellDescriptor(int index) const;

protected:
	int count_[2];
	int cartridgeCount_[2];
	const wAmmoSupplyDualDescriptor* desc_;
	int chainIndex_;
};

class WEAPONSBASE_API wAmmoSupplyDualDescriptor : public wAmmoSupplyDescriptor
{
public:
	wAmmoSupplyDualDescriptor(const ed::string& name) : wAmmoSupplyDescriptor(name) {}

	virtual void load(wDatabaseLoader* loader);

    virtual wAmmoSupplyDual* create(Lua::Loader* cfg) const;
	virtual const wShellDescriptor* getShellDescriptorByName(const ed::string & shells_name) const;

	int getRoundsCount(int i) const;
	const wShellDescriptor* getShellDescriptor(int i) const;

	const Math::Vec3d& getSecondBoxOffset() const { return secondBoxOffset_; }	// Смещение второго ящика с патронами
protected:
	int roundsCount_[2];
	const wShellDescriptor* shells_[2];
	Math::Vec3d secondBoxOffset_;
};

#endif
