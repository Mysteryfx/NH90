#ifndef __IIR_Filter_h__
#define __IIR_Filter_h__

#include "./_config.h"

namespace Lua
{ 
    class Config;
}

namespace Math
{

//			суть фильтра:
//	________________________________
//	  D0|			  D1|			| D2		
//		OS1----->[Z1]-->OS2-->[Z2]->OS3------>
//	 -C0|_______________|-C1_____________|
//

enum ED_MATH_EXTERN IIR_FilterPrototypes
{
	IIRFP_CHEB_0_5_DB = 0,
	IIRFP_CHEB_1_0_DB,
	IIRFP_CHEB_2_0_DB,
	IIRFP_CHEB_3_0_DB,
	IIRFP_BUTTERWORTH, 
	IIRFP_CRYT_DAMPING,
	IIRFP_BESSEL,
	//______________
	IIRFP_LAST_TYPE
};

class ED_MATH_EXTERN IIR_FilterCoefficients
{
public:
	IIR_FilterCoefficients();
	IIR_FilterCoefficients(double D_0, double D_1, double D_2,
		double C_0, double C_1);

	IIR_FilterCoefficients& operator = (const IIR_FilterCoefficients& cF)
	{
		D0 = cF.D0; D1 = cF.D1; D2 = cF.D2;
		C0 = cF.C0;	C1 = cF.C1;

		return *this;
	}

	double D0, D1, D2,
		C0, C1;
};

class ED_MATH_EXTERN IIR_Filter
{
//---------------------- МЕТОДЫ ----------------------------
public:
	IIR_Filter();
	virtual ~IIR_Filter();

	void			initHPF_LPFbyPrototype(bool IsHPF, int CascadeNum, IIR_FilterPrototypes Prototype, //инициализация ФВЧ (BPF) или ФНЧ (LPF) по аналоговым прототипам:
						double FreqDescr, double Freq0, double K); // IsBPF = true - ФВЧ, кол-во каскадов (1,2,3), аналоговый прототип, частота дискретизации, частота среза, к-т передачи фильтра (усиления, ослабления))
	
	void			initHPF_LPF(bool IsHPF, double B, double C, //инициализация ФВЧ (BPF) или ФНЧ (LPF) вручную:
						double FreqDescr, double Freq0, double K); // IsBPF = true - ФВЧ, к-т B, к-т С, частота дискретизации, частота среза, к-т передачи фильтра (усиления, ослабления))
	
	void			initBPF(double Q, double FreqDescr, double Freq0, double K); //инициализация ПФ: Q - добротность, определяет ширину полосы пропускания


	void			init(const IIR_FilterCoefficients& coeffIn); //явная инициализация

	void			createNextCascade(const IIR_FilterCoefficients& coeffIn); //добавить каскад
	void			createNextCascade(const IIR_Filter& pF); //добавить каскад по подобию существующего фильтра

	void			cleanNearestCascade();

	void			update(double Input = 0); //симуляция с подачей входа

	void			initHPF_LPFbyPrototypeFromState(Lua::Config& config, const char* table, double FreqDescr);
	void			initHPF_LPFFromState(Lua::Config& config, const char* table, double FreqDescr);
	void			initBPFFromState(Lua::Config& config, const char* table, double FreqDescr);
	void			initFromState(Lua::Config& config, const char* table);
	
	double	const	getOutput()	const;

	IIR_Filter& operator = (const IIR_Filter& pF)
	{
		coeff = pF.coeff;
		return *this;
	}

	//сборка каскадности: Filter1 >> Filter2 - двухкаскадный фильтр, Filter1 >> Filter2 >> Filter3 - трехкаскадный
	friend IIR_Filter& operator >> (IIR_Filter &Filter1, IIR_Filter &Filter2)
	{
		Filter1.pRightFilter = &Filter2; 
		Filter1.IsExternalCascade = true;
		return Filter2;
	}

//----------------------- ПОЛЯ ------------------------------
private:
	void initHPF_LPF(bool IsHPF, double B, double C, double FreqDescr, double Freq0, double K, bool nextCascade);

	//коэффициенты фильтра
	IIR_FilterCoefficients coeff;

	//хранящиеся ячейки
	double		Z1, Z2; 
	double		S1, S2, S3;

	double		Output;

	//Для сборки каскадности фильтра:
	IIR_Filter	*pRightFilter;
	bool		IsExternalCascade;
};

}

#endif