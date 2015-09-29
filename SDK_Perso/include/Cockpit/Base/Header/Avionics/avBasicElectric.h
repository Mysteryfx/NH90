#ifndef AVBASICELECTRIC_H
#define AVBASICELECTRIC_H

#include "CockpitBase.h"

namespace cockpit
{

// TODO: envisager la possibilité d'introduire des sources et des conducteurs
// Différents types d'énergie: électrique, hydraulique, pneumatique

// Dispositif électrique primaire
class COCKPITBASE_API avBasicElectric
{
public:
	avBasicElectric() : power(false) {}
	virtual void		setElecPower(bool flag_in) {power = flag_in;}
	virtual void		switchElecOnOff() {setElecPower(!getElecPower());}
	virtual bool		getElecPower() const {return power;}
protected:
	bool power;
};

}

#endif //AVBASICELECTRIC_H
