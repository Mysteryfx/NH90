#pragma  once
#include "weapons.h"

#include <ed/vector.h>

#include "Lua/Config.h"

#include "armored_object_interface.h"

class wWeaponSystem;

typedef enum armour_scheme_parts
{
    ASP_hull_elevation = 0,
    ASP_hull_azimuth,
    ASP_turret_elevation,
    ASP_turret_azimuth,
};

class WEAPONS_API wArmour_scheme: public wIArmoredTarget
{
    typedef struct armour_scheme_sector_t { double min_angle, max_angle, k; };
    typedef ed::vector<armour_scheme_sector_t> armour_scheme_part_t;
    armour_scheme_part_t    scheme[4];
    double                  armour; // armour width in mm
    double                  armour_K; //
    double                  armour_rho; // плотность
    wWeaponSystem*          ws;

    int                     load_part(Lua::Config& c, armour_scheme_part_t& part_);
    double                  get_k(const armour_scheme_part_t& part_, const double angle_)const;
public:
    wArmour_scheme();
    virtual int load_armour_scheme(Lua::Config& c);
    virtual void set_ws(wWeaponSystem* ws_){ws = ws_;};
    virtual ArmorData getArmorData(const Math::Vec3d& point) const;
	virtual ArmorData getArmorData(const Math::Vec3d& point, bool chassis) const;
    virtual wIArmoredTarget::ArmorData getAverageArmor()const{return wIArmoredTarget::ArmorData(armour,armour_K,armour_rho);};
};
