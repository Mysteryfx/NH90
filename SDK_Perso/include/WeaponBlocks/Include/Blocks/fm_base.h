#ifndef __FM_BASE_H__
#define __FM_BASE_H__

#include "WeaponBlocks.h"

#include "Math/Vector.h"
#include "Math/Rotation3.h"

#include "simulation_block_disc.h"

#ifndef WEAPONS_BENCH
//#include "serializer.h"
#endif

class wFMBaseDescriptor;

///////////////////////////////////////////
// Базовая FM 
///////////////////////////////////////////
class WEAPON_BLOCKS_API wFMBase : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wFMBase, wSimulationBlockDisc, wFMBaseDescriptor)

public:
	wFMBase(const wFMBaseDescriptor* desc, wSimulationSystem* system);
	virtual ~wFMBase() {};

	// getters/setters
	virtual void	setPosition(const Math::Vec3d& val);
	virtual void	setVelocity(const Math::Vec3d& val);
	virtual void	setRotation(const Math::Rot3d& val);

	const Math::Vec3d&	getPosition() const;

	// simulation
	virtual double simulate(double dt) = 0;

	// сериализация
/*#ifndef WEAPONS_BENCH
	virtual void serialize(Serializer &serializer);
#endif*/

protected:
	INPUT_WIRES(3, (
		WIRE(double, rho_),
		WIRE(double, sndSpeed_),
		WIRE(Math::Vec3d, wind_)
	))

	OUTPUT_WIRES(3, (
		WIRE(Math::Vec3d, pos_),
		WIRE(Math::Vec3d, vel_),
		WIRE(Math::Rot3d, rot_)
	))

	INPUT_PORT(Math::Vec3d, ppos_);
	INPUT_PORT(Math::Vec3d, pvel_);
	INPUT_PORT(Math::Rot3d, prot_);
};

namespace Lua { class Loader; }

class WEAPON_BLOCKS_API wFMBaseDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wFMBase)

protected:
	// Хелпер для создания
	template <class FMType, class MYType> 
	FMType* create_(MYType* self) const 
	{
		return new FMType(self);
	}
};

#endif
