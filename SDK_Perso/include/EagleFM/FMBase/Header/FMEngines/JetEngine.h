#pragma once

#include "Base.h"

#include "Optimizable.h"

#include "FMDynamics/DynamicBody.h"
#include "FMSpace/IBasicAtmosphere.h"
#include "FMFuelSystem/FuelPipeline.h"

namespace EagleFM
{
 
class FMBASE_API JetEngine : public Optimizable
{
//	--------------------		ФУНКЦИИ		---------------------
public:
	JetEngine(DynamicBody*, FuelPipeline* = 0, double dt = 0.04);
	virtual ~JetEngine() {}
	
	void			setFuelPipeline(FuelPipeline *pFPipeline) { if(pFPipeline) pFuelPipeline = pFPipeline; }

protected:
	//	имитация
	virtual void	doSimulate();
	void			simulateFuelFlow();
	void			simulateComprOmega();

public:

	//	Доступ
	double			getComprOmega()			const	{ return GG_Omega; }
	double			getRelativeComprRPM()	const	{ return GG_Omega/GG_Omega100; }
	double			getComprRotationPhase()	const	{ return GG_RotationPhase; }
	double			getFuelFlow()			const	{ return FuelFlow; } 
	
	enum OutParam
	{
		EOP_GG_RPM,
		EOP_T2,
		EOP_T3,
		EOP_T4,
		EOP_FLAME,
		EOP_PI_TGG,
		EOP_NU_TGG,
		EOP_AMF		
	};
	virtual double	getParam(int param)		const { return 0.0; }

	//TODO:
	//double		getReactiveMoment_l()	const	{return Omega*Clockwise...} 

protected:
	virtual double	calcGG_Moment()					{ return 0.0; }
	virtual	double	calcFuelFlow()					{ return 0.0; }


//	---------------------		ПОЛЯ		-------------------------
//	не хозяин:
	DynamicBody		*pDBody;
	FuelPipeline	*pFuelPipeline;				// топливная магистраль

//	хозяин:

	//	Характеристики ТРД
	double			GG_MOI;						// момент инерции вала газогенератора
	double			GG_Omega100;				// рабочая частота вращения вала [рад/с]
	double			FuelFlowTauInv;				// показатель сглаживания расхода топлива

	//	Параметры работы ТРД
//private:
	double			GG_Omega;					// частота вращения вала 
	double			GG_RotationPhase;			// фаза вращения вала 0...Pi2
	double			FuelFlow;					// расход топлива

public:
	double			Throttle;					// положение РУД
	bool			FuelOn;						// СТОП-кран

	double			AltitudeAboveGround;		// высота над поверхностью (для учета в ветре)
};

}

