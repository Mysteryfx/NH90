#ifndef __FuelPump_h__
#define __FuelPump_h__

#include "FMFuelSystem/IFuelElement.h"

namespace EagleFM
{

//топливный насос 
//имеет характеристики активного и пассивного (в выключенном состоянии) расхода

class FMBASE_API FuelPump : public IFuelElement
{
public:	
	FuelPump(double V = 1.0) : IFuelElement(V), ActiveDelivery(1.0), PassiveDelivery(0.0), IsActive(false) {}
	FuelPump(double AD, double PD, double V = 1.0) : IFuelElement(V), IsActive(false) 
	{
		setActiveDelivery(AD);
		setPassiveDelivery(PD);
	}

	void			setActiveDelivery(double val) { ActiveDelivery = __max(0.0, val); }
	void			setPassiveDelivery(double val) { PassiveDelivery = __max(0.0, __min(val, ActiveDelivery)); }

	virtual void	simulate(double dt);

private:
	double			ActiveDelivery;		//производительность в работающем состоянии
	double			PassiveDelivery;	//производительность в неработающем состоянии

public:
	bool			IsActive;
};

}

#endif