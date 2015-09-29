#ifndef __BLOCK_GYRO_STABILIZIER__
#define __BLOCK_GYRO_STABILIZIER__

#include "simulation_block_disc.h"
#include "Math/FOF.h"
#include "Math/PID.h"

class wGyroStabilizierDescriptor;

class wGyroStabilizier : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wGyroStabilizier, wSimulationBlockDisc, wGyroStabilizierDescriptor);
public:
	wGyroStabilizier(const wGyroStabilizierDescriptor* desc, wSimulationSystem* system);
	~wGyroStabilizier();

	virtual double simulate(double time);

protected:

	INPUT_WIRES(4, (
		WIRE(bool,			hasSignal_),	//Signal from seeker presence flag
		WIRE(Math::Vec3d,	Gd_),			//Discriminator output
		WIRE(Math::Rot3d,	realRot_),		//Missile orientation quat
		WIRE(Math::Vec3d,	omega_)			//Missile center of mass angular velocity (rad/s)
	));

	OUTPUT_WIRES(2, (
		WIRE(Math::Rot3d,	seekerRot_),	//Seeker direction vector global
		WIRE(Math::Vec3d,	omegaLOS_)		//Measured seeker local angular rate vector
	));
private:

	Math::Vec3d						prevGd_;		//Previous value of Gd signal to find derrivative
	Math::Vec3d						headBoresight_;	//Seeker head boresight local azimuth and elevation (rad)
	Math::PID_<Math::Vec3d, double>	omegaReqPid_;	//PID Gd -> head required angular rate
	Math::FOF<Math::Vec3d>			omega_;			//Achieved omega (rad/s)
};

class wGyroStabilizierDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wGyroStabilizier)

public:
	virtual wGyroStabilizier* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(double, dt);
	DECL_PARAM(double[3], omegaPidK);	//Coefficients of PID Gd -> head required angular rate
	DECL_PARAM(double, omegaMax);		//Seeker head angular rate max (rad/s)
	DECL_PARAM(double, omegaTau);		//Seeker head angular rate time constant(s)
	DECL_PARAM(double, gimbalLimit);	//Seeker head gimbal limit(rad)
};

#endif //__BLOCK_GYRO_STABILIZIER__
