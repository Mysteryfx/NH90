#pragma once

#include "Base.h"

#include "Optimizable.h"

#include "FMDynamics/DynamicBody.h"
#include "FMFuelSystem/FuelPipeline.h"

#include "FMSpace/IBasicAtmosphere.h"

#include "FMEngines/JetEngineElements.h"

#include "FMEngines/PistonEngineParams.h"

#include "FMEngines/PistonEngineAirDuctDefs.h"

namespace EagleFM
{
 
class EffectManager;

/* Base Piston Engine cylinder*/
struct FMBASE_API Cylinder
{
	virtual ~Cylinder(){}
	//характеристики
	double	angleBias;
	//параметры
	double	phase;
	double	chamberHeight;
	double	pressure;
	double	temperature;
	double	alpha;

	double	moment;

	bool	ignitionFlag;
	bool	exhaustFlag;

	double	exhaustPressure;
};

/* Base Piston Engine Cylinder Block*/
class FMBASE_API CylinderBlock 
{
public:
	CylinderBlock();
	virtual ~CylinderBlock();

	virtual void initialize(double inP /*init pressure*/, double inT /*init temperature*/);
	size_t count() const { return cs_.size(); } 

	double	compressionRatio;
	double	area;
	double	deadHeight;
	double	stroke;
	double	volume;

	ed::vector<Cylinder*> cs_; /*cylinders*/

	double	exhaustStartAng;
	double	exhaustEndAng;
	double	suctionStartAng;
	double	suctionEndAng;
	//параметры
	double	shaftPhase;
	double	ignitionAng;
};

struct FMBASE_API InjectorCylinder : public Cylinder
{
	bool	inj_Flag_;
	double	injection_;
};

class FMBASE_API InjectorCylinderBlock : public CylinderBlock
{
public:
	InjectorCylinderBlock();

	virtual void initialize(double inP /*init pressure*/, double inT /*init temperature*/) override;

	double	Injection_ang_;
};

//Модель двигателя внутреннего сгорания
class FMBASE_API PistonEngine : public Optimizable
{
//	--------------------		ФУНКЦИИ		---------------------
public:
	PistonEngine(DynamicBody*, EffectManager*, double dt = 0.04);
	virtual ~PistonEngine();
	
	void			setFuelPipeline(FuelPipeline *fpipeline) { if(fpipeline) fuelPipeline = fpipeline; }

	//void			setOilTemperature(double t) {}
	//void			setAfterCoolerCoolantTemperature() {}
	//void			setEngineCoolantTemperature() {}

	virtual void	initAirStart();
	virtual void	initHotStart();
	virtual void	initColdStart();

protected:
	//	имитация
	virtual void	doSimulate();
	virtual void	slow_simulate(); /*slow model rootine*/
	virtual void	simulateFuelFlow();

	virtual void	initBody() = 0;

public:

	//	Доступ
	double			getShaftOmega()				const	{ return shaftOmega; }
	double			getShaftRPM()				const	{ return shaftOmega*Omega_to_RPM; }
	double			getRelativeShaftRPM()		const	{ return shaftOmega/shaftOmega100; }
	virtual double	getShaftMOI()				const	{ return shaftMOI; }  /*can be modified during start*/
	double			getFuelFlow()				const	{ return fuelFlow; } 
	double			getPowerOverall()			const	{ return power_overall; }
	double			getExhaustThrust()			const	{ return exhaustThrust; }
	
	double			getManifoldPressure()		const	{ return manifold_P; }
	virtual double	getManifoldTemperature()	const	{ return manifold_T; }

	double			getBlowerPressure()			const	{ return blower_P; }
	double			getBlowerTemperature()		const	{ return blower_T; }

	double			getHeatToWater()			const	{ return heatToWater; }
	double			getHeatToOil()				const	{ return heatToOil; }
	
	const AirFlow&	getAirFlow()				const { return engineAirFlow_; }

	virtual void	setManifoldTemperature(double temp)	{ } /*external temperature set (for ex. Radiator)*/

	void			set_init_p_t(); /*init cylinders block, set intit pressure and temperature*/

	virtual void	set_throttle(double throttle_pos) { throttle = throttle_pos; }
	virtual double	get_throttle() const { return throttle;} 

	virtual double	getParam(int param)		const;

	//TODO:
	//double		getReactiveMoment_l()	const	{return Omega*Clockwise...} 

	double			calcEngineAirMassFlow(double manifoldPressure, double manifoldTemperature) const;

protected:
	virtual double	calcShaftMoment()				{ return 0.0; }
	virtual	double	calcFuelFlow() const			{ return 0.0; }
	virtual double  alpha_f()	const				{ return 1.0; }

	virtual void    after_Inlet_calc() {}		//handler for after inlet calculations 
	
	virtual CylinderBlock* create_cyl_block(); 

	virtual void	AMF_calculator();
	
	double			calcIgnitionK(double alfa) const;

	void init_cylinder_block(); 

//	---------------------		ПОЛЯ		-------------------------
//	не хозяин:
	DynamicBody*	dbody;
	FuelPipeline*	fuelPipeline;				// топливная магистраль
	EffectManager*	emanager;

//	хозяин:

	double			dtMain;						// dt быстрой модели

	JetInlet		inlet;

