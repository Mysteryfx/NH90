#ifndef __GearPost_h__
#define __GearPost_h__

#include "Base.h"

#include "Optimizable.h"

#include "FMEngineeringStructure/ADElement.h"
#include "FMEngineeringStructure/Amortizater.h"
#include "FMEngineeringStructure/Wheel.h"

namespace EagleFM
{

enum GearPostDir //направление уборки шасси (для расчета усилия на приводе)
{
	PD_FIXED,	//неубирающаяся стойка		
	PD_EXTERN,	//состояние задается извне
	PD_X,		//вдоль продольной оси самолета
	PD_LEFT,	//влево
	PD_RIGHT,	//вправо
	PD_FULL		//пространственно (по углам)
};

struct FMBASE_API GearPostInit
{
	GearPostInit()
	{
		WheelAxleOffset = 0.0;
		SelfAttitude	= false;	
		YawLimit		= 0.0;	
		MomentLimit		= 0.0;
		DamageOmega		= 10.0;
		DamageWobble	= 12.0;
		DamperCoeff		= 50.0;	
		AxleFric0s		= 190;
		AxleFric0f		= 15;
		AxleFricVs		= 0.2;
		AxleFricVf		= 3;
		Element			= -1;
		GroundFricFlag	= true;
		DamageLengthLim = 0.0;
		PDir			= PD_X;		
		PD_Angles[0] = PD_Angles[1] = PD_Angles[2] = 0;
		memset(MountArgs, 0, sizeof(MountArgs));
		MountArgs[0] = 1;	//номер модели геометрии подкосов
		//далее актуальные для данной модели параметры
		MountArgs[1] = 0.0; //StateAngle0		= 0.0;
		MountArgs[2] = 0.0; //StateAngle1		= 0.0;
		MountArgs[3] = 1;	//PostAngleSign	= true;
		MountArgs[4] = 1.0; //PostLength		= 1.0;	
		MountArgs[5] = 1.0; //MountLength		= 1.0;
		MountArgs[6] = 0.1; //MountPostR		= 0.1;
		MountArgs[7] = 0.0; //MountAngle1		= 0.0;
		MountArgs[8] = 0.0; //MountPivot_x	= 0.0;
		MountArgs[9] = 0.0; //MountPivot_y	= 0.0;
	}

	double		WheelAxleOffset;
	bool		SelfAttitude;	
	double		YawLimit;	
	double		MomentLimit;
	double		DamageOmega;
	double		DamageWobble;
	double		DamperCoeff;
	double		AxleFric0s;
	double		AxleFric0f;
	double		AxleFricVs;
	double		AxleFricVf;
	int			Element;
	bool		GroundFricFlag;
	double		DamageLengthLim;
	GearPostDir PDir;
	double		PD_Angles[3]; //yaw pitch roll для описания СК стойки для случая PDir = PD_Full
	double		MountArgs[50];
};

class RigidBody;
class EffectManager;

// Модель стойки шасси, включающая в себя модель колеса и амортизатора.
// Предназначена для вычисления параметров движения колеса и величины
// обжатия амортизатора в случае контакта колеса с подстилающей поверхностью.
// Также модель отвечает за АД расчет, как элемент планера
class FMBASE_API GearPost : public ADElement, public Optimizable
{
public:
	GearPost(DynamicBody*, RigidBody*, AerodynamicBody*, EffectManager*, const DynamicState&);

	void			initGeometry(const GearPostInit&);
	void			initAmorizater(const AmortizaterInit &AmInit) { Amort.init(AmInit); }
	void			initWheel(const WheelInit &WhInit) { Whl.init(WhInit); }
	void			initState(double St) { State = __max(0.0, __min(St, 1.0)); }
	void			initInAir();

	void			updateState(double St) { StateOld = State ; initState(St); }

	void			setImmortal(bool Flag) { Whl.FlagImmortal = Flag; }

	void			setDamageLengthLim(double DL) { DamageLengthLim = __max(0.0, DL); }

	//контакт с пов-стью(величина обжатия, точка контакта, нормаль к поверхности, единичный вектор в направлении стойки и время до следующего обновления для экстраполяции)
	void			setContact(double L, double Noise, const Vec3 &p_w, const Vec3 &N_w, const Vec3 &V_w, int SurfaceType, double dt); 

	void			setWheelIF(double IF) { Whl.setIF(IF); }

	void			setWheelRelativeBrakeMoment(double RBM) { Whl.setRelativeBrakeMoment(RBM); }

	void			setSelfAttitude(bool Flag) { SelfAttitude = Flag; if(!Flag) Wobble = 0.0; }
	void			setDamperCoeff(double Coeff) { DamperCoeff = Coeff; }
	void			setWheelAxleOffset(double Offset) { WheelAxleOffset = Offset; }

    void            repair();
private:
	//имитация
	virtual void	doSimulate();

