#pragma once

//	���������� - � ���� ��������� "������� ������ = f(������.�������,������.������ �������)",

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

		
		//������ ����������� ������� � ����������� �� �������� ���������� (����������� �� �������)
		static double	calcMulTemperature(double ComprPi, double NuAdiabat) 
		{
			return 1 + (pow(ComprPi, AdiabaticConstantAir) - 1)/__max(NuAdiabat, 0.001);
		}

		//������ ��� ����������� �� ������������ �� � ����� � ������� ������
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

		//������ �������� ����������� �� �������� ����������
		static double	calcPower(double AirMassFlow, double ComprPi, double ComprNu, double Temperature /*in temperature*/)
		{
			return SpecificHeatAirP*AirMassFlow*(Temperature*CompressorBase::calcMulTemperature(ComprPi, ComprNu) - Temperature);
		}

		static double	calcPower(double AirMassFlow, double ComprPi, double ComprNu, double T_In /*in temperature*/, double T_Out /*out temperature*/)
		{
			return SpecificHeatAirP*AirMassFlow*(T_Out - T_In);
		}

		//������ ������� ����������� �� �������� ����������
		static double	calcWindmillingMoment(double TAS, double AtmoDensity, double RPM, double K1, double K2)
		{
			return AtmoDensity*TAS*TAS*TAS*K1*(K2*TAS - RPM)/__max(0.01, RPM)/10000.0;
		}

	protected:
		//	��������������
		//��������� ����������� 
		double			Temperature0;		//��� ������� ����������� ����������
		double			Pressure0;
		double			ReToRe0Cryt;		//����������� ����� Re
		double			NuModifierPower;	//�������  - �������� ��������� ��� ��������� ����� Re
		
};

struct FMBASE_API JetCompressorData //����� ������ ��� �������� ����������� �������� (������������ ��� �������� ��������� �����������)
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
	double	AirMassFlow1, AirMassFlow2, AirMassFlow3;	// ��� ����� �������� (����������)
	double	ComprPi1, ComprPi2, ComprPi3;				// ��� ����� �������� (�������)
	double	NuAdiabatic;// ��� �����������
	double	NuK1;		// ����-� ���������� ��������� ��� �� ���������� �� ������� �����
	double	NuK2;		// ������ ����-� (��� ��������� ��� ���������� �� ������� ������� ������) 										
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

	//������ ���� ������� ������/��� ��� ���������� ����� �� �������� ���� ������� ����� (��� ������������ - ������������ ������� ��������)
	void	calcCorePiAndNu(double &ComprPi, double &Nu)
	{
		ComprPi = 1 + (ComprPi - 1)*PiCoreCoeff;
		Nu *= NuFanToNuCore;
	}

	//				������ ������� (�������) ���������� ������ ����������� �� ������� ���.�������� � ���������� ������� (������, �����������, ��������).
	//				���������� ���������: ��������� ������� ������� ������ � �������� ������� �������, ������ �� ������� (�������) �����: ������ �������, ������� ������, ���.
	void	calcCompressorWork(double RelativeRPM, double Temperature, double Pressure, double &BetaWork, double &AirMassFlowWork, double &ComprPiWork, double &NuAdiabatWork) ;
	//				������ ���������� ������ ����������� �� ������� ���. ��������, ���������� ������� � ��������� ��������� Beta = ComprPi/AirMassFlow
	//				��� ������� ����� LimitBeta �������������� Beta ������� �������� Pi = f(AMF) ��� ������ �������� (������ ����� � �������� ������ � �������� LimitBetaK ��� ����������� �������)
	bool	calcCompressorBeta(double RelativeRPM, double &Beta, AirFlow& Flow, double &ComprPi, double &NuAdiabat, bool *LimitBeta = 0) ;//��� �� ���������� ���� ����������� ������
	//				������ ���������� ������ ����������� �� ������� ���. ��������, ���������� ������� � ��������� ������� ������� (Flow.Mass)
	//				�� ����� LimitAMF_ByPi3 �������������� ������� ������ ������� ������ ��������� � ��������������� ���������� ������ �������
	bool	calcCompressorAMF(double RelativeRPM, AirFlow& Flow, double &ComprPi, double &NuAdiabat, bool LimitAMF_ByPi3 = false) ;

	//				������ ����������� ���������� �� ��������� �� ����������� �������� (������������) - ��� ����� (AMF1, Pi1; AMF2, Pi2, AMF3, Pi3), ����-�� ��������� ���
	void			calcDiagram(double RelativeRPM_Corr, double &AMF_Corr1, double &ComprPi1, double &AMF_Corr2, double &ComprPi2,  double &AMF_Corr3, double &ComprPi3, double &NuAdiabatWork, double &NuK1, double &NuK2);

	double			LimitBetaMult;		//��� ���������� ������������� ���� �������

protected:
	//	��������������
	//��������� ����������� 
	typedef ed::vector<JetCompressorData> ComprDataVector;
	ComprDataVector	Data;	
	bool			DataEmpty;
	int				DataSize;

	double			LimitBetaK;			//�-� ����������� Beta ������ ��� ������������� calcCompressorBeta

	//��� ���������� (���):
	double			PiCoreCoeff;		//�-� ������� ������� ������ ���������� ����� �� ������� ������ ������� �����
	double			NuFanToNuCore;		//�-� ������� ��� ���������� ����� �� ��� ������� �����

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

	//	��������������
	//��������� ����������� 
	double			Temperature0_;		//��� ������� ����������� ����������
	double			Pressure0_;
	double			ReToRe0Cryt_;		//����������� ����� Re
	double			NuModifierPower_;	//�������  - �������� ��������� ��� ��������� ����� Re

};


}