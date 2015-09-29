#ifndef GUN_HEAT_PROXY_DECS_3_H 
#define GUN_HEAT_PROXY_DECS_3_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Avionics/Weapon/GunHeating/GunHeatProxyDecs.h"

namespace cockpit {

struct GunHeatProxyModelDesc3 : public GunHeatProxyBaseDesc
{
	GunHeatProxyModelDesc3()
		:
		init_T_(0.0f) {}

	float init_T_; /*init temperature in K*/
};

}

#endif