#pragma once

#include "Avionics/Sensors/avIRSensor.h"

namespace cockpit {

//class eqSidewinder;

enum SWSEEKER_STATUS
{
	SWSKR_SCAN = 4,
	SWSKR_SLAVED
};

enum SWSEEKER_COMMAND
{
	SWSKR_TO_SCAN = 8,
	SWSKR_TO_SLAVE
};

class COCKPITBASE_API avSidewinderSeeker :	public avIRSensor
{
public:
	avSidewinderSeeker();
	virtual ~avSidewinderSeeker() {}

	virtual void initialize();
	void update(double delta);
	virtual void clear();
//	void setParent(eqSidewinder* ParentIn) {Parent = ParentIn;}
	void setParentType(const wsType &ParentIn) {ParentType = ParentIn;}

	// not avDevice
	virtual bool setCommand(unsigned Command, double Value = 0.0);

	virtual float getLockonDistance(const woPointer objPtr) const;
	wsType getParentType() const;

	double getScanVelocity() const {return ScanVelocity;}
	void setScanVelocity(double ValIn) {ScanVelocity = ValIn;}
	
	double getScanRadius() const {return ScanRadius;}
	void setScanRadius(double ValIn) {ScanRadius = ValIn;}

	bool getIsGimbalLimit() const {return IsGimbalLimit;}
	float getLockStrength() const {return lockStrength;}

	void slaveToDirection(const dVector& DirectionIn);
	void setInBoresight(bool Flag) {InBoresight = Flag;}

	float getGimbalLimit() const {return GimbalLimit;}
	void setGimbalLimit(float ValIn) {GimbalLimit = ValIn;}

protected:
    void initLimits();
	virtual void setTarget(const woPointer Item);
	virtual void update_angles(double delta);

	virtual bool check_gimbal_limits();
	virtual bool gimbal_limits(const double k) const;

	virtual void onAxesStop();

	virtual void cage();
	void toScan();
	void toSlave();
	
//	eqSidewinder* Parent;
	wsType	ParentType;
	
	bool	IsGimbalLimit;
	
	float	GimbalLimit;
	double	ScanVelocity;
	double  ScanRadius;

	dVector LocSlaveDir;

	float   lockStrength;

	bool    InBoresight;
};

}
