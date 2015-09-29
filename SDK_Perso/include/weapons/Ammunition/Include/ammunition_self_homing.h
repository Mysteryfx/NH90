#ifndef __AMMUNITION_SELF_HOMING__
#define __AMMUNITION_SELF_HOMING__

#include "Ammunition/ammunition_descriptor.h"
#include "ammunition_guided.h"

#include "IWeapon.h"
#include "IWorldNet.h"

class dbg_cone;
class dbg_limited_line;
class dbg_limited_line;

class WEAPONS_API wAmmunitionSelfHoming : public wAmmunitionGuided
{
public:
	wAmmunitionSelfHoming();
	virtual ~wAmmunitionSelfHoming();

	virtual double simulate();
	virtual void wAmmunitionSelfHoming::Parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects);
protected:	
	virtual void initCommon_(MovingObject * launcher,const wAmmunitionDescriptor* desc);

protected:

    bool        seeker_owned_target() const;

	bool*		 active_;
	bool*		 target_;
	Math::Vec3d* targetPos_;

    //OUTPUT 
    const Math::Vec3d* targetPos_from_seeker;
    const unsigned   * targetID_from_seeker;
    const bool       * hasSignal_;

    


	friend class wGuidedWeaponMail;
private:
	const double*		seekerFOV_;
	const Math::Rot3d*	seekerRot_;
	const Math::Vec3d*	targetLOS_;
	dbg_cone*			seekerHeadDrw_;	
	dbg_limited_line*	seekerHeadDirDrw_;	
	dbg_limited_line*	targetLOSDrw_;
	dbg_limited_line*	selfVelDrw_;
	dbg_limited_line*	errorDrw_;
	dbg_limited_line*	ADrw_;
};

class wAmmunitionSelfHomingDescriptor : public wAmmunitionGuidedDescriptor
{
public:
	wAmmunitionSelfHomingDescriptor(const ed::string& name);
	virtual ~wAmmunitionSelfHomingDescriptor();

protected:
	virtual wAmmunition* create_() const;
};


#endif //__AMMUNITION_SELF_HOMING__
