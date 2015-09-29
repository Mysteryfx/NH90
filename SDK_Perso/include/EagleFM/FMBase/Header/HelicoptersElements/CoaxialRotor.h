#ifndef __CoaxialRotor_h__
#define __CoaxialRotor_h__

#include "HelicoptersElements/Rotor.h"
#include "HelicoptersElements/FlowCoaxialRotorModel.h"

namespace EagleFM
{

class FMBASE_API CoaxialRotor : public Rotor
{
// ----------------------------- МЕТОДЫ ---------------------------------------
public:
	CoaxialRotor(DynamicBody *DBody, AerodynamicBody *ADBody, RigidBody *RBody, EffectManager* EffManager);
	virtual ~CoaxialRotor() { if(file)fclose(file);}

	void			createBlades(const BladeInit &UpRotor, int UpBladesNum, const BladeInit &DnRotor, int DnBladesNum, double K_Swiring = 0.2, double K_UpForDn = 0.1, int FRMSNum = 3, int BADSNum = 6); 

	// Работа
	virtual void	simulate(double dt); 

	virtual double	getInductiveFlow()	const;

private:
	void			createBlades(const BladeInit&, int, int FRMSNum = 3, int BADSNum = 6){} //запрет функции родича


// ----------------------------- ПОЛЯ ---------------------------------------

	int			UpRotorBladesNum;

public:
	double			DiffPitch;		


// ----------------------------- DEBUG ---------------------------------------
	Math::IIR_Filter		DebugFilterUpSn1;
	Math::IIR_Filter		DebugFilterUpSn2;

	Math::IIR_Filter		DebugFilterUpCs1;
	Math::IIR_Filter		DebugFilterUpCs2;

	Math::IIR_Filter		DebugFilterUpFl1;
	Math::IIR_Filter		DebugFilterUpFl2;

	Math::IIR_Filter		DebugFilterDnSn1;
	Math::IIR_Filter		DebugFilterDnSn2;

	Math::IIR_Filter		DebugFilterDnCs1;
	Math::IIR_Filter		DebugFilterDnCs2;

	Math::IIR_Filter		DebugFilterDnFl1;
	Math::IIR_Filter		DebugFilterDnFl2;


    bool			LogOn;
	double			TimeLog;
	double			TimeInterval;
	FILE			*file;
};

}

#endif