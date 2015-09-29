#ifndef __AMMO_SUPPLY_SIMPLE_H__
#define __AMMO_SUPPLY_SIMPLE_H__

#include "ammo_supply.h"

class wAmmoSupplyMixedDescriptor;

class wAmmoSupplyMixed : public wAmmoSupply
{
public:
    wAmmoSupplyMixed(const wAmmoSupplyMixedDescriptor* desc, Lua::Loader* cfg);
	virtual ~wAmmoSupplyMixed();

	virtual void setAmmoCount(int ammoCount);

	virtual void prepareBurst(Mail::Stream& stream);

	virtual const wShellDescriptor* getCurrentShell() const;
	virtual const wShellDescriptor* getShellForSight() const;
	virtual const wShellDescriptor* getShellByName(const ed::string & shells_name) const;
	virtual const wShellDescriptor* popNextShell();
	virtual bool isEmpty();

	virtual double getMass();
	virtual Math::Vec3d getCenterOfMass();		

	virtual int getRoundsCount();
	virtual int getInitialRoundsCount();

    virtual void discharge(); 

    virtual void     setAmmoType(unsigned t);
    virtual unsigned getAmmoType() const  { return mix_;}
protected:
	int      count_;
    unsigned shell_counter_;//fired for correct mix different shells with unlimit settings
    unsigned mix_;

	const wShellDescriptor		    * shell_for_sight_;
	const wAmmoSupplyMixedDescriptor* desc_;
};

class wAmmoSupplyMixedDescriptor : public wAmmoSupplyDescriptor
{
public:
	wAmmoSupplyMixedDescriptor(const ed::string& name) : wAmmoSupplyDescriptor(name) {}

	virtual void load(wDatabaseLoader* loader);

    virtual wAmmoSupplyMixed* create(Lua::Loader* cfg) const;

	int getRoundsCount() const { return roundsCount_; }
	const wShellDescriptor* getShellDescriptor(unsigned i) const { assert(i < shells_.size());  return shells_[i]; }
    
    const wShellDescriptor* getShellDescriptorByMixAndCount(unsigned mix,unsigned count) const;
	const wShellDescriptor* getShellDescriptorForSightByMix(unsigned mix) const;
	virtual const wShellDescriptor* getShellDescriptorByName(const ed::string & shells_name) const;
protected:
	int roundsCount_;
    ed::vector<const wShellDescriptor*> shells_;

    ed::vector< ed::vector<unsigned> > mixes_;
};

#endif
