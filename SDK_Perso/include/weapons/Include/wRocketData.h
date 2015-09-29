//                    file wRocketData.h
#pragma once
#include "Weapons.h"

#include "Math/Vector.h"
#include "cPosition.h"

class wSimulationSystemScheme;

// тип головки самонаведения ГСН:
const int InfraredSeeker	= 1;	// тепловая IR (infrared seeker)
const int ActiveRadar		= 2;	// активная-радиолокационная (active radar (AR) (+ИНС)) 
const int AntiRadar			= 3;	// антирадарная (пассивный радар +ИНС)
const int LaserHoming		= 4;	// лазерный подсвет (+ИНС)
const int Autopilot			= 5;	// автономная (ИНС+карта, GPS,TV,IIR...)
const int SemiActiveRadar	= 6;	// полуактивная-радиолокационная semi-active radar (SAR) -радиоподсвет
const int SemiAutoAT	    = 7;	// полуавтоматическое управление с платформы для ПТРК, летят на woPoint, координаты woPoint меняются платформой.
const int RemoteControl		= 8;	// телеуправление


		// параметры модели
	/*
	0  характерестическая площадь
	1  Сх_k0
	2  Cx_k1
	3  A    коэф отвала поляры
	
	4  Су_k0
	5  Cy_k1
	
	6  Alfa_max  максимальный балансировачный угол
	
	7  time_start	 стартовый интервал
	8  time_b		 бустерный интервал
	9  time_accel	 разгонный интервал
	10 time_march    маршевый интервал
	11 time_inertial инерциальный интервал
	12 time_break    тормозной интервал
	13 time_end		 оконечный интервал

	14 ffr_start	
	15 ffr_b		 расход массытяга двигателя на бустерном режиме 
	16 ffr_accel	 расход массытяга двигателя на разгонном режиме
	17 ffr_march     расход массытяга на маршевом режиме
	18 ffr_inertial
	19 ffr_break     расход массы на тормозном режиме
	20 ffr_end


	21  trust_start	
	22  trust_b		    тяга двигателя на бустерном режиме 
	23  trust_accel	    тяга двигателя на разгонном режиме
	24  trust_march     тяга на маршевом режиме
	25  trust_inertial
	26  trust_break     тяга на тормозном режиме
	27  trust_end
	*/
	
