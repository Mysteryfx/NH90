#pragma once

#include "Guns/Effects/Gun_Effect.h"

namespace weapons_gun
{
	class GunSynchronizerDesc : public Gun_Effect_Desc
	{
		friend class GunSynchronizer;

		unsigned Sectors;			//Sectors = число секторов разрешения на стрельбу 'Allowance sectors for one prop revolution
		double	 Transparency;
		unsigned CorrespondingEngine;
		double   propeller_RPM_nominal;
	public:
		virtual void l_read(Lua::Config& config) override;
		virtual void create(wAircraftGunMount* gun_mount) override;
	};

	class GunSynchronizer : public SimulateEffect
	{
		const GunSynchronizerDesc * descriptor;
		friend class GunSynchronizerDesc;
	public:
		virtual void simulate(double dt) override;
	protected:
		GunSynchronizer(const GunSynchronizerDesc * desc,wAircraftGunMount* gun_mount);
	};


}