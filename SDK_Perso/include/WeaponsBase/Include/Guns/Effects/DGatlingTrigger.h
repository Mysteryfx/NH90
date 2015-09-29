#pragma once

#pragma once

#include "Guns/Effects/Gatling_Effect.h"

namespace weapons_gun {

	//////////////////////////////////////////////////////////////////////////
	// DelayedGatlingEffect activate gun fire with a delay by RPM
	class DelayedGatlingTriggerDesc : public GatlingEffectDesc
	{
		friend class DelayedGatlingTrigger;
	public:
		virtual void l_read(Lua::Config& config) override;
		virtual void create(wAircraftGunMount* gun_mount) override;
	private:
		float RPM_lim_;
	};

	class DelayedGatlingTrigger : public GatlingEffect
	{
		friend class DelayedGatlingTriggerDesc;
		DelayedGatlingTrigger(const DelayedGatlingTriggerDesc* desc, wAircraftGunMount* gun_mount);

	public:
		virtual void simulate(double dt) override;
	private:
		float RPM_lim_;
	};

}