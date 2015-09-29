#ifndef GUN_HEAT_PROXY_H 
#define GUN_HEAT_PROXY_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "PropertyTable.h"
#include "Avionics/Weapon/GunHeating/GunHeatProxyDecs.h"

namespace weapons_gun{ class wGun; }

namespace cockpit {

class GunHeatProxyBase
{
protected:
	GunHeatProxyBase(const GunHeatProxyBaseDesc& desc);
public:

	~GunHeatProxyBase();

	double temperature() const;

protected:
	MaskingPropertyPtr* temp_ptr_;
	weapons_gun::wGun* gun_ptr_;
	ed::string group_name_;
};
}

#endif