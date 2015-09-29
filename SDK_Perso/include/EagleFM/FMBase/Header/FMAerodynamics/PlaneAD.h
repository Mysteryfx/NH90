#ifndef __PlaneAD_h__
#define __PlaneAD_h__

#include "Base.h"
#include "FMAerodynamics/AerodynamicBody.h"

#include <ed/vector.h>

namespace EagleFM
{

	// АД-калькулятор несущей поверхности,
	// хранящий столбец АД банка на конкретном числе М и способный 
	// вычислять аэродиначиские коэффициенты и фокус при данном числе М

	//		Формат импорта аэродинамических данных:
	// Cxa_Data [0]  - значение Cxa при AoA = -90°
	// Cxa_Data [1]  - AoA, разделяющий функцию Cxa(AoA) на 1-ый и 2-ой участки
	// Cxa_Data [2]  - AoA наименьшего сопротивления
	// Cxa_Data [3]  - минимальное значение Cxa_0
	// Cxa_Data [4]  - AoA, разделяющий функцию Cxa(AoA) на 2-ой и 3-ий участки
	// Cxa_Data [5]  - значение Cxa при AoA = Cxa_Data [4]
	// Cxa_Data [6]  - значение Cxa при AoA = 90°
	// Cxa_Data [7]  - поправочный коэффициент при боковом обтекании
	// Cxa_Data [8]  - поправочный коэффициент при обратном обтекании

	// Cya_Data [0]  - степень функции Cya на 1-ом участке
	// Cya_Data [1]  - AoA, разделяющий функцию Cya(AoA) на 1-ый и 2-ой участки
	// Cya_Data [2]  - значение Cya при AoA = Cya_Data [1]
	// Cya_Data [3]  - AoA, разделяющий функцию Cya(AoA) на 2-ой и 3-ий участки
	// Cya_Data [4]  - значение Cya при AoA = Cya_Data [3]
	// Cya_Data [5]  - AoA, принадлежащий 4-му участку функции Cya(AoA)
	// Cya_Data [6]  - значение Cya при AoA = Cya_Data [5]
	// Cya_Data [7]  - AoA, принадлежащий 4-му участку функции Cya(AoA)
	// Cya_Data [8]  - значение Cya при AoA = Cya_Data [7]
	// Cya_Data [9]  - AoA, разделяющий функцию Cya(AoA) на 4-ый и 5-ый участки
	// Cya_Data [10] - AoA, разделяющий функцию Cya(AoA) на 5-ый и 6-ой участки
	// Cya_Data [11] - значение Cya при AoA = Cya_Data [10]
	// Cya_Data [12] - AoA, разделяющий функцию Cya(AoA) на 6-ой и 7-ой участки
	// Cya_Data [13] - значение Cya при AoA = Cya_Data [12]
	// Cya_Data [14] - степень функции Cya на 7-ом участке
	// Cya_Data [15] - поправочный коэффициент при боковом обтекании
	// Cya_Data [16] - поправочный коэффициент при обратном обтекании

	// mza_Data [0]  - значение mza_0 при Cya = 0
	// mza_Data [1]  - поправочный коэффициент при обратном обтекании

	// Focus_Data [0]  - AoA, в пределах которого фокус неподвижен
	// Focus_Data [1]  - максимальное значение выноса фокуса вперед
	// Focus_Data [2]  - максимальное значение выноса фокуса вбок

class FMBASE_API Plane_ADC
{
public:
	// Конструктор принимает данные в указанном формате и пересчитывает их путем решения уравнения Гаусса во внутренний формат
	Plane_ADC(double Mach, double *Cxa_Data, double *Cya_Data, double *mza_Data, double *Focus_Data, double *CxaAdd_Data = 0, double *CyaAdd_Data = 0);

	void			setK_AspectRatio(double K); //пересчет АД по заданному к-ту изменения удлинения

	// Расчет аэродинамических коэффициентов и координат фокуса
	void			calcAerodynamics(double Azimuth, double InvElevation, double *Cxa, double *Cya, double *mza, double *Xfc, double *Zfc);

	// Доступ
	double			getMach()			const { return Mach; }
	double			getChordQuarter()	const { return Focus_Bank[2]; }

protected:
	//Банк АД при данном числе М:
	double			Mach;				// Число Маха
	double			Cxa_Bank[15];		// Массив коэффициентов для расчета Cxa
	double			Cya_Bank[34];		// Массив коэффициентов для расчета Cya
	double 			mza_Bank[2];		// Массив коэффициентов для расчета mza
	double			Focus_Bank[4];		// Массив коэффициентов для расчета координат фокуса

