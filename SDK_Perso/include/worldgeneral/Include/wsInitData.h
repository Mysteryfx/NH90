//              file wsInitData.h
#ifndef __wsInitData_h__
#define __wsInitData_h__

#include "WorldGeneral.h"
#include "..\..\WeaponsBase\Include\WeaponsBase.h"
#include "..\..\WeaponsBase\Include\wWeaponUtils.h"
#include "wsType.h"
#include "wsPilonLoad.h"
#include "GraphMath\vector.h"

#include "EffectConnectorType.h"

namespace Lua
{
    class Config;
}

class Position3;

namespace Graphics
{
	class ModelConnector;
	class ModelInstance;
}

enum wsBirthPlace // место рождения самолетов
{
	wsBirthPlace_Unknown,		// 
	wsBirthPlace_Air,			// в вздухе, автоматическая генерация
	wsBirthPlace_AirManual,     // не используется		// в вздухе, ручная расстановка
	wsBirthPlace_Ship,			// на корабле
	wsBirthPlace_RunWay,		// на ВПП
	wsBirthPlace_Park,			// на аэродромных стоянках
	wsBirthPlace_OpenPark,      // не используется		// на открытых аэродромных стоянках
	wsBirthPlace_ShelterPark,   // не используется	// в аэродромных укрытиях
	wsBirthPlace_Land,			// на земле, ручная расстановка
	wsBirthPlace_Heliport_Hot,	// на вертолетных площадках (RunWay)
	wsBirthPlace_Heliport_Cold, // на вертолетных площадках (Cold)
    wsBirthPlace_Ship_Cold,     // на корабле
	wsBirthPlace_Ship_Hot,
    wsBirthPlace_Park_Hot,  	// на аэродромных стоянках (hot)
};

enum wsBirthPlace_Generalized
{
	BIRTH_COLD,
	BIRTH_HOT,
	BIRTH_IN_AIR,
};

inline wsBirthPlace_Generalized general_birthplace_condition(unsigned place)
{
	switch (place)
	{
	case wsBirthPlace_Ship:			
	case wsBirthPlace_RunWay:	
	case wsBirthPlace_Heliport_Hot:	
	case wsBirthPlace_Land:			
	case wsBirthPlace_Park_Hot: 
		return BIRTH_HOT;
	case wsBirthPlace_Park:			
	case wsBirthPlace_OpenPark:      
	case wsBirthPlace_ShelterPark:   
	case wsBirthPlace_Heliport_Cold: 
	case wsBirthPlace_Ship_Cold:   	
		return BIRTH_COLD;
	}
	return BIRTH_IN_AIR;
}

enum wcCoalitionName;
struct wDetectorInfo;


struct customUnitProperty
{
	ed::string str_value;
	float      value;
};
typedef ed::unordered_map<ed::string, customUnitProperty> customUnitPropertiesMap;

struct WORLDGENERAL_API wsInitData
{	
	customUnitPropertiesMap AddPropList; // список дополнительных параметров ЛА


	const customUnitProperty * getProperty(const char * name) const 
	{
		auto it = AddPropList.find(name);
		if (it != AddPropList.end())
		{
			return &it->second;
		}
		return nullptr;
	}

	unsigned int	id;				// Порядковый номер.
    wcCoalitionName coalition;		// номер государства (коалиции)
    wsType          sort;			// Tип объекта 
    float           place[3];		// Координаты радиуса-вектора: x, y, z.	
	bool			ralt;			// Радиовысота, а не от уровня моря
	float           speed;			// Cкорость.
	float           psi;			// Курс в радианах.  
	float           M_fuel_;		// Начальная масса топлива, кг.
	float           M_Extra_fuel_;	// Начальная масса доп. топлива (напр. MW-50), кг.
	int             Skill_;			// Уровень мастерства (0,1,2,3,4-худшее)
	int             State_;			// Cтрана
    unsigned        Paint_;			// вариант раскраски
	ed::vector<wsPilonLoad> Laun_List;	// указатель на список подвесок
	int				N_inWing;		// номер в звене
	int				LeaderPosNum;	// номер позиции лидера в звене
	ed::string		BoardNumber;	// бортовой номер
	ed::string		name;		// позывной
    ed::string		unit_type;		// тип из миссии
	wsBirthPlace	BirthPlace;		// место рождения
	int				callsign;	// номер позывного
	float			frequency;
	bool			Suspended_;		// true - рождение самолета должно быть задержано до особого (сетевого) распоряжения.

