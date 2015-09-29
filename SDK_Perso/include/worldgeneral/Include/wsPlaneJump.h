#ifndef __wsPlaneJump_h__
#define __wsPlaneJump_h__

#include "WorldGeneral.h"
#include "wsInitData.h"
#include "wsFlightPlan.h"
#include "IwoAIPilon.h"
#include "woLAConst.h"
#include "wMovingObject.h"
#include "CannonLight.h"
#include "GraphicEffect.h"
#include "wpChaffFlare.h"
#include "IwInfo.h"
#include <array>

enum wConst;
enum wcCoalitionName;
struct ImpactCell;
struct WORLDGENERAL_API wsPlaneJump // Для пересадки из Human Plane в AI Plane
{
	wsPlaneJump();
	~wsPlaneJump();

	ed::string     unit_type;
	ObjectID		id;				// Локальный идентификатор объекта в мире
	ed::string		mid;			// идентификатор объекта в миссии
	wsType			obj_type;		// Тип объекта (см. wsType.h) - unsigned int
	wsFlags			flags;			// Особенности объекта (см. wsFlags.h) - bitset<32>
	wcCoalitionName	coalition;		// Свой/чужой ?
	unsigned int	netRefreshTime;	// Время обновления сетевого образа
	cPosition		pos;			// Матрица положения в мировых координатах - 12 * float
	float			v;				// Скалярная скорость
	cVector			V;				// Вектор скорости - 3 * float	
	bool			m_master;		// false - сетевой
	int				life;			// Уровень живучести
	int				life0;			// Начальный Уровень живучести
	wCamera			camera;			// Параметры камеры
    unsigned char   state;          // cтрана             
	// Переменные из woLA (blank)
    int				Launch_Num;     // Количество пилонов [Restore]
	int				N_obj;          // Номер самолета в звене
	float			M_fuel;         // текущая массa топлива, кг
	float			M_Cargo;        // текущая массa груза, кг
	float			M_BoostMixt;

	//chain_info          main_gun_chain;
	/*unsigned*/ short	nSnare;			// Кол-во пар тепловых ловушек
	ChaffFlarePayload	ChaffFlare;

    float			Alpha;          // Угол атаки
    float			Alpha_Req;      // Требующийся угол атаки
	       // Флаги аргументов рисования
	bool			Canopy_Dropped;      // false - фонарь есть 
	bool			Pilot_No;       // false - пилот есть 
    bool			Whirlwind;      // true - срыв потока [Init]
    bool			LandingEffect;  // true - сел
	bool			Wing_Stream;    // true - есть концевой вихрь[Init]
	unsigned char	Flaps_Flag;     // true - закрылки выпущены
			// Флаги разрушений
	bool			FlagIsDamaged;
	bool			LE_Dest;        // true - разрушен левый  элерон
	bool			RE_Dest;        // true - разрушен правый элерон
	bool			LZ_Dest;        // true - разрушен левый  закрылок
	bool			RZ_Dest;        // true - разрушен правый закрылок
	bool			LRN_Dest;       // true - разрушен левый  руль направления
	bool			RRN_Dest;       // true - разрушен правый руль направления
	bool			LRV_Dest;       // true - разрушен левый  руль высоты
	bool			RRV_Dest;       // true - разрушен правый руль высоты
	bool			LK_Dest;        // true - разрушен левый  киль
	bool			RK_Dest;        // true - разрушен правый киль
	bool			LS_Dest;        // true - разрушен левый  стабилизатор
	bool			RS_Dest;        // true - разрушен правый стабилизатор
	bool			Fonar_Dest;     // true - разрушен фонарь
	bool			Blin_Dest;      // true - разрушен блин локатора
	bool			LW_Dest;        // true - разрушено левое крыло
	bool			RW_Dest;        // true - разрушено правое крыло
	bool			LEng_Fair;      // true - горит левый двигатель
	bool			REng_Fair;      // true - горит правый двигатель
	bool			LEng_Smoke;      // true - дымится левый двигатель
	bool			REng_Smoke;      // true - дымится правый двигатель
	bool			ShipTaxiFlag;   // true - руление по палубе авианосца
	cVector			V_lf;           // lf - landing frame
    cVector			ac_lf;
	bool			FlagWingTail;	// true - включить дымную шашку на крыле 

