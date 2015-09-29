#ifndef __AIRCRAFT_GUN_EQUIPMENT_H__
#define __AIRCRAFT_GUN_EQUIPMENT_H__

#include "WeaponsBase.h"
#include "descriptor_base.h"

#include <ed/vector.h>

class wAircraftGunMountDescriptor;
class wAircraftGunMount;
class IwoLA;
class IwoAIPilon;

class wsType;

namespace Lua { class Loader;  class Config; }

// Этот класс опять же нужен потому что самолеты загружают свои данные не из дескрипторов
class WEAPONSBASE_API wAircraftGunEquipmentDescriptor
{
public:

	virtual void load(Lua::Config & config);

    // В cfg находятся дополнительные параметры пушек (для всех одинаковые!)
    // В таблице ammo находятся параметры ленты (количество патронов и т. д.)
    void createGuns(IwoLA                           * la,
                    Lua::Loader                     * cfg,
                    wAircraftGunMounts  & res) const;
protected:
	ed::vector<const wAircraftGunMountDescriptor*> gunMounts_;
};

//////////////////////////////////////////////////////////////////////////
void WEAPONSBASE_API createGunpodsByType(IwoLA* la, 
	IwoAIPilon*			pilon, 
	Lua::Loader			*cfg,
	wAircraftGunMounts	&res);

class WEAPONSBASE_API wAircraftGunpodEquipmentDescriptor : public wDescriptorBase
{
public:
	wAircraftGunpodEquipmentDescriptor(const ed::string name) : wDescriptorBase(name) {}

	virtual void load(wDatabaseLoader* loader);

	ed::vector<const wAircraftGunMountDescriptor*> gunMounts_;

	void createGunpod(IwoLA  * la, 
					  IwoAIPilon* pilon,
					  Lua::Loader* cfg, 
					  wAircraftGunMounts& res);
};

#endif 
