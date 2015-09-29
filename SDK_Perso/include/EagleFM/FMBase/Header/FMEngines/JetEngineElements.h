#pragma once

#include "Base.h"

#include "FMMathematics/math.h"
#include "FMEngines/IJetElement.h"

#include "unit_converter.h"

#include <ed/vector.h>

namespace EagleFM
{

class DynamicBody;
//"Кирпичики" реактивного двигателя любого типа:
//	- входное устройство 
//	- компрессор
//	- камера сгорания
//	- турбина
//	- сопло

//	Компрессор и турбина требуют подробные данные:
//	компрессор - в виде диаграммы "Степень сжатия = f(привед.обороты,привед.расход воздуха)",
//	турбина - зависимость "Привед.расход воздуха = f(Степень расширения)" и эллипс "кпд = f(норм.степ.расш., норм.привед.обороты)"
//  и используются для построения физичных моделей (основанных на физике процесса протекания воздуха по тракту)
//	реактивного (турбореактивного) двигателя

//		воздухозаборник  TODO: добавить поворот потока (если есть), ориентацию? точку приложения тяги от поворота потока

class FMBASE_API JetInlet : public IJetElement 
{
public:
	JetInlet(const Vec3& Pos = Vec3(0.0), double K_VelRec = 1.0);
	virtual ~JetInlet() {}

	void			setPos_l(const Vec3 & pos) { Pos_l = pos; } 
	void			setK_VelRec(double K_VelRec) {	K_VelRecovery = __max(0.0, __min(K_VelRec, 1.0)); }

	//инициализация параметров воздуха за бортом
	virtual void	calcInlet(DynamicBody *pDB, double &TemperatureH, double &PressureH, double *Density = 0); 

	//инициализация параметров воздуха за бортом и расчет параметров воздуха после воздухозаборника
	virtual void	calcInlet(DynamicBody *pDB, double AltAboveGr, double &TemperatureH, double &PressureH, double &Temperature, double &Pressure, double *Density = 0, double *TAS = 0);

    void			setAirDisturbance(double ampl)
    {
        disturbance_ampl = ampl;
    }

    const Vec3 &	getPos_l() const { return Pos_l;}
	double			getK_VelRec() const { return K_VelRecovery; }

protected:
    void            calcTurbulence(double &TemperatureH, double &PressureH);
	Vec3			Pos_l;	
	double			K_VelRecovery; //коэф-т потери скорости на входе (0...1 - закрыт от скоростного напора ... открыт полностью)
    // параметры турбулентности потока на входе
    double          disturbance_ampl;
    Math::IIR_Filter disturbance_filter;
    Randomizer      disturbance_rnd;
};

//		сверхзвуковой воздухозаборник

class FMBASE_API JetSuperSonicInlet : public JetInlet 
{
	JetSuperSonicInlet(Vec3 Pos = Vec3(0.0)) : JetInlet(Pos) {}
	virtual ~JetSuperSonicInlet() {}

	virtual void calcInlet(DynamicBody *pDB, double &Temperature, double &Pressure) {} //Under construction
};

//		камера сгорания

struct FMBASE_API JetCombustorInit
{
	JetCombustorInit()
	{
		PressureDrop		= 0.95;		
		AirToBurnerCoeff	= 0.2;	
		SpecificHeat		= 1.21;		
		Nu0					= 1.0;
		AlphaLimitLow		= 0.5;
		AlphaLimitHigh		= 3.7;
		AlphaLimitLow_K		= -0.00054;
		AlphaLimitHigh_K	= 0.0035;
		AlphaLimitLowPower	= 0.5;
		AlphaLimitHighPower = 0.5;
		Alpha1				= AlphaLimitLow;	
		Alpha2				= AlphaLimitHigh;
		CoeffAlphaExtern	= 1.0;
		CombastionChamberD	= 0.15;
		CombastionChamberN	= 6;
		CombastionChamberL	= 0.5;
		BodyHeatExchangeK	= 0;	//откл.нагрев
		BodyTemperatureK	= 17.0;
		BodyNaturalHeatTau	= 600.0;
		K_dFouling_dFuel	= 10.0;	
		K_dFouling_dAMF		= 0.1;
		FoulingAMF_Border	= 1.0;
		FoulingProbBorder	= 0.5;
	}

