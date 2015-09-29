#ifndef __BLOCK_SIMPLE_GYRO_STAB_SEEKER__
#define __BLOCK_SIMPLE_GYRO_STAB_SEEKER__

#include "simulation_block_disc.h"

class wSimpleGyroStabSeekerDescriptor;

class wSimpleGyroStabSeeker : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wSimpleGyroStabSeeker, wSimulationBlockDisc, wSimpleGyroStabSeekerDescriptor);
public:
	wSimpleGyroStabSeeker(const wSimpleGyroStabSeekerDescriptor* desc, wSimulationSystem* system);
	~wSimpleGyroStabSeeker();

	virtual double simulate(double time);

protected:

	INPUT_WIRES(3, (
		WIRE(bool,			hasSignal_),	//Signal from seeker presence flag
		WIRE(Math::Vec3d,	LOS_),			//Discriminator output
		WIRE(Math::Rot3d,	rot_)			//Missile orientation quat
	));

	OUTPUT_WIRES(2, (
		WIRE(Math::Rot3d,	seekerRot_),	//Seeker direction vector global
		WIRE(Math::Vec3d,	omegaLOS_)		//Measured seeker local angular rate vector
	));
private:

	Math::Vec3d				prevLOS_;
	bool					hadSignal_;
};

class wSimpleGyroStabSeekerDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wSimpleGyroStabSeeker)

public:
	virtual wSimpleGyroStabSeeker* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(double, dt);
};

#endif //__BLOCK_SIMPLE_GYRO_STAB_SEEKER__
