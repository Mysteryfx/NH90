#ifndef LANDSCAPE3__SHAPE_TABLE__
#define LANDSCAPE3__SHAPE_TABLE__
#include "../landscape3_config.h"

#include <ed/string.h>
#include <ed/map.h>
#include "lPointOnSurface.h"

// Типы позиционирования объектов
/*enum TYPE_POSITINING
{
	TP_BYNORMAL = 1,		// пересчитать систему координат, учитывая наклон поверхности
	TP_ONLYHEIGTH = 2,		// Оси нормируются, объект ставится на высоту земли
	TP_XZ_PLANE = 3,		// проецируем на плоскость грани вектора X и Z cPosition
	TP_NO = 4,				// Без трансформаций
	TP_REF_HEIGTH = 5		// Без трансформаций, только к высоте добавляется высота поверхности
};*/

namespace landscape3
{

/////////////////////////////////////////
// Запись о графическом объекте
struct ShapeDefinition	
{
	ed::string	shapename;		// Идентификатор
	ed::string	name;			// Имя-строка для конструирования gobj
	float		life;			// Живучесть (максимальный накапливаемый ущерб)
	float		fVisibility;	// Коэффициент корректировки порога видимости
	ed::string	iDestr;			// Индекс записи о развалинах
	ed::string	iShadow;		// Индекс тени
	bool		bLoadInit;		// Объект нужно инициализировать при загрузке программы
	bool		bStartInit;		// Объект нужно инициализировать при старте миссии
	int			burningTime;	// Время горения (сек)
	int			categoryFire;	// Категория пожара 1,2,3,4,5
	float       expMass;        // Мощность взрыва (кг.тротила)
	bool		bDrawOnMap;		// Рисовать на карте
	ed::string	uName;			// имя, показываемое пользователю
	int			ind;			// Номер объекта
	ed::string	classname;		// Имя класса
	ed::string	positioning;	// Тип позиционирования
	TYPE_POSITINING position_type;		// Тип позиционирования
	ed::string	typeDestructAnimation;	// Тип анимации при разрушении
	bool bCollidable;			// Collidable
	bool bBlockable;			// bBlokable
	bool bTarget;				// может выступать как цель

	EDTERRAIN_API ShapeDefinition();
};

class ShapeTable
{
public:

private:
	ed::map<ed::string, ShapeDefinition> shapes;
public:
	EDTERRAIN_API ShapeTable();
	EDTERRAIN_API ~ShapeTable();
	EDTERRAIN_API bool addTable(const char* filename, ed::string* errors = NULL, ed::string* errorshapes = NULL);
	EDTERRAIN_API bool addTable(const ed::string &filename, ed::string* errors = NULL, ed::string* errorshapes = NULL);
	EDTERRAIN_API ShapeDefinition* getShapeByName(const char* shapename);
	EDTERRAIN_API ShapeDefinition* getShapeByName(const ed::string &shapename);
};

}

#endif