typedef enum
{
/* 0 */	mdpS,
	mdpCx_k0, 
	mdpCx_k1,
	mdpCx_k2,
	mdpCx_k3,
	mdpCx_k4,
/* 6 */	mdpA,    //коэф отвала поляры       

	mdpCy_k0,
	mdpCy_k1,
/* 9 */	mdpCy_k2,
/*10 */ mdpAlfa_max,  //максимальный балансировачный угол
	mdpAlfaGasDynamicOmega, // угловая скорость создаваймая моментом газовых рулей

	mdpTime_start,	 //стартовый интервал
	mdpTime_b,		 //бустерный интервал
	mdpTime_accel,	 //разгонный интервал
	mdpTime_march,   // маршевый интервал
	mdpTime_inertial, //инерциальный интервал
	mdpTime_break,    //тормозной интервал
/* 18 */mdpTime_end,		 //оконечный интервал

	mdpFfr_start,	
	mdpFfr_b,		 //расход массытяга двигателя на бустерном режиме 
	mdpFfr_accel,	 //расход массытяга двигателя на разгонном режиме
	mdpFfr_march,    // расход массытяга на маршевом режиме
	mdpFfr_inertial,
	mdpFfr_break,     //расход массы на тормозном режиме
/* 25 */mdpFfr_end,


	mdpTrust_start,	
	mdpTrust_b,		    //тяга двигателя на бустерном режиме 
	mdpTrust_accel,	    //тяга двигателя на разгонном режиме
	mdpTrust_march,     //тяга на маршевом режиме
	mdpTrust_inertial,
	mdpTrust_break,     //тяга на тормозном режиме
/* 32 */mdpTrust_end,

	mdpSelfDestroyLatency, // таймер самоликвидации
	mdpEnergySystemTime, // время работы энергосистемы
	mdpSelfDestroyGroundDistance, // земная высота самоликвидации
	mdpControlSafeLatency,   // время задержки включения управленя (безопасный отлет ракеты от носителя)
	mdpLoftOnTrajectoryTargetDistance, // дальность до цели в момент пуска, выше которой выполняется маневра набора высоты
	mdpLoftRelaxTrajectoryTargetDistance, // дальность до цели на трассе, ниже которой начинается завершения маневра набора высоты 
	mdpLoftSine, // синус угла тректории набота горки
/* 40 */mdpFuseAccel, // продольное ускорение взведение узрывателя

	mdpLiftingVelocity, // модуль скорости сообщаймый катапультным устройством, вышибным зарядом и тд
	mdpACS_K0, // характристика системы САУ-РАКЕТА,  коэф фильтра второго порядка K0
	mdpACS_K1, // характристика системы САУ-РАКЕТА,  коэф фильтра второго порядка K1
	mdpACS_Passband, // характристика системы САУ-РАКЕТА,  полоса пропускания контура управления
	mdpD0, // дальность полета в горизонт с располагаемой перегрузкой Navail >= 1.0 на высоте H=0
/* 46 */mdpdD_dH, // крутизна зависимости  дальность полета в горизонт с располагаемой перегрузкой Navail >= 1.0 от высоты H ППС
	mdpdD_dH_back, // крутизна зависимости  дальность полета в горизонт с располагаемой перегрузкой Navail >= 1.0 от высоты H в ЗПС
	mdpNu, // безразмерный коэф. эффективности САУ ракеты
	mdpActiveFlightTime, // расчет времени полета

/*50*/  mdpFrontDLZ_k, // масштабный коэф коррекции дальности зоны пуска в ППС по неманевренной цели
	mdpFront2DLZ_k, // масштабный коэф коррекции дальности зоны пуска в ППС по маневренной цели
	mdpBackDLZ_k, // масштабный коэф коррекции дальности зоны пуска в ЗПС по маневренной цели
/*53*/	mdp_dDdG, // dD/d(gamma), крутизна характеристики снижения дальности пуска по углу визирования цели головкой ракеты на старте
	mdp_dDdA0, //  параметр измениения дальности от угла бросания
	mdp_dDdA1, //  параметр измениения дальности от угла бросания
	mdp_dDdA2, //  параметр измениения дальности от угла бросания
	mdp_dDdA3, //  параметр измениения дальности от угла бросания (крутизна х-ки по высоте)
	mdpParamCount
} ModelDataParams;

struct Warhead_Const
{
	enum Type
	{
		TYPE_AP,
		TYPE_HE,
		TYPE_SHAPED_EXPLOSIVE
	} type;
	float caliber;
	float mass;
	float explosiveMass;
	float shapedExplosiveMass;
	float shapedExplosiveArmorThickness;
};

struct WEAPONS_API Rocket_Const // Ракетные постоянные и настройки для 
{
	// законов управления. 
	// Характеристики ракеты
	unsigned char Name_;     //  имя ракеты
	int           Escort_;   //  сопровождение: 0 - нет, 1 - ЛА пуска, 
	                         //  2 - другой ЛА, 3 - c земли
	int           Head_Type_;// Тип головки самонаведения (ГСН)(cм выше) 
	float         sigma[3];
	float         M_;        //  полная масса в кг
    float         H_max_;    //  максимальная высота полета. 
    float         H_min_;    //  минимальная высота полета. 	
	float         Diam_;     //  Диаметр корпуса в мм 
	float         Cx_pil;    //  Cx как подвески
	float         D_max_;    //  максимальная дальность пуска
	float         D_min_;    //  минимальная дальность пуска
	bool          Head_Form_;// false - полусферическая форма головы, 
	                         // true   - оживальная (~коническая)
	float         Life_Time_;//  время жизни
	double        Nr_max_;   // Максимальная перегрузка при разворотах
    float         v_min_;    // Минимальная скорость. 
	float		  v_mid_;	 // Средняя скорость
    float         Mach_max_; // Максимальное число Маха.
	float         t_b_;      // время включения двигателя
    float         t_acc_;    // время работы ускорителя
    float         t_marsh_;  // время работы в маршевом режиме
	float         Range_max_;// максимальная дальность прицельного пуска, м
    float         H_min_t_;  // минимальная высота цели над рельефом, м. 	
	float         Fi_start_; // угол сопровождения и визирования при пуске  
	float         Fi_rak_;   // допустимый угол ракурса цели (rad)
	float         Fi_excort_;// угол сопровождения (визир.) цели ракетой.
	float		  Fi_search_;// предельный угол свободного поиска
	float		  OmViz_max_;// предельная скорость линии визирования
    float         Damage_;// повреждение, наносимое при прямом попадании
/*	int           Engine_Type_; // тип двигателя: 1 - твердотопливный;
	                         // 2 - Жидкостный Ракетный Двигатель(РД)(ЖРД); 
	                         // 3 - Прямоточный Воздушный РД ; 
	                         // 4 - ускоритель-1+ЖРД.
	                         // 5 - турбореактивный
	                         // 6 - турбореактивный + ускоритель 
	int           Stage_;    // количество ступеней.*/
	float         X_back_;   // координаты центра сопла в осях ракеты
	float         Y_back_; 
	float         Z_back_;
    bool          hasDetachableAccelerator_; // имеет отделяемый ускоритель
    float         X_back_acc_; // координаты сопла ускорителя в осях ракеты
    float         Y_back_acc_;
    float         Z_back_acc_;
	float		  Reflection;	// эффективная поверхность радиоотражения

