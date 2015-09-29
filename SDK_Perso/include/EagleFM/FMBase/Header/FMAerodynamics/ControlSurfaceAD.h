#ifndef __ControlSurfaceAD_h__
#define __ControlSurfaceAD_h__

#include "Base.h"
#include "FMAerodynamics/AerodynamicBody.h"

#include <ed/vector.h>

namespace EagleFM
{

// АД-калькулятор управляющей поверхности,
// хранящий столбец АД банка на конкретном числе М и способный 
// вычислять аэродиначиские коэффициенты и фокус при данном числе М

	//		Формат импорта аэродинамических данных:

	// Cxa_Data [0]  - коэффициент при 2-ой степени AoA
	// Cxa_Data [1]  - коэффициент при 1-ой степени AoA
	// Cxa_Data [2]  - свободный коэффициент
	// Cxa_Data [3]  - поправочный коэффициент при обратном обтекании

	// Cya_Data [0]  - степень функции Cya на 1-ом участке
	// Cya_Data [1]  - AoA, разделяющий функцию Cya(AoA) на 1-ый и 2-ой участки
	// Cya_Data [2]  - значение Cya при AoA = Cya_Data [1]
	// Cya_Data [3]  - AoA, разделяющий функцию Cya(AoA) на 2-ой и 3-ий участки
	// Cya_Data [4]  - значение Cya при AoA = Cya_Data [3]
	// Cya_Data [5]  - AoA, разделяющий функцию Cya(AoA) на 3-ий и 4-ый участки
	// Cya_Data [6]  - значение Cya при AoA = Cya_Data [5]
	// Cya_Data [7]  - AoA, разделяющий функцию Cya(AoA) на 4-ый и 5-ый участки
	// Cya_Data [8]  - значение Cya при AoA = Cya_Data [7]
	// Cya_Data [9]  - степень функции Cya на 5-ом участке
	// Cya_Data [10] - поправочный коэффициент при обратном обтекании

	// mza_Data [0]  - значение производной dmza_0
	// mza_Data [1]  - поправочный коэффициент при обратном обтекании

	// Focus_Data [0]  - AoA, в пределах которого фокус неподвижен
	// Focus_Data [1]  - значение производной выноса фокуса

class FMBASE_API ControlSurface_ADC
{

public:	
	//конструктор принимает данные в указанном формате и пересчитывает их путем решения уравнения Гаусса во внутренний формат
	ControlSurface_ADC(double Mach, double *Cxa_Data, double *Cya_Data, double *mza_Data, double *Focus_Data);

	double getMach() { return Mach; }

	// Расчет аэродинамических коэффициентов и координаты фокуса
	void calcAerodynamics(double Azimuth, double InvElevation, double Angle, double *dCxa, double *dCya, double *dmza, double *dXfc);

protected:
	//Банк АД при данном числе М:
	double	Mach;				// Число Маха
	double	Cxa_Bank[4];		// Массив коэффициентов для расчета dCxa
	double	Cya_Bank[21];		// Массив коэффициентов для расчета dCya
	double	mza_Bank[2];		// Массив коэффициентов для расчета dmza
	double	Focus_Bank[3];		// Массив коэффициентов для расчета координаты фокуса
};

typedef ed::vector<ControlSurface_ADC> CS_ADC_Vector;

// Класс, описывающий абстрактную модель управляющей поверхности.
// Используется при аэродинамическом моделировании
// элеронов, рулей направления, высоты и т.д.
// Для работы требует инициализации банка АД через наследников 
class FMBASE_API ControlSurfaceAD : public AerodynamicBody
{
	//	---МЕТОДЫ---

public:
	ControlSurfaceAD() {}

	ControlSurfaceAD(double CS_Area, const ed::string& file_name, double key_k = 1.0) { Area = CS_Area; loadBank(file_name, key_k); }

	void loadBank(const ed::string& file_name, double key_k = 1.0); //file_key - множитель как шифр данных


	//кому из наследников приспичет, может переопределить, а вообще функция законченная, сама умеет пользоваться АД-калькулятором данного типа
	virtual void calcAerodynamics(double *Arguments, double SpeedVim, double Mach, double AoA, double AoS, 
		Vec3 RotateSpeed_l_l, Vec3 *ADForce_l_l, Vec3 *ADForce_pos_l_l, Vec3 *OwnADDamperMoment_l_l,
		double *ShakeAmplitude, double *ShakeFrequency);

protected:
	//отпрыски класса должны инициализировать АД БД через эту функцию
	void init(double Mach, double *Cxa_Data, double *Cya_Data, double *mza_Data, double *Focus_Data); 

	//вспомогательные расчеты

	//перевод углов атаки и скольжения в углы азимута и места
	void AoAandAoBtoAzandIEl(double AoA, double AoS, Vec3 *VectorV, double *Azimuth, double *InvElevation, double *sinAz, double *cosAz, double *sinIEl);	

	// Расчет аэродинамических коэффициентов и координат фокуса с интерполяцией по АД БД
	void calcADKoeff(double Mach, double Azimuth, double InvElevation, double Angle, double *dCxa, double *dCya, double *dmza, double *dXfc);


	//	---ПОЛЯ---

	//таблица АД по числам М (adc[i] соответсвует i-му числу М - по возрастающей)
	CS_ADC_Vector adc;
};

}

#endif