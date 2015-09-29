#ifndef __ADElement_h__
#define __ADElement_h__

#include "Base.h"
#include "FMDynamics/DynamicBody.h"
#include "FMAerodynamics/AerodynamicBody.h"
#include "FMEngineeringStructure/ControlSurface.h"
#include "FMSpace/IBasicAtmosphere.h"

#include <ed/vector.h>

namespace EagleFM
{

class RigidBody;

//структура-хранилище, описывающая управляющую поверхность, включая ее аэродинамику
struct ADControl
{
	ADControl(AerodynamicBody* ab, ControlSurface* cs)
	{
		pADBody = ab;
		pControlSurface = cs;
	}

	//не хозяин
	AerodynamicBody		*pADBody;
	ControlSurface		*pControlSurface;
};

typedef ed::vector<ADControl> ADControlVector;

//Класс, представляющий аэродинамический элемент конструкции планера 
//(несущие, управляющие поверхности, фюзеляж, парашют, шассии и т.д.).
//Имеет собственные позицию и ориентацию. По данным об общей позиции и ориентации 
//планера, учитывая собственные данные, вычисляет по аэродинамическому телу (AerodynamicBody*)
//аэродинамическую силу, центр ее давления
//и собственный момент демпфирования

class FMBASE_API ADElement
{
public:
	ADElement(AerodynamicBody*);

	ADElement(AerodynamicBody*, const DynamicState&);

	void				initShakeFilter() { ShakeFilterOn = true; } //пока просто вклчается флаг

	void				initMachCrit(double Mach0, double Mach1, double Mach2, double K1, double K2); //три точки и два к-та

	void				initStrengthProp(RigidBody* pRBody, int Element, double ForceMax, double ForceDamage, double IF_p = 2.0); //задать прочностные характеристики

	void				addControlSurface(AerodynamicBody*, ControlSurface*);

	void				setIF(DynamicBody *pDBody, double IF, bool CheckAlredy = false); //поломать (0...1) 0 - совсем сломали, 1 - целый, checkAlredy = true - запрещает увеличивать IF

	//управление  (может следует сделать localDState public ??)
	void				setYawPitchRoll(float _yaw, float _pitch, float _roll)
	{
		selfDState.yaw		= _yaw;
		selfDState.pitch	= _pitch;
		selfDState.roll		= _roll;
	}
	void				setYaw(float _yaw)		{ selfDState.yaw	= _yaw;	}
	void				setPitch(float _pitch)	{ selfDState.pitch	= _pitch; }
	void				setRoll(float _roll)	{ selfDState.roll	= _roll;}

	void				setADFactor(int Num, double Value) { if(Num >= 0 && Num < 100) ADFactors[Num] = Value; } 
											
	//Аэродинамический расчет -		( физическое тело(планер),точки подстилающей поверхности, нормаль к подст.пов.,  тип поверхности
	void				calcElementsAerodynamics(DynamicBody *pDBody, const Vec3 &SurfacePointPosition_w, 
													const Vec3 &SurfaceNormal_w, int SurfaceType, bool ShakeOn = true);


	void				checkFailureLoad(double dt);	//проверить разрушающую АД нагрузку

	//		Доступ
	Vec3				getADForce_l()				const { return ADForce_l; }
	Vec3				getADForce_pos_l()			const { return ADForce_pos_l; }
	Vec3				getOwnADDamperMoment_l()	const { return OwnADDamperMoment_l; }

	double				getMach()					const { return Mach; }
	double				getSpeedVim()				const { return SpeedVim; }

	double				getAoA()					const { return AoA; }
	double				getAoS()					const { return AoS; }

	double				getNormalHeight()			const { return NormalHeight; }

	float				getYaw()					const { return selfDState.yaw; }
	float				getPitch()					const { return selfDState.pitch; }
	float				getRoll()					const { return selfDState.roll; }

	double				getIntegrityFactor()		const { return IntegrityFactor; }

	AerodynamicBody*	getADBody()					const { return pADBody;}

    double                 getShakeAmplitude() const { return ShakeAmpl; }
    const  DynamicState &  getDynamicState() const { return selfDState; }
private:
						//рачет АД силы (в случае выхода из ламинарной зоны - тряска углов и пересчет) 
	void				calcAerodynamicForces(double SpeedVim, double Mach, double AoA, double AoS, Vec3 RotV_l_l, 
												Vec3* ADForce_l_l, Vec3* ADForce_pos_l_l, Vec3* OwnADDamperMoment_l_l,
												double* ShakeAmpl, double* ShakeFreq);



protected:
	DynamicState		selfDState;					// физические свойства АД элемента планера

private:

//	не хозяин:
	ADControlVector		ADControls;					// управляющие элементы (элероны, рули высоты...)
	AerodynamicBody		*pADBody;					// аэродинамика элемента планера
	RigidBody			*pRBody;					// для разрушения (при определенном Element)


//	хозяин:
	Vec3				ADForce_l;					// Аэродинамическая сила
	Vec3				ADForce_pos_l;				// Координаты ЦД
	Vec3				OwnADDamperMoment_l;		// Демпфирующий момент

	double				ShakeAmpl;					// аплитуда АД тряски 
	double				ShakeFreq;					// частота АД тряски

	bool				ShakeFilterOn;				// генераторы АД тряски включены
public:
	Math::IIR_Filter	ShakeFilterAoA;				// генератор АД тряски по УА
	Math::IIR_Filter	ShakeFilterAoS;				// генератор АД тряски по УС
private:
	Randomizer			ShakeRndAoA;				// шум АД тряски по УА
	Randomizer			ShakeRndAoS;				// шум АД тряски по УС
	double				MachCrit0;					// тряска при критическом числе М (начало тряски)
	double				MachCrit1;
	double				MachCrit2;					// макс.тряска
	double				MachCritK1;					// к-т тряски при числе М выше MachCrit1
	double				MachCritK2;					// к-т тряски при числе М выше MachCrit2


	//		Прочностные характеристики
	int					Element;					// номер в таблице элементов ЛА (для DM)
	double				ForceMax;					// сила разрушения
	double				ForceDamage;				// сила деформации
	double				IF_pow;						// степень ослабления прочностных характеристик в зависимости от IntegrityFactor
	double				ForceMaxCurr;				// текущая макс.сила при превышении деформации (для однократного износа)
	double				ForcePrev;					// пред.сила для обнаружения факта пребывания и выхода из зоны деформации

	//		Параметры обтекания АД элемента планера
	double				SpeedVim,					// Скоростной напор
						Mach,						// Число Маха

						AoA,						// Угол атаки
						AoS,						// Угол скольжения

						NormalHeight,				// Расстояние до подстилающей поверхности

						IntegrityFactor,			// Коэффициент повреждения элемента конструкции
						ADFactors[100];				// Коэффициенты влияния на аэродинамику элемента конструкции


	bool				Turbulence;					// флаг учета турбулентности при расчете АД

};

}


#endif