#ifndef __FigureOfRevolutionsAD_h__
#define __FigureOfRevolutionsAD_h__

#include "Base.h"
#include "FMAerodynamics/AerodynamicBody.h"

#include <ed/vector.h>

namespace EagleFM
{

	// АД-калькулятор управляющей поверхности,
	// хранящий столбец АД банка на конкретном числе М и способный 
	// вычислять аэродиначиские коэффициенты и фокус при данном числе М

	//		Формат импорта аэродинамических данных:

	// Cxa_Data [0]  - минимальное значение Cxa_0
	// Cxa_Data [1]  - AoA, разделяющий функцию Cxa(AoA) на 1-ый и 2-ой участки
	// Cxa_Data [2]  - значение Cxa при AoA = Cxa_Data [1]
	// Cxa_Data [3]  - значение Cxa при AoA = 90°
	// Cxa_Data [4]  - поправочный коэффициент при боковом обтекании
	// Cxa_Data [5]  - поправочный коэффициент при обратном обтекании

	// Cya_Data [0]  - AoA, принадлежащий 1-му участку функции Cya(AoA)
	// Cya_Data [1]  - значение Cya при AoA = Cya_Data [0]
	// Cya_Data [2]  - AoA, разделяющий функцию Cya(AoA) на 1-ый и 2-ой участки
	// Cya_Data [3]  - AoA, разделяющий функцию Cya(AoA) на 2-ой и 3-ий участки
	// Cya_Data [4]  - значение Cya при AoA = Cya_Data [3]
	// Cya_Data [5]  - AoA, разделяющий функцию Cya(AoA) на 3-ий и 4-ый участки
	// Cya_Data [6]  - значение Cya при AoA = Cya_Data [5]
	// Cya_Data [7]  - степень функции Cya на 4-ом участке
	// Cya_Data [8]  - поправочный коэффициент при боковом обтекании
	// Cya_Data [9]  - поправочный коэффициент при обратном обтекании

	// Focus_Data [0]  - AoA, в пределах которого фокус неподвижен
	// Focus_Data [1]  - максимальное значение выноса фокуса при обтекании по AoA
	// Focus_Data [2]  - максимальное значение выноса фокуса при обтекании по AoS

class FMBASE_API FigureOfRevolutions_ADC
{
public:

	FigureOfRevolutions_ADC(double Mach, double *Cxa_Data, double *Cya_Data, double *Focus_Data);

	double getMach() { return Mach; }

	// Расчет аэродинамических коэффициентов и координаты фокуса
	void calcAerodynamics(double SolidAzimuth, double SolidAngleOfAttack, double *Cxa, double *Cya, double *Xfc);

protected:

	double	Mach;			// Число Маха
	double	Cxa_Bank[9];	// Массив коэффициентов для расчета Cxa
	double	Cya_Bank[17];	// Массив коэффициентов для расчета Cya
	double	Focus_Bank[4];	// Массив коэффициентов для расчета координаты фокуса
};

typedef ed::vector<FigureOfRevolutions_ADC> FOR_ADC_Vector;

// Класс, описывающий абстрактную модель тела вращения.
// Используется при аэродинамическом моделировании фюзеляжа. 
// Для работы требует инициализации банка АД через наследников 
class FMBASE_API FigureOfRevolutionsAD : public AerodynamicBody
{
	//	---МЕТОДЫ---

public:
	FigureOfRevolutionsAD() : mzwy(0.0), mzwz(0.0) {}

	FigureOfRevolutionsAD(double FoR_Area, const ed::string& file_name, double key_k = 1.0) : mzwy(0.0), mzwz(0.0) { Area = FoR_Area; loadBank(file_name, key_k); }

	void loadBank(const ed::string& file_name, double key_k = 1.0); //file_key - множитель как шифр данных

	void				set_mzwy(double k) { mzwy = k; }
	void				set_mzwz(double k) { mzwz = k; }

	//кому из наследников приспичет, может переопределить, а вообще функция законченная, сама умеет пользоваться АД-калькулятором данного типа
	virtual void calcAerodynamics(double *Arguments, double SpeedVim, double Mach, double AoA, double AoS, 
		Vec3 RotateSpeed_l_l, Vec3 *ADForce_l_l, Vec3 *ADForce_pos_l_l, Vec3 *OwnADDamperMoment_l_l,
		double *ShakeAmplitude, double *ShakeFrequency);

protected:
	//отпрыски класса должны инициализировать АД БД через эту функцию
	void init(double Mach, double *Cxa_Data, double *Cya_Data, double *Focus_Data); 

	//вспомогательные расчеты

	//перевод углов атаки и скольжения в пространственные углы атаки и азимута
	void AoAandAoBtoSAzandSAoA(double AoA, double AoS, Vec3 *VectorV, double *SolidAzimuth, double *SolidAoA, double *sinSAoA, double *cosSAoA);	

	// Расчет аэродинамических коэффициентов и координат фокуса с интерполяцией по АД БД
	void calcADKoeff(double Mach, double SolidAzimuth, double SolidAoA, double *Cxa, double *Cya, double *Xfc);

	//	---ПОЛЯ---

	double			mzwy;				//к-т демпфирования по угл.скорости курса
	double			mzwz;				//к-т демпфирования по угл.скорости тангажа


	//таблица АД по числам М (adc[i] соответсвует i-му числу М - по возрастающей)
	FOR_ADC_Vector adc;
};

}

#endif