	double	PressureDrop;		
	double	AirToBurnerCoeff;	
	double	SpecificHeat;		
	double	Nu0;
	double	AlphaLimitLow;
	double	AlphaLimitHigh;
	double	AlphaLimitLow_K;
	double	AlphaLimitHigh_K;
	double	AlphaLimitLowPower;
	double	AlphaLimitHighPower;
	double	Alpha1;	
	double	Alpha2;
	double	CoeffAlphaExtern;
	double	CombastionChamberD;	
	double	CombastionChamberN;	
	double	CombastionChamberL;	
	double	BodyHeatExchangeK;	
	double	BodyTemperatureK;
	double	BodyNaturalHeatTau;
	double	K_dFouling_dFuel;	
	double	K_dFouling_dAMF;
	double	FoulingAMF_Border;
	double	FoulingProbBorder;
};

class FMBASE_API JetCombustor : public IJetElement
{
public:
	JetCombustor();

	JetCombustor(const JetCombustorInit&);

	void			init(const JetCombustorInit&);

	void			setTemperatureBody(double TemperatureB) { TemperatureBody = TemperatureB; }

	void			clean() { Fouling = 0.0; } //принудительная очистка (просушка) свечи

	void			setAirFlow(const AirFlow&);

	void			simulate(double dt); //помимо расчета здесь моделируется процесс смачивания свечи

	//вспомогательные функции
	double			calcFuelHeat() const //тепло от горения
	{
		return CombustionEffectiveness*FuelFlow*KeroseneHeat;
	}	

	double			calcBodyHeatExchangeCoeff(double AirMassFlow, double Temperature) const; //(kW/grad K)

	double			calcBodyHeatExchange(double _AirMassFlow, double _Temperature) const //обмен тепла с телом
	{
		return (_Temperature - TemperatureBody)*calcBodyHeatExchangeCoeff(_AirMassFlow, _Temperature);
	}

	double			calcAddTemperature(double DeltaQ, double _AirMassFlow) const //расчет добавки температуры от тепла
	{
		return DeltaQ/SpecificHeat/__max(_AirMassFlow, 0.001);
	}

	void			calcFullTemperature(double AirMassFlow, double &Temperature) const; //температура после КС с учетом BodyHeatExchange

	double			calcMultPressure(double _AirMassFlow) const
	{
		if(PressureDrop < 0.5) 
			return 1 - PressureDrop*_AirMassFlow;
		return PressureDrop;
	}

	static double	calcAlphaByPressure(double Pressure, double K, double Power, double AbsLimit)
	{
		short Sgn = K > 0.0 ? 1 : -1;
		return 1 + Sgn*__min(__max(0.0, fabs(pow(Pressure, Power)*K)), fabs(AbsLimit - 1));
	}

	AirFlow			getAirFlow() const;
	bool			getCombustion() const { return Combustion; }

	double			getPressureDrop() const { return PressureDrop; }

	double			getTemperatureBody() const { return TemperatureBody; }

	bool			Ignition;			//зажигание
	double			FuelFlow;

protected:
	//	Параметры работы
    Randomizer		Rnd;
	double			RndIgnition;		//для проверки вероятности зажигания

	double			Fouling;			//степень влажности свечи зажигания (0 ... 1)
	bool			Combustion;			//флаг горения
	double			AirMassFlow;
	double			Temperature;
	double			Pressure;
	double			CombustionEffectiveness; //эффективность горения (0...1)
	double			TemperatureBody;	//нагрев тела

