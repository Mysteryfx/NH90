#ifndef __AerodyneFM_h__
#define __AerodyneFM_h__

//#include "Base.h"

#include "FMSpace/IBasicAtmosphere.h"
#include "FMEngineeringStructure/Airplane.h"
#include "FMEngineeringStructure/RigidBody.h"

#include "FMControlSystem/ControlSystem.h"

#include "FMElectricSystems/ElectricSystem.h"
#include "FMSensorsSystem/SensorsSystem.h"
#include "FMOxygenSystem/BaseOxygenSystem.h"

#include "FMEffects.h"
#include "FMLog/FM_Oscilloscope.h"

#define POS_BUFF_MAX 5

namespace Graphics {
	struct DynamicParam;}

namespace EagleFM
{

namespace Heat { 
	class HeatExchangeManager; }

struct FMBASE_API Payload //структура, описывающая внешнюю подвеску
{
	//0 - контейнер, 1 - элемент 
	double			Mass[2];  //[0] - масса контейнера, [1] - масса 1-го(!) элемента 
	double			Cxa0[2];  //сопротивление
	double			Cxa2[2];
	double			M_cr[2];  //критическое число М
	double			dM_sh[2]; //прибавка к числу М начала тряски
	int				ElementsCount; //кол-во элементов подвески (-1 - топливо)
	Vec3			Pos_l;
	Vec3			MassPos_l_l;
	unsigned    	Name;

	Payload()
	{
		Mass[0] = Mass[1] = 0.0;
		Cxa0[0] = Cxa0[1] = 0.0;
		Cxa2[0] = Cxa2[1] = 0.0;
		M_cr[0] = M_cr[1] = 0.0;
		dM_sh[0] = dM_sh[1] = 0.0;
		ElementsCount = 0;
		Pos_l = Vec3(0.0);
		MassPos_l_l = Vec3(0.0);
		Name = 0;
	}
};



struct FMDebugRenderObject
{
	EagleFM::Vec3 pos;
	EagleFM::Vec3 value;
};

typedef ed::vector<ed::string> DamagesList;

class FMBASE_API AerodyneFM
{
public:
	// создание/инициализация
	AerodyneFM(double dt = 0.006);
	virtual ~AerodyneFM();


	virtual void			initAirStart(const Vec3 &pos, const Vec3 &V_w, float yaw, float pitch, float roll);
	virtual void			initHotStart(const Vec3 &pos, float yaw, float pitch, const Vec3 &V_w = Vec3(0.0));
	virtual void			initColdStart(const Vec3 &pos, float yaw, float pitch, const Vec3 &V_w = Vec3(0.0));

	virtual void			setArcadeFlight(bool f)		{ FlagArcadeFlight = f; }
	virtual void			setImmortal(bool f)			{ FlagImmortal = f; if(RBody) RBody->FlagImmortal = FlagImmortal; Sensor::setFlagImmortal(FlagImmortal); }
	virtual void			setUnlimitedFuel(bool f)	{ FlagUnlimitedFuel = f; }

	virtual void			setFailureProbability(const ed::string& Name, double Probability, double TimeStart, double TimeInterval);
	virtual void			resetFailure(const ed::string& name);

	//загрузка ЛА и изменение нагрузки
	virtual void			setFuel(double FuelMass);
	virtual void			setBoostMixt(double BoostMixtMass){}
	virtual void			addFuel(double FuelMassIncrement);
	virtual void			setPayload(double Mass, const Vec3& Pos_l); //разместить груз внутри
	virtual void			setPayload(int Num, const Payload&); //разместить груз на подвеске


