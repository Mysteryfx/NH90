#pragma once

#include "Avionics/Sensors/IRMissiles/avR73Seeker.h"

namespace cockpit {

enum SDWND_STATUS
{
	SDWND_STAT_NA,
	SDWND_STAT_OFF,
	SDWND_STAT_COOL,
};

class COCKPITBASE_API eqR73: public avBasicElectric,
								     public avBreakable
{
public:
	eqR73(const wsType& type_ = wsType(0, 0, 0, 0));
	virtual ~eqR73() {}

	void initialize();
	void update(double delta);
	
	virtual void setElecPower(bool flag_in);

	const wsType& Type() const {return type;}
	int getMode() const {return 0;}

	void set_station(int p, int s = 0) 
	{
		pilon	 = p;
		subpilon = s;
	}

	unsigned int launch(IwHumanPlane* airplane);
	bool IsHeadReady() const;
	bool isTrainingWeapon() const;

	avR73seeker& getSeeker() {return Seeker;}
	const avR73seeker& getSeeker() const {return Seeker;}
	
	unsigned getStatus() const {return Status;}
	void setStatus(unsigned StatusIn);

	void slaveToDirection(const dVector& DirectionIn);

	const bool	&getIsSlave()const{return isSlave;};
	void		setIsSlave(bool valIn){isSlave=valIn;};

protected:
	avR73seeker	Seeker;

	wsType		type;
	int			pilon;
	int			subpilon;
	unsigned	Status;
	bool		isSlave;

	cVector		slaveDir;
};

}
