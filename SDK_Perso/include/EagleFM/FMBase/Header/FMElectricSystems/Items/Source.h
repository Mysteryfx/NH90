#pragma once

#include "Switch.h"

namespace EagleFM { namespace Elec {

class FMBASE_API Source : public Switch 
{
public:
	Source();
	virtual ~Source();

	virtual bool connectBeforeMe(ItemBase* itemIn);
	virtual void updateOwnVoltage(double valIn = -1.0);
	virtual bool canReceiveFrom(const ItemBase* pItem) const;

	virtual void setNominalVoltage(double nominal);
	virtual void setIsSourceLine();

	virtual double getInternalImpedance() const {return internalImpedance;}
	void setInternalImpedance(double valIn);
	
	virtual double getInternalConductance() const {return internalConductance;}
	
	double getEMF() const;
	double getShortCircuitCurrent() const;
	virtual double getCurrent() const;
	
	bool getIsOnline() const;
	void setIsOnline(bool valIn);

	void setModelingTime(double dtIn) {_dt = dtIn; setCallTime(dtIn);}
	double getModelingTime() const {return _dt;}

	void set_tC(double valIn) {tC = valIn;}
	double get_tC()	const { return tC; }
	// If you want to disable overheating, just set all values to zeroes.
	void setHeatingParameters(double _Ctemp, double _Ktemp, double _tCmax)
		{Ctemp = _Ctemp; Ktemp = _Ktemp; tCmax = _tCmax;}
	void updateTemperature(double dt, double t_ambC, double power);

protected:
	void setCallTime(double dt);
	bool updateTime();
	void updateOwnVoltageWhenOffline();

	//double getBusVoltageLcl() const;

	double internalImpedance;
	double internalConductance;
	double EMF; // ElectroMotive Force
	//double busVoltage;
	bool isOnline;

	double _dt; // internal time step

	double Ktemp; // Heat transfer coefficient (coefficient de transfert de chaleur), Вт / K ou Вт / C
	double Ctemp; // Coefficient of heat capacity (coefficient de capacité calorifique), Дж/K et Дж/C
	double tC;    // Current temperature of the source (Celcius)
	double tCmax; // Maximum work temperature (Celcius)

	double callTime, prevCallTimeWhenIsOffline;
};

} }
