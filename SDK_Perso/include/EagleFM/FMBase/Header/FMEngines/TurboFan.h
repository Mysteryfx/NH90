#pragma once

#include "Base.h"

#include "FMEngines/JetEngine.h"
#include "FMEngines/JetEngineElements.h"
#include "FMEngines/Compressor.h"

namespace EagleFM
{

// Модель типового турбовентилятора, позволяющая получить тягу двигателя по сумме тяг из сопел внешнего и 
// внутреннего контуров. 
// Основана на определении расхода воздуха через внутренний контур по условию баланса расходов через компрессор,
// внешний контур (вентилятор - внешнее сопло) и турбины с внутренним соплом.
// Решение организовано следующим образом:
// по приведенным оборотам и параметрам воздуха после воздухозаборника
// в вентиляторе находится кривая "Степень сжатия = f(Расход воздуха)",
// по рабочей линии определяются степень сжатия и КПД вентилятора, сжимается и нагревается воздух, 
// подается в компрессор. В компрессоре также по кривой и рабочей линии на ней (сначала, далее по бете
// из предыдущего шага) определяется начальный расход воздуха, степень сжатия и КПД. По заданному расходу
// в вентиляторе определяются параметры воздуха перед компрессором (давление, температура) путем поиска
// расхода через вентилятор = расход через компрессор (задан) + расход через внешнее сопло (1й внутренний решатор).
// Далее получается воздух за компрессором: давление и температура. Подогревается в КС, пропускается через
// турбины и внутреннее сопло (2й внутренний решатор): используя якобиан определяются сбалансированные 
// степени расширения турбин (условие равенства: расход через турбину компрессора = расход через турбину 
// вентилятора = расход через внутренне сопло). Разница между текущим расходом через компрессор
// и расходом через турбины и внутренне сопло сводится к минимуму (внешний решатор, незавимисмая
// переменная - бета компрессора) путем приращения текущей беты компрессора, 
// поиска производной Дельты (реакции на приращение) и определения баланса с учетом производной. 
// По найденному, соответсвующему полученной бете, расходу воздуха определяются мощности на валах,
// что дает интегрирование их вращения, и тяги сопел через разницу скоростей истечения и наружного воздуха.

class FMBASE_API TurboFan : public JetEngine
{

// ----------------------------- МЕТОДЫ ---------------------------------------
public:
	TurboFan(DynamicBody *DBody, FuelPipeline *pFPipeline = 0, double dt = 0.04);
	virtual ~TurboFan() {}

protected:
	virtual void				doSimulate();

	virtual double				calcGG_Moment() { return GG_Moment; }
	virtual double				calcFanMoment() { return FanMoment; }

	virtual double				calcFanBleedAMF(double FanPi, double FanNu);
	virtual double				calcComprBleedAMF(double ComprPi, double ComprNu);


	//				расчет потока через вентилятор и мощности вентилятора (по заданному расходу воздуха через внутреннюю часть вентилятора и входным параметрам), а также момента авторотации на нач.участке
	//				возвращается общий расход воздуха через вентилятор = расход воздуха через внешнюю часть (внешнее сопло + пробоины) + расход воздуха через внутренню часть (идущую на вход компрессора)
	double						calcFanAirFlow(double CoreAMF, double &T_Fan2, double &P_Fan2, double &T_Compr1, double &P_Compr1, double &FanPower, double &FanWindmilMoment, double TAS, double AtmoDensity);

	//				расчет расхода воздуха через турбины и сопло внутреннего контура при данных параметрах потока за КС (Old = true - используются текущие значения Pi турбин при инициализации)
	double						calcTurbinesAndNozzleGasFlow(const AirFlow&, double &GGT_Pi, double &FT_Pi, bool Old = false); 

	//	Доступ
public:

	double						getFanOmega()			const { return FanOmega; }
	double						getRelativeFanRPM()		const { return FanOmega/FanOmega100; }
	double						getFanRotationPhase()	const { return FanRotationPhase; }
	double						getCoreTurbinePower()	const { return GG_TurbinePower; }
	double						getFanTurbinePower()	const { return FanTurbinePower; }
	Vec3						getCoreThrust_l()		const { return CoreThrust_l; }
	Vec3						getFanThrust_l()		const { return FanThrust_l; }
	Vec3						getCoreThrustPos_l()	const { return GG_Nozzle.getPos_l(); }
	Vec3						getFanThrustPos_l()		const { return FanNozzle.getPos_l(); }

	double						calcFuelHeat()			const { return Combustor.calcFuelHeat(); }