	unsigned int	Tanker_ID;		// МИ танкера
	bool			FlagEjection;	// Флаг катапультирования
	int				NumTail;		// количество дымов от попаданий

	bool			FlagSoundMove;
	bool			FlagSoundFire;
	
	//    Флаги отказов
	std::array<bool, PlaneCommonFailuresCount> failuresMatrix;
	
	cVector			N;              // вектор перегрузки
	wConst			A_Task;			// Номер задачи.

	bool			FlagSnar;		// Флаг сброса ловушек
	int				formationSort;	// Вид строя
	double			formationSpace;	// Характерное расстояние в строю  
	unsigned char	N_Eng;			// Количество исправных двигателей
	ed::vector<IwoAIPilon*> pilon;		// Массив пилонов
	ImpactCell	*	SmokeArray;// массив пожаров
	float			DamageArray[NumCells];		// повреждения в ячейках
	float			DamageExplode;
	bool			fire_extinguisher; //использование огнетушителя
	cVector			Ordme;          // вектор угловой скорости.
	ed::string		boardNumber;	// бортовой номер	
	wsBirthPlace	BirthPlace;		// место рождения
	int				LeaderPosNum;	// номер позиции лидера в звене
	bool			shake;			// true - самолет задел провода ЛЭП
	float			angle_shake_max;// max угол отклонения самолета после стлкновения с проводами
	float			angle_shake;	// текущий угол отклонения самолета
	float			angle_step;		// шаг изменения угла отклонения	
	bool			Forsaj_Yes;
	bool			Forsaj_flag_Left;	// true - включен форсаж левого двиг.
	bool			Forsaj_flag_Right;	// true - включен форсаж правого двиг.
	int				Skill;			// Уровень мастерства
	float			C_x_Pil;        // Kоэффициент сопротивления сх от подвесок.
	float           M_Laun;			// Cуммарная масса подвесок
	bool            Jet_W;          // false - сброшено оружие и баки  
	wModelTime		Time;           // время
	Graphics::DParamList drawArgs;     // Хранилище для аргументов рисования
	// Переменные из woPlane
	       // Флаги аргументов рисования
	bool			Black_Tail_Flag;  // наличие черного хвоста
    bool			Bomb_Hatch_Flag;  // true - бомболюк открыт
    bool			Bomb_Hatch_Open;  // true - бомболюк действительно открыт
	bool			Shoe_Flag;        // true - есть торм.колодка. 
	bool			Gear_Flag;        //  true - шасси выпущено
	bool			Gear_Doors_Flag;  // true - fairing doors flag
	bool			Wing_Config;      // конфигурация крыльев
	float			AirBrakeValue;// выпущен ли тормозной щиток
	bool			Hook_Flag;        // false - гак убран, true - выпущен 
	bool			Nos_Flag;         //  true - носки выпущены 
	bool			Fonar_Flag;       // false - фонарь опущен, true - поднят 
	// посадочные фары
	unsigned short	lights_status_logical; // маска данных по осветительному оборудованию
	float			Stick_MG;         // обжатие стоек основного шасси
	bool			Stick_MG_T;       // триггер для Stick_MG
	bool			ParFlag;          // торм. паращют false - нет;
	float			Shtok;            // ход передней стойки шасси
	bool			Propeller_Go;     // true - запуск двигателя
	bool			Propeller_Stop;   // true - остановка двигателя
	bool			Propeller_Flight; // true - полетный режим двигателя
	bool			Propeller_Taxi;   // true - работа двигателя при рулежке
	bool			Hose_Flag;        // true - выпущен заправочный конус заправщика
	bool			Refuel_Boom_Flag; // true - выпущена заправочная штанга
	int				parkingTermId;
	// Прочие
	int			    NumberRefuelPoint;// номер соска заправщика
	Waypoint		Goto_wpt;         // Точка "K" маршрута
	int				callsign;		  // позывной
	unsigned int	landPlaceID;

    players_payload_choice   user_selected_payload;

	GunsLoadFactors guns_load_factors_;

	bool			under_civil_registration;
	bool			hardpoint_racks;//pilons

	bool			is_engine_started;

	bool			Personage;

	wsInitData*		unit_init_data;

	void setLiveryHandle(unsigned int livery){_livery=livery;}
	unsigned int getLiveryHandle()const{return _livery;}
private:
	unsigned int	_livery;          // вариант раскраски
};

#endif
