#ifndef DXVolumeEffect_H
#define DXVolumeEffect_H

#include "Renderer\VolumeEffect.h"

namespace Graphics
{

// интерфейс объемных эффектов DirectX
class DXVolumeEffect : public VolumeEffect
{
protected:
	Position3 pos;
public:
	virtual void UpdatePos(const Position3& _pos) {pos = _pos;}
	virtual const Position3& GetPos() {return pos;}
};

} // namespace Graphics

#endif // DXVolumeEffect_H
