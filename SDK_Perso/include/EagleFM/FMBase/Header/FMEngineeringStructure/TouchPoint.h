#ifndef __TouchPoint_h__
#define __TouchPoint_h__

#include "Base.h"

#include "FMDynamics/DynamicBody.h"
#include "FMEngineeringStructure/Amortizater.h"


namespace EagleFM
{

// Модель узловой точки планера ЛА, суть: амортизатор, 
// предназначена для моделирования удара ЛА о подстилающую
// поверхность и вычисления силы реакции опоры и трения планера
// о подстилающую поверхность.
class FMBASE_API TouchPoint
{
public:
	TouchPoint(DynamicBody *DBody);

	void	initForceField(AmortizaterInit AmInit) { ForceField.init(AmInit); }
	void	setRadius(double);

	//контакт с пов-стью(величина обжатия, точка контакта, нормаль к поверхности, время до следующего обновления)
	void	setContact(double L, double Noise, const Vec3 &p_w, const Vec3 &N_w, const Vec3 &V_w, int SurfaceType, double dt); 

	void	simulate(double dt);

	Vec3	getForce_l()		const { return Force_l; }
	Vec3	getForce_pos_w()	const { return Force_pos_w; }
	Vec3	getRelativeVel_l()	const { return RelativeVel_l; }
	double	getRelativeSpeed()	const { return RelativeSpeed; }

	double	getFullDepth()		const { return ExternLength; }

	double	getWaterFactor()	const { return WaterFactor;	}

	int		getSurfaceType()	const { return SurfaceType; }


	bool	FlagImmortal;

private:
//	не хозяин:
	DynamicBody		*pDBody;

//	хозяин:
	Amortizater		ForceField;				// силовое поле контактной точки

	Vec3			Force_l;
	Vec3			Force_pos_w;

	Vec3			RelativeVel_l;
	double			RelativeSpeed;
	Vec3			StopPosition_w;			// Координаты точки покоя
		
	double			FrictionForceFactor;	// Коэффициент силы трения контактной точки
	Vec3			SurfacePointPosition_w;	// Координаты расчетной точки на подстилающей поверхности
	Vec3			Normal_w;				// Нормаль к подстилающей поверхности
	Vec3			SurfaceV_w;				// Скорость поверхности	
	int				SurfaceType;			// Тип поверхности
	double			SurfaceNoise;			// шум поверхности (не прибавлять! нужен для определения особенностей силы трения)	

	double			WaterFactor;			// степень заполнения водой

	bool			StopFlag;				// Флаг остановки

	double			ExternLength;			// для оптимизации
	bool			WasUpdate;				// для оптимизации
	double			TimeUpdate;				// для оптимизации

};

}

#endif