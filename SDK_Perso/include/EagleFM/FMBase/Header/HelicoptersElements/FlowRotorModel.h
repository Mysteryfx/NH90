//Модель Yo-Yo для расчета вектора индуктивной скорости в области винта.
//Результатом расчета модели является массив скоростей размером с количество
//колец, на которые разбивается винт. Достаточно 3х колец
//Модель основывается на теореме о кол-ве движения и методе элемента лопасти (МЭЛ):
//расчет индуктивной скорости по тяге и тяги по индуктивной скорости и поиск
//решения (пересечения кривых) за несколько итераций
//Все расчеты проводятся в СК управления, так как средний угол атаки
//лопасти по азимуту в таких осях постоянный
//Модель необходимо инициализировать параметрами движения всего вертолета, 
//геометрическими параметрами винта и аэродинамикой лопасти

#ifndef __FlowRotorModel_h__
#define __FlowRotorModel_h__

#include "FMDynamics/DynamicBody.h" 
#include "FMAerodynamics/AerodynamicBody.h"
#include "HelicoptersElements/IFlowRotorModel.h"

namespace EagleFM
{

class FlowRotorModel : public IFlowRotorModel
{
//------------------- МЕТОДЫ -------------------------------
public:
	FlowRotorModel(DynamicBody*, AerodynamicBody*); 
	virtual ~FlowRotorModel();

	virtual void			init(const FlowRotorModelInit&);
	virtual void			initMachCorr(double *MachCorrM, double *MachCorrK, int MachCorrSize); //иницилизировать таблицу корректировки АД лопасти по числу М
	virtual void			initThrustIAS_Corr(double ThrustCorrMainK, double ThrustCorrK, double ThrustCorrOffset, double ThrustCorrPower); //инициализировать корректировку потребной тяги по приборной скорости

	virtual void			setSegmentsNum(int); //разбить винт на N колец
	virtual void			setIF(double*); //массив IF лопастей
	virtual void			setControl(double generalPitch, double xPitch, double zPitch, double rotateSpeed) 
							{ GeneralPitch = generalPitch, ControlX_Pitch = xPitch, 
								ControlZ_Pitch = zPitch; RotateSpeed = rotateSpeed; }

	
protected:
	virtual void			doSimulate();

	void					setAtmoProperties();
	//вспомогательные функции для расчета инд.скорости методом схождения:
	double					calcRingInductiveSpeed(double Thrust, int i); //промежуточный расчет инд.ск. при данной тяги 
	double					calcRingThrust(double v_ind, int i); //промежуточный расчет тяги при данной инд.ск. для данного сегмента

	double					calcMachCorr(double); //расчет корректировки по числу Маха для расчета тяги элемента лопасти

	double					calcThrustCorrK();

public:
	const virtual  Vec3*	getInductiveVelocity(double Azimuth, bool Param = true) const; //Param - для наследника
	virtual double			getSegmentsNum() const { return SegmentsNum; }
	virtual double			getThrust() const { return RotorThrust; }
	virtual double			getMu() const { return 0; } //TODO
	virtual double			getRotorAoA() const { return RotorAoA; }
	virtual double			getVindAvr() const { return VindAver; }

//------------------- ПОЛЯ -------------------------------

protected:
//не хозяин
	DynamicBody			*pDBody;			//сам вертолет
	AerodynamicBody		*pFanADBody;		//аэродинамика лопасти

//хозяин
	int					SegmentsNum;		//кол-во колец винта (элементов лопасти в МЭЛ)
	
	//управление
	double				GeneralPitch;
	double				ControlX_Pitch;	//углы управления с учетом РВ 
	double				ControlZ_Pitch;
	double				RotateSpeed;		//угловая скорость вращения винта

	double				RotorV;				//скорость вертолетика в центре винта с учетом ветра 
	Vec3				ControlAxis;		//ось управления (отн.которой угол установки лопасти не меняется)
	double				RotorAoA;			//угол атаки к плоскости управления
	double				sinRAoA;
	double				cosRAoA;
	
	double				AtmoDensity;
	double				AtmoSoundSpeed;
	Vec3				Wind_l;
	bool				Turbulence;

	double				BladePitch;			
	double				Flapping;			//средний взмах (для учета РВ в AoA элемента лопасти)
	double				*RingRmin;			//массив радиусов колец винта	
	double				*RingRmid;		
	double				*RingRmax;		
	double				*RingIF;			//средний Integrity Factor на кольце (массив по кольцам)
	double				*RingBladeThrust;	//тяга одной лопасти на кольце (массив по кольцам) - для расчета Flapping

	double				RotorThrust;		//Тяга винта по индуктивной модели
	Vec3				*Vind;				//ради чего вся это модель (массив скоростей по кольцам)
	double				VindAver;			//средняя инд.скорость

	//		Настроечные параметры: 
	int					BladesNum;			//кол-во лопастей
	bool				Clockwise;			//true - вращение по часовой стрелке
	double				RotorRmin;			//радиусы винта
	double				RotorRmax;
	double				RotorTwist0;		//крутка винта
	double				RotorTwist1;
	double				BladeMass;

	double				*MachCorrM;			//таблица корректировщика Маха - числа М
	double				*MachCorrK;			//к-т корректировки на данном числе М
	int					MachCorrSize;		//размер массива

	//Vec3				RotorPoint_l;		//центр винта
	Matrix3				RotorMR;			//конструктивный наклон вала винта
	double				K_ADForceLost;		//потери подъемной силы на внешнем кольце
	double				K_PitchFlappReg;	//коэфф-т РВ
	
	double				SinPhaseFlappReg;	//sin опережения на РВ 
	double				CosPhaseFlappReg;	//cos опережения на РВ

	bool				ThrustCorrOn;		//механизм возможной корретировки потребной тяги по приборной скорости 
	double				ThrustCorrMainK;	//Vind *= 1 + ThrustCorrMainK*EXP(-ThrustCorrMainK*((IAS-ThrustCorrOffset)^ThrustCorrPower)
	double				ThrustCorrK;		//ВНИМАНИЕ! Механизм востребовался из-за серьезных расхождений полученных потребных тяг для Ка-50
	double				ThrustCorrOffset;	//(совпадающих с данными фирмы "Камов") с растчеными по вертикальным скоростям (данные фирмы) потребными тягами:
	double				ThrustCorrPower;	//расхождение привело к завышенным возможностям по динамическим потолкам и вертикальным скоростям при V<>0.
};

}

#endif