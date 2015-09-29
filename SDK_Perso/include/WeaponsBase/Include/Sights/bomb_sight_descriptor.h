#ifndef __BOMB_SIGHT_DESCR_H__
#define __BOMB_SIGHT_DESCR_H__

#include "WeaponsBase.h"
#include "descriptor_base.h"
#include "wsType.h"

// Данные для бомбового прицела
class WEAPONSBASE_API wBombSightDataDescriptor : public wDescriptorBase
{
public:
    wBombSightDataDescriptor(const ed::string& name) : wDescriptorBase(name) {}

    virtual void load(wDatabaseLoader* loader);

    double getCharTime(double height, double vel);

	double getBombletCharTime(){ return bomblet_char_time_; };

	wsType getType(){ return type_; };

protected:
    struct AimingEntry_
    {
        double height, char_time, dv;
    };

    double charTime_;
    double v0_;
    ed::vector<AimingEntry_> charTimes_;	 // first - height, second - char_time
	double bomblet_char_time_;

    wsType type_;
};

WEAPONSBASE_API wBombSightDataDescriptor* getBombSightDataByWsType(const wsType& wstype);

#endif // __BOMB_SIGHT_DESCR_H__
