#ifndef __BrakeChute_h__
#define __BrakeChute_h__

#include "Base.h"

#include "Optimizable.h"
#include "FMEngineeringStructure/ADElement.h"

namespace EagleFM
{

class EffectManager;

class FMBASE_API BrakeChute : public ADElement, public Optimizable
{
public:
	BrakeChute(DynamicBody*, AerodynamicBody*, EffectManager*);
	virtual ~BrakeChute();

	void				initGeometry(Vec3 p_l, double length);

	//void				setImmortal(bool Flag) {}

	//контакт с пов-стью(величина обжатия, точка контакта, нормаль к поверхности, единичный вектор в направлении стойки и время до следующего обновления для экстраполяции)
	//void				setContact(double L, double Noise, const Vec3 &p_w, const Vec3 &N_w, const Vec3 &V_w, int SurfaceType, double dt); 

    void				reset();
    void				repair();
private:
	//имитация
	virtual void		doSimulate();

public:
	//доступ
	double				getPhase()		const { return phase; }
	double				getElevation()	const { return elevation; }
	double				getAzimuth()	const { return azimuth; }
	double				getHorDev()		const { return horDev; }
	double				getVertDev()	const { return vertDev; }

	bool				open;			//команда на выпуск   TODO: wire??

	Vec3				locPos_l;		//точка крепления к ЛА
	double				len;			//расстояние от центра парашюта до точки крепления
	double				grouhdSafeH;	//запас до земли, чтобы не уходить под нее

protected:
	double				phase;			//0 - закрыто, готово к использованию, -1 - использовано, 1 - раскрыто, > 0 < 1 - раскрывается
	double				elevation;		//угол отклонения по вертикали
	double				azimuth;		//угол отклонения по горизонтали
	double				horDev;			//горизонтальные колебания  -1 ... 1
	double				vertDev;		//вертикальные колебания	-1 ... 1
	double				devFreqPhase;	

//	не хозяин:	
	DynamicBody*		dbody;
	EffectManager*		emanager;
};

}

#endif