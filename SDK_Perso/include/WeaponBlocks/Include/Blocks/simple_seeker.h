#ifndef __BLOCK_SIMPLE_SEEKER_H__
#define __BLOCK_SIMPLE_SEEKER_H__

#include "seeker.h"

class wSimpleSeekerDescriptor;

class wSimpleSeeker : public wSeeker
{
	DECLARE_BLOCK(wSimpleSeeker, wSeeker, wSimpleSeekerDescriptor);
public:
	wSimpleSeeker(const wSimpleSeekerDescriptor* desc);

protected:

	virtual bool getTargetLOS(Math::Vec3d & LOS, double time, lead_data_ptr_t data_ptr);


	INPUT_WIRES(2, (
		WIRE(bool,			target_),	//Target presence flag
		WIRE(Math::Vec3d,	targetPos_)	//Target position vector
	));

	OUTPUT_WIRES(0, (
	));
};

class wSimpleSeekerDescriptor : public wSeekerDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wSimpleSeeker)

public:
	virtual wSimpleSeeker* create(Lua::Loader* runtime_cfg);

public:
};

#endif
