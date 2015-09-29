#ifndef __wShapeTable_h__
#define __wShapeTable_h__
#include "edObjects.h"

#include <stdio.h>

// В таблицах объектов хранятся индексы разрушенных объектов (в той
// же самой таблице) и индексы объектов-теней (в таблице теней у Виталика).
// Если отображение не взаимно однозначно (например, тень от самолета
// с изменяемой геометрией может быть разной), то индекс указывает на
// первый из них, а остальные должны быть в таблице следом за ним.
// Головная боль по смене теней ложится на программиста.
// Если индекс разрушенного объекта равен нулю, то после исчерпания
// жизни он исчезает без следа и не рисуется. Если же он ссылается сам
// на себя, то объект вообще гордо игнорирует жалкие попытки разрушить
// его. Живучесть -1 применяется в таблицах только для того, чтобы
// обломки рисовались (не рисуются только те, у которых она строго нулевая).
// Некоторые константы индексов определены в wShapeTable.h

#include <ed/string.h>

#include "Serializer\Serializer.h"
#include "lTerraBasic.h"

namespace Graphics{
	class Model;
}

class gShapeObject;

struct EDOBJECTS_API wShape	// Запись о графическом объекте
{
	// Тип анимации при разрушении
	enum TYPE_DESTRUCT_ANIMATION
	{
		NONE = 0,			// Просто
		FALLDOWN = 1,		// Под землю
		INCLINE = 2,		// На бок

		AUTO = 10			// автоматический выбор (для совместимости)
	};
public:
	// Идентификация:
	ed::string shapename;	// Уникальное имя объекта
	
	ed::string filename;// Имя-строка для конструирования gobj
	float life;			// Живучесть (максимальный накапливаемый ущерб)
	float fVisibility;	// Коэффициент корректировки порога видимости
	Graphics::Model* model;	// Модель
	bool bLoadInit;		// Объект нужно инициализировать при загрузке программы
	bool bStartInit;	// Объект нужно инициализировать при старте миссии
	bool bInit;			// Объект уже был инициализирован
	int burningTime;	// Время горения (сек)
    int categoryFire;	// Категория пожара 1,2,3,4,5
    float expMass;      // Мощность взрыва (кг.тротила)
	bool bCollidable;	// Collidable (участвует в проверке столкновений)
	bool bDrawOnMap;	// Рисовать на карте
	ed::string username;	// Имя для MIssionEditor
	bool bTarget;		// может выступать как цель
	bool bBlockable;		// Объединение при отрисовке
	// Тип позиционирования объекта на земле
	TYPE_POSITINING TypePositioning;
	// Имя класса объекта (для фабрики классов)
	ed::string classname;
	// Тип анимации при разрушении
	TYPE_DESTRUCT_ANIMATION typeDestructAnimation;
	// разрушеный объект
	wShape* pDestrShape;

	//Старье
	int	iDestr;			// Индекс записи о развалинах
	int	itbl;			// Номер таблицы
	int	ind;			// Номер объекта

	//Старье
	char* name;
	const char* uName;

	wShape();
	// Загрузить объект
	virtual Graphics::Model* GetModel();

	// Инициализация таблиц при загрузке программы
	static	void Init(bool bNotLoad=false, bool bLoadModels=false);		

	static	void Finish(void);		// Освобождение памяти при завершении программы
	static  void InitLand(void);	// Реинициализация только наземных сооружений
	static  void Start(void);		// Инициализация таблиц при старте миссии
	static  void StartNet(void);	// Инициализация таблиц при старте миссии
	static  void Stop(void);		// Разгрузка таблиц после завершения миссии
	static  void Save(FILE *f);		// Сохранение списка инициализированных объектов
	static  void Restore(FILE *f);	// Восстановление таблиц из файла и инициализация объектов

	// Устарело!!!
	static gShapeObject *Get(int itbl, int ind);
	static gShapeObject *Get(const char* shapename);

	// Получение объекта по индексам с генерацией, если bInit == false
	static wShape* GetDecl(int itbl, int ind);
	// Получение объекта по имени
	static wShape* GetDecl(const char* shapename);
	// assign new position type for all shape objects
	static void SetPositionType(unsigned char PositionType = 0);
};

EDOBJECTS_API Serializer &operator <<(Serializer &, wShape *&);

// Для совместимости
//extern int W_MAX_SHAPE_TABLES;
//extern int* W_MAX_SHAPES;
//extern wShape** wShapeTables;	// Массив таблиц графических объектов	


/////////////////////////////////////////////////////
// Константы индексов в нулевой таблице
const int gShape_EMPTY			= 0;
const int gShape_VORONKA		= gShape_EMPTY + 1;
const int gShape_VORONKA_BETON	= gShape_VORONKA + 1;
const int gShape_smoke_ball01	= gShape_VORONKA_BETON + 1;	// 15 шт.
const int gShape_fire_ball01	= gShape_smoke_ball01 + 15;	// 15 шт.
const int gShape_roket_smoke_ball01	= gShape_fire_ball01 + 15;
const int gShape_rocket_long_trail01	= gShape_roket_smoke_ball01 + 1;
const int gShape_water_exp01	= gShape_rocket_long_trail01 + 1;
const int gShape_black_long_trail01 = gShape_water_exp01 + 1;
const int gShape_Sun01 = gShape_black_long_trail01 + 1;
const int gShape_Sun02 = gShape_Sun01 + 1;
const int gShape_burst01 = gShape_Sun02 + 1;
const int gShape_explode_wave = gShape_burst01 + 1;
const int gShape_water_circle = gShape_explode_wave + 1;
const int gShape_ship_trace01 = gShape_water_circle + 1;	// 4 шт.
const int gShape_fire_long_trail01 = gShape_ship_trace01 + 4;	// 10 шт.
const int gShape_flame_ball        = gShape_fire_long_trail01 + 10;
const int gShape_MIG_23_OBLOMOK    = gShape_flame_ball +2;
const int gShape_SU27_SRYW_L = gShape_MIG_23_OBLOMOK+39*3;
const int gShape_SLED=gShape_SU27_SRYW_L+12;
const int gShape_TRACE=gShape_SLED+1;
const int gShape_VORONKA_grexp=gShape_TRACE+1;
const int gShape_semisphere=gShape_VORONKA_grexp+1;
// Номера таблиц	
const int wShapeN_Table0			= 0;  // Разное
const int wShapeN_ShipTable			= 1;  // Корабли
const int wShapeN_BombTable			= 2;  // Бомбы
const int wShapeN_MissileTable		= 3;  // Ракеты и снаряды
const int wShapeN_ContainerTable	= 4;  // Подвесные контенеры и баки 
const int wShapeN_PlaneTable		= 5;  // Самолеты
const int wShapeN_TechnicsTable		= 6;  // Наземная техника
const int wShapeN_SAM_AND_AAG		= 7;  // Наземные средства противовоздушной обороны
const int wShapeN_StructureTable	= 8;  // Строения
const int wShapeN_ParachuteTable	= 9;  // Сброшенные парашюты
const int wShapeN_PartsTable		= 10; // Прочие летающие предметы
const int wShapeN_TreesTable		= 11; // Лесопосадки
const int wShapeN_ForestTable		= 12; // Леса

// Список сброшенных парашютов
const int wShape_B_52_PAR_GR   = 1;
const int wShape_F_117_PAR_GR  = 2;
const int wShape_TORM_1_PAR_GR = 3;
const int wShape_TORM_PAR_GR   = 4;

#endif
