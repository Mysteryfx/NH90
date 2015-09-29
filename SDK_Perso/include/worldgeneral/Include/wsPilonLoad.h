//              file wsPilonLoad.h
#ifndef __wsPilonLoad_h__
#define __wsPilonLoad_h__

#include <ed/string.h>

#include "WorldGeneral.h"
#include "wsType.h"
#include "GraphMath\vector.h"

class WORLDGENERAL_API pylon_FM_related_data_
{
public:
      pylon_FM_related_data_():
      M_empty_(0),
      M_item_(0),
      Cx_empty_(0),
      Cx_item_(0)
      {

      }
      inline float get_mass(unsigned count) const { return M_empty_  + get_items_mass(count); }
      inline float get_items_mass(unsigned count) const { return count * M_item_; }
      inline float get_drag(unsigned count) const { return Cx_empty_ + get_items_drag(count);}
      inline float get_items_drag(unsigned count)  const { return count * Cx_item_; }
public:
    float M_empty_;
    float M_item_;
    float Cx_empty_;
    float Cx_item_;
};

enum kind_of_shipping_enum
{
	SUBMUNITION_ONLY,
	SUBMUNITION_AND_CONTAINER_SEPARATELY,
	SOLID_MUNITION,
};


enum pilon_release_type
{
	RELEASE_TYPE_RAIL,
	RELEASE_TYPE_CATAPULT,
	RELEASE_TYPE_HATCH,
	RELEASE_TYPE_ROTARY_LAUNCHER_3_SIDE = 3,
	RELEASE_TYPE_ROTARY_LAUNCHER_4_SIDE = 4,
	RELEASE_TYPE_ROTARY_LAUNCHER_6_SIDE = 6,
	RELEASE_TYPE_ROTARY_LAUNCHER_8_SIDE = 8,
};

inline bool isRotaryLauncher(unsigned type)
{
	return type >= RELEASE_TYPE_ROTARY_LAUNCHER_3_SIDE; 
}

class WORLDGENERAL_API wsPilonLoad // Задание исходной подвески пилона
{ 
public:
	wsType			weapon_type;	   // Тип подвески. (Тип контейнера для контейнерных НУРС.)
	int				weapon_count;	   // Количество на этом пилоне (Для НУРС - полное количество ракет на подвеске.)
	int				full_weapon_count; // Количество на этом пилоне (Для НУРС - полное количество ракет на подвеске.) 
									   //  не уменьшается в процессе стрельбы
	int				unloaded_weapon_count; // Требуется для кокректной разгрузки пилона
	wsType			container_type;	   // Тип начинки контейнера (Для контейнерных НУРС - тип ракет в контейнере)
	wsType			adapter_type;	   // name of adapter.  to ship as separate item 
	unsigned char	Type_;  // тип пилона: 0-рельсовый, 1-катапультный, 2 - люк
    Vector3         pos;
	Vector3			Fi_;// наклон оси пилона относительно оси x y z, градусы,в случае наличия коннектора и use_full_connector_position, применяются к позиции коннектора
    int				Num_pil_;// Номер пилона

    ed::string     LauncherCLSID;
    ed::string     connector_name;
    bool            use_full_connector_position;

	unsigned char   kind_of_shipping;
	const wsType &  get_loadout_type() const
	{
		if (can_contain_submunition())
			return container_type;
		else
			return weapon_type;
	}

	bool can_contain_submunition() const
	{
		return  weapon_type.Level1() == wsType_Weapon &&
			   (weapon_type.Level2() == wsType_GContainer && 
				weapon_type.Level3() == wsType_Cannon_Cont) ||
		        weapon_type.Level3() == wsType_Container;
	}

	wsPilonLoad();
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wsPilonLoad& data);

    pylon_FM_related_data_ FM_data;

	inline float get_mass(unsigned count)		 const { return FM_data.get_mass(count); }
	inline float get_items_mass(unsigned count)  const { return FM_data.get_items_mass(count); }
	inline float get_drag(unsigned count)		 const 
	{
		if (Type_ >= RELEASE_TYPE_HATCH) // in closed hatch
			return 0;
		return FM_data.get_drag(count);
	}
	inline float get_items_drag(unsigned count)  const 
	{
		if (Type_ >= RELEASE_TYPE_HATCH) // in closed hatch
			return 0;
		return FM_data.get_items_drag(count); 
	}

};
 
#endif