	AirDuct			air_duct_;

	JetNozzle		throttleNozzle;

	double			throttle;					// throttle lever pos

	//	Характеристики ДВС
	double			shaftMOI;					// момент инерции вала 
	double			shaftOmega100;				// рабочая частота вращения вала [рад/с]
	//double			fuelFlowTauInv;				// показатель сглаживания расхода топлива

	bool			fastModelOn;				//флаг работы быстрой модели (для гистерезиса переключения)
	double			shaftOmegaTransition;		//граница переключения с медленной модели на быструю
	
	double			shaftOmegaTerminal;			//до этих оборотов параметры компрессора лежат на рабочей линии 

	double			stallAirFlowK;				//коэф-ты ухудшения параметров при неустойчивой работе компрессора
	double			stallPiK;				
	double			stallNuK;

	double			power_indicator;			//индикаторная мощность [в лошадиных силах](For 1kg/s Air_Mass-Flow)
	double			pressureDeterorator;		//параметр расчета мощности
	double			m_fric0;					//момент трения при малых оборотах
	double			m_fric0_coeff;				//к-т для расчета момента трения при малых оборотах
	double			n_frict_coeff;				//к-ты расчета мощности трения по оборотам
	double			n_frict_coeff_1;
	double			n_frict_total_coeff;
	double			m_fric_slow;				//м-т трения в медленной модели

	double			exhaustThrustK;				// настроечный к-т тяги выхлопов
	double			exhaustArea;				// см2 - площадь сопла реактивного патрубка,

	double			nu0;	//ЭТО И НИЖЕ ДЛЯ МАНИФОЛДА
	double			compressRatio;			// степень сжатия
	double			manifoldPressureMax;	// max manifold pressure =1700 mmHg
	double			displacement;			// litres
	double			stroke;					// ход поршня, м
	double			v_pist_0;
	double			nu1;
	double			dnu2;					//constants for nu vs rpm curve

	double			p2_start_throttle_k;	//к-т для настройки разряжения в манифолде на запуске
	double			p2_start_throttle_lim;	//-||-
	double			primerFuelFlow_;

	//	Параметры работы ДВС
//private:
	double			shaftOmega;				//частота вращения вала 
	double			fuelFlow;				//расход топлива

	double			power_overall;			//выходная мощность		

	double			power_indic_act;		//мощность, выдаваемая блоком цилиндров
	double			blowerPower;			//мощность, потребляемая наддувом

	double			exhaustThrust;			//суммарная тяга выхлопов

	double			carburettorAlpha;		//альфа карбюратора :) /*cylinder alpha in injector engine*/
	double			evaporatedFuel;			//испаренное топливо
	double			liquidFuelFlow;			//недоиспаренное топливо

	AirFlow			engineAirFlow_;			//характеристики воздушного потока (меняются по тракту)
	double			beta;					//текущее отношение степени сжатия нагнетателя к расходу воздуха (используется как независимая переменная при решении уравнения)
	double			temperatureH;			//температура за бортом
	double			pressureH;				//давление на высоте
	double			temperature1;			//температура после воздухозаборника
	double			pressure1;				//давление после воздухозаборника
	double			blower_T;				//температура после нагнетателя
	double			blower_P;				//давление после нагнетателя
	double			manifold_T;				//manifold temperature
	double			manifold_P;				//manifold pressure

	double			ignitionEff;			//горение 0...1

	double			sparkPlugAbility;		//параметры чистоты свечей (0-грязные ... 1-чистые)
	double			sparkPlugEffiency;

	double			heatToWater;			//нагрев
	double			heatToOil;

	double			manifold_prime_alpha;
	double			manifoldEvaporatedFuel;	//испаренное топливо в манифолде

	//alphas
	double			last_slow_man_A;		//last slow model alpha
	double			manifoldAlpha;
	double			commonAlpha_;

	//		медленная модель
    Randomizer		rnd;
	double			mixtConsumption;
	double			shaftPhase;				//фаза вала двигателя (для цилиндров)



	double			liquidFuel; /*primer fuel*/ 
	double			vapourFuel; 
	double			fuelPartialPressure;

	CylinderBlock*	cyl_block;

	double		shaftCyclePhase; //фаза работы двигателя (для анимации выхлопов)

public:
	double			loadMoment;					// момент загрузки
	double			starterMoment_;				// момент стартера    TODO!!!! сделать как в реактивном движке вирт.функцию
	double			loadMOI;					// момент инерции загрузки

	bool			fuelOn;						// СТОП-кран
	bool			jetOpen;					// прайм-впрыск

	double			magnetosK;					// к-т горения от зажигания (0...1)

	double			temperatureBody;			//температура тела двигателя

	double			inletDeltaT;				//изменение температуры воздуха после ВЗ (подогрев телом двигателя)
	double			aftercoolerDeltaT;			//изменение температуры воздуха перед кабюратором (в радиаторе)

	double			altitudeAboveGround;		//высота над поверхностью (для учета в ветре)

	bool			broken;						//сломан (стопор)

#ifdef DEBUG
    double          frictionPower;
#endif

public:

	static double calcManifoldTemperature(double piWrk, double nuAdiabatic, double temperature);
	static double calcManifoldPressure(double pi, double pressure, double amf);
};

}

