#ifndef GUN_HEAT_PROXY_3_H 
#define GUN_HEAT_PROXY_3_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "CockpitBase.h"

#include "FMHeatExchange/heat_body.h"
#include "FMHeatExchange/heat_group.h"
#include "Avionics/Weapon/GunHeating/GunHeatProxyBase.h"
#include "Avionics/Weapon/GunHeating/GunHeatProxyDecs3.h"

namespace cockpit {

class COCKPITBASE_API GunHeatProxy3 : public GunHeatProxyBase
{
protected:

	GunHeatProxy3(const GunHeatProxyModelDesc3& desc);

public:

	static GunHeatProxy3* create(const GunHeatProxyModelDesc3& decs);

	void			update_heat_sources(float atmo_temp /*in K*/, float heater_power /*kW*/); /*atmo temperature and electric heater*/

private:
	EagleFM::Heat::HeatBody barrel_;
	EagleFM::Heat::HeatBody body_;
	EagleFM::Heat::HeatBody atmosphere_;
	EagleFM::Heat::HeatGroup heat_group_;
};
}

#endif