#pragma once

//	компрессор - в виде диаграммы "Степень сжатия = f(привед.обороты,привед.расход воздуха)",

#include "Base.h"

#include "FMEngines/IJetElement.h"

#include <ed/vector.h>
#include <array>

namespace EagleFM {

class FMBASE_API CompressorBase : public IJetElement
{
	public:
		CompressorBase();
		CompressorBase(double Temperature0, double Pressure0, double ReToRe0Cryt, double NuModifierPower);
		virtual ~CompressorBase() {}

		virtual void	init(double Temperature0, double Pressure0, double ReToRe0Cryt, double NuModifierPower);

		
		//расчет температуры нагрева в компрессоре по заданным параметрам (домножается на текущую)
		static double	calcMulTemperature(double ComprPi, double NuAdiabat) 
		{
			return 1 + (pow(ComprPi, AdiabaticConstantAir) - 1)/__max(NuAdiabat, 0.001);
		}

		//расчет КПД компрессора по температурам до и после и степени сжатия
		static double	calcNuByTemperatureAndComprPi(double Temperature1, double Temperature2, double ComprPi)
		{
			if (Temperature2 == Temperature1 ||
				ComprPi      <= 1.0)
			{
				return 1.0;
			}
			else
			{
				return (pow(ComprPi, AdiabaticConstantAir) - 1)/(Temperature2/Temperature1 - 1);
			}
		}

		//расчет мощности компрессора по заданным параметрам
		static double	calcPower(double AirMassFlow, double ComprPi, double ComprNu, double Temperature /*in temperature*/)
		{
			return SpecificHeatAirP*AirMassFlow*(Temperature*CompressorBase::calcMulTemperature(ComprPi, ComprNu) - Temperature);
		}

		static double	calcPower(double AirMassFlow, double ComprPi, double ComprNu, double T_In /*in temperature*/, double T_Out /*out temperature*/)
		{
			return SpecificHeatAirP*AirMassFlow*(T_Out - T_In);
		}

		//расчет момента авторотации по заданным параметрам
		static double	calcWindmillingMoment(double TAS, double AtmoDensity, double RPM, double K1, double K2)
		{
			return AtmoDensity*TAS*TAS*TAS*K1*(K2*TAS - RPM)/__max(0.01, RPM)/10000.0;
		}

	protected:
		//	характеристики
		//диаграмма компрессора 
		double			Temperature0;		//для расчета приведенных параметров
		double			Pressure0;
		double			ReToRe0Cryt;		//критическое число Re
		double			NuModifierPower;	//степень  - скорость ухудшения КПД отношения чисел Re
		
};

struct FMBASE_API JetCompressorData //набор данных при заданных приведенных оборотах (используется для описания диаграммы компрессора)
{
	JetCompressorData()
	{
		RPM_Corr = 1.0;
		AirMassFlow1 = 11.0;
		AirMassFlow2 = 10.5;
		AirMassFlow3 = 10.0;
		ComprPi1 = 1.1; 
		ComprPi2 = 1.0;
		ComprPi3 = 0.9;
		NuAdiabatic = 1.0;
		NuK1 = 20.0;
		NuK2 = 1.0;
	}

	double	RPM_Corr;
	double	AirMassFlow1, AirMassFlow2, AirMassFlow3;	// три точки параболы (переменная)
	double	ComprPi1, ComprPi2, ComprPi3;				// три точки параболы (функция)
	double	NuAdiabatic;// КПД компрессора
	double	NuK1;		// коэф-т экспоненты ухудшения КПД по отколнению от рабочей линии
	double	NuK2;		// второй коэф-т (вес ухудшения при отклонении от рабочей степени сжатия) 										
};

class FMBASE_API JetCompressor : public CompressorBase
{
public:
	JetCompressor();
	JetCompressor(double Temperature0, double Pressure0, double ReToRe0Cryt, double NuModifierPower);
	virtual ~JetCompressor() {}

	void			setLimitBetaK(double val) { LimitBetaK = __max(0.0, val); }

	void			addData(const JetCompressorData&);

	void	setCorePiAndNuCoeffs(double CorePiK, double CoreNuK);

	//расчет пары степень сжатия/КПД для внутренней части по заданной паре внешней части (для вентиляторов - компрессоров низкого давления)
	void	calcCorePiAndNu(double &ComprPi, double &Nu)
	{
		ComprPi = 1 + (ComprPi - 1)*PiCoreCoeff;
		Nu *= NuFanToNuCore;
	}