	void			model_2_simulate(const Vec3& F_l);

public:
	//доступ
	const Vec3	&	getGearForce_l()			const { return GearForce_l; }
	const Vec3	&	getGearForce_pos_l()		const { return GearForce_pos_l; }

	const Vec3	&	getSelfAttitudeForce_l()	const { return SelfAttForce_l; }		//прикладывать к ц.м!
	const Vec3	&	getSelfAttitudeMoment_l()	const { return SelfAttMoment_l; }

	double			getMountForce()				const { return MountForce; }
	double			getMountMoment()			const { return MountMoment; }

	bool			getSelfAttitude()			const { return SelfAttitude; }
	double			getDamperCoeff()			const { return DamperCoeff; }

	double			getIF()						const { return IntegrityLength; }
	bool			getDamageLengthFlag()		const {	return DamageLength > 0.0; }
	bool			getDamageYawFlag()			const { return DamageYaw != -1000.0; }


	double			getState()					const { return State; }
	double			getStrutCompression()		const; 
	bool			getFlagImmortal()			const { return Whl.FlagImmortal; }
	double			getWheelRotationPhase()		const;
	double			getWheelBrakeMoment()		    const { return Whl.getBrakeMoment(); }
    double			getWheelBrakeMomentRelative()	const { return Whl.getRelativeBrakeMoment(); }
	double			getWheelFrictMomentMax()	    const { return Whl.getFrictionMomentMax(); }
	bool			getWheelBrakeBlock()		    const { return Whl.getBrakeBlock(); }
	double			getWheelSpeedX()			    const { return Whl.getSpeedX_l_l(); }
	double			getWheelIF()					const { return Whl.getIF(); }

	double				MountPlunger;	//состояние подкоса (привода стойки) 0...PlungerMax (-1 - разорван)

private:
	Amortizater			Amort;
	Wheel				Whl;
	
	Vec3				GearForce_l;
	Vec3				GearForce_pos_l;

	Vec3				SelfAttForce_l;	//сила без момента (прикладывать к ц.м) на самоориентирующейся стойке
	Vec3				SelfAttMoment_l;//момент от демпфера на самоориентирующейся стойке

	double				MountForce;		//усилие на подкосе (приводе) стойки
	double				MountMoment;	//момент на подкосе (приводе) стойки 


	bool				ContactFlag;	//наличие контакта

	double				IntegrityLength;//откусывает высоту стойки (при разрыве пневматика)
	double				DamageLength;	//фиксирует State стойки (поломка)
	double				DamageYaw;		//фиксирует Yaw стойки (поломка)

	double				DamageLengthLim;//допуск пересжатия до поломки по State

	//геометрия стойки
	double				State;			//положение стойки убрано (0) / выпущено (1)
	GearPostDir			PDir;
	Matrix3				PD_Matrix;		//СК стойки для PD_Full
	double				WheelAxleOffset;//вынос оси колеса отн. оси стойки
	//параметры самоориентирующейся стойки:
	bool				SelfAttitude;	//true - свободноориентирующаяся стойка
	double				YawLimit;		//ограничение поворота стойки
	double				MomentLimit;	//макс. разворачивающий момент (если 0 - нет ограничения)
	double				DamageOmega;	//порог скорости заклинивания при ударе об ограничитель вращения
	double				Wobble;			//счетчик скорости шимми
	double				DamageWobble;	//порог счетчика скорости шимми 
	double				DamperCoeff;	//коэф-т демпфирования 
	double				AxleOmega;		//угловая скорость разворота стойки отн. оси 
	double				AxleFric0s;		//сухое трение оси на малой скорости
	double				AxleFric0f;		//сухое трение оси на большой скорости
	double				AxleFricVs;		//порог малой скорости сухого трения
	double				AxleFricVf;		//порог большой скорости сухого трения
	bool				GroundFricFlag;	//костыльная вещь, true - считать увязывание в грунте

	double				MountArgs[50];	//параметры описания геометрии привода и подкоса: [0] - номер модели, далее актуальные для данной модели параметры (см InitGeomtry() в GearPost.cpp)


	double				ExternLength;	//для оптимизации
	bool				WasUpdate;		//для оптимизации
	double				TimeUpdate;

	//параметры поверхности:
	Vec3				Normal_w;
	Vec3				SurfaceV_w;
	double				SurfaceNoise;	//шум поверхности (не прибавлять! нужен для определения особенностей силы трения)

	int					Element;		//номер стойки в таблице элементов ЛА (для DM)

	double				SparkTime;		// для эффекта искр
	double				DustTime;		// для эффекта пыли
	double				WheelBlockTime;	// для звукого эффекта

	double				StateOld;

//	не хозяин:	
	DynamicBody			*pDBody;
	RigidBody			*pRBody;
	EffectManager		*pEManager;
};

}

#endif