#pragma once

#include "CockpitBase.h"

#include "Utilities/ccUtilities.h"
#include "wMovingObject.h"
#include "AI/AI_Gun_Handler.h"

class IwoLA;

namespace gunner_AI {

class COCKPITBASE_API GunHandlerAI : public IGunHandler
{
public:

	GunHandlerAI();

	virtual void l_read(Lua::Config& config, IwoLA* la) override;

	virtual void fire(bool on) override;

	virtual float get_ammo_rest() const override;  /*returns ammo rest*/

protected:

	IwoLA* host_la_;
};

}