    double          gunLoadFactor; // Загрузка пушки в долях (0-1.0)
	int             gunAmmoType;   // Набивка патронов пушки

	bool			uncontrolled;

    ed::string     missionID;

	bool			under_civil_registration;//fully disarmed
	bool			hardpoint_racks;//pilons

    wsInitData();
	~wsInitData();
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wsInitData& data);
};
    
struct wsAirPos
{
	float           point[3];   // Координаты радиуса-вектора: x, y, z.	
	bool			ralt;
	float           psi;        // Курс в радианах.
	float			speed;		// Скорость
};

struct WeaponGuidance_Const
{
	WeaponGuidance_Const(unsigned int channels_, bool TWS_) : channels(channels_), TWS(TWS_) {}
	unsigned int	channels;
	bool			TWS;
};

enum tanker_type
{
    NOT_A_TANKER,
    /*
    Two different methods are used to connect tanker to receiver: 
    the flying boom system (sometimes called boom and receptacle), 
    and the probe and drogue system. The less popular wing-to-wing system is no longer used.
    */
    BOOM_AND_RECEPTACLE = 1,
    PROBE_AND_DROGUE_1  = 2,//1 basket
    PROBE_AND_DROGUE_2  = 3,//2 basket
    PROBE_AND_DROGUE_3  = 4,//3 basket
};

struct  EngineNozzlePos
{
    float       Diam_s_;       // Диаметр сопла при полном форсаже
    float       Length_s_;     // Длина факела сопла при полном форсаже
    float       K_for_;        // Коэф-т для длины факела и диаметра
	int			referenced_engine; //engine number which this nozzle handled , (for example all 4 nozzles of Harrier serve with one engine)
	float		smokiness_level;
    Vector3     pos;
	Vector3	    orientation;
};
typedef ed::vector<EngineNozzlePos>  engines_nozzle_pos_info;

struct  CrewMemberData
{
    unsigned char ejection_seat_name;
    unsigned char drop_canopy_name;
	unsigned char pilot_name;
    Vector3       pos;
	Vector3		  canopy_pos;
	int			  ejection_play_arg;
	bool		  ejection_through_canopy;
	Vector3		  canopy_ejection_dir;
};
typedef ed::vector<CrewMemberData>  crew_members_info;


struct ChaffFlare_Position // Координаты тепловых ловушек в самолетной системе координат 
{                  
    Vector3         pos;
    Vector3         direction;
};
typedef ed::vector<ChaffFlare_Position>  chaff_flare_dispenser_pos_info;

struct FirePos_Info 
{              
	FirePos_Info() : present_(false) {}

	Vector3      pos_;
	bool         present_;
};

typedef ed::vector<FirePos_Info>  FirePosInfos;

enum WingType
{
	FIXED_WING			= 0,
	VARIABLE_GEOMETRY	= 1,
	FOLDED_WING			= 2,
};


// модель повреждений
struct cells_properties
{
	cells_properties(){critical_damage = 0; arg_max = 1.0f;droppable = true; }

	float critical_damage;
	float arg_max;       //максимальное значение основного аргумента при критическом повреждении (по умолчанию 1)
	ed::vector<int> args;
	ed::vector<int> deps_cells; // зависящие части модели

	bool	droppable;
};
struct RefuelingPoint
{
	enum ClientType { 
		CLIENT_TYPE_NULL = 0,
		CLIENT_TYPE_SMALL = 1 << 0,
		CLIENT_TYPE_LARGE = 1 << 1
	};
	unsigned int clientType;
	Vector3 pos;	
};

typedef ed::vector<RefuelingPoint> RefuelingPoints;



struct HelicopterEngine_Const //вертолетный двигатель //(МОЖЕТ СТОИТ ПРОСТО ИНКЛУДИТЬ СТРУКТУРУ ИНИЦИАЛИЗАЦИИ ИЗ EagleFM??)
{
	float			PowerTO;		//взлетная мощность (кВт)
	float			PowerMax;		//максимальная мощность
	float			PowerWEP;		//кратковременная мощность
	float			PowerTH_k[4][3];//коэф-ты полинома расчета мощности (кВт) на высоте (км) по температуре на уровне моря 
	float			SFC_k[3];		//коэф-ты полинома расчета расхода топлива по мощности кг/кВт/час 
	float			PowerRPM_k[3];	//Обороты ТК от относительной мощности
	float			PowerRPM_Min;	//отн. мощность до МГ 
	ed::string		SoundName;		//имя файла звука
};

