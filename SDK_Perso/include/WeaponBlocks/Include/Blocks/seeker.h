#ifndef __BLOCK_SEEKER_H__
#define __BLOCK_SEEKER_H__

#include "simulation_block.h"

class wSeekerDescriptor;

class viSearch;
class viObject;

class wSeeker : public wSimulationBlockStateless
{
	DECLARE_BLOCK(wSeeker, wSimulationBlockStateless, wSeekerDescriptor);
public:
	wSeeker(const wSeekerDescriptor* desc);

	virtual void update(double time, lead_data_ptr_t data_ptr);

protected:

	virtual bool getTargetLOS(Math::Vec3d & LOS, double time, lead_data_ptr_t data_ptr);
	virtual bool checkSource(viObject * pObject);
	virtual Math::Vec3d getWeighDirToSource(viObject * pObject, double time, lead_data_ptr_t data_ptr);

	INPUT_WIRES(4, (
		WIRE(bool,			active_),	//Seeker activity flag		
		WIRE(Math::Rot3d,	rot_), 		//Seeker orientation quaternion (global)
		WIRE(Math::Vec3d,	pos_), 		//Seeker pos vector
		WIRE(bool,			elecPower_)	//Elec power
	));

	OUTPUT_WIRES(4, (
		WIRE(bool,			hasSignal_),//Seeker has signal from target
		WIRE(Math::Vec3d,	Gd_),		//Discriminator Gain Function vector (proportional to target local angular error vector)
		WIRE(Math::Vec3d,	LOS_),		//Target LOS
		WIRE(double,		FOV_)		//FOV
	));
private:
	static bool viObjectCheckHandler(viObject *item, viSearch *search, void *data);
	double getGain(double fiNorm);
	double discriminate(double fiNorm);	
};

class wSeekerDescriptor : public wSimulationBlockStatelessDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wSeeker)

public:
	virtual wSeeker* create(Lua::Loader* runtime_cfg);

public:
	DECL_PARAM(double, sensitivity);//Sensitivity (у.е.)
	DECL_PARAM(double, delay);		//Seeker activation delay(sec)
	DECL_PARAM(double, FOV);		//Field of View (rad)
	enum
	{
		SIZE_DGF = 10,
		SIZE_RWF = 10
	};	
	DECL_PARAM(double[SIZE_DGF], DGF); 	//Discriminator Gain Function	
	DECL_PARAM(double[SIZE_RWF], RWF); 	//Responce Weighting Function
};

#endif
