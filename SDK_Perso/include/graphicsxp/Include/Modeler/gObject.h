/*
//	Файл:			gObject.h
//	Подсистема:		Graphics (interface)
//
//	Классы для формирования графических объектов
//
*/

#ifndef __gObject_h__
#define __gObject_h__

#include "graphicsxp.h"

#include    "cPosition.h"
#include    "gGeometry.h"
#include   "zDataTypes.h"

#include <ed/vector.h>
#include <ed/string.h>

typedef ed::vector<ed::string> TexFilenameList;
// Константы, возвращаемые функцией Class()
const unsigned char		gObjectClass		= 1;	// Объект базового (абстрактного) класса
const unsigned char		gObjectClassShape	= 2;	// Объект класса gShapeObject
const unsigned char		gObjectClassText	= 6;	// 
const unsigned char		gObjectClassNumber	= 7;	//
const unsigned char		gObjectClasslShape	= 9;	// Объект класса lShapeObject


// Константы для pos_type
const unsigned char		gObjectPosWorld		 = 1;	// в мировых координатах
const unsigned char		gObjectPosCamera	 = 2;	// в координатах относительно камеры
const unsigned char		gObjectPosOrtho		 = 3;	// в ортогональной проекции
const unsigned char		gObjectPosWindow	 = 4;	// в оконных координатах
const unsigned char		gObjectPosWindowFirst= 5;	// в оконных координатах
const unsigned char		gObjectPosWorldMFD	 = 6;	// в мировых координатах (Виталик - cортировка для MFD)

// Константы для flags (не используются)
const unsigned char		gObjectOffset		= 0x01;
const unsigned char		gObjectNoise		= 0x02;
const unsigned char		gObjectOffsetClose	= 0x04;
const unsigned char		gObjectDynamic		= 0x08;
const unsigned char		gObjectTexRepeat	= 0x10;

// Константы для zone (не используются)
const unsigned char		gObjectZoneNear		= 2;
const unsigned char		gObjectZoneMiddle	= 1;
const unsigned char		gObjectZoneFar		= 0;

// Константы для layer.
const unsigned char		gObjectLayerSea			=  0; // Море.
const unsigned char		gObjectLayerGround		=  1; // Поверхность суши.
const unsigned char		gObjectLayerSurface		=  2; // Плоские объекты на поверхности суши.
const unsigned char		gObjectLayerSpot		=  3; // Воронки.
const unsigned char		gObjectLayerStructure	=  4; // Объемные   объекты, стоящие на поверхности.
const unsigned char		gObjectLayerAir			=  5; // Воздушные  объекты.

// Вспомогательные слои.
const unsigned char		gObjectLayerBlend		= 12; // Прозрачные объекты.
const unsigned char		gObjectLayerNoIdent		= 99; // Не идентифицируемый слой.

class gObject	// Базовый абстрактный класс ------------------------------------
{
public:
		cPosition*		pos;		// Положение объекта
		unsigned char	pos_type;	// Правило задания положения (см. константы выше)
		unsigned char	flags;		// -------------------------Дополнительные установки
		float			distance;	// Расстояние до камеры (только в СК мира).
		unsigned char	zone;		// -------------------------Зона отрисовки (не используется)
		unsigned char	layer;		// -------------------------Слой отрисовки
virtual unsigned char	Class(void){return gObjectClass;}
};


// Класс объектов, геометрия которых хранится в новой базе данных


struct m16F
{
float		 m [16];
unsigned int mflags;
};

class	gDrawCommand
{
public:	int		   dataType; //	Тип команды (соответствует Bazar).
};

class	gCMDexecute : public gDrawCommand
{
public:
int					  layer; //	Идентификатор слоя фрагмента.
//boxV			*		box; //	Описание габаритов.
int               boxOffset; // x64: box offset
int				     mflags; // Тип матрицы.
//float			*	 matrix; //	Результирующая матрица преобразований фрагмента.
int            matrixOffset; //	x64: matrix offset
//matV			*  material; //	Указатель на текущий материал.
int          materialOffset; // x64: material offset
//gColor4f		*	 colorA; // Ambient  - составляющая цвета материала.
//gColor4f		*	 colorD; // Diffuse	 - составляющая цвета материала.
//gColor4f		*	 colorS; // Specular - составляющая цвета материала.
int colorAoffset;
int colorDoffset;
int colorSoffset;
int					 tindex; // Идентификатор текстуры (индекс в массиве).
//v3V				*  vertices; //	Указатель на массив вершин.
//n3V				*	normals; //	Указатель на массив нормалей.
//tv2V			*	tcoords; //	Указатель на массив текстурных координат.
//void			*    colors; // Указательна массив цвета вершин.
//void			*primitives; //	Указатель на описание примитивов.
int verticesOffset;
int normalsOffset;
int tcoordsOffset;
int colorsOffset;
int primitivesOffset;
unsigned int	      vnumb; //	Количество вершин.
unsigned int	      vtype; //	Тип представления вершин.
unsigned int		  pnumb; //	Количество примитивов.
unsigned int	      ptype; //	Тип примитивов.
unsigned int	  drawFlags; //	Флаги рисования.
unsigned int	  semantics; // Семантика.
};