struct CannonSightConst
{
	enum Type { TYPE_NO_SIGHT, TYPE_IP_CALCULATOR, TYPE_TARGET_TRACKER };
	unsigned type;
};
struct Plane_Const;
struct Helicopter_Const;

namespace plane_lights { 
	class	woLA_LightPrototype;
}

class  wAircraftGunEquipmentDescriptor;

struct AI_aim_controller
{
	AI_aim_controller(){teta[0]  =
		teta[1]  =

		gamma[0] =
		gamma[1] =

		course_to_gamma[0] =
		course_to_gamma[1] = 0.0f;};

	float teta[2];	 // тангаж
	float gamma[2];  // крен
	float course_to_gamma[2]; // путевое управление
};

struct Internal_Cargo_Capacity
{
	int nominal;	 // номинальная вместимость
	int maximal;     // максимальная вместимость
};	

struct wShape;
struct WORLDGENERAL_API Aircraft_Descriptor // Common description
{   
protected:
	wShape  *	shape;
public:
	Aircraft_Descriptor();
	virtual ~Aircraft_Descriptor();
	ed::string			unit_type;
	wsType				ws_type;
    float				M_empty_;		// масса сухого самолета в кг
	float				M_nominal_;		// номинальная масса в кг
    float				Massa_max_;		// максимальная масса в кг
    float				M_fuel_max_;	// максимальная масса топлива в кг
	float				M_Ex_fuel_max_;	// максимальная масса доп. топливной смеси (напр. MW-50) в кг
    Vector3				Main_gear_pos_; // координаты X,Y и Z нижней кромки выпущенного основного шасси в самолетной с-ме координат
    Vector3				Nose_gear_pos_; // координаты X,Y и Z нижней кромки выпущенного переднего шасси в самолетной с-ме координат

	float				Nose_gear_amortizer_direct_stroke;
	float				Nose_gear_amortizer_reversal_stroke;
	float				Main_gear_amortizer_direct_stroke;
	float				Main_gear_amortizer_reversal_stroke;
	float				Nose_gear_amortizer_normal_weight_stroke;
	float				Main_gear_amortizer_normal_weight_stroke;
	bool				Nose_gear_door_close_after_retract;
	bool				Main_gear_door_close_after_retract;



	inline float		nose_gear_amortizer_length() const { return Nose_gear_amortizer_direct_stroke - Nose_gear_amortizer_reversal_stroke; }
	inline float		main_gear_amortizer_length() const { return Main_gear_amortizer_direct_stroke - Main_gear_amortizer_reversal_stroke; }

	float				D_front_gear_;  // Диаметр колеса переднего шасси, м
	float				D_main_gear_;   // Диаметр колеса осовного шасси, м
	float				Tand_gear_max_;	// Максимальный тангенс угла поворота пер. стойки шасси  


	float				Length_;		// общая длина самолета,вертолета с вращ. винтами, м
	float				Height_;		// общая высота самолета,вертолета м
	float				Vy_max_;		// максимальная скороподъемность,м/сек

	unsigned			engines_count_;
	float				apu_rpm_delay_;	// задержка запуска (для звука)
	bool				Can_See_Ground_;// true - радар видит на фоне земли 
	float				Reflection;	    // эффективная поверхность радиоотражения
	float				RadarMaxDist;	// максимальная дальность обнаружения "большой цели" (100м2)
	float				Kmax;			// отношение дальности "теплозаметности" к Su-27 режиме max
	float				Ny_max_;		// предельная эксплуатационная перегрузка
	float				Range_;			// дальность полета, км
	unsigned char		Tanker_Type_;	// тип заправщика	
	Vector3             Air_refuel_receptacle_pos_;
	bool				bigParkingRamp;

    Internal_Cargo_Capacity      internal_cargo_capacity; //максимальный вес внутреннего груза

	crew_members_info			 crew_info_;

	FirePosInfos				fires_pos_info_;

	ed::vector<unsigned short int>		 net_animation_args_;
	ed::map<int,ed::string>			 dmg_parts;
	ed::map<int, cells_properties*>		 dmg_cells_properties;

	engines_nozzle_pos_info				 engines_nozzles_;

	typedef std::pair<float, float>			mech_times; // фаза, скорость 
	typedef ed::vector<mech_times>			mech_times_table;
	
	enum mech_time_type
	{
		CANOPY_OPEN_TIMES,
		CANOPY_CLOSE_TIMES
	};

	ed::vector<mech_times_table>				 mech_timing;

