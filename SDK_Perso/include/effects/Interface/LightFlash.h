#ifndef _LightFlash_h_
#define _LightFlash_h_

#include "viLight.h"
#include "Killer.h"

namespace Effects
{

// Вспышка света (от взрыва, выстрела и т.д.)
class LightFlash : public viLight,
                   public Suicide
{
public:

	virtual ~LightFlash()
    {}

	virtual void SetPhase(float phase) = 0;

};

} // namespace Effects

#endif // _LightFlash_h_
