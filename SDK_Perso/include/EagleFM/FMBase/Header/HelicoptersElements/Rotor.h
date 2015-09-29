#ifndef __Rotor_h__
#define __Rotor_h__

#include "HelicoptersElements/Blade.h"
#include "HelicoptersElements/FlowRotorModel.h"

#include <ed/vector.h>

namespace EagleFM
{

typedef ed::vector<Blade*> Blades; 

class EffectManager;

class FMBASE_API Rotor
{
// ----------------------------- МЕТОДЫ ---------------------------------------
public:
	// Создание и инициализация
	Rotor(DynamicBody*, AerodynamicBody*, RigidBody*, EffectManager*);
	~Rotor();
	
	void			initReductorFriction(double Friction) { ReductorFriction = __max(0.0, Friction); }
	void			initTailRotorN(double TRN) { TailRotorN = TRN; }

	void			initInductiveMachCorr(double *MachCorrM, double *MachCorrK, int MachCorrSize); //иницилизировать таблицу корректировки АД лопасти по числу М в индуктивной модели
	void			initThrustIAS_Corr(double ThrustCorrMainK, double ThrustCorrK, double ThrustCorrOffset, double ThrustCorrPower); //инициализировать вспомогательный механизм корректировки потребной тяги винта по приборной скорости

	void			createBlades(const BladeInit&, int, //создать нужное количество лопастей
						int FlowRotorModelSegmentsNum = 3, int BladesADSegmentNum = 6); //3 - кол-во колец винта модели расчета инд. скор.; 6 - кол-во сегментов АД части лопасти
	//void			deleteBlade(int Num); ???????
	void			cleanBlades();

	void			setReductorMoment(double Moment) { ReductorMoment = Moment; }
	void			setTailRotorMoment(double Moment) { TailRotorMoment = Moment; }
	void			setReductorMOI(double MOI) { ReductorMOI = MOI; }
	void			setTailRotorMOI(double MOI) { TailRotorMOI = MOI; }
	void			setAuxDeviceMoment(double Moment) { AuxDeviceMoment = Moment; }
	void			setRotorBrakeFriction(double Friction) { RotorBrakeFriction = __max(0.0, Friction); }
	void			setDeicerRate(double Rate);
	
	void			setCirculation0(double _Circulation0)  { Circulation0 = __max(0.0, _Circulation0); }

	void			setBladeRigidBodyElement(int Num, int Element); //сопоставление индексу в списке элементов твердого тела для повреждений
	void			setBladeContact(int Num, double Radius, double OmegaMax); //контакт лопасти Num с препятствием: Radius - радиус точки контакта от оси вращения лопасти, OmegaMax - предельная разрушающая скорость вращения (но ломать надо снаружи!)
	void			setBladeIF(int Num, double IF, bool CheckAlredy = false); //checkAlredy = true - запрещает увеличивать IF

	void			setHuntingVibranceCalc(bool OnOff);
	
	void			setUnderlyingSurface(double Height, Vec3 Normal, double Azimuth); 

	void			initAirStart(double Omega);
	void			initHotStart(double Omega);
	void			initColdStart();

	void			resetMovement(double Kick = 0.0); //Kick - степень отдачи, отброс назад

	// Работа
	virtual void	simulate(double dt);

	void			applyForces();

	// Доступ
	double			getRotorRPM()						const { return fabs(Omega_l*Omega_to_RPM); } //по модулю
	double			getRotorOmega()						const { return Clockwise ? -Omega_l : Omega_l; } //это со знаком! (> 0 когда винт вращается правильно)
	double			getBladePhase(int Num)				const; 
	double			getBladeFlapping(int Num)			const; 
	double			getBladeHunting(int Num)			const; 
	double			getBladeRelativeFlapping(int Num)	const; 
	double			getBladeRelativeHunting(int Num)	const; 
	double			getBladeDeflection(int Num)			const;
	double			getBladeDeltaLimFlapping(int Num)	const;
	double			getBladeIF(int Num)					const;
	int				getBladesNum()						const { return (int)blades.size(); }
	double			getTailRotorN()						const { return TailRotorN; }
	double			getRotorBrakeFriction()				const { return RotorBrakeFriction; }
	virtual double	getInductiveFlow()					const;
	double			getMu()								const;
	double			getRotorAoA()						const;
	double			getShakeValue()						const { return ShakeValue; }
	double			calcBladeFlappingHeight(int Num, double Azimuth = -1.0)	const;
	bool			getCirculation(double lim = 10.0)	const;
	double			getVindAvr()						const;

	//DEBUG!!!!!!!!!!!!!
	double			getThrust()		const; //TEMP!!!!!!!!!!! must be applyForces()!!!!
	Vec3			getForce_l()	const { return Force_l;	}
	Vec3			getMoment_l()	const { return Moment_l;	}
	double			getRotorMoment()const { return RotorMoment; }
	double			getVind(int RingNum) const; //TEMP DEBUG!!!!!!!
    

    void            repair();
    bool            getIsDamaged() const;

protected:

// ----------------------------- ПОЛЯ ---------------------------------------

//Не хозяин:
	DynamicBody		*pDBody;			//динамика вертолета
	AerodynamicBody *pFanADBody;		//АД допасти
	RigidBody		*pRBody;			//твердое тело
	EffectManager	*pEManager;			//для эффектов

//Хозяин:
	Blades			blades; 
	IFlowRotorModel	*FRModel;

	Vec3			Force_l;			//в осях вертолета!!!
	Vec3			Moment_l;

	double			Omega_l;			//угловая скорость вращения винта относительно вертолета
	double			Omega_w;			//угловая скорость вращения винта в мире
	bool			Clockwise;			//true - вращение по часовой

	double			RotorStopTimer;		//для состояния покоя

	double			ShakeValue;			//тряска с лопастей

	Matrix3			MR;					//ориентация колонки винта в осях вертолета
	Matrix3			MR_T;				//MR.transposed()

	double			K_PitchFlappReg;	//коэфф-т регулятора взмаха
	double			PhaseFlappReg;		//опережение на регулятор взмаха (atan(K_PitchReg))

	double			RotorMoment;		//Момент от винта на валу (потребный) - суммарный от лопастей
	double			ReductorMoment;		//Момент, прикладываемый к винту от редуктора
	double			ReductorMOI;		//момент инерции редуктора
	double			TailRotorMoment;	//Момент, сьедаемый рулевым винтом (компенсирующим)
	double			TailRotorMOI;		//Момент инереции рулевого винта (компенсирующего)
	double			TailRotorN;			//Передаточное число в редукторе между НВ и РВ
	double			AuxDeviceMoment;	//Момент от потребителей (генераторы, гидравлика...) редуктора
	double			ReductorFriction;	//трение редуктора
	double			RotorBrakeFriction;	//трение, создаваемое тормозом винта (0....) - прибавляется к ReductorFriction
	
	double			VindAvr;			//средняя индуктивная скорость на всем винте

	double			Circulation0;		//установка извне ВК

	double			TimeDefVortexPhaseMax; //для хаотичности вихревого кольца (случайный интервал переопределения азимута макс.инд.скорости от вихревого кольца)
	Randomizer		Rnd;	

	double			BladeLimBlowTime;	//таймер ударов лопастей по упорам для управления звуковым эффектом

public:
	double			CyclicX_Pitch;		
	double			CyclicZ_Pitch;		
	double			GeneralPitch;	
};

}

#endif