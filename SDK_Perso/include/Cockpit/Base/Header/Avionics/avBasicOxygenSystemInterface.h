#pragma once

#include "CockpitBase.h"
#include "Avionics/avDevice.h"

namespace cockpit
{
	
class COCKPITBASE_API avBasicOxygenSystemInterface : public avDevice
{
public:
	~avBasicOxygenSystemInterface(){}

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void update(){};
	//end of interface

	float getOxyStarvEffect1(){return m_fEffect_1;};
	float getOxyStarvEffect2(){return m_fEffect_2;};

protected:
	avBasicOxygenSystemInterface(){}

	//////////////////////////////////////////////////////////////////////////
	//Startvation
	virtual void  calculate_starvation(double dt, float oxygen_pressure /*in Pa*/);

	float m_fO2_Starvation_limit;
	float m_fO2_Starvation;
	float m_fEffect_1;
	float m_fEffect_2;
	double m_NoOxytime;
	double m_OxyStarvDeathTime;

private:

};

}