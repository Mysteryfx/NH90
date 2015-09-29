#pragma once
#include "seeker.h"

class wLaserSpotSeekerDescriptor;

class wLaserSpotSeeker : public wSeeker
{
    DECLARE_BLOCK(wLaserSpotSeeker, wSeeker, wLaserSpotSeekerDescriptor);
public:
    wLaserSpotSeeker(const wLaserSpotSeekerDescriptor* desc);
    
    void update(double time, lead_data_ptr_t data_ptr);
protected:
    unsigned  get_code(lead_data_ptr_t data_ptr) const; 
    bool      getTargetLOS(Math::Vec3d & LOS, double time, lead_data_ptr_t data_ptr);
    
    INPUT_WIRES(3, (
        WIRE(unsigned       , laser_code),			//laser code
        WIRE(bool           , have_directly_set_target),
        WIRE(Math::Vec3d    , directly_set_target)
    ))

    unsigned _target_id_;
    OUTPUT_WIRES(2, (
        WIRE(Math::Vec3d,target_pos),		//target_pos
        WIRE(unsigned   ,target_id)			//target_id
    ))
};

class wLaserSpotSeekerDescriptor : public wSeekerDescriptor
{
    DECLARE_BLOCK_DESCRIPTOR(wLaserSpotSeeker)

public:
    virtual wLaserSpotSeeker* create(Lua::Loader* runtime_cfg);

public:
};