	//	характеристики
	double			PressureDrop;		//коэф-т потери давления в КС (Pressure *= PressureDrop, если задано PressureDrop > 0.5, иначе Pressure*(1 - PressureDrop*AirMassFlow));
	double			AirToBurnerCoeff;	//Часть воздуха непосредственно на горение (0 ... 1)
	double			SpecificHeat;		//теплоемкость КС
	double			Nu0;				//КПД КС
	double			AlphaLimitHigh;		//для расчета границ возможного горения:
	double			AlphaLimitLow;
	double			AlphaLimitHigh_K;
	double			AlphaLimitLow_K;
	double			AlphaLimitHighPower;
	double			AlphaLimitLowPower;
	double			Alpha1;				//границы ухудшатора КПД (Nu = Nu0, если Alpha > Aplha1 и Alpha < Alpha2)			
	double			Alpha2;
	double			CoeffAlphaExtern;	//коэф-т расширения границ при подаче внешнего воздуха
	double			CombastionChamberD;	//диаметр кольцевой камеры сгорания (для расчета к-та нагрева тела)
	double			CombastionChamberN;	//кол-во камер (для расчета к-та нагрева тела)
	double			CombastionChamberL;	//длина камера (для расчета к-та нагрева тела)
	double			BodyHeatExchangeK;	//эмперический к-т (для расчета к-та нагрева тела)
	double			BodyTemperatureK;	//эмперический к-т для расчета приращения нагрева тела
	double			BodyNaturalHeatTau;	//тау остывания тела
	double			K_dFouling_dFuel;	//коэф-т смачиваемости свечи (K_dFouling_dFuel*FuelFlow)
	double			K_dFouling_dAMF;	//коэф-т продувки свечи (K_dFouling_dAMF*AirMassFlow)
	double			FoulingAMF_Border;	//предельный расход воздуха продувки (меньше - продувки нет)
	double			FoulingProbBorder;	//порог вероятности росжига топлива при заброшенной свечи (0...1: 0 - росжиг 100%)
};


//		турбина

struct FMBASE_API JetTurbineInit
{
	JetTurbineInit()
	{
		AMF_CorrMax = 10.0;
		PiTerminal = 1.5;
		Temperature0 = unitConv::CelciusToKelvin(0.0);
		Pressure0 = PressureMCA_SL; 
		ReToRe0Cryt = 0.001;//типа откл.
		NuModifierPower = 1.5;
		PiSummit = 3.0;
		RPM_CorrSummit = 10000.0; 
		EllipseAngle = 0.0;
		EllipseOblongness = 1.0; 
		AirWorkCondition = false;
	}

	double	AMF_CorrMax;
	double	PiTerminal;
	double	Temperature0;
	double	Pressure0; 
	double	ReToRe0Cryt;
	double	NuModifierPower;	
	double	PiSummit;
	double	RPM_CorrSummit; 
	double	EllipseAngle;
	double	EllipseOblongness; 
	bool	AirWorkCondition;
};

class FMBASE_API JetTurbine : public IJetElement
{
public:
	JetTurbine();
	JetTurbine(const JetTurbineInit&);
	
	void	init(const JetTurbineInit&);
	void	addNuData(double a, double Nu);

	void	setIF(double _IF) { IF = __max(0.0, __min(_IF, 1.0)); }

	double	calcPower(double AirMassFlow, double TurbinePi, double RPM, double &Temperature, double &Pressure);
	double	calcPowerByNu(double AirMassFlow, double TurbinePi, double TurbineNu, double &Temperature, double &Pressure); //то же самое, но при заранее расчитанном Nu
	
	//вспомогательные функции
	double	calcAirMassFlow(double TurbinePi, double Temperature, double Pressure); 
	double	calcMultTemperature(double TurbinePi, double Nu) const { return 1 - Nu*(1 - pow(1/TurbinePi, AdiabaticConstant)); }
	double	calcNu(double TurbinePi, double RPM, double Temperature, double Pressure);

	//доступ
	double	getTemperature0()	const { return Temperature0; }
	double	getPressure0()		const { return Pressure0; }
	double	getAMF_CorrMax()	const { return AMF_CorrMax;	}
	double	getIF()				const { return IF; }


protected:
	//		характеристики
	//для приведения (sqrt(Temperature)/sqrt(Temperature0), Pressure0/Pressure)
	double				Temperature0; 
	double				Pressure0;

	//для опр-я ухудшатора по числу Re при расчете мощности 
	double				ReToRe0Cryt;	//критическое число Re
	double				NuModifierPower;//степень при отношении чисел Re - скорость ухудшения КПД
	
	double				AdiabaticConstant;	//в зависимости от условий работы турбины либо AdiabaticConstantAir, либо AdiabaticConstantGas
	double				SpecificHeatP;		//-||- либо SpecificHeatAirP, либо SpecificHeaGastP  

	//описание характеристики "Привед.расход воздуха = f(степень расширения)"
	double				AMF_CorrMax; 
	double				PiTerminal;

