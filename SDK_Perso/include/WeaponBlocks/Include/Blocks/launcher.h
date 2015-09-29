#ifndef __LAUNCHER_H__
#define __LAUNCHER_H__

#include "simulation_block_disc.h"

class wBlockLauncherDescriptor;

class wBlockLauncher : public wSimulationBlockDisc
{
    DECLARE_BLOCK(wBlockLauncher, wSimulationBlockDisc, wBlockLauncherDescriptor);
public:
    wBlockLauncher(const wBlockLauncherDescriptor* desc, wSimulationSystem* system);

    virtual double simulate(double time);

protected:
    void onFire_(bool);

protected:
    INPUT_WIRES(7, (
        WIRE(Math::Vec3d, pos_),
        WIRE(Math::Vec3d, vel_),
        WIRE(Math::Rot3d, rot_),
        WIRE(Math::Vec3d, omega_),

        WIRE(int, country_),
        WIRE(int, launcher_id_),
		WIRE(int, state_)
        ))

        OUTPUT_WIRES(0, (
        //		WIRE(type, name),
        ))

        INPUT_PORT(bool, fire_);
};

class wAmmunitionDescriptor;

class wBlockLauncherDescriptor : public wSimulationBlockDiscDescriptor
{
    DECLARE_BLOCK_DESCRIPTOR(wBlockLauncher)

public:
    virtual wBlockLauncher* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

    const wAmmunitionDescriptor* getAmmunitionDesc() const;

    virtual void serialize(wBlockSerializer& ser);

public:
    //	DECL_PARAM(type, name);
    DECL_PARAM(bool, is_server);

protected:
    const wAmmunitionDescriptor* ammunition_;
};

#endif
