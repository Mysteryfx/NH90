// cLaserRangefinder.h
#ifndef __LASER_RANGER__
#define __LASER_RANGER__

#include "cLinear.h"
#include "wMovingObject.h"

struct LaserImpactData
{
    double  dist;
    dVector point1;
    dVector point2;
    ObjectID objID;
};

class cLaserRangefinder
{
public:
    cLaserRangefinder():m_maxRange(9999.0),m_range(0.0), m_isEnabled(true), m_isOn(false) {};
    cLaserRangefinder(double maxValue, bool isEnabled_, bool isOn):m_maxRange(maxValue),m_range(0.0),m_isEnabled(isEnabled_),m_isOn(isOn) {};

    double                      takeRange(const dVector &a, const dVector &b, ObjectID& targetID);
    double                      getRange() const { return m_range; };
    double                      getMaxRange() const { return m_maxRange; };
    inline double               resetRange() { return m_range = 0.0; };
    void                        turnOnOff(bool isOn) { if(m_isEnabled) m_isOn = isOn; };
    inline double               getOnOff() const { return m_isOn; };
    void                        setEnabled(bool enabled_);

 protected:
    double m_range; 
    double m_maxRange;
    bool   m_isOn;
    bool   m_isEnabled;
};
#endif // !__LASER_RANGER__