	// внешнее управление
	void					setMovingFreeze(bool f) { MovingFreeze = f; }
    void                    slaveToPosition(const Rot3 * orient,const Vec3  *pos,const Vec3 *vel);
    bool                    isSlavedToPosition() const { return slaved; };
	void					setPosition(const Vec3 &pos);
	void					setOrientaion(const Matrix3 &matrix);
	void					setExternalForce(const Vec3 &Force, const Vec3 &Pos, bool LocalForce = true, bool LocalPos = true);
	void					setExternalForce(const Vec3 &Force, bool LocalForce = true);
	void					setExternalMoment(const Vec3 &Moment, bool LocalMoment = true);
	virtual void			setRigidContact(int Element, double Length, double SurfaceNoise, const Vec3 &p_w, const Vec3 &Normal_w, const Vec3 &V_w, int Type, double dt); //контакт с землей
	virtual  int			getGearNumBySegment(const ed::string & segment) const { return -1; } //custom gear contact
	virtual void			setGearContact( int GearNum, double Length	 , double SurfaceNoise, const Vec3 &p_w, const Vec3 &Normal_w, const Vec3 &V_w, int Type, double dt); //контакт колесами с землей
	virtual void			setGearContact( int GearNum, const Vec3 &dp_w, double SurfaceNoise, const Vec3 &p_w, const Vec3 &Normal_w, const Vec3 &V_w, int Type, double dt) //форма с длинной в векторной форме ( нужно чтобы в некоторых случаях трактовать как жесткий контакт)
	{
		setGearContact(GearNum,length(dp_w),SurfaceNoise,p_w,Normal_w,V_w,Type,dt);
	}
	virtual void			setStructureIF(int Element, double IF); //нанести увечие элементу конструкции (IF - целостность конструкции)
	virtual void			setRefuelFlow(bool AirRefuel, double FuelFlow); //задать расход дозаправки (AirRefuel = true - воздушная, = false - наземная, расход топлива = 0 - топливо не идет, < 0 - нет контакта)
    virtual void			onTankerConnect(bool connect) {} ;
    virtual bool            isAirRefuelConnectionAvailable() const { return true; }
    //	информация
	virtual void			setUnderlyingSurface(const Vec3 &SurfacePoint_w, const Vec3 &SurfaceNormal_w); //подстилающая поверхность
	virtual void			setUnderlyingSurface2(double surface_height,
											  	  double surface_height_with_objects,
												  unsigned	 surface_type){}

	void					setDustFlag(bool Flag) { DustFlag = Flag; } //наличие пыли
	virtual void			setAnyData (const ed::string& Name, void* data);
	virtual void			setProperty(const ed::string& Name,float value )				{	setAnyData(Name,&value);	}
	virtual void			setProperty(const ed::string& Name,const ed::string& value)		{	setAnyData(Name,(void*)value.c_str());	}


	// имитация
	virtual void			simulate(double);

	// доступ
	DynamicBody*			getDynamicBody()	{ return DBody; }
	RigidBody*				getRigidBody()		{ return RBody; }
	Airplane*				getAirplane()		{ return airplane; }
	EffectManager*			getEffectManager()	{ return &effManager; }
	//для объекта мира со встроенным интерполятором достаточно сообщить скорости
	Vec3					getVelocity_w()		const;
	Vec3					getRotateSpeed_w()	const;

	Vec3					getVelocity_l()		const;
	Vec3					getRotateSpeed_l()	const;

	//для тех, у кого нет интерполятора:
	Vec3					getPosition()		const;
	Vec3					getTimePosition()	const;
	Matrix3					getOrientation()	const;

	double					getYaw()			const;
	double					getPitch()			const;
	double					getRoll()			const;

	Vec3					getWind_l()			const;
	Vec3					getWind_w()			const;

	double					getCommonAoA();		//доступ к углам атаки и скольжения в осях ЛА
	double					getCommonAoS();		

	double					getShakeValue()		const { return ShakeValue; }

	//расчет
	Vec3					calcGFactor(); //перегрузка в центре масс в ЛСК
	Vec3					calcGFactor(const Vec3 &pos_l); //перегрузка в заданной точке в ЛСК
	
	//доступ к накопившемуся массиву определившихся внутри повреждений:
	void					getDamageContacts(DamageContactVector&);


	//доступ к элементам:
	virtual ControlSystem*	  getControlSystem() = 0;
	virtual ElectricSystem*   getElectricSystem() = 0;
	virtual SensorsSystem*	  getSensorsSystem() = 0;
	virtual BaseOxygenSystem* getOxygenSystem() {return nullptr;}

	//доступ к разным данным
	virtual bool			getDestroyedFlag()									const {return false;}
    virtual bool			isSystemsDamaged()                                  const {return false;}
	virtual double			getInternalFuel()									const;
	virtual double			getExternalFuel()									const;
	virtual double			getBoostMixt()										const;

	virtual double			getRefuelFlow(bool AirRefuel, double* Fuel = 0)		const;  //узнать состояние дозаправки (< 0 - нет контакта, = 0 - топливо не идет, Fuel - полученное топливо с момента начала дозаправки)
	virtual Payload			getPayload(int Num)									const;	//заценить подвеску
	virtual double			getFlowVelocity()									const;	//индуктивная скорость за ЛА (может Vec3 ??)
	virtual double			getSurfaceNormalGearCoeff(int gearNum)				const;  //для управления шумом поверхности каждой стойки индивидуально
	virtual bool			getAnyData(const ed::string& Name, void* data)		const;
	virtual double			getEnumeratedData(unsigned index)		const { return 0; } //external FM integration

	double					getModelingDeltaTime() const; //шаг интегрирования динамики

	bool					getMovingFreezed() const { return MovingFreeze; }