	//описание характеристики "КПД = f(норм.степень расширения, норм.привед.обороты)" (эллипс)
	struct TurbineNuData{
		double	a;	//большая полуось эллипса
		double	Nu;	//КПД
	};
	typedef ed::vector<TurbineNuData> TurbineDataVector;
	TurbineDataVector	NuData;
	bool				NuDataEmpty; //TEMP!!!
	int					NuDataSize;
	double				RPM_CorrSummit;		//привед.обороты при макс.КПД (центр эллипса)		
	double				PiSummit;			//степень расширения при макс.КПД
	double				CosEllipseAngle;	//угол наклона эллипса (для перехода в его СК)
	double				SinEllipseAngle;	
	double				EllipseOblongness;	//вытянутость эллипса: отношение малой полуоси к большой (для нахождения эллипса)
	double				NuMin;				//границы КПД (определяются по таблице)
	double				NuMax;

	//		Параметры
	double				IF; //Износ турбины 1...0.91 (1 - без износа)
};

//		сопло 

class FMBASE_API JetNozzle : public IJetElement
{
public:
	JetNozzle(double _AreaMax = 1.0, bool AirWorkCondition = false); 
	virtual ~JetNozzle() {}

	void		init(double _AreaMax, bool AirWorkCondition);

	void		setPos_l(Vec3 P_l) { Pos_l = P_l; }
	void		setDir_l(Vec3 D_l);
	void		setAzimuth(double Val);
	void		setElevation(double Val);
	void		setAzimuthAndElevation(double Az, double El);

	void		setK_Vel(double K) { K_Vel = __max(0.0, __min(K, 1.0)); }

	void		setRelativeArea(double RArea)	{ Area = __max(RArea*AreaMax, 0.0); } //площадь сопла можно регулировать

	double		calcAirMassFlow(double Temperature, double Pressure, double PressureH) const; //расчет расхода воздуха по температуре и давлению перед соплом и давлению после сопла

	double		calcFlowVelocity(double Temperature, double Pressure, double PressureH); //расчет скорости истечения газа по температуре и давлению перед соплом и давлению после сопла
	double		calcFlowRelativeVelocity(DynamicBody *pDBody, double AltAboveGr, double Temperature, double Pressure, double PressureH); //расчет разности скорости истечения газа из сопла и скорости движения воздуха снаружи

	Vec3		getPos_l()			const { return Pos_l; }
	Vec3		getDir_l()			const { return Dir_l; }
	double		getRelativeArea()	const { return AreaMax > 0.0 ? Area/AreaMax : 0.0; }


protected:
	double		Area;
	double		AreaMax;
	double		K_Vel;				//к-т скорости выхода
	double		AdiabaticConstant;	//в зависимости от условий работы сопла либо AdiabaticConstantAir, либо AdiabaticConstantGas
	double		SpecificHeatP;		//в зависимости от условий работы сопла либо SpecificHeatAirP, либо SpecificHeaGastP  
	double		Kg;					//в зависимости от условий работы сопла либо KgAir, либо KgGas
	double		P_P_Cryt;			//в зависимости от условий работы сопла либо P_P_CrytAir, либо P_P_CrytGas

	
	Vec3		Pos_l;				//положение среза в ЛСК ЛА
	Vec3		Dir_l;				//вектор направления в ЛСК ЛА (ось Х смотрит вперед по полету)
	double		Elevation;			//углы поворота в ЛСК ЛА 
	double		Azimuth;
};

//		различные дренажные отверствия, клапаны и пробоины представляются соплами компрессора

class FMBASE_API JetCompressorNozzle : public JetNozzle
{
public:
	JetCompressorNozzle(double _AreaMax = 1.0, double PowerPi = 1.0, bool AirWorkCondition = true); 

	void		init(double _AreaMax, double _PowerPi, bool AirWorkCondition);

	//расчет параметров воздуха в сопле по параметрам компрессора и воздуха на входе в компрессор
	void		calcCompressorFlow(double CompressorPi, double CompressorNu, double &Temperature, double &Pressure); 


private:
	double		PowerPi;			//степень сжатия на компрессоре (Pi^PowerPi) 0...1 (0 - начало компрессора, 1 - конец)
};
typedef ed::vector<JetCompressorNozzle> JetCompressorNozzleVector; 


}