    // Kill distances - this distance is used to fire a fuze
    double        KillDistance;

	double        ccm_k0;				//базовый к-т вероятности ухода на другую цель (ловушку)

	// IR lock params:
	double		  SeekerSensivityDistance;
	bool		  SeekerCooled;


	float		  ModelData[mdpParamCount] ;

	Warhead_Const warhead;
    // These are warheads used to simulate explosions
    // Due to the architecture of blocksim we have to use two schemes - 
    // one for server object (which actually makes damage), and other for
    // client object (which does not make any damage)
    wSimulationSystemScheme* warheadServer, *warheadClient;

	struct Exhaust
	{
		double r, g, b, a;
		bool active;
	};
	
	Exhaust  exhausts[2];
	int		 exhausts_count;
	
	bool getVelocityOnHeight(float H, float & missVmax, float & missVaverage) const;

	float getDmaxLaunch(float H) const;
	
	float getDmaxLaunch(float H, float targetСosAspect, float targetVel) const;

	float getDFlight(float H, float distToTarget, float targetСosAspect, float targetVel, float & V, double & TOF) const;

private:

	bool getDmaxFlight_(float H, float & launchDist) const;

	bool calcAngles_(float H, float targetСosAspect, float missileVel, float targetVel, float & sinA, float & sinB) const;

	float getDmaxLaunch_(float H, float & flightDistMax, float & Vmax, float & Vav) const;

	float getDmaxLaunch_(float H, float targetСosAspect, float targetVel, float & flightDistMax, float & Vmax, float & Vav) const;
};

struct WEAPONS_API Bomb_Const // Характеристики бомбы. 
{                    	        
	unsigned char Bomb_Name_;   // имя бомбы
	int           Bomb_Type_;   // тип: 0 - остальные; 3-TV управляемые
	                            //  1 - для бетона; 2 - c парашютом
	                            //  4 - c лазерной подсветкой; 5-контейнерные;
								//  6 - со спутниковым наведением
	float         Bomb_Massa_;  // масса в кг
    float         H_min_;       //  минимальная  высота сброса, м 	
	float         H_max_;       //  максимальная высота сброса, м 
	float		  C_x_;			// коэффициент сопротивления как подвески
	float		  VyHold_;		// установившаяся скорость падения, м/с
	float		  Ag_;			// горизонтальное ускорение торможения, м/сc

    Warhead_Const warhead;
	// These are warheads used to simulate explosions
    // Due to the architecture of blocksim we have to use two schemes - 
    // one for server object (which actually makes damage), and other for
    // client object (which does not make any damage)
    wSimulationSystemScheme* warheadServer, *warheadClient;
};



void WEAPONS_API DLZ(Rocket_Const * data,
					 float   H, // H - высота пуска ракеты
					 float  Vn, // Vn - скорость носителя при пуске
					 float	fi, // угол визирования цели
					 float  Vs, // Vs - скорость сближения в момент пуска ( сближение - положит. удаление - отриц)
					 float  dH, // dH - разность высот цели и носителя в момент пуска
					 

					 const cVector& Vp ,
					 const cVector& Vt ,
					 const cVector& r , // радиус вектор носитель-цель
					
					 float& d1, // d1 - расчетная дальность пуска по не маневрирующей цели а ППС/ЗПС по маневрирующей в ЗПС
					 float& d2, // d2 - расчетная дальность пуска по маневрирующей в ППС
					 float& d3 ); // d3 - дальность полета