#ifndef __SHELL_NEW_H__
#define __SHELL_NEW_H__

#include "wMovingObject.h"

#include "WeaponsBase.h"
#include "activity_group.h"

#include "Math/Vector.h"

#include "Ammunition/ammunition_name.h"

class Serializer;

class wShellDescriptor;

// Приходится наследовать от MovingObject (например, из-за системы видов), отстой
class WEAPONSBASE_API wShell :  public MovingObject, public wActivityGroupMember, public wINamedAmmunition
{
public:

	virtual eSimulationResult simulate(double dt, bool& exploded) = 0;

	virtual Math::Vec3d getVelocity() const = 0;

	virtual const wShellDescriptor & getDesc () = 0;

protected:
	friend class wShellMail;
	void netDestroy();

	struct snd {
		Sound::Source move;
	} _snd;
};

#endif
