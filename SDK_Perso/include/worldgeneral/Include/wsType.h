#ifndef __wsType_h__
#define __wsType_h__

#include "WorldGeneral.h"
#include "Serializer\Serializer.h"

/*	Структура wsType служит для классификации объектов
	с использованием 4-уровневой иерархии. Например:
	  Оружие
	  Ракета
	  Класс воздух-воздух
	  Р-73
*/

// **************** 1 level ***************
const unsigned char	wsType_Air			= 1;
const unsigned char	wsType_Ground		= 2;
const unsigned char	wsType_Navy			= 3;
const unsigned char	wsType_Weapon		= 4;
const unsigned char	wsType_Static		= 5;
const unsigned char	wsType_Destroyed	= 6;
const unsigned char wsType_Test1		= 200;

// **************** 2 level ***************
// Air objects (wType_Air)
const unsigned char	wsType_Airplane		= 1;
const unsigned char	wsType_Helicopter	= 2;
const unsigned char	wsType_Free_Fall	= 3;
// Weapon (wsType_Weapon)
const unsigned char	wsType_Missile		= 4;  // Ракетa
const unsigned char	wsType_Bomb		    = 5;  // Бомбa
const unsigned char	wsType_Shell		= 6;  // Cнаряд
const unsigned char	wsType_NURS		    = 7;  // НеУправляемые Реактивные Снаряды и их контейнеры
// Ground objects (wsType_Ground)
const unsigned char wsType_Moving		= 8;
const unsigned char wsType_Standing 	= 9;
const unsigned char wsType_Tank		 	=17;  // танк
const unsigned char wsType_SAM		 	=16;  // объект ПВО
//  Navy objects  (wsType_Navy)
const unsigned char	wsType_Ship         =12;  // Корабль	
//  object (wsType_Static)
const unsigned char	wsType_Airdrome		=13;  // Аэродром
const unsigned char	wsType_Explosion    =14;  // Взрыв

const unsigned char wsType_GContainer   =15;  // Контейнер помеховый/управляющий/пушечный
const unsigned char	wsType_AirdromePart	=18;  // Аэродром
const unsigned char	wsType_WingPart		=19;  // Падающее крыло (только для падающих обломков крыла) 

const unsigned char wsType_Test2		= 200;

// **************** 3 level ***************
// Air objects, wsType_Airplane
const unsigned char	wsType_Fighter		= 1;
const unsigned char	wsType_F_Bomber 	= 2;
const unsigned char	wsType_Intercepter 	= 3;
const unsigned char	wsType_Intruder 	= 4;
const unsigned char	wsType_Cruiser 	 	= 5;
const unsigned char	wsType_Battleplane  = 6;
// Free-fall air objects
const unsigned char	wsType_Snars 	 	= 31;
const unsigned char	wsType_Parts 	 	= 35; // Обломки 
const unsigned char	wsType_FuelTank 	= 43; // Топливные баки
const unsigned char	wsType_Canopy    	= 44; // Топливные баки
const unsigned char	wsType_ShellCartridge = 45; // гильза 

// wsType_Missile
const unsigned char	wsType_AA_Missile	= 7;  // Ракетa воздух-воздух
const unsigned char	wsType_AS_Missile	= 8;  // Ракетa воздух-земля
const unsigned char	wsType_SA_Missile	= 34; // Ракетa земля-воздух
const unsigned char	wsType_SS_Missile	= 11; // Ракетa земля-земля

const unsigned char	wsType_AA_TRAIN_Missile	= 100;  // Ракетa воздух-воздух тренировочная
const unsigned char	wsType_AS_TRAIN_Missile	= 101;  // Ракетa воздух-земля	тренировочная

// wsType_Bomb		     Бомбы
const unsigned char	wsType_Bomb_A       = 9;  // Бомбa общего назначения
const unsigned char	wsType_Bomb_Guided  = 36; // Бомбa управляемая
const unsigned char	wsType_Bomb_BetAB   = 37; // Бомбa бетонобойная
const unsigned char	wsType_Bomb_Cluster = 38; // Бомбa кассетная
const unsigned char	wsType_Bomb_Antisubmarine = 39;// Бомбa глубинная
const unsigned char	wsType_Bomb_ODAB    = 40; //Бомбa объемно-детонирующая
const unsigned char	wsType_Bomb_Fire    = 41; // Бомбa зажигательная
const unsigned char	wsType_Bomb_Nuclear = 42; // Бомбa атомная
const unsigned char	wsType_Bomb_Lighter = 49; // Бомбa осветительная
// wsType_Shell		  // Cнаряды
const unsigned char	wsType_Shell_A		= 10;  // снаряд

