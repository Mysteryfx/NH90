#ifndef __AMMUNITION_VIKHR_H__
#define __AMMUNITION_VIKHR_H__

#include "Ammunition/ammunition_descriptor.h"
#include "ammunition_guided.h"

class dbg_cylinder;

extern wGuidedWeaponMail guidedWeaponMail;

typedef wGuidedWeaponMail		wVikhrMail;
typedef GuidedWeaponNetMessage	VikhrNetMessage;

class WEAPONS_API wAmmunitionVikhr : public wAmmunitionGuided
{
public:
	wAmmunitionVikhr();
	virtual ~wAmmunitionVikhr();

	virtual double simulate();

    // From IwMissile
	void		 setTarget(ObjectID id);
    virtual void MissileSetTarget(const cVector& tpos);

	void		Parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects=0);
	virtual viFlag getViFlag();
protected:
	virtual void initCommon_(MovingObject * launcher,const wAmmunitionDescriptor* desc);

    void OnNetworkMessage(const VikhrNetMessage& msg);

protected:

	Math::Vec3d	*coneOrigin_, *coneDir_;
	double		*coneNearDist_, *coneNearRad_; 

	dbg_cylinder* signalVisual_;

    friend class wGuidedWeaponMail;
};

class wNURSSight;

class wAmmunitionVikhrDescriptor : public wAmmunitionDescriptor
{
public:
	wAmmunitionVikhrDescriptor(const ed::string& name);
    virtual ~wAmmunitionVikhrDescriptor();

    virtual void load(wDatabaseLoader* loader);
    
    wNURSSight* getSight() { return sight_; }

protected:
	virtual wAmmunition* create_() const;

protected:
    wNURSSight* sight_;
};


#endif
