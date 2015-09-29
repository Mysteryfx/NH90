#ifndef __Wheel_h__
#define __Wheel_h__

#include "Base.h"

#include "FMMathematics/math.h"

namespace EagleFM
{

struct FMBASE_API WheelInit
{
	WheelInit()
	{
		Radius = 1.0;
		MOI = 1.0;
		StaticFrictionFactor = 0.6;
		SideFrictionFactor = 0.5;
		RollFrictionFactor = 0.03;
		GlideFrictionFactor = 0.5;
		BrakeMomentMax = 0.0;
		RotationPhase = 0.0;
		DamageForceFactor = 0.0;
		DamageSpeed = 0.0;
		DamageSpeedTimeLim = 3.0;
		ContactPosShiftK = 0.0;
	}

	double Radius;
	double MOI;
	double StaticFrictionFactor;
	double SideFrictionFactor;
	double RollFrictionFactor;
	double GlideFrictionFactor;
	double BrakeMomentMax;
	double RotationPhase;
	double DamageForceFactor;
	double DamageSpeed;
	double DamageSpeedTimeLim;
	double ContactPosShiftK;
};

//Модель колеса, позволяющая по известным параметрам 
//движения колеса по подстилающей поверхности определить
//вектор силы трения колеса.
class FMBASE_API Wheel
{

public:
	Wheel();
	Wheel(const WheelInit&);

	void	init(const WheelInit&);

	void	setIF(double IF) { IntegrityFactor = IF; }

	// управление
	void	setForce_l_l(Vec3 F_l_l) { Force_l_l = F_l_l; }
	void	setRotationPhase(double RotationPhase);
	void	setSpeedReqX_l_l(double Vx) { SpeedReqX_l_l = Vx; }
	void	setSpeedX_l_l(double Vx) { SpeedReqX_l_l = SpeedX_l_l = Vx; }
	void	setSpeedZ_l_l(double Vz); 
	void	setVy(double vy) { Vy = vy; }
	void	setRelativeBrakeMoment(double RBM) { BrakeMoment = RBM*BrakeMomentMax;	}
    double 	getRelativeBrakeMoment() const     { return BrakeMoment / BrakeMomentMax; }

	// моделирование колеса
	void	simulate(double dTime);

	// доступ
	Vec3	getForce_l_l()			const { return Force_l_l; }
	double	getRotationPhase()		const { return RotationPhase; }
	double	getRadius()				const { return Radius; }
	double	getMOI()				const { return MOI; }
	double	getSpeedX_l_l()			const { return SpeedX_l_l; }
	double	getSpeedZ_l_l()			const { return SpeedZ_l_l; }
	double	getSlideFlag()			const { return SlideFlag; }
	double	getBrakeMoment()		const { return BrakeMoment; }
	double	getFrictionMomentMax()	const { return FrictionMomentMax; }
	bool	getBrakeBlock()			const { return BrakeBlockFlag; }
	double	getContactPosShift()	const { return ContactPosShift; }
	double	getIF()					const { return IntegrityFactor; }


	bool	FlagImmortal;

	//		характеристики колеса:
private:
	double	Radius;					// Радиус колеса
	double	MOI;					// момент инерции

	double 	StaticFrictionFactor;	// коэффициент трения колеса в состоянии покоя
	double	SideFrictionFactor;		// коэффициент бокового трения колеса при движении
	double	RollFrictionFactor;		// коэффициент трения качения колеса

	double	GlideFrictionFactor;	// коэффициент трения скольжения колеса

	double	DamageForceFactor;		// коэффициент прочности покрышки колеса
	double	DamageSpeed;			// скорость разрушения покрышки колеса
	double	DamageSpeedTimeLim;		// макс.время неразрушения при разрушающей скорости
	double	BrakeMomentMax;			// Максимальный тормозной момент

	double	ContactPosShiftK;		// к-т смещения точки приложения силы от раскрутки при касании (по ускорению раскрутки)


	//		параметры движения колеса:
public:	
	double	RateFactor;				// коэффициент масштабирования коэффициентов трения колеса при «зарывании» в грунт
	double	PrecipitationFactor;	// коэффициент масштабирования коэффициентов трения колеса в соответствии с погодными условиями (дождь, снег и пр.)
	int		SurfaceType;			// Тип подстилающей поверхности

private:	
	double	RotationPhase;			// Фаза поворота колеса
	double	SpeedX_l_l;				// касательная скорость колеса
	double	SpeedZ_l_l;				// осевая скорость колеса
	double	SpeedReqX_l_l;			// касательная скорость колеса для раскрутки колеса
	double  Vy;						// вертик.скор. 
	double	dX;						// касательное отклонение колеса от точки покоя
	double	dZ;						// осевое отклонение колеса от точки покоя	
	double	Moment;					// Момент с вала (для автомобильного колеса)
	double	BrakeMoment;			// Тормозной момент
	double	FrictionMomentMax;		// Макс.момент до срыва в юз
	double	StopSpeed;				// минимальная скорость колеса, разграничивающая состояния покоя и движения колеса
	double	DamageSpeedTime;		// время воздействия разрушающей скорости
	double	IntegrityFactor;		// степень разрушения колеса (1 - целое, 0.5 - разрушен пневматик, 0 - полностью отсутствет)

	Vec3	Force_l_l;				// Сила приложенная к колесу

	bool	StopFlag;				// Флаг остановки колеса

	bool	BrakeBlockFlag;			// Флаг блокировки тормозами
	bool	SlideFlag;				// Флаг бокового скольжения

	double	ContactPosShift;		// смещение точки приложения силы от раскрутки при касании
};

}

#endif