//Navy objects (wsType_Navy,wsType_Cruiser)
const unsigned char wsType_AirCarrier	= 12;  // Авианесущий крейсер
const unsigned char wsType_HCarrier	    = 13;  // Крейсер с вертолетами
const unsigned char wsType_ArmedShip    = 14;  // Вооруженный корабль
const unsigned char wsType_CivilShip    = 15;  // Невооруженный корабль
//         wsType_Airdrome
const unsigned char wsType_RW1	        = 20;  // Аэродром с 1 ВПП	
const unsigned char wsType_Heliport		= 40;  // Вертолётодром

//	wsType_Explosion   Типы взрывов
const unsigned char wsType_GroundExp    = 29;  // Наземный взрыв
//           wsType_NURS
const unsigned char wsType_Container    = 32;  // Контейнер НУРС-ов
const unsigned char wsType_Rocket       = 33;  // Неуправляемая ракета
//           wsType_GContainer
const unsigned char wsType_Control_Cont = 44;  // Управляющий контейнер
const unsigned char wsType_Jam_Cont     = 45;  // Помех контейнер
const unsigned char wsType_Cannon_Cont  = 46;  // Пушечный контейнер
const unsigned char wsType_Support      = 47;  // Балочный держатель
const unsigned char wsType_Snare_Cont   = 48;  // Контейнер ловушек
const unsigned char wsType_Smoke_Cont   = 50;  // Дымогенератор

// Test object (wType_Test2)
const unsigned char wsType_Test3		= 200;
	
//  Ground object (wsType_Moving || wsType_Standing )
const unsigned char wsType_NoWeapon     = 25;  // Безоружные наземные
const unsigned char wsType_Gun          = 26;  // C пушкой
const unsigned char wsType_Miss         = 27;  // C ракетами
const unsigned char wsType_ChildMiss    = 28;  // С ракетами, но одни без wsType_Miss не пускают
const unsigned char wsType_MissGun      = 104;  // C тем и другим //Раньше было 29, но это наземный взрыв
const unsigned char wsType_Civil        = 100;  //Гражданский объект 
//***************************************************
//
//const unsigned char wsType_Radar        = 101;  //Какой  либо радар без оружия
const unsigned char wsType_Radar_Miss   = 102;  //Пусковая установка с совмещенным радаром
const unsigned char wsType_Radar_MissGun=103;  //Тунгуска (c радаром)
const unsigned char wsType_Radar_Gun    =105; //Шилка, Gepard (есть радар и пушка)

//************  class wsType  **********************************
class WORLDGENERAL_API wsType 
{
	union {
		struct {
			unsigned char	level1;
			unsigned char	level2;
			unsigned char	level3;
			unsigned char	level4;
		};
		unsigned int		type;
	};

	friend WORLDGENERAL_API Serializer& operator << (Serializer& serializer, wsType& data);

public:
    wsType() { type = 0; };
    wsType(const unsigned int _type) { type = _type; };
    wsType(const wsType& a) { type = a.type; };
	wsType(	const unsigned char l1, 
			const unsigned char l2, 
			const unsigned char l3, 
            const unsigned char l4) : level1(l1), level2(l2), level3(l3), level4(l4) {};

    const unsigned int Type() const { return type; }
    const unsigned int Type(const wsType& a) { return type = a.type; }
    const bool operator==(const wsType& t) const { return type == t.type; }
    const bool operator!=(const wsType& t) const { return type != t.type; }
    const bool operator< (const wsType& t) const { return type < t.type; }
    const unsigned int operator=(const wsType& t) { return type = t.type; }
    const int size() const { return sizeof(type); }

    const unsigned char	Level1() const { return level1; }
    const unsigned char Level2() const { return level2; }
    const unsigned char Level3() const { return level3; }
    const unsigned char Level4() const { return level4; }

    void	Level1(unsigned char l1) { level1 = l1; }
    void	Level2(unsigned char l2) { level2 = l2; }
    void	Level3(unsigned char l3) { level3 = l3; }
    void	Level4(unsigned char l4) { level4 = l4; }

	const unsigned int Level1_3() const;
};

#endif
 
