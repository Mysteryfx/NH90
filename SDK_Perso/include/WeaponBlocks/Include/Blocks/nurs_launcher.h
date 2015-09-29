#ifndef __NURS_LAUNCHER_H__
#define __NURS_LAUNCHER_H__

#include <ed/string.h>
#include "simulation_block_disc.h"

class wNursLauncherDescriptor;
class wConstraintFixed;

#include "Math/Vector.h"
#include "Math/Matrix.h"

#include "constraint_rail.h"
#include "attachable.h"
	
class wAmmunitionDescriptor;

class wNursLauncher : public wSimulationBlockDisc, wIRailOwner
{
	DECLARE_BLOCK(wNursLauncher, wSimulationBlockDisc, wNursLauncherDescriptor);

public:
	wNursLauncher(const wNursLauncherDescriptor* desc, wSimulationSystem* system);
	virtual ~wNursLauncher();

	virtual double simulate(double dt);

	virtual void detachClientFromRail(wConstraintRail* rail);

protected:
	void doLaunch_(int what);

	void setAttachmentPoint_(wConstraintFixed* pnt);

protected:
	// Рельсы и нурсы
	ed::vector<std::pair<wConstraintRail*, wAmmunition*> > rails_;

protected:
	wConstraintFixed* attachmentPoint_;

protected:
	INPUT_WIRES(0, ())

	OUTPUT_WIRES(4, (
		WIRE(Math::Vec3d, pos_),
		WIRE(Math::Vec3d, vel_),
		WIRE(Math::Rot3d, rot_),
		WIRE(Math::Vec3d, omega_)
	))

	INPUT_PORT_OBJ_PTR_C(wConstraintFixed, attachmentPointPrt_);
	INPUT_PORT_OBJ_PTR(wAmmunition, owner_);

	INPUT_PORT(int, launch_);
};


///////////////////////////////////////////////////////////////////////
// Descriptor
///////////////////////////////////////////////////////////////////////
class wNursLauncherDescriptor : public wSimulationBlockDiscDescriptor, public wAttachableDescriptor<wNursLauncherDescriptor>
{
	DECLARE_BLOCK_DESCRIPTOR(wNursLauncher);
public:
	struct rail_data
	{
		Math::Vec3d position;
		Math::Rot3d orientation;
		double length;
	};

public:
	wNursLauncherDescriptor();

	wNursLauncher* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

	virtual void serialize(wBlockSerializer& ser);

	int getRailsCount() const;
	const rail_data& getRail(int i) const;

	const ed::string& getModelName() const { return model_name; }

protected:
	void load_();

public:
	DECL_PARAM(ed::string, model_name);
	DECL_PARAM(int, rails_count);
	DECL_PARAM(ed::string, nurs_type);
	wAmmunitionDescriptor* nurs_desc;

protected:
	bool loaded_;
	
	ed::vector<rail_data> rails_;
};

#endif 
