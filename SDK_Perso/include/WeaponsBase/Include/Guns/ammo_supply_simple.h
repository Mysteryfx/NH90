#ifndef __AMMO_SUPPLY_SIMPLE_H__
#define __AMMO_SUPPLY_SIMPLE_H__

#include "ammo_supply.h"

class wAmmoSupplySimpleDescriptor;

class wAmmoSupplySimple : public wAmmoSupply
{
public:
    wAmmoSupplySimple(const wAmmoSupplySimpleDescriptor* desc, Lua::Loader* cfg);
	virtual ~wAmmoSupplySimple();

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

	virtual void reset();

    virtual void discharge(); 

protected:
	int count_;
	const wAmmoSupplySimpleDescriptor* desc_;
};

class wAmmoSupplySimpleDescriptor : public wAmmoSupplyDescriptor
{
public:
	wAmmoSupplySimpleDescriptor(const ed::string& name) : wAmmoSupplyDescriptor(name) {}

	virtual void load(wDatabaseLoader* loader);

    virtual wAmmoSupplySimple* create(Lua::Loader* cfg) const;

	int getRoundsCount() const { return roundsCount_; }
	virtual const wShellDescriptor* getShellDescriptor() const { return shells_; }
	virtual const wShellDescriptor* getShellDescriptorByName(const ed::string & shells_name) const;

protected:
	int roundsCount_;
	const wShellDescriptor* shells_;
};

#endif
