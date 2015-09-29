#pragma once
#include "cLinear.h"
#include "ClipVolumes/IntersectionType.h"
#include "Renderer/VolumeEffect.h"
#include "wTime.h"

class wcaCar;
class cPosition;
class Position3;
class ClipVolume;
namespace Graphics {class ModelParser;}

class wcaCarState
{
public:
	wcaCarState(wcaCar* host)
		:_host(host){};
	virtual ~wcaCarState() {};

	// выдаст текущую позицию (по времени)
	virtual cPosition &Position(wModelTime t) = 0;

	// рассчет скорости
	virtual float ScalarVelocity() const = 0;
	virtual const cVector &VectorVelocity() const = 0;
	virtual const float Omy() = 0;

	virtual double GetPathPar(wModelTime t) const = 0;
	virtual IntersectionType TestVolume(ClipVolume *volume) = 0;
	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects) = 0;

	virtual void update() {};

protected:
	wcaCar* _host;
};