#ifndef __AMMUNITION_NURS_H__
#define __AMMUNITION_NURS_H__

#include "weapons.h"

#include "wsType.h"
#include "Ammunition/ammunition_descriptor.h"
#include "Sights/nurs_sight.h"

class wAmmunitionNURSDescriptor;

WEAPONS_API wAmmunitionNURSDescriptor* wGetAmmunitionNURSByType(wsType type);

// Смена типа - тяжелая операция и часто ее делать не стоит
// Если необходимо считать прицел для нескольких ракет одновременно, 
// лучше иметь несколько прицелов
WEAPONS_API void setNURS_SightByType(wNURSSight* sight, wsType type);
WEAPONS_API void setNURS_SightByDesc(wNURSSight* sight, const wAmmunitionNURSDescriptor* desc);

class wAmmunitionNURSDescriptor : public wAmmunitionDescriptor
{
public:
	wAmmunitionNURSDescriptor(const ed::string& name) : wAmmunitionDescriptor(name) {}

	virtual void load(wDatabaseLoader* loader);

	double getDistMin() const { return distMin_; }
	double getDistMax() const { return distMax_; }
	double getCxPil() const { return cxPil_; }

    const wNURSSight::EngineData& getSightEngineData() const { return sightEngineData_; }
    const wNURSSight::FMData& getSightFMData() const { return sightFMData_; }

protected:
	double distMin_, distMax_;		// Минимальная и максимальная эффективная дальность
	double cxPil_;					// Cx если висит

    // sighting
    wNURSSight::EngineData sightEngineData_;
    wNURSSight::FMData     sightFMData_;
};


#endif