	plane_lights::woLA_LightPrototype		  * lights;
	AI_aim_controller						  * aim_controller;
	chaff_flare_dispenser_pos_info				chaff_flare_dispenser_info_;

	wAircraftGunEquipmentDescriptor			  * guns;

	wDetectorInfo							  * detector;
	//коннекторы для стандартных эффектов по их типу
	Graphics::ModelConnector		*effectConnectors[CT_VALUES_COUNT];	

	virtual void	  				read(Lua::Config & config);

	virtual	const Plane_Const		*Airplane() const  { return nullptr;};
	virtual	const Helicopter_Const	*Helicopter() const { return nullptr;};
	
	const mech_times_table	  & get_mech_times_table(mech_time_type timing_type) const;
	const cells_properties*		get_cells_properties(int index) const;
	const char *				get_damage_part_name(int index) const;


	wShape *			getShapeDeclaration() const;
	wShape *			getDestructionShapeDeclaration() const;
	float				getLife()  const;

	/*прочитать стандартные коннекторы для эффектов*/
	void				InitEffectsConnectors(Graphics::ModelInstance &instance);

	WeaponGuidance_Const getWeaponGuidance(const wsType & missile_type) const;

	ed::string			sounderName;
};


struct SFM_info;
struct WORLDGENERAL_API Plane_Const : public Aircraft_Descriptor// Cамолетные постоянные и настройки для 
{   
	~Plane_Const();
	virtual	const Plane_Const * Airplane() const  { return this;};
	void					   read(Lua::Config & config);
	// законов управления.
     float			H_max_;			// максимальная высота полета.
    float			fc_;			// средний расход топлива двигателями кг/кГс.ч  
    float			V_pr_min_;		// минимальная приборная скорость

    float			V_opt_;			// оптимальная скорость на развороте
    float			V_to_;			// взлетная скорость
    float			V_land_;		// посадочная скорость
    bool			Forsaj_Yes_;	// есть ли форсаж
    bool			Shield_Yes_;	// есть ли тормозной щиток
		

 
	float			Alpha_to_;		// взлетный угол атаки
    float			Gamma_max_;		// Ограничение углов крена (градусы)                             
    float			Ny_min_;		// минимальная отрицательная перегрузка Ny
 
    float			vm_land_;		// максимальная скорость у земли, м/с
    float			vm_h_;			// максимальная скорость на высоте (10000м), м/с
    float			S_;				// Площадь крыльев, m
    float			Span_;			// Размах крыльев, m
	WingType		Wing_type_;

    float			P_max_;			// Суммарная максимальная тяга двигателей, кГ.
    float			P_for_;			// Суммарная максимальная форсажная тяга двигателей, кГ.
    
    float			Kflaps_;		// Kflaps для маневренной конфигурации конфигурации
    float			Mach_Alt_;		// максимальная скорость на высоте, число Маха
    float			Ny_max_e_;		// предельная эксплуатационная перегрузка
    float			Kab;			// отношение дальности "теплозаметности" к Su-27 режиме afterburn
    bool            IsTanker;
	Vector3         Wing_tip_pos;
	unsigned char   brakeshute_name_;
	
	bool			DifferentialStabilizer_;

	RefuelingPoints				 Refueling_points;
	ed::vector<bool>			 Refueling_points_compartibility;
 	SFM_info				   * sfm_data;

	ed::string					 aircrat_carrier_type;
};


struct WORLDGENERAL_API Helicopter_Const : public Aircraft_Descriptor// Вертолетные характеристики 
{                 
	~Helicopter_Const();
	virtual	const Helicopter_Const * Helicopter() const { return this;};
	void					   read(Lua::Config & config);

	float		Screw_Height_;  // высота до несущего винта вертолета,м.
	float		Screw_Diam_;	// Диаметр несущих винтов, м.
	unsigned char Fan_Num_;		// Количество лопастей несущего винта
	float		Fan_S_;			// Площадь 1 лопасти, м*м
	float		N_ob_;			// количество оборотов несущего винта,об/мин
	float		BladeChord;
	float		RotorMOI;

	Vector3     RotorPos;
	float		ThrustK;		// корректировка по тяге
	float		FuselageCxa0;
	float		FuselageCxa90;
	float		FuselageArea;
	float		Centering;		// центровка, считается от проекции винта на ось X!!!
	Vector3     TailPos;
	float		TailFinArea;	// площадь вертикального киля
	float		TailStabArea;	// полная площадь стабилизатора (всех консолей)

