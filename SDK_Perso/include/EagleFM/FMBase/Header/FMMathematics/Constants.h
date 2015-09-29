#ifndef __Constants_h__
#define __Constants_h__

#include <math.h>

//различные константы

namespace EagleFM
{
	static const double Pi =  M_PI;
	static const double Pi05 = M_PI_2;
	static const double Pi2 = 2.0*M_PI;

	static const double KgAir = 1.4;			// Коэф-т k для воздуха (отношение теплоемкости газа в адиабатике Cp/Cv)
	static const double KgGas = 1.333;			// Коэф-т k для газа 
	static const double Deg_to_Rad = Pi/180.0;

	static const double Omega_to_RPM = 60.0/Pi2;			// перевод радиан в секунду в обороты в минуту
	static const double	EarthRadius = 6340000.0;		// Радиус Земли
	static const double	UniversalGasConstant = 8.314;	// Универсальная газовая постоянная
	static const double	AirMolecularWeight = 29.0;		// Молекулярный вес воздуха
	static const double	PressureMCA_SL = 101325;		// Давление на уровне моря стандартной атмосферы
	static const double	PressureAt_to_Pa = 98066.5;		// перевод технических атмосфер (кгс/см2) в Паскали
	static const double	HydrostaticConstant = 0.034;	// Газовая постоянная
	static const double	SoundSpeedFactor = 20.0; 		// Коэффициент для расчета скорости звука
	static const double	SpecificHeatAirP = 1005.0;		// Теплоемкость воздуха при постоянном давлении
	static const double	SpecificHeatGasP = 1149.0;		// Теплоемкость газа после камеры сгорания при постоянном давлении
	static const double	AdiabaticConstantAir = 1/3.5;	// показатель степени в адиабатике воздуха
	static const double	AdiabaticConstantGas = 1/4.0;	// показатель степени в адиабатике газа (горения) 
	static const double	KeroseneHeat = 42900.0;			// Теплоемкость керосина
	static const double	HeatToOil =	0.0085;				// отдача тепла в масло
	static const double	Stehiometric = 14.8;
	static const double	P_P_CrytAir = 0.528;			// критическое отношение давлений для воздуха
	static const double	P_P_CrytGas = 0.54;				// критическое отношение давлений для воздуха
	static const double	R = 287.0;
	static const double	AirAdiab1 = 1.0/AdiabaticConstantAir;
	static const double	US_GAL_to_cu_m = 0.003785411784;		//convert US Galons to cubic meters
	static const double	M_02	=	0.032;						//O2 mol mass (kg/mol) 
	static const double	Hoursepower_to_Watt = 745.699872;		//перевод мощности в лошадиных силах в мощность в ваттах

}

#endif