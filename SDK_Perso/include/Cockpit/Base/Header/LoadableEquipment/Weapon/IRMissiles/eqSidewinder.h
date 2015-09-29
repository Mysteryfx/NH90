#pragma once

#include "Avionics/Sensors/IRMissiles/avSidewinderSeeker.h"

namespace cockpit {

enum SDWND_STATUS
{
	SDWND_STAT_NA,
	SDWND_STAT_OFF,
	SDWND_STAT_COOL,
	SDWND_STAT_RDY
};

class COCKPITBASE_API eqSidewinder : public avBasicElectric,
								     public avBreakable
{
public:
	eqSidewinder(const wsType& type_ = wsType(0, 0, 0, 0));
	virtual ~eqSidewinder() {}

	void initialize();
	void update(double delta);
	
	virtual void setElecPower(bool flag_in);

	const wsType& Type() const {return type;}
	
	void set_station(int p, int s = 0) 
	{
		pilon	 = p;
		subpilon = s;
	}

	unsigned int launch(IwHumanPlane* airplane);
	bool IsReadyToFire() const;
	bool isTrainingWeapon() const;

	avSidewinderSeeker& getSeeker() {return Seeker;}
	const avSidewinderSeeker& getSeeker() const {return Seeker;}
	
	unsigned getStatus() const {return Status;}
	void setStatus(unsigned StatusIn);

protected:
	avSidewinderSeeker Seeker;

	wsType		type;
	int			pilon;
	int			subpilon;
	unsigned	Status;
};

}
