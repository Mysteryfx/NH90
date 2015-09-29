//Интерфейс модели Yo-Yo для расчета вектора индуктивной скорости в области винта.
//Результатом расчета модели является массив скоростей размером с количество
//колец, на которые разбивается винт. 

#ifndef __IFlowRotorModel_h__
#define __IFlowRotorModel_h__

#include "Optimizable.h"

namespace EagleFM
{

struct FlowRotorModelInit
{
	FlowRotorModelInit()
	{
		BladesNum		= 0;
		Clockwise		= true;
		RotorRmin		= 0.0;
		RotorRmax		= 1.0;
		RotorTwist0		= 0.0;
		RotorTwist1		= 0.0;
		RotorTwistMin	= -100.0;
		RotorTwistMax	= 100.0;
		BladeChord		= 1.0;
		BladeMass		= 10.0;
		RotorPoint_l	= Vec3(0.0);
		RotorMR			= Matrix3(1.0);	
		K_ADForceLost	= 0.93;
		K_PitchFlappReg	= 0.84;	
		SwirlingLim		= 10.0;

	}
	int					BladesNum;			//кол-во лопастей
	bool				Clockwise;			//true - вращение по часовой стрелке
	double				RotorRmin;			//радиусы винта
	double				RotorRmax;
	double				RotorTwist0;		//крутка лопасти
	double				RotorTwist1;
	double				RotorTwistMin;
	double				RotorTwistMax;
	double				BladeChord;			//TODO: может придется задавать полиномом
	double				BladeMass;
	Vec3				RotorPoint_l;		//центр винта
	Matrix3				RotorMR;			//конструктивный наклон вала винта
	double				K_ADForceLost;		//потери подъемной силы на внешнем кольце
	double				K_PitchFlappReg;	//коэфф-т РВ
	double				SwirlingLim;		//ограничение для закручивания потока
};

class IFlowRotorModel : public Optimizable
{
public:
	IFlowRotorModel(double dt) : Optimizable(dt) {} 
	virtual ~IFlowRotorModel() {}

	virtual void			init(const FlowRotorModelInit&) = 0;
	virtual void			initMachCorr(double *MachCorrM, double *MachCorrK, int MachCorrSize) = 0; //иницилизировать таблицу корректировки АД лопасти по числу М
	virtual void			initThrustIAS_Corr(double ThrustCorrMainK, double ThrustCorrK, double ThrustCorrOffset, double ThrustCorrPower) = 0; //инициализировать корректировку потребной тяги по приборной скорости

	virtual void			setSegmentsNum(int) = 0; //разбить винт на N колец
	virtual void			setIF(double*) = 0; //массив IF лопастей
	virtual void			setControl(double generalPitch, double xPitch, double zPitch, double rotateSpeed) = 0; 

	const virtual  Vec3*	getInductiveVelocity(double Azimuth, bool Param = true) const = 0;
	virtual double			getSegmentsNum() const = 0;
	virtual double			getThrust() const = 0;
	virtual double			getMu() const = 0;
	virtual double			getVindAvr() const = 0;
	virtual double			getRotorAoA() const = 0;
	
    bool                calculationCrashed;
};
}

#endif