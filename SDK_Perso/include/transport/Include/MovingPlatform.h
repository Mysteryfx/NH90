#pragma once

class MovingPlatform
{
public:
	virtual ~MovingPlatform(){}

	virtual double tacticalSpeed() const = 0;
    virtual bool can_tacticalSpeed() const = 0; // платформа принципиально поддерживает установку тактической скорости
	virtual void setTacticalSpeed(double speed) = 0;
    //устанавливаем максимальную скорость в процентах от исходной (при повреждении, например)
    virtual void setTopSpeedHandicap(double _MaxSpeedK) = 0;
	virtual void setTacticalDir(const osg::Vec2d& dir) = 0;
    virtual bool can_TacticalDir() const = 0; // платформа принципиально поддерживает установку тактического направления
	virtual const osg::Vec2d& tacticalDir() const = 0;
	virtual bool tacticalDirArrived() const = 0;
    virtual bool tacticalDirFixed() const = 0;
	virtual bool isStaying() const = 0;
    virtual bool isStayingEx() const = 0;
    virtual int getGearNum() const = 0;
    virtual void get_swings(double& swing_p, double& swing_v, double& swing_r) const = 0;
};