class	gSCexecute : public gDrawCommand
{
public:
char		 Name[ENAMECSZ]; // Имя сцены (для отладки).
INSTANCE		   Instance; // Привязка сцены.
int				   NCommand; //	Количество команд в сцене.
//gDrawCommand	**  Command; //	Указатель на первую команду сцены.
int           CommandOffset; //	x64: смещение до массива смещений команд
};

class	gLDexecute : public gDrawCommand
{
public:
int					nLevels; // Количество уровней детализации (16-ть максимум).
							 // Уровни в БД д.б. упорядочены по убыванию степени детализации.
float		   distance[16]; // Дистанции отключения уровней.
//gSCexecute	   *sc	   [16]; //	Адреса сцен детализации.
int            scOffset[16]; // x64: offset of detail scenes
};

class	gCEexecute : public gDrawCommand
{
public:
//kinematicsV		*kinematics; // Указатель на описание кинематики в БД.
int kinematicsOffset;   // x64: kinematics offset
int					  knumb; // Количество элементов кинематики.
m16F			     matrix; //	Матрица кинематических преобразований.
};

class GRAPHICSXP_API gShapeObject : public gObject
{
// Макроопределение локального хипа для графических объектов
// (см. cLocalCreate.h)
private:
//	friend bool LoadProc(INSTANCE,ICache*);
public:
//	static	HINSTANCE  objectLibrary;
//	static	ICache		    *pICache; // Указатель на интерфейс базы.
//	static может быть, если используется только одна база данных!
//	ICache					*pICache;
//	INSTANCE			    Instance; // Привязка к объекту в базе данных.
//	unsigned int				  id; // ------------------------------Идентификатор объекта в мире.
	// Для объектов, не входящих в таблицы, itbl = ind = -1
	int							itbl; // Номер таблицы графических объектов.
	int							 ind; // Индекс в таблице графических объектов.
//	unsigned int			   level; // ------------------------------Не используется.
//	unsigned int			 nLevels; // ------------------------------Количество уровней детализации объекта.
	boxV						*box; // ------------------------------???????
	gBox						 Box; // Описание габаритов объекта.
//	static	Context			 context; // ------------------------------???????
//	FlatList				flatlist; // ------------------------------???????

	gShapeObject(void);
	gShapeObject(const ed::string&, const ed::string&, TexFilenameList&);
//	gShapeObject(char*);
//	gShapeObject(char*,int,int);
//	gShapeObject(char*,MappedFile*);
//	gShapeObject(INSTANCE);
//	gShapeObject(INSTANCE,MappedFile*);
//	gShapeObject(INSTANCE,int,int);

	// инициализация gShapeObject, создание scene list на 1 команду
	// если nVertices != 0 && Vertices == 0	то выделяется память под 
	// Vertices, Normals, Primitives, TextureCoords
//	gShapeObject(int			nVertices,			// кол-во вершин
//				 v3V			Vertices[],			// массив вершин
//				 n3V			Normals[],			// массив нормалей
//				 int			nPrimitives,		// кол-во примитивов
//				 unsigned short Primitives[],		// примитивы
//				 char*			TextureFileName,	// имя файла текстуры
//				 tv2V			TextureCoords[],	// текстурные координаты
//				 unsigned int  drawFlags);			// флаг рисования

	~gShapeObject();

	float		*		 args; // Массив аргументов.
	void		*	  uMemory; // Указатель на невыровненный блок памяти.
	//gSCexecute	*	SceneList; // Описание объекта (выровнен на 32 байта).
    char        *	ScenePtr; // Описание объекта (выровнен на 32 байта).
	int				SceneComm; // Общее количество команд.
	int				SceneSize; // Размер описания объекта в 4-х байтовых словах.
	int				ScenePrim; // Количество примитивов.
//								  Данные для определения столкновений.
	CollisionHeaderV*	collisionH; // Заголовок.
	CollisionV		*	collisionI; // Трехмерный массив.
	ChPointsV		*	collisionP; // Характерные отрезки.
	int                  lineCount; // Количество характерных отрезков.

friend	class gsPresort;

virtual unsigned char	Class(void){return gObjectClassShape;}
};

#endif