	//расширение АД от ЙоЙо (добавка к Cxa и Cya для доп.горбов )
	double			CyaAdd_Bank[19];	// полиномы добавки Cya ([0] - флаг наличия добавок) 
	double			CxaAdd_Bank[10];	// полиномы добавки Cxa ([0] - флаг наличия добавок)

	//Банк модификации АД при измененном удлинении
	bool			AspectRatioFlag;	// факт изменения удлинения (необходимость учитывать добавки)
	double			DeltaCya_Bank[10];	// полиномы для расчета ухудшения Cya 
	double			DeltaCxa_Bank[9];	// полиномы для расчета ухудшения Cxa
	
};

typedef ed::vector<Plane_ADC> Plane_ADC_Vector;

// Класс, описывающий абстрактную модель несущей поверхности.
// Используется при аэродинамическом моделировании крыла и стабилизатора.
// Для работы требует инициализации банка АД через наследников
class FMBASE_API PlaneAD : public AerodynamicBody
{
	//	---МЕТОДЫ---
public:
	PlaneAD() : Span(1.0), mzwz(0.0), K_AspectRatio(0.0), Cya_daoa(0.0), Skew_l_l(0.0) {}

	PlaneAD(double P_Area, double P_Span, const ed::string& file_name, double key_k = 1.0) : 
						Span(P_Span), mzwz(0.0), K_AspectRatio(0.0), Cya_daoa(0.0), Skew_l_l(0.0) { Area = P_Area; loadBank(file_name, key_k); }

	void				loadBank(const ed::string& file_name, double key_k = 1.0); //file_key - множитель как шифр данных

	void				set_mzwz(double k) { mzwz = k; }

	void				setK_AspectRatio(double K); //к-т для пересчета АД при изменении удлинения ( = 1/Pi*(1/AspectRatio - 1/AspectRatio0))

	void				setCya_daoa(double Cyad) { Cya_daoa = Cyad; }

	//кому из наследников приспичет, может переопределить, а вообще функция законченная, сама умеет пользоваться АД-калькулятором данного типа
	virtual void		calcAerodynamics(double *Arguments, double SpeedVim, double Mach, double AoA, double AoS,
		Vec3 RotateSpeed_l_l, Vec3 *ADForce_l_l, Vec3 *ADForce_pos_l_l, Vec3 *OwnADDamperMoment_l_l,
		double *ShakeAmplitude, double *ShakeFrequency);

	double				getSpan()			const { return Span; }
	double				getK_AspectRatio()	const { return K_AspectRatio; }
	Vec3				getSkew_l_l()		const { return Skew_l_l; }

	double				getChordQuarter()	const { return adc.size() > 0 ? adc[0].getChordQuarter() : 1.0; }

protected:
	//отпрыски класса должны инициализировать АД БД через эту функцию
	void				init(double Mach, double *Cxa_Data, double *Cya_Data, double *mza_Data, double *Focus_Data, double *CxaAdd_Data = 0, double *CyAdd_Data = 0);  //это в случае наличия добавок ЙоЙо


	//вспомогательные расчеты
public:	
	//перевод углов атаки и скольжения в углы азимута и места
	void				AoAandAoBtoAzandIEl(double AoA, double AoS, Vec3 *VectorV, double *Azimuth, double *InvElevation, double *sinAz, double *cosAz, double *sinIEl);	

	// Расчет аэродинамических коэффициентов и координат фокуса с интерполяцией по АД БД
	void				calcADKoeff(double Mach, double Azimuth, double InvElevation, double *Cxa, double *Cya, double *mza, double *Xfc, double *Zfc);


	//	---ПОЛЯ---
protected:

	//таблица АД по числам М (adc[i] соответсвует i-му числу М - по возрастающей)
	Plane_ADC_Vector	adc;

	double				Span;				//размах несущей поверхности

	double				mzwz;				//к-т демпфирования по угл.скорости тангажа

	double				K_AspectRatio;		//коэффициент пересчета удлинения (без модификации = 0)
	Vec3				Skew_l_l;			//скос потока при измененном удлинении в СК элемента

	double				Cya_daoa;			//Суa по производной УА (для описания нестационарности или гистерезиса УА)
};

}

#endif