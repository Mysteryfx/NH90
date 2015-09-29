#pragma once

#include <stdint.h>
#include "WeaponsBase.h"
#include "descriptor_base.h"

#include "Math/Vector.h"

namespace Lua { class Loader; }
namespace weapons_gun {
	class wGun; 
	class wGunDescriptor;
	class Gun_Effect_Desc;
}

class wAmmoSupplyDescriptor;
class IwoLA;
class IwoAIPilon;

class WEAPONSBASE_API wAircraftGunMountDescriptor : public wDescriptorBase
{
	friend class wAircraftGunMount;

	ed::string  short_name;
	ed::string display_name;
public:
	wAircraftGunMountDescriptor(const ed::string& name);
	~wAircraftGunMountDescriptor();

	virtual void load(wDatabaseLoader* loader);

	//create LA gun
	wAircraftGunMount* create(IwoLA* la, 
		int8_t gun_id, 
		Lua::Loader* cfg) const;

	//crate LA gunpod
	wAircraftGunMount* create(IwoLA* la, 
		IwoAIPilon* pilon,
		int8_t gun_id, 
		Lua::Loader* cfg) const;

	const weapons_gun::wGunDescriptor* getGunDescriptor() const { return gunDescriptor_; }
	const wAmmoSupplyDescriptor* getAmmoSupplyDescriptor() const { return ammoSupply_; }

	const Math::Vec3d& getAmmoSupplyPosition() const { return ammoSupplyPosition_; }
	const Math::Vec3d& getMuzzlePosition() const { return muzzlePosition_; }
	const Math::Vec3d& getEjectorPosition() const { return ejectorPosition_; }
	const Math::Vec3d& getEjectorDir() const { return ejectorDir_; }

	double              getAzimuthInitial() const { return azimuthInitial_;}
	double              getElevationInitial() const { return elevationInitial_;}
	const ed::string &  getConnectorName() const { return connectorName_; };
	const ed::string &  getEjectorConnectorName() const { return ejectorConnectorName_; };
	unsigned char	    getDropCartridge() const { return drop_cartridge_; }
	bool                getAftGunMount() const { return aft_gun_mount_; } 
	double              getEffectiveFireDistance() const { return effective_fire_distance_; } 
	float				getBarrelCircularError() const  { return barrel_circular_error_; }

	const ed::string & getShortName() const { return short_name; }
	const ed::string & getDisplayName() const { return display_name; }
protected:
	const weapons_gun::wGunDescriptor* gunDescriptor_;
	const wAmmoSupplyDescriptor* ammoSupply_;
	Math::Vec3d ammoSupplyPosition_;
	Math::Vec3d muzzlePosition_;
	Math::Vec3d ejectorPosition_;
	Math::Vec3d ejectorDir_;
	double      azimuthInitial_;
	double      elevationInitial_;
	ed::string connectorName_;
	ed::string ejectorConnectorName_;
	bool  aft_gun_mount_;

	float barrel_circular_error_;

	unsigned char  drop_cartridge_;
	double effective_fire_distance_;

	ed::vector<weapons_gun::Gun_Effect_Desc*> effects_;
};