	Vector3     MOI_;			// моменты инерции
	float		V_max_; // максимальная непревышаемая скорость (при пикировании), км/ч
	float		V_max_cruise_;  // максимальная крейсерская скорость, км/ч

	float		H_stat_max_L_;  // cтатический потолок c учетом земли, м
	float		H_stat_max_;    // cтатический потолок, м
	float		H_din_2_;		// динамический потолок с 2-мя двиг., м
	float		H_din_1_;		// динамический потолок с 1-м двиг., м
	float		Flight_Time_T_; // продолжительность полета типовая,мин

	float		Flight_Time_M_; // продолжительность полета макс.,мин

	float		LeadStockMain;	// ход штока основных стоек
	float		LeadStockSupport;	// ход штока доп. стоек
	unsigned char Scheme_;		// схема вертолета

	float		Vland_max_;		// макс. вертикал.посадочная скорость м/сек
	ed::string	SoundName;		// имя файла звука винта

	HelicopterEngine_Const	*pEngineData;

	CannonSightConst cannonSightConst;
    float       m_fCargoMaxWeight; // максимальный вес внешнего груза
    float       m_fCargoMaxRadius; // радиус отображения грузов в меню
	double		m_l_helicopter_hook;//расстояние от крюка до геометрического центра вертолета
	double		m_h_max_gear_hook;//максимальное расстояние от земли до геометрического центра вертолета, если шасси касаются земли
};


// Cхемы вертолетов Scheme_:
// 0 - Однороторная с хвостовым винтом; к.п.д. трансмиссии = 0.78;
// 1 - Двухроторная с перекрывающимися роторами, 2 мотора; к.п.д.= 0.86;
// 2 - Двухроторная продольная, 2 мотора; к.п.д. трансмиссии = 0.82;

//**************** Характеристики машин и танков.**************************
struct Vehicle_Const // Характеристики машин и танков 
{                  
	unsigned char	Name_;	        // имя машины
	unsigned char	Crew_obsolete;  // экипаж
	unsigned char	W_Num;          // кол-во систем вооружений
	float			Mass_;			// масса, кг
	float			Length_;		// длина, м
	float			Width_;			// ширина, м
    float			Height_;        // высота, м
	float			v_max_;			// предельная скорость по шоссе, км/ч
	float			v_max_g_;		// предельная скорость не по шоссе, км/ч
	float			v_cru_;			// экономическая скорость по шоссе, км/ч
	float			Range_;			// Запас хода в км.
	float			Fuel_;			// Запас топлива, л.
	float			Fording_;		// максимальная глубина брода, м
	float			Gradient_;		// максимальный уклон, %
	float			SideSlope_;		// максимальный боковой наклон, %
	float			VertObstacle_;	// размер вертикального барьера, м
	float			Trench_;		// ширина рва, м
	float			P_max_;			// мощность двигателя, л.с.
	bool			Steering_;		// true - рулевое управление, false - гусеницы
	unsigned char	Gear_Type_;		// 0 - обычный автомобиль, 1 - внедорожник колесный,
									// 2 - гусеничная машина,  3 - железнодорожный объект,
									// 4 - стационарный объект.
	float			T_100_;			// время разгона до 100 км/ч
	float           R_min_;			// Минимальный радиус разворота
	float			X_gear_f_;		// координаты нижней кромки переднего 
	float			Y_gear_f_;      // правого и заднего правого колеса
	float			Z_gear_f_;		// в связанной системе координат.
	float			X_gear_b_;
	float			Y_gear_b_;
	float			Z_gear_b_;
	float			R_max_;			// Радиус колеса
	float			R_track_;		// Радиус гусеницы
	float           Weapon_Dist_;	// макс.дальность действия вооружения, км.
	float           AirFind_Dist_;	// макс.дальность обнаружения целей,км.
	float			TDist_min_;		// минимальная дальность стрельбы, м
	float			TH_max_;		// максимальная высота цели, м 
	float			TH_min_;        // минимальная высота цели, м
	float			Fi_Viz_max_;	// максимальный угол возвышения пилона(пушки), рад
	int				Num_of_Missiles_;// количество ракет
	int				Num_of_Shells_;	// количество снарядов
	bool			Dependence_;	// true - зависимая
	unsigned char	FireCond_;		// 0 - ведет огонь в стационарном положении
	                                // 1 - в походном
	bool			mobility;		// true - подвижна
	float			xFire;			// х-координата пожара
	float			yFire;			// y-координата пожара
	float			zFire;			// z-координата пожара
};

#endif