	//доступ к частностям состояния (-1...1 или 0...1 или 0...Pi2):
	virtual double			getEngineRelativeRPM(int EngineNum, bool Core = true)	const; //относительная частота вращения двигателя (0...1) (Core - вал внутреннего контура, для одноконтурных не имеет значения)
	virtual double			getEngineRotationPhase(int EngineNum, bool Core = false)const; //фаза вращения двигателя (0...Pi2)
	virtual	double			getTailControlState(int ControlNum)						const; //отклонение хвостовой управ.поверхности (0 - лев., 1 - прав.)
	virtual	double			getWingControlState(int ControlNum)						const; //отклонение крыльевой управ.поверхности (0 - лев.эл, 1 - прав.эл., 2 - лев.закр., 3 - прав.закр., 4 - лев.носок, 5 - прав.закр. и т.д.)
	virtual	double			getStabilizerControlState(int ControlNum)				const; //отклонение управ.поверхности стабилизатора 
	virtual	double			getFuselageControlState(int ControlNum)					const; //отклонение управ.поверхности на ф-же (торм.щитки - даже крыльевые, фонари)
	virtual	double			getGearPostState(int GearPostNum)						const; //положение стойки (0.0 - убрано ... 1.0 - выпущена, GearPostNum: 0 - ПОШ, 1 - левая ГОШ, 2 - правая ГОШ)
	virtual	double			getGearDoorState(int GearPostNum)						const; //положение створки (0.0 - закрыта ... 1.0 - открыта)
	virtual double			getStrutCompression(int GearPostNum)					const; //величина обжатия стойки шасси (0.0 - обжата ... 1.0 - свободна)
	virtual	double			getNoseGearPostYaw()									const; //поворот носовой стойки
	virtual double			getWheelRotationPhase(int WheelNum)						const; //вращение колеса
	//еще добавить при необходимости...
    
    virtual void            repair()  {};

	DamagesList& get_damages() { return damages_list_; }

	 //for external FM connection
	virtual		void setDrawArgs(Graphics::DynamicParam * param,size_t amout) {} //for external FM connection
	virtual		void setCommand (int command,float value) {} //for external FM connection


	inline const ed::vector<FMDebugRenderObject> & getDebugRenderObjects() const { return dbg_lines; }

	/**
		* Watch function.
		* Provides a human readable string of watch's contents.
		* Returns number of bytes needed for the full info.
		* MAY be called from another thread!
	*/
	virtual size_t debugWatch(int level, char *buf,size_t maxlen) // 
	{
		return 0;
	}

protected:
	void		updateAoAandAoS();
	virtual		void collect_external_forces(double dt) {} ; //for external FM connection

	ed::vector<FMDebugRenderObject> dbg_lines;

	void oscilloscope_init(IcChart* data);

	virtual void oscilloscope_init(){}

	FM_Oscilloscope oscilloscope;
	
//хозяин:
	//	ФИЗИКА
	DynamicBody				*DBody;

	//	ПЛАНЕР
	Airplane				*airplane;

	//	ЖЕСТКОЕ ТЕЛО 
	RigidBody				*RBody;

	//	Подвески
	ed::vector<Payload>	Pylons;

	//	различные параметры
	Vec3					PosBuffer[POS_BUFF_MAX]; //буферы и счетчики для сглаживания шумов в состоянии покоя на поверхности
	Vec3					YawPitchRollBuffer[POS_BUFF_MAX];
	int						StopContactsNum;
	Vec3					StopForce_l;
	Vec3					StopMoment_l;
	bool					MovingStop;
	double					MovingStopTime;
	unsigned short			MovingStopNum;

	Vec3					Wind_l;			//ветер в центре ЛА
	Vec3					Wind_w;

	bool					DustFlag;					//наличие пыли


	double					CommonAoA;		//Общие углы атаки и скольжения
	double					CommonAoS;
	bool					CommonAoA_ready;
	bool					CommonAoS_ready;

	double					ShakeValue;		//тряска ЛА (внутри используется для физичной тряски, наружу - для головы)

	bool					MovingFreeze;	//для внешнего волшебного стоп-крана
    //  dbg
    Rot3                    slave_pos_orientation;
    Vec3                    slave_pos;
    Vec3                    slave_velocity;
    bool                    slaved;

	//	опции:
	bool					FlagArcadeFlight;
	bool					FlagImmortal;
	bool					FlagUnlimitedFuel;


    bool					ReturnForceOn;	//переворот упавшего на спину аппарата при Immortal
    double					ReturnForceAlt;	//высота начала действия силы

	//	эффекты:
	EffectManager			effManager;

	// occured faliures
	DamagesList				damages_list_;

	//heat manager
	Heat::HeatExchangeManager&	heat_manager_;
};
}



#endif