	//				расчет рабочих (средних) параметров работы компрессора по текущим отн.оборотам и параметрам воздуха (расход, температура, давление).
	//				полученные параметры: отношение рабочей степени сжатия к рабочему расходу воздуха, тройка на рабочей (средней) линии: расход воздуха, степень сжатия, КПД.
	void	calcCompressorWork(double RelativeRPM, double Temperature, double Pressure, double &BetaWork, double &AirMassFlowWork, double &ComprPiWork, double &NuAdiabatWork) ;
	//				расчет параметров работы компрессора по текущим отн. оборотам, параметрам воздуха и заданному параметру Beta = ComprPi/AirMassFlow
	//				при наличии флага LimitBeta ограничивается Beta точками параболы Pi = f(AMF) при данных оборотах (строго снизу и нестрого сверху с допуском LimitBetaK для возможности помпажа)
	bool	calcCompressorBeta(double RelativeRPM, double &Beta, AirFlow& Flow, double &ComprPi, double &NuAdiabat, bool *LimitBeta = 0) ;//так же возвращает факт ограничения сверху
	//				расчет параметров работы компрессора по текущим отн. оборотам, параметрам воздуха и заданному расходу воздуха (Flow.Mass)
	//				по флагу LimitAMF_ByPi3 ограничивается степень сжатия третьей точкой диаграммы и пересчитывается присланный расход воздуха
	bool	calcCompressorAMF(double RelativeRPM, AirFlow& Flow, double &ComprPi, double &NuAdiabat, bool LimitAMF_ByPi3 = false) ;

	//				расчет приведенных параметров по диаграмме по приведенным оборотам (интерполяция) - три точки (AMF1, Pi1; AMF2, Pi2, AMF3, Pi3), коэф-ты ухудшения КПД
	void			calcDiagram(double RelativeRPM_Corr, double &AMF_Corr1, double &ComprPi1, double &AMF_Corr2, double &ComprPi2,  double &AMF_Corr3, double &ComprPi3, double &NuAdiabatWork, double &NuK1, double &NuK2);

	double			LimitBetaMult;		//для управления ограничителем беты снаружи

protected:
	//	характеристики
	//диаграмма компрессора 
	typedef ed::vector<JetCompressorData> ComprDataVector;
	ComprDataVector	Data;	
	bool			DataEmpty;
	int				DataSize;

	double			LimitBetaK;			//к-т ограничения Beta сверху при использовании calcCompressorBeta

	//для вентилятов (КНД):
	double			PiCoreCoeff;		//к-т расчета степени сжатия внутренней части по степени сжатия внешней части
	double			NuFanToNuCore;		//к-т расчета КПД внутренней части по КПД внешней части

};

//////////////////////////////////////////////////////////////////////////
// Variable pressure ratio blower
class FMBASE_API VariablePRBlower 
{
public:
	struct BlowerCalcData
	{
		double in_eng_RPM_;
		double in_rel_eng_RPM_;
		AirFlow& in_Flow_;
		double out_bl_Pi_;
		double out_bl_Nu_;
	};

protected:
	enum BlowerParams{
		VBLOWER_RATIO = 0,
		VBLOWER_K0,
		VBLOWER_K1,
		VBLOWER_P_MAX
	};

public:
	/*RATIO , K0, K1*/
	typedef std::array<double,VBLOWER_P_MAX> ParamsArray;
	typedef ed::vector<ParamsArray> VariablePRBlowerParams;

	VariablePRBlower();

	void init(double Temperature0, double Pressure0, double ReToRe0Cryt, double NuModifierPower,
		VariablePRBlowerParams& parameters);

	bool calcCompressorAMF(BlowerCalcData& calc_params);

	void set_control(double control);
	void set_blower_speed(unsigned int blower_speed /*1, 2, N*/);

protected:

	void update_Nu();

	unsigned int blower_speed_; /*0,1,2...N. 0 - not init*/

	double Control_; /* Control_MIN; 1.0 */

	double K_adiabatic_;
	double K_adiabatic0_;
	double Nu_;

	VariablePRBlowerParams params_;
	ParamsArray* cur_param_;

	//	характеристики
	//диаграмма компрессора 
	double			Temperature0_;		//для расчета приведенных параметров
	double			Pressure0_;
	double			ReToRe0Cryt_;		//критическое число Re
	double			NuModifierPower_;	//степень  - скорость ухудшения КПД отношения чисел Re

};


}