#ifndef GUN_HEAT_PROXY_DECS_H 
#define GUN_HEAT_PROXY_DECS_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

namespace weapons_gun { class wGun; }

namespace cockpit {

struct GunHeatProxyBaseDesc
{
	GunHeatProxyBaseDesc()
		: gun_(nullptr){}

	ed::string name_;
	weapons_gun::wGun* gun_;
};
}

#endif