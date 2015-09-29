#ifndef __BLOCK_WCMD_GUIDANCE__
#define __BLOCK_WCMD_GUIDANCE__

#include "simulation_block_disc.h"
#include "cx_interpolator.h"
#include "cAlgorithm.h"
#include "Math/PID.h"

class wWCMDGuidanceDescriptor;

class wWCMDGuidance : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wWCMDGuidance, wSimulationBlockDisc, wWCMDGuidanceDescriptor);
public:
	wWCMDGuidance(const wWCMDGuidanceDescriptor* desc, wSimulationSystem* system);
	~wWCMDGuidance();

	virtual double simulate(double time);

protected:

	INPUT_WIRES(3, (
		WIRE(Math::Vec3d,	pos_),			// position
		WIRE(Math::Vec3d,	vel_),          // velocity
		WIRE(Math::Rot3d,	rot_)			// rotation
	));

	INPUT_PORT_CONST(Math::Vec3d, targetPos_);   // target position
	INPUT_PORT_CONST(bool,		  hasWCMD_);	   // has WCMD flag 
	INPUT_PORT_CONST(double,	  drop_height_);  // Drop height for cluster bombs

	OUTPUT_WIRES(1, (
		WIRE(Math::Vec3d,	fins_)         // fin angles to control 
	));
private:

	Math::PID_<Math::Vec3d, double> pid_;
	double s_;
	CxInterpolator cxInterp_;
	double initial_dist;
	bool   isInitDistance;

};

class wWCMDGuidanceDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wWCMDGuidance)

public:
	virtual wWCMDGuidance* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(double, dt);
	DECL_PARAM(double, PID_integr);
	DECL_PARAM(double, PID_Koef);
	DECL_PARAM(double, PID_differ);
	DECL_PARAM(double, char_time);
	DECL_PARAM(double, bomblet_char_time);
};

#endif //__BLOCK_WCMD_GUIDANCE__