    JetInlet                &   getInlet() { return Inlet; }
    // ----------------------------- ПОЛЯ ---------------------------------------    
protected:
	//	Элементы турбовентилятора
	JetInlet					Inlet;					//воздухозабрник
	JetCompressor				Fan;					//вентилятор
	JetNozzle					FanNozzle;				//сопло вентилятора
	JetCompressor				Compressor;				//комрпессор
	JetCombustor				Combustor;				//камера сгорания
	JetTurbine					GG_Turbine;				//турбина компрессора
	JetTurbine					FanTurbine;				//турбина вентилятора
	JetNozzle					GG_Nozzle;				//сопло внутреннего контура
	JetCompressorNozzleVector	FanNozzles;				//в конкретном двигателе можно добавить любое отверствие в вентилятор (m = 0...1 - расположение отверствия "до вентилятора"..."после")
	JetCompressorNozzleVector	CompressorNozzles;		//в конкретном двигателе можно добавить любое отверствие в компрессор (m = 0...1 - расположение отверствия "до компрессора"..."после")

	//	Характеристики турбовентилятора
	double						FanMOI;					//момент инерции вала вентилятора
	double						FanOmega100;			//рабочая частота вращения вала вентилятора [рад/с]
	double						FanOmegaTerminal;		//до этих оборотов параметры вентилятора лежат на рабочей линии 
	double						GG_OmegaTerminal;		//до этих оборотов параметры компрессора лежат на рабочей линии 
	double						FanStallAirFlowK;		//коэф-ты ухудшения параметров при неустойчивой работе вентилятора
	double						FanStallPiK;				
	double						FanStallNuK;
	double						ComprStallAirFlowK;		//коэф-ты ухудшения параметров при неустойчивой работе компрессора
	double						ComprStallPiK;				
	double						ComprStallNuK;
	double						FanFrictionMoment;		//момент трения вала вентилятора
	double						GG_FrictionMoment;		//момент трения вала компрессора
	double						FanWindmilMomentK1;		//к-ты для настройки авторотации вентилятора при оборотах ниже терминальных
	double						FanWindmilMomentK2;
	double						FanWindmilMomentMin;	//ограничения момента авторотации вентилятора
	double						FanWindmilMomentMax; 
	double						GG_WindmilMomentK1;		//к-ты для настройки авторотации компрессора  при оборотах ниже терминальных
	double						GG_WindmilMomentK2;
	double						GG_WindmilMomentMin;	//ограничения момента авторотации компрессора
	double						GG_WindmilMomentMax;
	double						FanMomentMin;			//ограничения (по мощности на малых оборотах) момента вентилятора
	double						FanMomentMax; 
	double						GG_MomentMin;			//ограничения (по мощности на малых оборотах) момента компрессора
	double						GG_MomentMax;

	
	//	Параметры работы
	Vec3						CoreThrust_l;			//тяга внутреннего контура
	Vec3						FanThrust_l;			//тяга внешнего контура

	double						FanOmega;				//частота вращения вентилятора
	double						FanRotationPhase;		//фаза вращения вентилятора 0...Pi2

	double						GG_TurbinePower;		//мощность турбины внутреннего контура
	double						FanTurbinePower;		//мощность турбины внешнего контура

	AirFlow						CoreAirFlow;			//характеристики воздушного потока через внутренний контур (меняются по тракту)

	double						FanBeta;				//текущее отношение степени сжатия вентилятора к расходу воздуха (используется как независимая переменная при решении уравнения)
	double						ComprBeta;				//текущее отношение степени сжатия компрессора к расходу воздуха (используется как независимая переменная при решении уравнения)

	double						TemperatureH;			//температура за бортом
	double						PressureH;				//давление на высоте
	double						TemperatureFan1;		//температура после воздухозаборника перед вентилятором
	double						PressureFan1;			//давление после воздухозаборника перед вентилятором
	double						TemperatureFan2;		//температура после вентилятора перед соплом внешнего контура
	double						PressureFan2;			//давление после вентилятора перед соплом внешнего контура
	double						TemperatureCompr1;		//температура после вентилятора перед компрессором
	double						PressureCompr1;			//давление после вентилятора перед компрессором
	double						TemperatureCompr2;		//температура после компрессора
	double						PressureCompr2;			//давление после компрессора
	double						Temperature3;			//температура после камеры сгорания
	double						Pressure3;				//давление после камеры сгорания
	double						Temperature4;			//температура после турбины компрессора
	double						Pressure4;				//давление после турбины компрессора
	double						Temperature5;			//температура после турбины вентилятора перед соплом внутреннего контура
	double						Pressure5;				//давление после турбины вентилятора перед соплом внутреннего контура

	double						SurgeThrottle;			//дроссель помпажа (0 - нет ... 1 - помпаж, но вообще подбирается по конкретному двигателю)

	double						GG_Moment;				//крутящий момент на валу компрессора

	double						FanMoment;				//крутящий момент на валу вентилятора


// ----------------------------- DEBUG и TEMP ---------------------------------------    
};

}