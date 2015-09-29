#pragma once

#include "Avionics/Sensors/avIRSensor.h"

namespace cockpit {

enum R73SEEKER_STATUS
{
	R73SKR_SLAVED = 4,
};

enum R73SEEKER_COMMAND
{
	R73SKR_TO_SLAVE = 8,
};

class COCKPITBASE_API avR73seeker : public avIRSensor
{
public:
	avR73seeker();
	virtual ~avR73seeker() {}

	virtual void initialize();
	void update(double delta);
	virtual void clear();
	void setParentType(const wsType &ParentIn) {ParentType = ParentIn;}

	// not avDevice
	virtual bool setCommand(unsigned Command, double Value = 0.0);

	virtual float getLockonDistance(const woPointer objPtr) const;
	wsType getParentType() const;

	double getScanVelocity() const {return ScanVelocity;}
	void setScanVelocity(double ValIn) {ScanVelocity = ValIn;}
	
	bool getIsGimbalLimit() const {return IsGimbalLimit;}

	void slaveToDirection(const dVector& DirectionIn);

	float getGimbalLimit() const {return GimbalLimit;}
	void setGimbalLimit(float ValIn) {GimbalLimit = ValIn;}

	static void	setSensivity(const double val){Sensivity = val;};
	static double	getSensivity(){return Sensivity;};

protected:
    void initLimits();
	virtual void setTarget(const woPointer Item);
	virtual void update_angles(double delta);

	virtual bool check_gimbal_limits();
	virtual bool gimbal_limits(const double k) const;

	virtual void onAxesStop();

	virtual void cage();
	void toSlave();
	
	wsType	ParentType;
	
	bool	IsGimbalLimit;
	
	float	GimbalLimit;
	double	ScanVelocity;

	dVector LocSlaveDir;

	static double	Sensivity;
};

}
